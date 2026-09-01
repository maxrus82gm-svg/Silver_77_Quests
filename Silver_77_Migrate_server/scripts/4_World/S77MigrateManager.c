class S77MigrateUnitState
{
    DayZInfected m_Infected;
    string m_InfectedId;
    string m_ScenarioId;
    string m_RuntimeGroupId;
    string m_ClassName;
    vector m_MigrationTarget;
    vector m_FinalTargetCenter;
    vector m_RouteOffset;
    ref TVectorArray m_RoutePoints;
    int m_RoutePointIndex;
    float m_RoutePointReachRadius;
    ref TVectorArray m_Waypoints;
    int m_WaypointIndex;
    string m_Mode;
    bool m_VanillaBusy;
    bool m_Released;
    int m_ResumeAfterTime;
    int m_NextPathRetryTime;
    int m_LogIntervalMs;
    int m_NextLogTime;
    int m_StuckRecoveryEnabled;
    float m_StuckDetectionSeconds;
    float m_StuckMinMovementMeters;
    int m_RouteProgressWatchdogEnabled;
    float m_RouteProgressCheckSeconds;
    float m_RouteProgressMinProgressMeters;
    float m_RouteProgressMaxBacktrackMeters;
    int m_RouteProgressBadCheckLimit;
    float m_StuckRecoveryFreeSeconds;
    float m_StuckRecoveryStatusCheckSeconds;
    float m_StuckStimulusForwardDistance;
    float m_StuckStimulusLifetimeSeconds;
    float m_StuckStimulusStrengthMultiplier;
    int m_FinalHoldEnabled;
    float m_FinalHoldRadius;
    float m_FinalHoldReturnRadius;
    float m_FinalHoldCheckSeconds;
    bool m_StuckSampleValid;
    vector m_StuckSamplePosition;
    float m_StuckSampleDistance;
    int m_StuckSampleTime;
    bool m_RouteProgressSampleValid;
    float m_RouteProgressPreviousDistance;
    int m_NextRouteProgressCheckTime;
    int m_RouteProgressBadCheckCount;
    int m_RouteProgressStaggerMs;
    string m_RecoveryResumeMode;
    int m_RecoveryFreeUntilTime;
    int m_NextRecoveryStatusCheckTime;
    int m_RecoveryCalmAfterTime;
    int m_NextHoldCheckTime;
    int m_HoldCalmAfterTime;
    bool m_DeathLogged;

    void S77MigrateUnitState(DayZInfected infected, string infectedId, string scenarioId, string runtimeGroupId, vector migrationTarget, vector finalTargetCenter, vector routeOffset, TVectorArray routePoints, float routePointReachRadius, S77MigrateScenarioConfig config, int logIntervalMs)
    {
        m_Infected = infected;
        m_InfectedId = infectedId;
        m_ScenarioId = scenarioId;
        m_RuntimeGroupId = runtimeGroupId;
        m_ClassName = infected.GetType();
        m_MigrationTarget = migrationTarget;
        m_FinalTargetCenter = finalTargetCenter;
        m_RouteOffset = routeOffset;
        m_RoutePoints = routePoints;
        m_RoutePointIndex = 0;
        m_RoutePointReachRadius = routePointReachRadius;
        m_Waypoints = new TVectorArray();
        m_WaypointIndex = 0;
        m_Mode = "MIGRATION";
        m_VanillaBusy = false;
        m_Released = false;
        m_ResumeAfterTime = 0;
        m_NextPathRetryTime = 0;
        m_LogIntervalMs = logIntervalMs;
        m_NextLogTime = 0;
        m_StuckRecoveryEnabled = config.stuckRecoveryEnabled;
        m_StuckDetectionSeconds = config.stuckDetectionSeconds;
        m_StuckMinMovementMeters = config.stuckMinMovementMeters;
        m_RouteProgressWatchdogEnabled = config.routeProgressWatchdogEnabled;
        m_RouteProgressCheckSeconds = config.routeProgressCheckSeconds;
        m_RouteProgressMinProgressMeters = config.routeProgressMinProgressMeters;
        m_RouteProgressMaxBacktrackMeters = config.routeProgressMaxBacktrackMeters;
        m_RouteProgressBadCheckLimit = config.routeProgressBadCheckLimit;
        m_StuckRecoveryFreeSeconds = config.stuckRecoveryFreeSeconds;
        m_StuckRecoveryStatusCheckSeconds = config.stuckRecoveryStatusCheckSeconds;
        m_StuckStimulusForwardDistance = config.stuckStimulusForwardDistance;
        m_StuckStimulusLifetimeSeconds = config.stuckStimulusLifetimeSeconds;
        m_StuckStimulusStrengthMultiplier = config.stuckStimulusStrengthMultiplier;
        m_FinalHoldEnabled = config.finalHoldEnabled;
        m_FinalHoldRadius = config.finalHoldRadius;
        m_FinalHoldReturnRadius = config.finalHoldReturnRadius;
        m_FinalHoldCheckSeconds = config.finalHoldCheckSeconds;
        m_StuckSampleValid = false;
        m_StuckSamplePosition = Vector(0.0, 0.0, 0.0);
        m_StuckSampleDistance = 0.0;
        m_StuckSampleTime = 0;
        m_RouteProgressSampleValid = false;
        m_RouteProgressPreviousDistance = 0.0;
        m_NextRouteProgressCheckTime = 0;
        m_RouteProgressBadCheckCount = 0;
        m_RouteProgressStaggerMs = Math.Round(Math.RandomFloatInclusive(0.0, m_RouteProgressCheckSeconds) * 1000.0);
        m_RecoveryResumeMode = "MIGRATION";
        m_RecoveryFreeUntilTime = 0;
        m_NextRecoveryStatusCheckTime = 0;
        m_RecoveryCalmAfterTime = 0;
        m_NextHoldCheckTime = 0;
        m_HoldCalmAfterTime = 0;
        m_DeathLogged = false;
    }
}

class S77MigrateGroupState
{
    string m_RuntimeGroupId;
    string m_ScenarioId;
    ref array<ref S77MigrateUnitState> m_Members;
    ref TVectorArray m_RoutePoints;
    ref array<float> m_RouteActivationRadii;
    ref array<bool> m_RouteActivationArmed;
    int m_RouteActivationEnabled;
    float m_RouteActivationTriggerPercent;
    float m_RouteStimulusLifetimeSeconds;
    float m_RouteStimulusStrengthMultiplier;
    vector m_FinalTargetCenter;
    int m_FinalActivationEnabled;
    float m_FinalActivationTriggerPercent;
    float m_FinalActivationDistance;
    float m_FinalStimulusLifetimeSeconds;
    float m_FinalStimulusStrengthMultiplier;
    bool m_FinalActivationArmed;
    bool m_StimulusFailureLogged;

    void S77MigrateGroupState(string runtimeGroupId, S77MigrateScenarioConfig config, TVectorArray routePoints)
    {
        m_RuntimeGroupId = runtimeGroupId;
        m_ScenarioId = config.scenarioId;
        m_Members = new array<ref S77MigrateUnitState>();
        m_RoutePoints = routePoints;
        m_RouteActivationRadii = new array<float>();
        m_RouteActivationArmed = new array<bool>();

        for (int i = 0; i < routePoints.Count(); i++)
        {
            m_RouteActivationRadii.Insert(config.GetRouteActivationRadius(i));
            m_RouteActivationArmed.Insert(true);
        }

        m_RouteActivationEnabled = config.routeActivationEnabled;
        m_RouteActivationTriggerPercent = config.routeActivationTriggerPercent;
        m_RouteStimulusLifetimeSeconds = config.routeStimulusLifetimeSeconds;
        m_RouteStimulusStrengthMultiplier = config.routeStimulusStrengthMultiplier;
        m_FinalTargetCenter = config.GetTargetPosition();
        m_FinalActivationEnabled = config.finalActivationEnabled;
        m_FinalActivationTriggerPercent = config.finalActivationTriggerPercent;
        m_FinalActivationDistance = config.finalActivationDistance;
        m_FinalStimulusLifetimeSeconds = config.finalStimulusLifetimeSeconds;
        m_FinalStimulusStrengthMultiplier = config.finalStimulusStrengthMultiplier;
        m_FinalActivationArmed = true;
        m_StimulusFailureLogged = false;
    }
}

