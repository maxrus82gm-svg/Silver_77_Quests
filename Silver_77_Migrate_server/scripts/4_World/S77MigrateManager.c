class S77MigrateUnitState
{
    DayZInfected m_Infected;
    string m_InfectedId;
    string m_ScenarioId;
    string m_ClassName;
    vector m_MigrationTarget;
    ref TVectorArray m_Waypoints;
    int m_WaypointIndex;
    string m_Mode;
    bool m_VanillaBusy;
    int m_ResumeAfterTime;
    int m_NextPathRetryTime;
    int m_LogIntervalMs;
    int m_NextLogTime;

    void S77MigrateUnitState(DayZInfected infected, string infectedId, string scenarioId, vector migrationTarget, int logIntervalMs)
    {
        m_Infected = infected;
        m_InfectedId = infectedId;
        m_ScenarioId = scenarioId;
        m_ClassName = infected.GetType();
        m_MigrationTarget = migrationTarget;
        m_Waypoints = new TVectorArray();
        m_WaypointIndex = 0;
        m_Mode = "MIGRATION";
        m_VanillaBusy = false;
        m_ResumeAfterTime = 0;
        m_NextPathRetryTime = 0;
        m_LogIntervalMs = logIntervalMs;
        m_NextLogTime = 0;
    }
}

class S77MigrateManager
{
    protected static ref S77MigrateManager s_Instance;

    protected const string EVENT_LOG_PREFIX = "[S77Migrate][EVENT]";
    protected const string MIGRATION_LOG_PREFIX = "[S77Migrate]";
    protected const int UPDATE_INTERVAL_MS = 250;
    protected const int LOG_CHECK_INTERVAL_MS = 1000;
    protected const int STORM_RAMP_STEP_MS = 10000;
    protected const int AGGRO_COOLDOWN_MS = 5000;
    protected const int PATH_RETRY_INTERVAL_MS = 5000;
    protected const float WAYPOINT_TOLERANCE = 1.75;
    protected const float ARRIVAL_TOLERANCE = 4.0;
    protected const float MIGRATION_SPEED = 1.0;

    protected ref S77MigrateConfig m_Config;
    protected ref array<ref S77MigrateScenarioConfig> m_Scenarios;
    protected ref array<ref S77MigrateUnitState> m_Units;
    protected ref PGFilter m_PathFilter;
    protected bool m_EventStarted;
    protected bool m_RouteUpdateScheduled;
    protected bool m_LogScheduled;
    protected bool m_StartGroupsScheduled;
    protected bool m_WeatherStartScheduled;
    protected bool m_WeatherCompletionScheduled;
    protected bool m_StormRampStartScheduled;
    protected bool m_StormRampUpdateScheduled;
    protected bool m_WeatherControlActive;
    protected bool m_PreviousMissionWeather;
    protected bool m_Stopped;
    protected bool m_Initialized;
    protected int m_StormRampStartTime;
    protected float m_EffectiveStormRampSeconds;

    void S77MigrateManager()
    {
        m_Scenarios = new array<ref S77MigrateScenarioConfig>();
        m_Units = new array<ref S77MigrateUnitState>();
        m_PathFilter = new PGFilter();
        int includeFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE;
        int excludeFlags = PGPolyFlags.DISABLED | PGPolyFlags.SWIM | PGPolyFlags.SWIM_SEA | PGPolyFlags.JUMP | PGPolyFlags.CLIMB | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH;
        m_PathFilter.SetFlags(includeFlags, excludeFlags, PGPolyFlags.NONE);
        m_PathFilter.SetCost(PGAreaType.BUILDING, 5.0);
    }

