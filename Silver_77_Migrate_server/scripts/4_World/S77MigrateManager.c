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

    void S77MigrateUnitState(DayZInfected infected, string infectedId, string scenarioId, vector migrationTarget)
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
    }
}

class S77MigrateManager
{
    protected static ref S77MigrateManager s_Instance;

    protected const string LOG_PREFIX = "[S77Migrate][TEST001]";
    protected const int UPDATE_INTERVAL_MS = 250;
    protected const int AGGRO_COOLDOWN_MS = 5000;
    protected const int PATH_RETRY_INTERVAL_MS = 5000;
    protected const float WAYPOINT_TOLERANCE = 1.75;
    protected const float ARRIVAL_TOLERANCE = 4.0;
    protected const float MIGRATION_SPEED = 1.0;
    protected const float FORMATION_SPACING = 1.25;
    protected const float FORMATION_JITTER = 0.10;

    protected ref S77MigrateScenarioConfig m_Config;
    protected ref array<ref S77MigrateUnitState> m_Units;
    protected ref PGFilter m_PathFilter;
    protected bool m_ScenarioStarted;
    protected bool m_RouteUpdateScheduled;
    protected bool m_LogScheduled;

    void S77MigrateManager()
    {
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

        s_Instance.InitializeScenario();
    }

    static void Shutdown()
    {
        if (!s_Instance)
            return;

        s_Instance.Stop();
        s_Instance = null;
    }

    protected void InitializeScenario()
    {
        m_Config = S77MigrateConfigLoader.LoadScenario001();

        if (!m_Config)
        {
            Print(LOG_PREFIX + " ERROR: scenario config could not be loaded");
            return;
        }

        if (m_Config.enabled != 1)
        {
            Print(LOG_PREFIX + " Scenario disabled. Set enabled=1 in " + S77_MIGRATE_SCENARIO_001_CONFIG + " and restart the server.");
            return;
        }

        int delayMs = Math.Round(m_Config.spawnDelaySeconds * 1000.0);
        Print(LOG_PREFIX + " Scenario enabled; one-time spawn scheduled in " + m_Config.spawnDelaySeconds.ToString() + " seconds");
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartScenario, delayMs, false);
    }

    protected void StartScenario()
    {
        if (m_ScenarioStarted)
        {
            Print(LOG_PREFIX + " Duplicate start ignored");
            return;
        }

        m_ScenarioStarted = true;

        vector spawnCenter = m_Config.GetSpawnPosition();
        vector targetCenter = m_Config.GetTargetPosition();

        for (int i = 0; i < m_Config.infectedCount; i++)
        {
            string className = m_Config.infectedTypes.Get(i % m_Config.infectedTypes.Count());
            vector formationOffset = GetFormationOffset(i, m_Config.infectedCount);
            SpawnMigrationInfected(i, className, spawnCenter + formationOffset, targetCenter + formationOffset);
        }

        Print(LOG_PREFIX + " Scenario " + m_Config.scenarioId + " started; spawned=" + m_Units.Count().ToString() + "/" + m_Config.infectedCount.ToString());

        if (m_Units.Count() == 0)
            return;

        m_RouteUpdateScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateRoutes, UPDATE_INTERVAL_MS, true);

        int logIntervalMs = Math.Round(m_Config.logIntervalSeconds * 1000.0);
        m_LogScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LogUnitStates, logIntervalMs, true);
        LogUnitStates();
    }

    protected void SpawnMigrationInfected(int index, string className, vector spawnPosition, vector migrationTarget)
    {
        if (!GetGame().ConfigIsExisting("CfgVehicles " + className) || !GetGame().IsKindOf(className, "DayZInfected"))
        {
            Print(LOG_PREFIX + " ERROR: invalid infected class: " + className);
            return;
        }

        int createFlags = ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS;
        DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx(className, spawnPosition, createFlags));

        if (!infected)
        {
            Print(LOG_PREFIX + " ERROR: CreateObjectEx failed for class=" + className + " position=" + spawnPosition.ToString());
            return;
        }

        infected.SetOrientation(Vector(27.359158, 0.0, 0.0));

        string infectedId = "INF_" + (index + 1).ToString();
        S77MigrateUnitState state = new S77MigrateUnitState(infected, infectedId, m_Config.scenarioId, migrationTarget);
        m_Units.Insert(state);

        BuildPath(state);
        Print(LOG_PREFIX + " Spawned id=" + infectedId + " class=" + className + " position=" + infected.GetPosition().ToString() + " target=" + migrationTarget.ToString());
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
            Print(LOG_PREFIX + " ERROR: AIWorld is unavailable for id=" + state.m_InfectedId);
            return false;
        }

        vector sampledStart;
        vector sampledTarget;
        vector currentPosition = state.m_Infected.GetPosition();

        if (!aiWorld.SampleNavmeshPosition(currentPosition, 8.0, m_PathFilter, sampledStart))
        {
            Print(LOG_PREFIX + " PATH ERROR: no navmesh near current position for id=" + state.m_InfectedId + " position=" + currentPosition.ToString());
            return false;
        }

        if (!aiWorld.SampleNavmeshPosition(state.m_MigrationTarget, 15.0, m_PathFilter, sampledTarget))
        {
            Print(LOG_PREFIX + " PATH ERROR: no navmesh near target for id=" + state.m_InfectedId + " target=" + state.m_MigrationTarget.ToString());
            return false;
        }

        TVectorArray newWaypoints = new TVectorArray();
        if (!aiWorld.FindPath(sampledStart, sampledTarget, m_PathFilter, newWaypoints) || newWaypoints.Count() < 2)
        {
            Print(LOG_PREFIX + " PATH ERROR: FindPath failed for id=" + state.m_InfectedId + " from=" + sampledStart.ToString() + " to=" + sampledTarget.ToString());
            return false;
        }

        state.m_Waypoints = newWaypoints;
        state.m_WaypointIndex = 1;
        state.m_NextPathRetryTime = 0;
        Print(LOG_PREFIX + " Path built for id=" + state.m_InfectedId + " waypoints=" + newWaypoints.Count().ToString());
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
            Print(LOG_PREFIX + " ARRIVED id=" + state.m_InfectedId + " class=" + state.m_ClassName + " position=" + position.ToString());
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
        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (!state || !state.m_Infected)
            {
                Print(LOG_PREFIX + " scenario=" + m_Config.scenarioId + " id=UNKNOWN entity=missing");
                continue;
            }

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

            string logLine = LOG_PREFIX;
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
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartScenario);

        if (m_RouteUpdateScheduled)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateRoutes);

        if (m_LogScheduled)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LogUnitStates);

        for (int i = 0; i < m_Units.Count(); i++)
        {
            S77MigrateUnitState state = m_Units.Get(i);
            if (state && state.m_Infected)
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

    protected vector GetFormationOffset(int index, int infectedCount)
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
        float offsetX = (column - centerColumn) * FORMATION_SPACING;
        float offsetZ = (row - centerRow) * FORMATION_SPACING;

        offsetX = offsetX + Math.RandomFloatInclusive(-FORMATION_JITTER, FORMATION_JITTER);
        offsetZ = offsetZ + Math.RandomFloatInclusive(-FORMATION_JITTER, FORMATION_JITTER);

        return Vector(offsetX, 0.0, offsetZ);
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