class S77MigrateManager
{
    protected static ref S77MigrateManager s_Instance;

    protected const string EVENT_LOG_PREFIX = "[S77Migrate][EVENT]";
    protected const string MIGRATION_LOG_PREFIX = "[S77Migrate]";
    protected const string AI_STIMULUS_NOISE_PATH = "CfgVehicles SurvivorBase NoiseShout";
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
    protected ref array<ref S77MigrateGroupState> m_Groups;
    protected ref array<ref S77MigrateUnitState> m_Units;
    protected ref PGFilter m_PathFilter;
    protected ref NoiseParams m_StimulusParams;
    protected NoiseSystem m_NoiseSystem;
    protected bool m_StimulusReady;
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
    protected int m_NextRuntimeGroupSerial;
    protected float m_EffectiveStormRampSeconds;
    protected FileHandle m_ProfileLogFile;
    protected bool m_ProfileLogReady;

    void S77MigrateManager()
    {
        m_Scenarios = new array<ref S77MigrateScenarioConfig>();
        m_Groups = new array<ref S77MigrateGroupState>();
        m_Units = new array<ref S77MigrateUnitState>();
        m_PathFilter = new PGFilter();
        int includeFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE;
        int excludeFlags = PGPolyFlags.DISABLED | PGPolyFlags.SWIM | PGPolyFlags.SWIM_SEA | PGPolyFlags.JUMP | PGPolyFlags.CLIMB | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH;
        m_PathFilter.SetFlags(includeFlags, excludeFlags, PGPolyFlags.NONE);
        m_PathFilter.SetCost(PGAreaType.BUILDING, 5.0);
    }

    protected string BuildLogLine(string message)
    {
        int runtimeTimeMs = 0;
        if (GetGame())
            runtimeTimeMs = GetGame().GetTime();

        return "[timeMs=" + runtimeTimeMs.ToString() + "] " + message;
    }

    protected void WriteProfileLog(string line)
    {
        if (!m_ProfileLogReady || m_ProfileLogFile == 0)
            return;

        FPrintln(m_ProfileLogFile, line);
    }

    protected void LogInfo(string message)
    {
        if (!m_Config || m_Config.loggingEnabled != 1)
            return;

        string line = BuildLogLine(message);
        Print(line);
        WriteProfileLog(line);
    }

    protected void LogStuckDebug(string message)
    {
        if (!m_Config || m_Config.loggingEnabled != 1 || m_Config.stuckDebugLoggingEnabled != 1)
            return;

        string line = BuildLogLine(message);
        Print(line);
        WriteProfileLog(line);
    }

    protected void LogError(string message)
    {
        string line = BuildLogLine(message);
        Print(line);
        WriteProfileLog(line);
    }

    protected void OpenProfileLog()
    {
        if (m_ProfileLogReady)
            return;

        m_ProfileLogFile = OpenFile(S77_MIGRATE_LOG_FILE, FileMode.APPEND);
        if (m_ProfileLogFile == 0)
        {
            m_ProfileLogReady = false;
            LogError(EVENT_LOG_PREFIX + " ERROR: profile log could not be opened path=" + S77_MIGRATE_LOG_FILE + "; RPT logging remains active");
            return;
        }

        m_ProfileLogReady = true;
    }

    protected void CloseProfileLog()
    {
        if (!m_ProfileLogReady || m_ProfileLogFile == 0)
            return;

        CloseFile(m_ProfileLogFile);
        m_ProfileLogReady = false;
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
            LogInfo(EVENT_LOG_PREFIX + " Duplicate initialization ignored");
            return;
        }

        m_Initialized = true;

        m_Config = S77MigrateConfigLoader.Load();
        if (!m_Config)
        {
            LogError(EVENT_LOG_PREFIX + " ERROR: unified migration config could not be loaded");
            return;
        }

        OpenProfileLog();
        LogInfo(EVENT_LOG_PREFIX + " SESSION_START profileLog=" + S77_MIGRATE_LOG_FILE + " loggingEnabled=" + m_Config.loggingEnabled.ToString() + " stuckDebugLoggingEnabled=" + m_Config.stuckDebugLoggingEnabled.ToString());

        for (int scenarioIndex = 0; scenarioIndex < m_Config.scenarios.Count(); scenarioIndex++)
        {
            S77MigrateScenarioConfig scenario = m_Config.scenarios.Get(scenarioIndex);
            if (!scenario)
            {
                LogError(EVENT_LOG_PREFIX + " ERROR: null scenario skipped at index=" + scenarioIndex.ToString());
                continue;
            }

            if (!scenario.IsValid())
            {
                LogError(EVENT_LOG_PREFIX + " ERROR: invalid scenario skipped at index=" + scenarioIndex.ToString() + " scenarioId=" + scenario.scenarioId);
                continue;
            }

            if (scenario.enabled == 1)
                m_Scenarios.Insert(scenario);
            else
                LogInfo(MIGRATION_LOG_PREFIX + " scenario=" + scenario.scenarioId + " disabled");
        }

        if (m_Config.enabled != 1)
        {
            LogInfo(EVENT_LOG_PREFIX + " Event disabled. Set enabled=1 in " + S77_MIGRATE_CONFIG + " and restart the server.");
            return;
        }

        if (m_Scenarios.Count() == 0)
        {
            LogInfo(EVENT_LOG_PREFIX + " No enabled migration scenarios; event start aborted");
            return;
        }

        if (NeedsAIStimulus())
            PrepareAIStimulus();

        int delayMs = Math.Round(m_Config.eventDelaySeconds * 1000.0);
        int weatherRequestingScenarios = CountWeatherRequestingScenarios();
        bool shouldStartWeatherTransition = false;
        string weatherDecision = "SKIP_GLOBAL_DISABLED";

        if (m_Config.weatherEnabled == 1)
        {
            weatherDecision = "SKIP_NO_SCENARIO_REQUEST";
            if (weatherRequestingScenarios > 0)
            {
                shouldStartWeatherTransition = true;
                weatherDecision = "TRANSITION";
            }
        }

        LogInfo(EVENT_LOG_PREFIX + " WEATHER_DECISION globalEnabled=" + m_Config.weatherEnabled.ToString() + " requestingScenarios=" + weatherRequestingScenarios.ToString() + " enabledScenarios=" + m_Scenarios.Count().ToString() + " result=" + weatherDecision);