    static void Init()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!s_Instance)
            s_Instance = new S77MigrateManager();

        s_Instance.InitializeEvent();
    }

    static void Shutdown()
    {
        if (!s_Instance)
            return;

        s_Instance.Stop();
        s_Instance = null;
    }

    protected void InitializeEvent()
    {
        if (m_Initialized)
        {
            Print(EVENT_LOG_PREFIX + " Duplicate initialization ignored");
            return;
        }

        m_Initialized = true;

        m_Config = S77MigrateConfigLoader.Load();
        if (!m_Config)
        {
            Print(EVENT_LOG_PREFIX + " ERROR: unified migration config could not be loaded");
            return;
        }

        for (int scenarioIndex = 0; scenarioIndex < m_Config.scenarios.Count(); scenarioIndex++)
        {
            S77MigrateScenarioConfig scenario = m_Config.scenarios.Get(scenarioIndex);
            if (!scenario)
            {
                Print(EVENT_LOG_PREFIX + " ERROR: null scenario skipped at index=" + scenarioIndex.ToString());
                continue;
            }

            if (!scenario.IsValid())
            {
                Print(EVENT_LOG_PREFIX + " ERROR: invalid scenario skipped at index=" + scenarioIndex.ToString() + " scenarioId=" + scenario.scenarioId);
                continue;
            }

            if (scenario.enabled == 1)
                m_Scenarios.Insert(scenario);
            else
                Print(MIGRATION_LOG_PREFIX + " scenario=" + scenario.scenarioId + " disabled");
        }

        if (m_Config.enabled != 1)
        {
            Print(EVENT_LOG_PREFIX + " Event disabled. Set enabled=1 in " + S77_MIGRATE_CONFIG + " and restart the server.");
            return;
        }

        if (m_Scenarios.Count() == 0)
        {
            Print(EVENT_LOG_PREFIX + " No enabled migration scenarios; event start aborted");
            return;
        }

        int delayMs = Math.Round(m_Config.eventDelaySeconds * 1000.0);
        if (m_Config.weatherEnabled == 1)
        {
            m_WeatherStartScheduled = true;
            Print(EVENT_LOG_PREFIX + " Global weather scheduled in " + m_Config.eventDelaySeconds.ToString() + " seconds; transition=" + m_Config.weatherTransitionSeconds.ToString() + " seconds; scenarios=" + m_Scenarios.Count().ToString());
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginWeatherTransition, delayMs, false);
        }
        else
        {
            m_StartGroupsScheduled = true;
            Print(EVENT_LOG_PREFIX + " Weather disabled; all enabled groups scheduled in " + m_Config.eventDelaySeconds.ToString() + " seconds");
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartAllScenarios, delayMs, false);
        }
    }

    protected void BeginWeatherTransition()
    {
        m_WeatherStartScheduled = false;
        if (m_Stopped || m_EventStarted)
            return;

        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            Print(EVENT_LOG_PREFIX + " ERROR: Weather API is unavailable; event start aborted");
            return;
        }

        m_PreviousMissionWeather = weather.GetMissionWeather();
        weather.MissionWeather(true);
        m_WeatherControlActive = true;

        float transitionSeconds = m_Config.weatherTransitionSeconds;
        float windSpeed = m_Config.weatherWindMagnitude;
        if (windSpeed < 0.1)
            windSpeed = 0.1;

        weather.GetOvercast().Set(m_Config.weatherOvercast, transitionSeconds, transitionSeconds);
        weather.GetFog().Set(m_Config.weatherFog, transitionSeconds, transitionSeconds);
        weather.GetRain().Set(m_Config.weatherRain, transitionSeconds, transitionSeconds);
        weather.SetWindSpeed(windSpeed);
        weather.SetStorm(0.0, m_Config.weatherStormThreshold, m_Config.weatherStormTimeoutSeconds);

        string weatherLog = EVENT_LOG_PREFIX;
        weatherLog = weatherLog + " Weather transition started duration=" + transitionSeconds.ToString();
        weatherLog = weatherLog + " overcast=" + m_Config.weatherOvercast.ToString();
        weatherLog = weatherLog + " fog=" + m_Config.weatherFog.ToString();
        weatherLog = weatherLog + " wind=" + windSpeed.ToString();
        weatherLog = weatherLog + " rain=" + m_Config.weatherRain.ToString();
        weatherLog = weatherLog + " stormFinalDensity=" + m_Config.weatherStormDensity.ToString();
        Print(weatherLog);

        ScheduleStormRamp(transitionSeconds);

        if (transitionSeconds <= 0.0)
        {
            CompleteWeatherTransition();
            return;
        }

        int transitionMs = Math.Round(transitionSeconds * 1000.0);
        m_WeatherCompletionScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CompleteWeatherTransition, transitionMs, false);
    }

    protected void ScheduleStormRamp(float transitionSeconds)
    {
        if (m_Config.weatherStormEnabled != 1)
            return;

        m_EffectiveStormRampSeconds = m_Config.weatherStormRampSeconds;
        if (m_EffectiveStormRampSeconds > transitionSeconds)
            m_EffectiveStormRampSeconds = transitionSeconds;

        if (transitionSeconds <= 0.0 || m_EffectiveStormRampSeconds <= 0.0)
        {
            ApplyStormDensity(m_Config.weatherStormDensity, 1.0);
            return;
        }

        float rampDelaySeconds = transitionSeconds - m_EffectiveStormRampSeconds;
        if (rampDelaySeconds <= 0.0)
        {
            BeginStormRamp();
            return;
        }

        m_StormRampStartScheduled = true;
        int rampDelayMs = Math.Round(rampDelaySeconds * 1000.0);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginStormRamp, rampDelayMs, false);
        Print(EVENT_LOG_PREFIX + " Storm ramp scheduled after=" + rampDelaySeconds.ToString() + " seconds duration=" + m_EffectiveStormRampSeconds.ToString() + " step=" + (STORM_RAMP_STEP_MS / 1000).ToString() + " seconds");
    }

    protected void BeginStormRamp()
    {
        m_StormRampStartScheduled = false;
        if (m_Stopped || !m_WeatherControlActive || m_EventStarted)
            return;

        m_StormRampStartTime = GetGame().GetTime();
        ApplyStormDensity(0.0, 0.0);
        m_StormRampUpdateScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateStormRamp, STORM_RAMP_STEP_MS, true);
    }

    protected void UpdateStormRamp()
    {
        if (m_Stopped || !m_WeatherControlActive || m_EventStarted || m_EffectiveStormRampSeconds <= 0.0)
        {
            StopStormRampCallbacks();
            return;
        }

        float elapsedSeconds = (GetGame().GetTime() - m_StormRampStartTime) / 1000.0;
        float progress = elapsedSeconds / m_EffectiveStormRampSeconds;
        if (progress > 1.0)
            progress = 1.0;

        ApplyStormDensity(m_Config.weatherStormDensity * progress, progress);
        if (progress >= 1.0)
            StopStormRampCallbacks();
    }

    protected void ApplyStormDensity(float density, float progress)
    {
        Weather weather = GetGame().GetWeather();
        if (!weather)
            return;

        weather.SetStorm(density, m_Config.weatherStormThreshold, m_Config.weatherStormTimeoutSeconds);
        Print(EVENT_LOG_PREFIX + " Storm ramp progress=" + progress.ToString() + " density=" + density.ToString());
    }

    protected void StopStormRampCallbacks()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginStormRamp);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateStormRamp);
        m_StormRampStartScheduled = false;
        m_StormRampUpdateScheduled = false;
    }

    protected void CompleteWeatherTransition()
    {
        m_WeatherCompletionScheduled = false;
        if (m_Stopped)
            return;

        StopStormRampCallbacks();
        if (m_Config.weatherStormEnabled == 1)
            ApplyStormDensity(m_Config.weatherStormDensity, 1.0);
        else
            ApplyStormDensity(0.0, 0.0);

        Print(EVENT_LOG_PREFIX + " Weather transition completed; starting all enabled migration groups");
        StartAllScenarios();
        ReleaseWeatherControl();
    }

    protected void ReleaseWeatherControl()
    {
        if (!m_WeatherControlActive)
            return;

        Weather weather = GetGame().GetWeather();
        if (weather)
        {
            weather.MissionWeather(m_PreviousMissionWeather);
            Print(EVENT_LOG_PREFIX + " Weather control released; previous MissionWeather state restored=" + m_PreviousMissionWeather.ToString());
        }

        m_WeatherControlActive = false;
    }

    protected void StartAllScenarios()
    {
        m_StartGroupsScheduled = false;
        if (m_Stopped)
            return;

        if (m_EventStarted)
        {
            Print(EVENT_LOG_PREFIX + " Duplicate group start ignored");
            return;
        }

        m_EventStarted = true;
        for (int scenarioIndex = 0; scenarioIndex < m_Scenarios.Count(); scenarioIndex++)
            StartScenario(m_Scenarios.Get(scenarioIndex));

        Print(EVENT_LOG_PREFIX + " All enabled scenarios started; scenarios=" + m_Scenarios.Count().ToString() + " totalSpawned=" + m_Units.Count().ToString());
        if (m_Units.Count() == 0)
            return;

        m_RouteUpdateScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateRoutes, UPDATE_INTERVAL_MS, true);

        m_LogScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LogUnitStates, LOG_CHECK_INTERVAL_MS, true);
        LogUnitStates();
    }

    protected void StartScenario(S77MigrateScenarioConfig config)
    {
        vector spawnCenter = config.GetSpawnPosition();
        vector targetCenter = config.GetTargetPosition();
        float spawnFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        float targetFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        string scenarioPrefix = MIGRATION_LOG_PREFIX + " scenario=" + config.scenarioId;

        string formationLog = scenarioPrefix;
        formationLog = formationLog + " Formation spawnSpacing=" + config.spawnFormationSpacing.ToString();
        formationLog = formationLog + " spawnJitter=" + config.spawnFormationJitter.ToString();
        formationLog = formationLog + " spawnRotation=" + spawnFormationRotation.ToString();
        formationLog = formationLog + " targetSpacing=" + config.targetFormationSpacing.ToString();
        formationLog = formationLog + " targetJitter=" + config.targetFormationJitter.ToString();
        formationLog = formationLog + " targetRotation=" + targetFormationRotation.ToString();
        Print(formationLog);

        int beforeCount = m_Units.Count();
        for (int i = 0; i < config.infectedCount; i++)
        {
            string className = config.infectedTypes.Get(i % config.infectedTypes.Count());
            vector spawnOffset = GetFormationOffset(i, config.infectedCount, config.spawnFormationSpacing, config.spawnFormationJitter, spawnFormationRotation);
            vector targetOffset = GetFormationOffset(i, config.infectedCount, config.targetFormationSpacing, config.targetFormationJitter, targetFormationRotation);
            SpawnMigrationInfected(config, i, className, spawnCenter + spawnOffset, targetCenter + targetOffset);
        }

        int spawnedCount = m_Units.Count() - beforeCount;
        Print(scenarioPrefix + " started; spawned=" + spawnedCount.ToString() + "/" + config.infectedCount.ToString());
    }

    protected void SpawnMigrationInfected(S77MigrateScenarioConfig config, int index, string className, vector spawnPosition, vector migrationTarget)
    {
        string scenarioPrefix = MIGRATION_LOG_PREFIX + " scenario=" + config.scenarioId;
        if (!GetGame().ConfigIsExisting("CfgVehicles " + className) || !GetGame().IsKindOf(className, "DayZInfected"))
        {
            Print(scenarioPrefix + " ERROR: invalid infected class: " + className);
            return;
        }

        int createFlags = ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS;
        DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx(className, spawnPosition, createFlags));

        if (!infected)
        {
            Print(scenarioPrefix + " ERROR: CreateObjectEx failed for class=" + className + " position=" + spawnPosition.ToString());
            return;
        }

        infected.SetOrientation(Vector(27.359158, 0.0, 0.0));

        string infectedId = "INF_" + (index + 1).ToString();
        int logIntervalMs = Math.Round(config.logIntervalSeconds * 1000.0);
        S77MigrateUnitState state = new S77MigrateUnitState(infected, infectedId, config.scenarioId, migrationTarget, logIntervalMs);
        m_Units.Insert(state);

        BuildPath(state);
        Print(scenarioPrefix + " Spawned id=" + infectedId + " class=" + className + " position=" + infected.GetPosition().ToString() + " target=" + migrationTarget.ToString());
    }

    protected bool BuildPath(S77MigrateUnitState state)
    {
        if (!state || !state.m_Infected || !state.m_Infected.IsAlive())
            return false;

        state.m_Waypoints = new TVectorArray();
        state.m_WaypointIndex = 0;
        state.m_NextPathRetryTime = GetGame().GetTime() + PATH_RETRY_INTERVAL_MS;

        AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();
        if (!aiWorld)
        {
            Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " ERROR: AIWorld is unavailable for id=" + state.m_InfectedId);
            return false;
        }

        vector sampledStart;
        vector sampledTarget;
        vector currentPosition = state.m_Infected.GetPosition();

        if (!aiWorld.SampleNavmeshPosition(currentPosition, 8.0, m_PathFilter, sampledStart))
        {
            Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " PATH ERROR: no navmesh near current position for id=" + state.m_InfectedId + " position=" + currentPosition.ToString());
            return false;
        }

        if (!aiWorld.SampleNavmeshPosition(state.m_MigrationTarget, 15.0, m_PathFilter, sampledTarget))
        {
            Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " PATH ERROR: no navmesh near target for id=" + state.m_InfectedId + " target=" + state.m_MigrationTarget.ToString());
            return false;
        }

        TVectorArray newWaypoints = new TVectorArray();
        if (!aiWorld.FindPath(sampledStart, sampledTarget, m_PathFilter, newWaypoints) || newWaypoints.Count() < 2)
        {
            Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " PATH ERROR: FindPath failed for id=" + state.m_InfectedId + " from=" + sampledStart.ToString() + " to=" + sampledTarget.ToString());
            return false;
        }

        state.m_Waypoints = newWaypoints;
        state.m_WaypointIndex = 1;
        state.m_NextPathRetryTime = 0;
        Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " Path built for id=" + state.m_InfectedId + " waypoints=" + newWaypoints.Count().ToString());
        return true;
    }

    protected void UpdateRoutes()
    {
        int now = GetGame().GetTime();

        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (!state || !state.m_Infected || !state.m_Infected.IsAlive())
                continue;

            if (state.m_Mode == "ARRIVED")
                continue;

            DayZInfectedInputController controller = state.m_Infected.GetInputController();
            if (!controller)
                continue;

            EntityAI vanillaTarget = controller.GetTargetEntity();
            int mindState = controller.GetMindState();
            bool vanillaBusyNow = vanillaTarget != null || mindState != DayZInfectedConstants.MINDSTATE_CALM;

            if (vanillaBusyNow)
            {
                ReleaseRouteControl(controller);
                state.m_Mode = "AGGRO";
                state.m_VanillaBusy = true;
                state.m_ResumeAfterTime = 0;
                continue;
            }

            if (state.m_VanillaBusy)
            {
                ReleaseRouteControl(controller);

                if (state.m_ResumeAfterTime == 0)
                    state.m_ResumeAfterTime = now + AGGRO_COOLDOWN_MS;

                if (now < state.m_ResumeAfterTime)
                    continue;

                state.m_VanillaBusy = false;
                state.m_ResumeAfterTime = 0;
                state.m_Mode = "MIGRATION";

                if (!BuildPath(state))
                    continue;
            }

            if (!state.m_Waypoints || state.m_Waypoints.Count() < 2)
            {
                ReleaseRouteControl(controller);

                if (state.m_NextPathRetryTime == 0 || now >= state.m_NextPathRetryTime)
                    BuildPath(state);

                continue;
            }

            FollowPath(state, controller);
        }
    }

    protected void FollowPath(S77MigrateUnitState state, DayZInfectedInputController controller)
    {
        vector position = state.m_Infected.GetPosition();

        while (state.m_WaypointIndex < state.m_Waypoints.Count() && HorizontalDistance(position, state.m_Waypoints.Get(state.m_WaypointIndex)) <= WAYPOINT_TOLERANCE)
            state.m_WaypointIndex++;

        if (state.m_WaypointIndex >= state.m_Waypoints.Count() || HorizontalDistance(position, state.m_MigrationTarget) <= ARRIVAL_TOLERANCE)
        {
            ReleaseRouteControl(controller);
            state.m_Mode = "ARRIVED";
            Print(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " ARRIVED id=" + state.m_InfectedId + " class=" + state.m_ClassName + " position=" + position.ToString());
            return;
        }

        vector waypoint = state.m_Waypoints.Get(state.m_WaypointIndex);
        vector direction = vector.Direction(position, waypoint);
        direction[1] = 0.0;

        if (direction.Length() <= 0.01)
            return;

        float headingRadians = direction.VectorToAngles()[0] * Math.DEG2RAD;
        controller.OverrideHeading(true, headingRadians);
        controller.OverrideMovementSpeed(true, MIGRATION_SPEED);
        state.m_Mode = "MIGRATION";
    }

    protected void LogUnitStates()
    {
        int now = GetGame().GetTime();
        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (!state || !state.m_Infected)
            {
                Print(EVENT_LOG_PREFIX + " ERROR: unit state or infected entity is missing");
                continue;
            }

            if (!state.m_Infected.IsAlive())
                continue;

            if (state.m_NextLogTime > now)
                continue;

            state.m_NextLogTime = now + state.m_LogIntervalMs;

            DayZInfectedInputController controller = state.m_Infected.GetInputController();
            EntityAI target;
            int mindState = -1;

            if (controller)
            {
                target = controller.GetTargetEntity();
                mindState = controller.GetMindState();
            }

            string targetPresent = "NO";
            if (target)
                targetPresent = "YES";

            string waypointStatus = state.m_WaypointIndex.ToString() + "/0";
            if (state.m_Waypoints)
                waypointStatus = state.m_WaypointIndex.ToString() + "/" + state.m_Waypoints.Count().ToString();

            vector position = state.m_Infected.GetPosition();
            float distanceToTarget = vector.Distance(position, state.m_MigrationTarget);

            string logLine = MIGRATION_LOG_PREFIX;
            logLine = logLine + " scenario=" + state.m_ScenarioId;
            logLine = logLine + " id=" + state.m_InfectedId;
            logLine = logLine + " class=" + state.m_ClassName;
            logLine = logLine + " position=" + position.ToString();
            logLine = logLine + " waypoint=" + waypointStatus;
            logLine = logLine + " distance=" + distanceToTarget.ToString();
            logLine = logLine + " target=" + targetPresent;
            logLine = logLine + " mind=" + MindStateToString(mindState);
            logLine = logLine + " mode=" + state.m_Mode;

            Print(logLine);
        }
    }

    protected void Stop()
    {
        m_Stopped = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartAllScenarios);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginWeatherTransition);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CompleteWeatherTransition);
        StopStormRampCallbacks();

        m_StartGroupsScheduled = false;
        m_WeatherStartScheduled = false;
        m_WeatherCompletionScheduled = false;

        ReleaseWeatherControl();

        if (m_RouteUpdateScheduled)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateRoutes);

        if (m_LogScheduled)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LogUnitStates);

        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (state && state.m_Infected && state.m_Infected.IsAlive())
                ReleaseRouteControl(state.m_Infected.GetInputController());
        }
    }

    protected void ReleaseRouteControl(DayZInfectedInputController controller)
    {
        if (!controller)
            return;

        controller.OverrideMovementSpeed(false, 0.0);
        controller.OverrideHeading(false, 0.0);
    }

    protected vector GetFormationOffset(int index, int infectedCount, float spacing, float jitter, float rotationDegrees)
    {
        if (infectedCount < 1)
            return Vector(0.0, 0.0, 0.0);

        int columns = Math.Ceil(Math.Sqrt(infectedCount));
        int rows = Math.Ceil((infectedCount * 1.0) / columns);
        int row = index / columns;
        int column = index % columns;

        float columnIndexSum = 0.0;
        float rowIndexSum = 0.0;

        for (int rowIndex = 0; rowIndex < rows; rowIndex++)
        {
            int rowCount = infectedCount - rowIndex * columns;
            if (rowCount > columns)
                rowCount = columns;

            columnIndexSum = columnIndexSum + rowCount * (rowCount - 1) * 0.5;
            rowIndexSum = rowIndexSum + rowIndex * rowCount;
        }

        float centerColumn = columnIndexSum / infectedCount;
        float centerRow = rowIndexSum / infectedCount;
        float offsetX = (column - centerColumn) * spacing;
        float offsetZ = (row - centerRow) * spacing;
        float rotationRadians = rotationDegrees * Math.DEG2RAD;
        float rotatedX = offsetX * Math.Cos(rotationRadians) - offsetZ * Math.Sin(rotationRadians);
        float rotatedZ = offsetX * Math.Sin(rotationRadians) + offsetZ * Math.Cos(rotationRadians);

        rotatedX = rotatedX + Math.RandomFloatInclusive(-jitter, jitter);
        rotatedZ = rotatedZ + Math.RandomFloatInclusive(-jitter, jitter);

        return Vector(rotatedX, 0.0, rotatedZ);
    }

    protected float HorizontalDistance(vector from, vector to)
    {
        from[1] = 0.0;
        to[1] = 0.0;
        return vector.Distance(from, to);
    }

    protected string MindStateToString(int mindState)
    {
        switch (mindState)
        {
            case DayZInfectedConstants.MINDSTATE_CALM:
                return "CALM";
            case DayZInfectedConstants.MINDSTATE_DISTURBED:
                return "DISTURBED";
            case DayZInfectedConstants.MINDSTATE_ALERTED:
                return "ALERTED";
            case DayZInfectedConstants.MINDSTATE_CHASE:
                return "CHASE";
            case DayZInfectedConstants.MINDSTATE_FIGHT:
                return "FIGHT";
        }

        return "UNKNOWN(" + mindState.ToString() + ")";
    }
}