        if (shouldStartWeatherTransition)
        {
            m_WeatherStartScheduled = true;
            LogInfo(EVENT_LOG_PREFIX + " Global weather scheduled in " + m_Config.eventDelaySeconds.ToString() + " seconds; transition=" + m_Config.weatherTransitionSeconds.ToString() + " seconds; scenarios=" + m_Scenarios.Count().ToString());
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginWeatherTransition, delayMs, false);
        }
        else
        {
            m_StartGroupsScheduled = true;
            LogInfo(EVENT_LOG_PREFIX + " Weather transition skipped; all enabled groups scheduled in " + m_Config.eventDelaySeconds.ToString() + " seconds");
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartAllScenarios, delayMs, false);
        }
    }

    protected int CountWeatherRequestingScenarios()
    {
        int requestingScenarios = 0;
        for (int scenarioIndex = 0; scenarioIndex < m_Scenarios.Count(); scenarioIndex++)
        {
            S77MigrateScenarioConfig scenario = m_Scenarios.Get(scenarioIndex);
            if (scenario && scenario.weatherChangeEnabled == 1)
                requestingScenarios++;
        }

        return requestingScenarios;
    }

    protected void BeginWeatherTransition()
    {
        m_WeatherStartScheduled = false;
        if (m_Stopped || m_EventStarted)
            return;

        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            LogError(EVENT_LOG_PREFIX + " ERROR: Weather API is unavailable; event start aborted");
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
        LogInfo(weatherLog);

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
        LogInfo(EVENT_LOG_PREFIX + " Storm ramp scheduled after=" + rampDelaySeconds.ToString() + " seconds duration=" + m_EffectiveStormRampSeconds.ToString() + " step=" + (STORM_RAMP_STEP_MS / 1000).ToString() + " seconds");
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
        LogInfo(EVENT_LOG_PREFIX + " Storm ramp progress=" + progress.ToString() + " density=" + density.ToString());
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

        LogInfo(EVENT_LOG_PREFIX + " Weather transition completed; starting all enabled migration groups");
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
            LogInfo(EVENT_LOG_PREFIX + " Weather control released; previous MissionWeather state restored=" + m_PreviousMissionWeather.ToString());
        }

        m_WeatherControlActive = false;
    }

    protected bool NeedsAIStimulus()
    {
        for (int i = 0; i < m_Scenarios.Count(); i++)
        {
            S77MigrateScenarioConfig scenario = m_Scenarios.Get(i);
            if (scenario && (scenario.finalActivationEnabled == 1 || scenario.stuckRecoveryEnabled == 1 || scenario.routeProgressWatchdogEnabled == 1 || (scenario.routeActivationEnabled == 1 && scenario.routePoints.Count() > 0)))
                return true;
        }

        return false;
    }

    protected void PrepareAIStimulus()
    {
        m_StimulusReady = false;

        if (!GetGame().ConfigIsExisting(AI_STIMULUS_NOISE_PATH))
        {
            LogError(EVENT_LOG_PREFIX + " ERROR: AI stimulus preset is unavailable: " + AI_STIMULUS_NOISE_PATH + "; migration and configured arrival behavior remain active without AI stimulus");
            return;
        }

        m_NoiseSystem = GetGame().GetNoiseSystem();
        if (!m_NoiseSystem)
        {
            LogError(EVENT_LOG_PREFIX + " ERROR: NoiseSystem is unavailable; infected will continue route and configured arrival behavior without AI stimulus");
            return;
        }

        m_StimulusParams = new NoiseParams();
        m_StimulusParams.LoadFromPath(AI_STIMULUS_NOISE_PATH);
        m_StimulusReady = true;
        LogInfo(EVENT_LOG_PREFIX + " AI stimulus ready preset=" + AI_STIMULUS_NOISE_PATH);
    }

    protected void StartAllScenarios()
    {
        m_StartGroupsScheduled = false;
        if (m_Stopped)
            return;

        if (m_EventStarted)
        {
            LogInfo(EVENT_LOG_PREFIX + " Duplicate group start ignored");
            return;
        }

        m_EventStarted = true;
        for (int scenarioIndex = 0; scenarioIndex < m_Scenarios.Count(); scenarioIndex++)
            StartScenario(m_Scenarios.Get(scenarioIndex));

        LogInfo(EVENT_LOG_PREFIX + " All enabled scenarios started; scenarios=" + m_Scenarios.Count().ToString() + " totalSpawned=" + m_Units.Count().ToString());
        if (m_Units.Count() == 0)
            return;

        m_RouteUpdateScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateRoutes, UPDATE_INTERVAL_MS, true);

        if (m_Config.loggingEnabled == 1)
        {
            m_LogScheduled = true;
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LogUnitStates, LOG_CHECK_INTERVAL_MS, true);
            LogUnitStates();
        }
    }

    protected void StartScenario(S77MigrateScenarioConfig config)
    {
        vector spawnCenter = config.GetSpawnPosition();
        vector targetCenter = config.GetTargetPosition();
        TVectorArray routePoints = config.GetRoutePoints();
        m_NextRuntimeGroupSerial++;
        string runtimeGroupId = config.scenarioId + "_RUN_" + m_NextRuntimeGroupSerial.ToString();
        S77MigrateGroupState groupState = new S77MigrateGroupState(runtimeGroupId, config, routePoints);
        m_Groups.Insert(groupState);
        float spawnFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        float targetFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        string scenarioPrefix = MIGRATION_LOG_PREFIX + " scenario=" + config.scenarioId + " group=" + runtimeGroupId;

        string formationLog = scenarioPrefix;
        formationLog = formationLog + " Formation spawnSpacing=" + config.spawnFormationSpacing.ToString();
        formationLog = formationLog + " spawnJitter=" + config.spawnFormationJitter.ToString();
        formationLog = formationLog + " spawnRotation=" + spawnFormationRotation.ToString();
        formationLog = formationLog + " targetSpacing=" + config.targetFormationSpacing.ToString();
        formationLog = formationLog + " targetJitter=" + config.targetFormationJitter.ToString();
        formationLog = formationLog + " targetRotation=" + targetFormationRotation.ToString();
        formationLog = formationLog + " routePoints=" + routePoints.Count().ToString();
        LogInfo(formationLog);

        int beforeCount = m_Units.Count();
        for (int i = 0; i < config.infectedCount; i++)
        {
            string className = config.infectedTypes.Get(i % config.infectedTypes.Count());
            vector spawnOffset = GetFormationOffset(i, config.infectedCount, config.spawnFormationSpacing, config.spawnFormationJitter, spawnFormationRotation);
            vector routeOffset = GetFormationOffset(i, config.infectedCount, config.targetFormationSpacing, config.targetFormationJitter, targetFormationRotation);
            SpawnMigrationInfected(config, groupState, i, className, spawnCenter + spawnOffset, targetCenter, routeOffset, routePoints);
        }

        int spawnedCount = m_Units.Count() - beforeCount;
        LogInfo(scenarioPrefix + " started; spawned=" + spawnedCount.ToString() + "/" + config.infectedCount.ToString());
    }

    protected void SpawnMigrationInfected(S77MigrateScenarioConfig config, S77MigrateGroupState groupState, int index, string className, vector spawnPosition, vector finalTargetCenter, vector routeOffset, TVectorArray routePoints)
    {
        string scenarioPrefix = MIGRATION_LOG_PREFIX + " scenario=" + config.scenarioId + " group=" + groupState.m_RuntimeGroupId;
        if (!GetGame().ConfigIsExisting("CfgVehicles " + className) || !GetGame().IsKindOf(className, "DayZInfected"))
        {
            LogError(scenarioPrefix + " ERROR: invalid infected class: " + className);
            return;
        }

        int createFlags = ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS;
        DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx(className, spawnPosition, createFlags));

        if (!infected)
        {
            LogError(scenarioPrefix + " ERROR: CreateObjectEx failed for class=" + className + " position=" + spawnPosition.ToString());
            return;
        }

        string infectedId = "INF_" + (index + 1).ToString();
        int logIntervalMs = Math.Round(config.logIntervalSeconds * 1000.0);
        vector migrationTarget = finalTargetCenter + routeOffset;
        S77MigrateUnitState state = new S77MigrateUnitState(infected, infectedId, config.scenarioId, groupState.m_RuntimeGroupId, migrationTarget, finalTargetCenter, routeOffset, routePoints, config.routePointReachRadius, config, logIntervalMs);
        m_Units.Insert(state);
        groupState.m_Members.Insert(state);

        BuildPath(state);
        LogInfo(scenarioPrefix + " SPAWNED id=" + infectedId + " class=" + className + " position=" + infected.GetPosition().ToString() + " target=" + migrationTarget.ToString() + " routePoints=" + routePoints.Count().ToString());
    }

    protected bool BuildPath(S77MigrateUnitState state)
    {
        if (!state || state.m_Released || !state.m_Infected || !state.m_Infected.IsAlive())
            return false;

        state.m_Waypoints = new TVectorArray();
        state.m_WaypointIndex = 0;
        state.m_NextPathRetryTime = GetGame().GetTime() + PATH_RETRY_INTERVAL_MS;

        AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();
        if (!aiWorld)
        {
            LogError(MIGRATION_LOG_PREFIX + " scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " ERROR: AIWorld is unavailable");
            return false;
        }

        vector sampledStart;
        vector sampledTarget;
        vector currentPosition = state.m_Infected.GetPosition();
        vector currentRouteTarget = GetCurrentRouteTarget(state);

        if (!aiWorld.SampleNavmeshPosition(currentPosition, 8.0, m_PathFilter, sampledStart))
        {
            LogError(MIGRATION_LOG_PREFIX + " PATH_ERROR scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " reason=NO_NAVMESH_START position=" + currentPosition.ToString());
            return false;
        }

        if (!aiWorld.SampleNavmeshPosition(currentRouteTarget, 15.0, m_PathFilter, sampledTarget))
        {
            LogError(MIGRATION_LOG_PREFIX + " PATH_ERROR scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " reason=NO_NAVMESH_TARGET target=" + currentRouteTarget.ToString());
            return false;
        }

        TVectorArray newWaypoints = new TVectorArray();
        if (!aiWorld.FindPath(sampledStart, sampledTarget, m_PathFilter, newWaypoints) || newWaypoints.Count() < 2)
        {
            LogError(MIGRATION_LOG_PREFIX + " PATH_ERROR scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " reason=FIND_PATH_FAILED from=" + sampledStart.ToString() + " to=" + sampledTarget.ToString());
            return false;
        }

        state.m_Waypoints = newWaypoints;
        state.m_WaypointIndex = 1;
        state.m_NextPathRetryTime = 0;
        ResetStuckSample(state, "BUILD_PATH");
        ScheduleRouteProgressBaseline(state, GetGame().GetTime());
        LogInfo(MIGRATION_LOG_PREFIX + " BUILD_PATH scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " routePoint=" + state.m_RoutePointIndex.ToString() + "/" + state.m_RoutePoints.Count().ToString() + " waypoints=" + newWaypoints.Count().ToString() + " target=" + currentRouteTarget.ToString());
        return true;
    }

    protected vector GetCurrentRouteTarget(S77MigrateUnitState state)
    {
        if (state.m_RoutePoints && state.m_RoutePointIndex < state.m_RoutePoints.Count())
            return state.m_RoutePoints.Get(state.m_RoutePointIndex) + state.m_RouteOffset;

        return state.m_MigrationTarget;
    }

    protected bool ManualRouteComplete(S77MigrateUnitState state)
    {
        return !state.m_RoutePoints || state.m_RoutePointIndex >= state.m_RoutePoints.Count();
    }

    protected void UpdateRoutes()
    {
        int now = GetGame().GetTime();
        UpdateGroupActivations();

        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (!state || !state.m_Infected)
                continue;

            if (!state.m_Infected.IsAlive())
            {
                if (state.m_StuckSampleValid)
                    ResetStuckSample(state, "DEATH");
                if (!state.m_DeathLogged)
                {
                    state.m_DeathLogged = true;
                    LogInfo(MIGRATION_LOG_PREFIX + " DEATH scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " position=" + state.m_Infected.GetPosition().ToString());
                }
                SuspendRouteProgressWatchdog(state);
                continue;
            }

            if (state.m_Released)
                continue;

            DayZInfectedInputController controller = state.m_Infected.GetInputController();
            if (!controller)
                continue;

            if (state.m_Mode == "STUCK_RECOVERY")
            {
                UpdateStuckRecovery(state, controller, now);
                continue;
            }

            if (state.m_Mode == "HOLD_FREE")
            {
                UpdateHoldFree(state, controller, now);
                continue;
            }

            EntityAI vanillaTarget = controller.GetTargetEntity();
            int mindState = controller.GetMindState();
            bool vanillaBusyNow = vanillaTarget != null || mindState != DayZInfectedConstants.MINDSTATE_CALM;

            if (vanillaBusyNow)
            {
                EnterVanillaBusy(state, controller);
                continue;
            }

            if (state.m_VanillaBusy)
            {
                if (!ResumeAfterVanillaBusy(state, controller, now))
                    continue;
            }

            if (!state.m_Waypoints || state.m_Waypoints.Count() < 2)
            {
                ReleaseRouteControl(controller);

                if (state.m_NextPathRetryTime == 0 || now >= state.m_NextPathRetryTime)
                    BuildPath(state);

                continue;
            }

            if (UpdateStuckDetection(state, controller, now))
                continue;

            if (UpdateRouteProgressWatchdog(state, controller, now))
                continue;

            FollowPath(state, controller);
        }

    }

    protected void EnterVanillaBusy(S77MigrateUnitState state, DayZInfectedInputController controller)
    {
        bool firstHandoff = !state.m_VanillaBusy || state.m_Mode != "AGGRO";
        ReleaseRouteControl(controller);
        state.m_RecoveryResumeMode = GetMovementIntent(state);
        state.m_Mode = "AGGRO";
        state.m_VanillaBusy = true;
        state.m_ResumeAfterTime = 0;
        ResetStuckSample(state, "AGGRO");
        SuspendRouteProgressWatchdog(state);

        if (firstHandoff)
            LogInfo(MIGRATION_LOG_PREFIX + " VANILLA_AGGRO_HANDOFF scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " resumeMode=" + state.m_RecoveryResumeMode + " position=" + state.m_Infected.GetPosition().ToString());
    }

    protected bool ResumeAfterVanillaBusy(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);

        if (state.m_ResumeAfterTime == 0)
            state.m_ResumeAfterTime = now + AGGRO_COOLDOWN_MS;

        if (now < state.m_ResumeAfterTime)
            return false;

        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_Mode = state.m_RecoveryResumeMode;
        if (state.m_Mode != "RETURN_TO_HOLD")
            state.m_Mode = "MIGRATION";

        bool pathBuilt = BuildPath(state);
        if (pathBuilt)
            LogInfo(MIGRATION_LOG_PREFIX + " VANILLA_AGGRO_RESUME scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " position=" + state.m_Infected.GetPosition().ToString() + " target=" + GetCurrentRouteTarget(state).ToString());

        return pathBuilt;
    }

    protected string GetMovementIntent(S77MigrateUnitState state)
    {
        if (state && state.m_Mode == "RETURN_TO_HOLD")
            return "RETURN_TO_HOLD";

        return "MIGRATION";
    }

    protected void UpdateStuckRecovery(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);

        if (now < state.m_RecoveryFreeUntilTime)
            return;

        if (now < state.m_NextRecoveryStatusCheckTime)
            return;

        state.m_NextRecoveryStatusCheckTime = now + Math.Round(state.m_StuckRecoveryStatusCheckSeconds * 1000.0);

        EntityAI vanillaTarget = controller.GetTargetEntity();
        int mindState = controller.GetMindState();
        bool vanillaBusyNow = vanillaTarget != null || mindState != DayZInfectedConstants.MINDSTATE_CALM;
        if (vanillaBusyNow)
        {
            state.m_RecoveryCalmAfterTime = 0;
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_WAIT_BUSY scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " nextCheckSeconds=" + state.m_StuckRecoveryStatusCheckSeconds.ToString());
            return;
        }

        if (state.m_RecoveryCalmAfterTime == 0)
        {
            state.m_RecoveryCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_CALM scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cooldownMs=" + AGGRO_COOLDOWN_MS.ToString());
            return;
        }

        if (now < state.m_RecoveryCalmAfterTime)
            return;

        state.m_Mode = state.m_RecoveryResumeMode;
        if (state.m_Mode != "RETURN_TO_HOLD")
            state.m_Mode = "MIGRATION";

        state.m_VanillaBusy = false;
        state.m_RecoveryFreeUntilTime = 0;
        state.m_NextRecoveryStatusCheckTime = 0;
        state.m_RecoveryCalmAfterTime = 0;
        ResetStuckSample(state, "STUCK_RECOVERY_RESUME");

        if (BuildPath(state))
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_RESUME scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " position=" + state.m_Infected.GetPosition().ToString() + " target=" + GetCurrentRouteTarget(state).ToString());
    }

    protected void UpdateHoldFree(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);
        ResetStuckSample(state, "HOLD_FREE");
        SuspendRouteProgressWatchdog(state);

        if (now < state.m_NextHoldCheckTime)
            return;

        state.m_NextHoldCheckTime = now + Math.Round(state.m_FinalHoldCheckSeconds * 1000.0);

        float distance = HorizontalDistance(state.m_Infected.GetPosition(), state.m_FinalTargetCenter);
        if (distance <= state.m_FinalHoldRadius)
        {
            state.m_HoldCalmAfterTime = 0;
            return;
        }

        EntityAI vanillaTarget = controller.GetTargetEntity();
        int mindState = controller.GetMindState();
        bool vanillaBusyNow = vanillaTarget != null || mindState != DayZInfectedConstants.MINDSTATE_CALM;
        if (vanillaBusyNow)
        {
            state.m_HoldCalmAfterTime = 0;
            LogInfo(MIGRATION_LOG_PREFIX + " HOLD_RETURN_WAIT_BUSY scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " distance=" + distance.ToString() + " holdRadius=" + state.m_FinalHoldRadius.ToString());
            return;
        }

        if (state.m_HoldCalmAfterTime == 0)
        {
            state.m_HoldCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            LogInfo(MIGRATION_LOG_PREFIX + " HOLD_OUTSIDE scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " distance=" + distance.ToString() + " holdRadius=" + state.m_FinalHoldRadius.ToString() + " cooldownMs=" + AGGRO_COOLDOWN_MS.ToString());
            return;
        }

        if (now < state.m_HoldCalmAfterTime)
            return;

        StartReturnToHold(state);
    }

    protected bool UpdateStuckDetection(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        if (state.m_StuckRecoveryEnabled != 1)
            return false;

        if (state.m_Mode != "MIGRATION" && state.m_Mode != "RETURN_TO_HOLD")
            return false;

        if (!state.m_StuckSampleValid)
        {
            SetStuckSample(state, now);
            return false;
        }

        int detectionMs = Math.Round(state.m_StuckDetectionSeconds * 1000.0);
        if (now < state.m_StuckSampleTime + detectionMs)
            return false;

        vector position = state.m_Infected.GetPosition();
        vector controlTarget = GetCurrentControlTarget(state);
        float currentDistance = HorizontalDistance(position, controlTarget);
        float movedDistance = HorizontalDistance(position, state.m_StuckSamplePosition);
        float progress = state.m_StuckSampleDistance - currentDistance;
        float elapsedSeconds = (now - state.m_StuckSampleTime) / 1000.0;
        string sampleResult = "STUCK";
        if (movedDistance >= state.m_StuckMinMovementMeters)
            sampleResult = "MOVING";

        LogStuckDebug(MIGRATION_LOG_PREFIX + " STUCK_SAMPLE_CHECK scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " positionA=" + state.m_StuckSamplePosition.ToString() + " positionB=" + position.ToString() + " elapsedSeconds=" + elapsedSeconds.ToString() + " movedXZ=" + movedDistance.ToString() + " threshold=" + state.m_StuckMinMovementMeters.ToString() + " result=" + sampleResult);

        if (movedDistance >= state.m_StuckMinMovementMeters)
        {
            SetStuckSample(state, now);
            return false;
        }

        LogInfo(MIGRATION_LOG_PREFIX + " STUCK_DETECTED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " positionA=" + state.m_StuckSamplePosition.ToString() + " positionB=" + position.ToString() + " elapsedSeconds=" + elapsedSeconds.ToString() + " movedXZ=" + movedDistance.ToString() + " threshold=" + state.m_StuckMinMovementMeters.ToString() + " target=" + controlTarget.ToString() + " progressDiagnostic=" + progress.ToString());
        TriggerStuckRecovery(state, controller, now, position, controlTarget, "STUCK");
        return true;
    }

    protected bool UpdateRouteProgressWatchdog(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        if (state.m_RouteProgressWatchdogEnabled != 1)
            return false;

        if (state.m_Mode != "MIGRATION" && state.m_Mode != "RETURN_TO_HOLD")
            return false;

        if (!state.m_Waypoints || state.m_WaypointIndex >= state.m_Waypoints.Count())
            return false;

        if (state.m_NextRouteProgressCheckTime == 0)
        {
            ScheduleRouteProgressBaseline(state, now);
            return false;
        }

        if (now < state.m_NextRouteProgressCheckTime)
            return false;

        vector position = state.m_Infected.GetPosition();
        vector logicalTarget = GetCurrentRouteTarget(state);
        float currentDistance = HorizontalDistance(position, logicalTarget);

        if ((!ManualRouteComplete(state) && currentDistance <= state.m_RoutePointReachRadius) || (ManualRouteComplete(state) && state.m_Mode == "RETURN_TO_HOLD" && currentDistance <= state.m_FinalHoldReturnRadius) || (ManualRouteComplete(state) && state.m_Mode == "MIGRATION" && currentDistance <= ARRIVAL_TOLERANCE))
        {
            SuspendRouteProgressWatchdog(state);
            return false;
        }

        int checkIntervalMs = Math.Round(state.m_RouteProgressCheckSeconds * 1000.0);
        if (!state.m_RouteProgressSampleValid)
        {
            state.m_RouteProgressSampleValid = true;
            state.m_RouteProgressPreviousDistance = currentDistance;
            state.m_RouteProgressBadCheckCount = 0;
            state.m_NextRouteProgressCheckTime = now + checkIntervalMs;
            return false;
        }

        float previousDistance = state.m_RouteProgressPreviousDistance;
        float progress = previousDistance - currentDistance;
        float backtrack = currentDistance - previousDistance;
        string reason = "";

        if (backtrack >= state.m_RouteProgressMaxBacktrackMeters)
        {
            reason = "BACKTRACK";
        }
        else if (progress >= state.m_RouteProgressMinProgressMeters)
        {
            state.m_RouteProgressBadCheckCount = 0;
        }
        else
        {
            state.m_RouteProgressBadCheckCount++;
            if (state.m_RouteProgressBadCheckCount >= state.m_RouteProgressBadCheckLimit)
                reason = "NO_PROGRESS";
        }

        state.m_RouteProgressPreviousDistance = currentDistance;
        state.m_NextRouteProgressCheckTime = now + checkIntervalMs;

        if (reason == "")
            return false;

        string routeProgressLog = MIGRATION_LOG_PREFIX + " ROUTE_PROGRESS_LOST scenario=" + state.m_ScenarioId;
        routeProgressLog = routeProgressLog + " group=" + state.m_RuntimeGroupId;
        routeProgressLog = routeProgressLog + " id=" + state.m_InfectedId;
        routeProgressLog = routeProgressLog + " mode=" + state.m_Mode;
        routeProgressLog = routeProgressLog + " logicalTarget=" + logicalTarget.ToString();
        routeProgressLog = routeProgressLog + " previous=" + previousDistance.ToString();
        routeProgressLog = routeProgressLog + " current=" + currentDistance.ToString();
        routeProgressLog = routeProgressLog + " progress=" + progress.ToString();
        routeProgressLog = routeProgressLog + " backtrack=" + backtrack.ToString();
        routeProgressLog = routeProgressLog + " badChecks=" + state.m_RouteProgressBadCheckCount.ToString();
        routeProgressLog = routeProgressLog + " reason=" + reason;
        LogInfo(routeProgressLog);

        TriggerStuckRecovery(state, controller, now, position, logicalTarget, "ROUTE_PROGRESS_LOST");
        return true;
    }

    protected void TriggerStuckRecovery(S77MigrateUnitState state, DayZInfectedInputController controller, int now, vector position, vector controlTarget, string cause)
    {
        vector stimulusTarget = controlTarget;
        vector direction = vector.Direction(position, controlTarget);
        direction[1] = 0.0;
        if (direction.Length() <= 0.01)
        {
            stimulusTarget = GetCurrentRouteTarget(state);
            direction = vector.Direction(position, stimulusTarget);
            direction[1] = 0.0;
        }

        string resumeMode = GetMovementIntent(state);
        BeginStuckRecoveryState(state, controller, now, resumeMode);

        if (direction.Length() <= 0.01)
        {
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_ATTEMPT scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " target=" + stimulusTarget.ToString() + " stimulusPosition=UNAVAILABLE forwardDistance=" + state.m_StuckStimulusForwardDistance.ToString() + " lifetime=" + state.m_StuckStimulusLifetimeSeconds.ToString() + " strength=" + state.m_StuckStimulusStrengthMultiplier.ToString());
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_SKIPPED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " controlTarget=" + controlTarget.ToString() + " logicalTarget=" + stimulusTarget.ToString() + " reason=NO_USABLE_DIRECTION");
            return;
        }

        direction.Normalize();
        vector stimulusPosition = position + direction * state.m_StuckStimulusForwardDistance;
        stimulusPosition[1] = position[1];
        LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_ATTEMPT scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " target=" + stimulusTarget.ToString() + " stimulusPosition=" + stimulusPosition.ToString() + " forwardDistance=" + state.m_StuckStimulusForwardDistance.ToString() + " lifetime=" + state.m_StuckStimulusLifetimeSeconds.ToString() + " strength=" + state.m_StuckStimulusStrengthMultiplier.ToString());

        S77MigrateGroupState groupState = GetGroupState(state);
        if (!groupState)
        {
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_SKIPPED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " target=" + stimulusTarget.ToString() + " stimulusPosition=" + stimulusPosition.ToString() + " reason=GROUP_NOT_FOUND");
            return;
        }

        bool stimulusCreated = EmitAIStimulus(groupState, stimulusPosition, state.m_StuckStimulusLifetimeSeconds, state.m_StuckStimulusStrengthMultiplier, "STUCK_RECOVERY");
        if (stimulusCreated)
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_OK scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " target=" + stimulusTarget.ToString() + " stimulusPosition=" + stimulusPosition.ToString());
        else
            LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_STIMULUS_FAILED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " cause=" + cause + " position=" + position.ToString() + " target=" + stimulusTarget.ToString() + " stimulusPosition=" + stimulusPosition.ToString());
    }

    protected void BeginStuckRecoveryState(S77MigrateUnitState state, DayZInfectedInputController controller, int now, string resumeMode)
    {
        ReleaseRouteControl(controller);
        state.m_RecoveryResumeMode = resumeMode;
        state.m_Mode = "STUCK_RECOVERY";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_RecoveryFreeUntilTime = now + Math.Round(state.m_StuckRecoveryFreeSeconds * 1000.0);
        state.m_NextRecoveryStatusCheckTime = state.m_RecoveryFreeUntilTime;
        state.m_RecoveryCalmAfterTime = 0;
        ResetStuckSample(state, "STUCK_RECOVERY");
        SuspendRouteProgressWatchdog(state);
        LogInfo(MIGRATION_LOG_PREFIX + " STUCK_RECOVERY_RELEASE scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " resumeMode=" + resumeMode + " position=" + state.m_Infected.GetPosition().ToString() + " freeSeconds=" + state.m_StuckRecoveryFreeSeconds.ToString());
    }

    protected void SetStuckSample(S77MigrateUnitState state, int now)
    {
        state.m_StuckSampleValid = true;
        state.m_StuckSamplePosition = state.m_Infected.GetPosition();
        state.m_StuckSampleDistance = HorizontalDistance(state.m_StuckSamplePosition, GetCurrentControlTarget(state));
        state.m_StuckSampleTime = now;
        LogStuckDebug(MIGRATION_LOG_PREFIX + " STUCK_SAMPLE_START scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " positionA=" + state.m_StuckSamplePosition.ToString() + " sampleTimeMs=" + now.ToString() + " detectionSeconds=" + state.m_StuckDetectionSeconds.ToString() + " threshold=" + state.m_StuckMinMovementMeters.ToString() + " target=" + GetCurrentControlTarget(state).ToString());
    }

    protected void ResetStuckSample(S77MigrateUnitState state, string reason)
    {
        if (!state)
            return;

        if (state.m_StuckSampleValid)
        {
            int sampleAgeMs = 0;
            if (GetGame())
                sampleAgeMs = GetGame().GetTime() - state.m_StuckSampleTime;

            LogStuckDebug(MIGRATION_LOG_PREFIX + " STUCK_SAMPLE_RESET scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " reason=" + reason + " positionA=" + state.m_StuckSamplePosition.ToString() + " sampleAgeMs=" + sampleAgeMs.ToString());
        }

        state.m_StuckSampleValid = false;
        state.m_StuckSampleTime = 0;
        state.m_StuckSampleDistance = 0.0;
    }

    protected void ScheduleRouteProgressBaseline(S77MigrateUnitState state, int now)
    {
        SuspendRouteProgressWatchdog(state);
        if (!state || state.m_RouteProgressWatchdogEnabled != 1)
            return;

        if (state.m_Mode != "MIGRATION" && state.m_Mode != "RETURN_TO_HOLD")
            return;

        state.m_NextRouteProgressCheckTime = now + state.m_RouteProgressStaggerMs;
    }

    protected void SuspendRouteProgressWatchdog(S77MigrateUnitState state)
    {
        if (!state)
            return;

        state.m_RouteProgressSampleValid = false;
        state.m_RouteProgressPreviousDistance = 0.0;
        state.m_NextRouteProgressCheckTime = 0;
        state.m_RouteProgressBadCheckCount = 0;
    }

    protected vector GetCurrentControlTarget(S77MigrateUnitState state)
    {
        if (state.m_Waypoints && state.m_WaypointIndex >= 0 && state.m_WaypointIndex < state.m_Waypoints.Count())
            return state.m_Waypoints.Get(state.m_WaypointIndex);

        return GetCurrentRouteTarget(state);
    }

    protected S77MigrateGroupState GetGroupState(S77MigrateUnitState state)
    {
        if (!state)
            return null;

        for (int i = 0; i < m_Groups.Count(); i++)
        {
            S77MigrateGroupState groupState = m_Groups.Get(i);
            if (groupState && groupState.m_RuntimeGroupId == state.m_RuntimeGroupId)
                return groupState;
        }

        return null;
    }

    protected void UpdateGroupActivations()
    {
        for (int groupIndex = 0; groupIndex < m_Groups.Count(); groupIndex++)
        {
            S77MigrateGroupState groupState = m_Groups.Get(groupIndex);
            if (!groupState)
                continue;

            if (groupState.m_RouteActivationEnabled != 1 && groupState.m_FinalActivationEnabled != 1)
                continue;

            int aliveCount = CountActiveGroupMembers(groupState);
            if (aliveCount <= 0)
                continue;

            if (groupState.m_RouteActivationEnabled == 1)
            {
                for (int pointIndex = 0; pointIndex < groupState.m_RoutePoints.Count(); pointIndex++)
                    UpdateRoutePointActivation(groupState, pointIndex, aliveCount);
            }

            if (groupState.m_FinalActivationEnabled == 1)
                UpdateFinalActivation(groupState, aliveCount);
        }
    }

    protected void UpdateRoutePointActivation(S77MigrateGroupState groupState, int pointIndex, int aliveCount)
    {
        vector pointCenter = groupState.m_RoutePoints.Get(pointIndex);
        float radius = groupState.m_RouteActivationRadii.Get(pointIndex);
        int insideCount = CountRouteMembersInside(groupState, pointCenter, radius);
        int requiredCount = CalculateRequiredCount(aliveCount, groupState.m_RouteActivationTriggerPercent);
        bool armed = groupState.m_RouteActivationArmed.Get(pointIndex);
        int pointNumber = pointIndex + 1;

        if (insideCount >= requiredCount)
        {
            if (!armed)
                return;

            string activationLog = MIGRATION_LOG_PREFIX + " ROUTE_ACTIVATION scenario=" + groupState.m_ScenarioId;
            activationLog = activationLog + " group=" + groupState.m_RuntimeGroupId;
            activationLog = activationLog + " point=" + pointNumber.ToString();
            activationLog = activationLog + " alive=" + aliveCount.ToString();
            activationLog = activationLog + " inside=" + insideCount.ToString();
            activationLog = activationLog + " required=" + requiredCount.ToString();
            activationLog = activationLog + " percent=" + groupState.m_RouteActivationTriggerPercent.ToString();
            activationLog = activationLog + " radius=" + radius.ToString();
            LogInfo(activationLog);

            EmitAIStimulus(groupState, pointCenter, groupState.m_RouteStimulusLifetimeSeconds, groupState.m_RouteStimulusStrengthMultiplier, "point=" + pointNumber.ToString());
            groupState.m_RouteActivationArmed.Set(pointIndex, false);
            return;
        }

        if (!armed)
        {
            groupState.m_RouteActivationArmed.Set(pointIndex, true);
            LogInfo(MIGRATION_LOG_PREFIX + " ROUTE_ACTIVATION_REARMED scenario=" + groupState.m_ScenarioId + " group=" + groupState.m_RuntimeGroupId + " point=" + pointNumber.ToString() + " inside=" + insideCount.ToString() + " required=" + requiredCount.ToString());
        }
    }

    protected void UpdateFinalActivation(S77MigrateGroupState groupState, int aliveCount)
    {
        int insideCount = CountFinalMembersInside(groupState);
        int requiredCount = CalculateRequiredCount(aliveCount, groupState.m_FinalActivationTriggerPercent);

        if (insideCount >= requiredCount)
        {
            if (!groupState.m_FinalActivationArmed)
                return;

            string activationLog = MIGRATION_LOG_PREFIX + " FINAL_ACTIVATION scenario=" + groupState.m_ScenarioId;
            activationLog = activationLog + " group=" + groupState.m_RuntimeGroupId;
            activationLog = activationLog + " alive=" + aliveCount.ToString();
            activationLog = activationLog + " inside=" + insideCount.ToString();
            activationLog = activationLog + " required=" + requiredCount.ToString();
            activationLog = activationLog + " percent=" + groupState.m_FinalActivationTriggerPercent.ToString();
            activationLog = activationLog + " radius=" + groupState.m_FinalActivationDistance.ToString();
            LogInfo(activationLog);

            EmitAIStimulus(groupState, groupState.m_FinalTargetCenter, groupState.m_FinalStimulusLifetimeSeconds, groupState.m_FinalStimulusStrengthMultiplier, "FINAL");
            groupState.m_FinalActivationArmed = false;
            return;
        }

        if (!groupState.m_FinalActivationArmed)
        {
            groupState.m_FinalActivationArmed = true;
            LogInfo(MIGRATION_LOG_PREFIX + " FINAL_REARMED scenario=" + groupState.m_ScenarioId + " group=" + groupState.m_RuntimeGroupId + " inside=" + insideCount.ToString() + " required=" + requiredCount.ToString());
        }
    }

    protected bool EmitAIStimulus(S77MigrateGroupState groupState, vector position, float lifetime, float strengthMultiplier, string pointLabel)
    {
        if (!m_StimulusReady || !m_NoiseSystem || !m_StimulusParams)
        {
            if (!groupState.m_StimulusFailureLogged)
            {
                LogInfo(MIGRATION_LOG_PREFIX + " WARNING: AI stimulus unavailable scenario=" + groupState.m_ScenarioId + " group=" + groupState.m_RuntimeGroupId + "; migration route remains active");
                groupState.m_StimulusFailureLogged = true;
            }
            return false;
        }

        m_NoiseSystem.AddNoiseTarget(position, lifetime, m_StimulusParams, strengthMultiplier);

        string stimulusType = "AI_STIMULUS";
        if (pointLabel == "FINAL")
            stimulusType = "FINAL_STIMULUS";

        string stimulusLog = MIGRATION_LOG_PREFIX + " " + stimulusType + " scenario=" + groupState.m_ScenarioId;
        stimulusLog = stimulusLog + " group=" + groupState.m_RuntimeGroupId;
        stimulusLog = stimulusLog + " " + pointLabel;
        stimulusLog = stimulusLog + " position=" + position.ToString();
        stimulusLog = stimulusLog + " lifetime=" + lifetime.ToString();
        stimulusLog = stimulusLog + " strength=" + strengthMultiplier.ToString();
        stimulusLog = stimulusLog + " preset=" + AI_STIMULUS_NOISE_PATH;
        LogInfo(stimulusLog);
        return true;
    }

    protected int CountActiveGroupMembers(S77MigrateGroupState groupState)
    {
        int count = 0;
        for (int i = 0; i < groupState.m_Members.Count(); i++)
        {
            if (IsActiveGroupMember(groupState, groupState.m_Members.Get(i)))
                count++;
        }

        return count;
    }

    protected int CountRouteMembersInside(S77MigrateGroupState groupState, vector pointCenter, float radius)
    {
        int count = 0;
        for (int i = 0; i < groupState.m_Members.Count(); i++)
        {
            S77MigrateUnitState state = groupState.m_Members.Get(i);
            if (!IsActiveGroupMember(groupState, state))
                continue;

            if (HorizontalDistance(state.m_Infected.GetPosition(), pointCenter) <= radius)
                count++;
        }

        return count;
    }

    protected int CountFinalMembersInside(S77MigrateGroupState groupState)
    {
        int count = 0;
        for (int i = 0; i < groupState.m_Members.Count(); i++)
        {
            S77MigrateUnitState state = groupState.m_Members.Get(i);
            if (!IsActiveGroupMember(groupState, state))
                continue;

            if (HorizontalDistance(state.m_Infected.GetPosition(), groupState.m_FinalTargetCenter) <= groupState.m_FinalActivationDistance)
                count++;
        }

        return count;
    }

    protected bool IsActiveGroupMember(S77MigrateGroupState groupState, S77MigrateUnitState state)
    {
        return state && state.m_RuntimeGroupId == groupState.m_RuntimeGroupId && !state.m_Released && state.m_Infected && state.m_Infected.IsAlive();
    }

    protected int CalculateRequiredCount(int aliveCount, float triggerPercent)
    {
        if (aliveCount <= 0)
            return 0;

        int requiredCount = Math.Ceil(aliveCount * triggerPercent / 100.0);
        if (requiredCount < 1)
            requiredCount = 1;

        if (requiredCount > aliveCount)
            requiredCount = aliveCount;

        return requiredCount;
    }

    protected void FollowPath(S77MigrateUnitState state, DayZInfectedInputController controller)
    {
        vector position = state.m_Infected.GetPosition();

        if (!ManualRouteComplete(state))
        {
            vector routePointTarget = GetCurrentRouteTarget(state);
            if (HorizontalDistance(position, routePointTarget) <= state.m_RoutePointReachRadius)
            {
                state.m_RoutePointIndex++;
                ReleaseRouteControl(controller);
                ResetStuckSample(state, "ROUTE_POINT_CHANGED");
                SuspendRouteProgressWatchdog(state);
                LogInfo(MIGRATION_LOG_PREFIX + " ROUTE_POINT_REACHED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " reached=" + state.m_RoutePointIndex.ToString() + "/" + state.m_RoutePoints.Count().ToString());
                BuildPath(state);
                return;
            }
        }

        if (state.m_Mode == "RETURN_TO_HOLD" && HorizontalDistance(position, state.m_MigrationTarget) <= state.m_FinalHoldReturnRadius)
        {
            EnterHoldFree(state, controller, "HOLD_RETURNED");
            return;
        }

        while (state.m_WaypointIndex < state.m_Waypoints.Count() && HorizontalDistance(position, state.m_Waypoints.Get(state.m_WaypointIndex)) <= WAYPOINT_TOLERANCE)
        {
            state.m_WaypointIndex++;
            ResetStuckSample(state, "WAYPOINT_CHANGED");
        }

        if (state.m_WaypointIndex >= state.m_Waypoints.Count())
        {
            ReleaseRouteControl(controller);

            if (!ManualRouteComplete(state))
            {
                state.m_Waypoints.Clear();
                state.m_WaypointIndex = 0;
                state.m_NextPathRetryTime = GetGame().GetTime() + PATH_RETRY_INTERVAL_MS;
                ResetStuckSample(state, "BUILD_PATH_RETRY");
                SuspendRouteProgressWatchdog(state);
                return;
            }

            CompleteFinalArrival(state, controller, "ARRIVAL");
            return;
        }

        if (ManualRouteComplete(state) && HorizontalDistance(position, state.m_MigrationTarget) <= ARRIVAL_TOLERANCE)
        {
            CompleteFinalArrival(state, controller, "ARRIVAL");
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
        if (state.m_Mode != "RETURN_TO_HOLD")
            state.m_Mode = "MIGRATION";
    }

    protected void CompleteFinalArrival(S77MigrateUnitState state, DayZInfectedInputController controller, string reason)
    {
        if (state.m_FinalHoldEnabled == 1)
        {
            EnterHoldFree(state, controller, reason);
            return;
        }

        ReleaseUnit(state, controller, reason);
    }

    protected void EnterHoldFree(S77MigrateUnitState state, DayZInfectedInputController controller, string reason)
    {
        if (!state || !state.m_Infected || !state.m_Infected.IsAlive())
            return;

        ReleaseRouteControl(controller);
        if (state.m_Waypoints)
            state.m_Waypoints.Clear();

        state.m_WaypointIndex = 0;
        state.m_RoutePointIndex = state.m_RoutePoints.Count();
        state.m_Mode = "HOLD_FREE";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_NextPathRetryTime = 0;
        state.m_RecoveryFreeUntilTime = 0;
        state.m_NextRecoveryStatusCheckTime = 0;
        state.m_RecoveryCalmAfterTime = 0;
        state.m_HoldCalmAfterTime = 0;
        state.m_NextHoldCheckTime = GetGame().GetTime() + Math.Round(state.m_FinalHoldCheckSeconds * 1000.0);
        ResetStuckSample(state, "HOLD_FREE");
        SuspendRouteProgressWatchdog(state);

        string eventName = "HOLD_ENTER";
        if (reason == "HOLD_RETURNED")
            eventName = "HOLD_RETURNED";

        LogInfo(MIGRATION_LOG_PREFIX + " " + eventName + " scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " reason=" + reason + " position=" + state.m_Infected.GetPosition().ToString() + " holdRadius=" + state.m_FinalHoldRadius.ToString() + " returnRadius=" + state.m_FinalHoldReturnRadius.ToString());
    }

    protected void StartReturnToHold(S77MigrateUnitState state)
    {
        if (!state || !state.m_Infected || !state.m_Infected.IsAlive())
            return;

        state.m_RoutePointIndex = state.m_RoutePoints.Count();
        state.m_Mode = "RETURN_TO_HOLD";
        state.m_RecoveryResumeMode = "RETURN_TO_HOLD";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_HoldCalmAfterTime = 0;
        ResetStuckSample(state, "RETURN_TO_HOLD");
        SuspendRouteProgressWatchdog(state);

        if (BuildPath(state))
            LogInfo(MIGRATION_LOG_PREFIX + " HOLD_RETURN_START scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " position=" + state.m_Infected.GetPosition().ToString() + " target=" + state.m_MigrationTarget.ToString() + " returnRadius=" + state.m_FinalHoldReturnRadius.ToString());
    }

    protected void ReleaseUnit(S77MigrateUnitState state, DayZInfectedInputController controller, string reason)
    {
        if (!state || state.m_Released)
            return;

        ReleaseRouteControl(controller);
        state.m_Released = true;
        state.m_Mode = "RELEASED";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_NextPathRetryTime = 0;
        state.m_RecoveryFreeUntilTime = 0;
        state.m_NextRecoveryStatusCheckTime = 0;
        state.m_RecoveryCalmAfterTime = 0;
        state.m_HoldCalmAfterTime = 0;
        state.m_NextHoldCheckTime = 0;
        ResetStuckSample(state, "RELEASE_" + reason);
        SuspendRouteProgressWatchdog(state);
        if (state.m_Waypoints)
            state.m_Waypoints.Clear();
        state.m_WaypointIndex = 0;
        LogInfo(MIGRATION_LOG_PREFIX + " RELEASED scenario=" + state.m_ScenarioId + " group=" + state.m_RuntimeGroupId + " id=" + state.m_InfectedId + " mode=" + state.m_Mode + " reason=" + reason + " position=" + state.m_Infected.GetPosition().ToString());
    }

    protected void LogUnitStates()
    {
        int now = GetGame().GetTime();
        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (!state || !state.m_Infected)
            {
                LogError(EVENT_LOG_PREFIX + " ERROR: unit state or infected entity is missing");
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

            string routePointStatus = state.m_RoutePointIndex.ToString() + "/0";
            if (state.m_RoutePoints)
                routePointStatus = state.m_RoutePointIndex.ToString() + "/" + state.m_RoutePoints.Count().ToString();

            vector position = state.m_Infected.GetPosition();
            float distanceToTarget = HorizontalDistance(position, state.m_FinalTargetCenter);

            string logLine = MIGRATION_LOG_PREFIX;
            logLine = logLine + " scenario=" + state.m_ScenarioId;
            logLine = logLine + " group=" + state.m_RuntimeGroupId;
            logLine = logLine + " id=" + state.m_InfectedId;
            logLine = logLine + " class=" + state.m_ClassName;
            logLine = logLine + " position=" + position.ToString();
            logLine = logLine + " routePoint=" + routePointStatus;
            logLine = logLine + " waypoint=" + waypointStatus;
            logLine = logLine + " distance=" + distanceToTarget.ToString();
            logLine = logLine + " target=" + targetPresent;
            logLine = logLine + " mind=" + MindStateToString(mindState);
            logLine = logLine + " mode=" + state.m_Mode;

            LogInfo(logLine);
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
            if (state && !state.m_Released && state.m_Infected && state.m_Infected.IsAlive())
                ReleaseRouteControl(state.m_Infected.GetInputController());
        }

        LogInfo(EVENT_LOG_PREFIX + " SESSION_END groups=" + m_Groups.Count().ToString() + " units=" + m_Units.Count().ToString());
        CloseProfileLog();
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
