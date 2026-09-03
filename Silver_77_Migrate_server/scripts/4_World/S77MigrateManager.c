class S77MigrateUnitState
{
    DayZInfected m_Infected;
    string m_InfectedId;
    string m_GroupId;
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
    int m_StuckReverseEnabled;
    float m_StuckReverseDistanceMeters;
    float m_StuckReverseMaxSeconds;
    float m_StuckReverseRetrySeconds;
    float m_StuckPostReverseFreeSeconds;
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
    string m_RecoveryPhase;
    vector m_RecoveryStartPosition;
    vector m_RecoveryForwardDirection;
    vector m_ReverseStartPosition;
    vector m_ReverseTarget;
    int m_ReverseStartedTime;
    int m_NextReverseAttemptTime;
    int m_PostReverseFreeUntilTime;
    int m_NextHoldCheckTime;
    int m_HoldCalmAfterTime;
    bool m_DeathLogged;

    void S77MigrateUnitState(DayZInfected infected, string infectedId, string groupId, string runtimeGroupId, vector migrationTarget, vector finalTargetCenter, vector routeOffset, TVectorArray routePoints, float routePointReachRadius, S77MigrateScenarioConfig config, int logIntervalMs)
    {
        m_Infected = infected;
        m_InfectedId = infectedId;
        m_GroupId = groupId;
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
        m_StuckReverseEnabled = config.stuckReverseEnabled;
        m_StuckReverseDistanceMeters = config.stuckReverseDistanceMeters;
        m_StuckReverseMaxSeconds = config.stuckReverseMaxSeconds;
        m_StuckReverseRetrySeconds = config.stuckReverseRetrySeconds;
        m_StuckPostReverseFreeSeconds = config.stuckPostReverseFreeSeconds;
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
        m_RecoveryPhase = "NONE";
        m_RecoveryStartPosition = Vector(0.0, 0.0, 0.0);
        m_RecoveryForwardDirection = Vector(0.0, 0.0, 0.0);
        m_ReverseStartPosition = Vector(0.0, 0.0, 0.0);
        m_ReverseTarget = Vector(0.0, 0.0, 0.0);
        m_ReverseStartedTime = 0;
        m_NextReverseAttemptTime = 0;
        m_PostReverseFreeUntilTime = 0;
        m_NextHoldCheckTime = 0;
        m_HoldCalmAfterTime = 0;
        m_DeathLogged = false;
    }
}

class S77MigrateGroupState
{
    string m_RuntimeGroupId;
    string m_GroupId;
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
        m_GroupId = config.groupId;
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
    protected ref array<ref S77MigrateScenarioConfig> m_GroupConfigs;
    protected ref array<ref S77MigrateScenarioConfig> m_PendingGroups;
    protected ref array<ref S77MigrateGroupState> m_Groups;
    protected ref array<ref S77MigrateUnitState> m_Units;
    protected ref PGFilter m_PathFilter;
    protected ref NoiseParams m_StimulusParams;
    protected NoiseSystem m_NoiseSystem;
    protected bool m_StimulusReady;
    protected bool m_LaunchBusy;
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
        m_GroupConfigs = new array<ref S77MigrateScenarioConfig>();
        m_PendingGroups = new array<ref S77MigrateScenarioConfig>();
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

        string builtLine226 = "[timeMs=";
        builtLine226 = builtLine226 + runtimeTimeMs.ToString();
        builtLine226 = builtLine226 + "] ";
        builtLine226 = builtLine226 + message;
        return builtLine226;
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
            string logMessage273 = EVENT_LOG_PREFIX;
            logMessage273 = logMessage273 + " ERROR: profile log could not be opened path=";
            logMessage273 = logMessage273 + S77_MIGRATE_LOG_FILE;
            logMessage273 = logMessage273 + "; RPT logging remains active";
            LogError(logMessage273);
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

    static bool ActivateById(string activationId)
    {
        if (!s_Instance || !s_Instance.m_Initialized || !s_Instance.m_Config)
        {
            string notReadyLog = "[S77Migrate][EVENT] ACTIVATION_REQUEST activationId=";
            notReadyLog = notReadyLog + activationId;
            notReadyLog = notReadyLog + " result=NOT_READY";
            Print(notReadyLog);
            return false;
        }

        return s_Instance.RequestActivation(activationId);
    }

    static bool StartGroupById(string groupId)
    {
        if (!s_Instance || !s_Instance.m_Initialized || !s_Instance.m_Config)
        {
            string notReadyLog = "[S77Migrate][EVENT] GROUP_START_REQUEST groupId=";
            notReadyLog = notReadyLog + groupId;
            notReadyLog = notReadyLog + " result=NOT_READY";
            Print(notReadyLog);
            return false;
        }

        return s_Instance.RequestGroup(groupId);
    }

    static bool StartEventById(string eventId)
    {
        if (!s_Instance || !s_Instance.m_Initialized || !s_Instance.m_Config)
        {
            string notReadyLog = "[S77Migrate][EVENT] EVENT_START_REQUEST eventId=";
            notReadyLog = notReadyLog + eventId;
            notReadyLog = notReadyLog + " result=NOT_READY";
            Print(notReadyLog);
            return false;
        }

        return s_Instance.RequestEvent(eventId);
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
        string logMessage327 = EVENT_LOG_PREFIX;
        logMessage327 = logMessage327 + " SESSION_START profileLog=";
        logMessage327 = logMessage327 + S77_MIGRATE_LOG_FILE;
        logMessage327 = logMessage327 + " loggingEnabled=";
        logMessage327 = logMessage327 + m_Config.loggingEnabled.ToString();
        logMessage327 = logMessage327 + " stuckDebugLoggingEnabled=";
        logMessage327 = logMessage327 + m_Config.stuckDebugLoggingEnabled.ToString();
        LogInfo(logMessage327);

        for (int groupIndex = 0; groupIndex < m_Config.groups.Count(); groupIndex++)
        {
            S77MigrateScenarioConfig group = m_Config.groups.Get(groupIndex);
            if (!group)
            {
                string logMessage334 = EVENT_LOG_PREFIX;
                logMessage334 = logMessage334 + " ERROR: null GROUP skipped at index=";
                logMessage334 = logMessage334 + groupIndex.ToString();
                LogError(logMessage334);
                continue;
            }

            m_GroupConfigs.Insert(group);
        }

        if (m_Config.enabled != 1)
        {
            string logMessage352 = EVENT_LOG_PREFIX;
            logMessage352 = logMessage352 + " Event disabled. Set enabled=1 in ";
            logMessage352 = logMessage352 + S77_MIGRATE_CONFIG;
            logMessage352 = logMessage352 + " and restart the server.";
            LogInfo(logMessage352);
            return;
        }

        if (NeedsAIStimulus())
            PrepareAIStimulus();

        bool startupFound = false;
        for (int activationIndex = 0; activationIndex < m_Config.activations.Count(); activationIndex++)
        {
            S77MigrateActivationConfig activation = m_Config.activations.Get(activationIndex);
            if (!activation || activation.enabled != 1 || activation.type != "STARTUP")
                continue;

            startupFound = true;
            RequestActivation(activation.activationId);
        }

        if (!startupFound)
            LogInfo(EVENT_LOG_PREFIX + " No enabled STARTUP activation; waiting for public API request");
    }

    protected bool RequestActivation(string activationId)
    {
        string requestLog = EVENT_LOG_PREFIX;
        requestLog = requestLog + " ACTIVATION_REQUEST activationId=";
        requestLog = requestLog + activationId;
        LogInfo(requestLog);

        S77MigrateActivationConfig activation = m_Config.FindActivation(activationId);
        if (!activation)
        {
            string notFoundLog = EVENT_LOG_PREFIX;
            notFoundLog = notFoundLog + " ACTIVATION_REQUEST activationId=";
            notFoundLog = notFoundLog + activationId;
            notFoundLog = notFoundLog + " result=NOT_FOUND";
            LogInfo(notFoundLog);
            return false;
        }

        if (activation.enabled != 1)
        {
            string disabledLog = EVENT_LOG_PREFIX;
            disabledLog = disabledLog + " ACTIVATION_REQUEST activationId=";
            disabledLog = disabledLog + activationId;
            disabledLog = disabledLog + " result=DISABLED";
            LogInfo(disabledLog);
            return false;
        }

        string resolvedLog = EVENT_LOG_PREFIX;
        resolvedLog = resolvedLog + " ACTIVATION_RESOLVED activationId=";
        resolvedLog = resolvedLog + activation.activationId;
        resolvedLog = resolvedLog + " type=";
        resolvedLog = resolvedLog + activation.type;
        resolvedLog = resolvedLog + " targetType=";
        resolvedLog = resolvedLog + activation.targetType;
        resolvedLog = resolvedLog + " targetId=";
        resolvedLog = resolvedLog + activation.targetId;
        LogInfo(resolvedLog);

        if (activation.targetType == "GROUP")
            return RequestGroup(activation.targetId);

        if (activation.targetType == "EVENT")
            return RequestEvent(activation.targetId);

        string invalidTargetLog = EVENT_LOG_PREFIX;
        invalidTargetLog = invalidTargetLog + " ACTIVATION_REQUEST activationId=";
        invalidTargetLog = invalidTargetLog + activationId;
        invalidTargetLog = invalidTargetLog + " result=INVALID_TARGET";
        LogInfo(invalidTargetLog);
        return false;
    }

    protected bool RequestGroup(string groupId)
    {
        string requestLog = EVENT_LOG_PREFIX;
        requestLog = requestLog + " GROUP_START_REQUEST groupId=";
        requestLog = requestLog + groupId;
        LogInfo(requestLog);

        S77MigrateScenarioConfig group = m_Config.FindGroup(groupId);
        if (!group)
        {
            string notFoundLog = EVENT_LOG_PREFIX;
            notFoundLog = notFoundLog + " GROUP_START_REQUEST groupId=";
            notFoundLog = notFoundLog + groupId;
            notFoundLog = notFoundLog + " result=NOT_FOUND";
            LogInfo(notFoundLog);
            return false;
        }

        if (group.enabled != 1)
        {
            string disabledLog = EVENT_LOG_PREFIX;
            disabledLog = disabledLog + " GROUP_START_REQUEST groupId=";
            disabledLog = disabledLog + groupId;
            disabledLog = disabledLog + " result=DISABLED";
            LogInfo(disabledLog);
            return false;
        }

        array<ref S77MigrateScenarioConfig> requestedGroups = new array<ref S77MigrateScenarioConfig>();
        requestedGroups.Insert(group);
        return BeginLaunchBatch(requestedGroups, "GROUP", groupId);
    }

    protected bool RequestEvent(string eventId)
    {
        string requestLog = EVENT_LOG_PREFIX;
        requestLog = requestLog + " EVENT_START_REQUEST eventId=";
        requestLog = requestLog + eventId;
        LogInfo(requestLog);

        S77MigrateEventConfig eventConfig = m_Config.FindEvent(eventId);
        if (!eventConfig)
        {
            string notFoundLog = EVENT_LOG_PREFIX;
            notFoundLog = notFoundLog + " EVENT_START_REQUEST eventId=";
            notFoundLog = notFoundLog + eventId;
            notFoundLog = notFoundLog + " result=NOT_FOUND";
            LogInfo(notFoundLog);
            return false;
        }

        if (eventConfig.enabled != 1)
        {
            string disabledLog = EVENT_LOG_PREFIX;
            disabledLog = disabledLog + " EVENT_START_REQUEST eventId=";
            disabledLog = disabledLog + eventId;
            disabledLog = disabledLog + " result=DISABLED";
            LogInfo(disabledLog);
            return false;
        }

        array<ref S77MigrateScenarioConfig> resolvedGroups = new array<ref S77MigrateScenarioConfig>();
        for (int resolveIndex = 0; resolveIndex < eventConfig.groupIds.Count(); resolveIndex++)
        {
            string referencedGroupId = eventConfig.groupIds.Get(resolveIndex);
            S77MigrateScenarioConfig referencedGroup = m_Config.FindGroup(referencedGroupId);
            if (!referencedGroup)
            {
                string invalidReferenceLog = EVENT_LOG_PREFIX;
                invalidReferenceLog = invalidReferenceLog + " EVENT_START_REQUEST eventId=";
                invalidReferenceLog = invalidReferenceLog + eventId;
                invalidReferenceLog = invalidReferenceLog + " result=INVALID_TARGET groupId=";
                invalidReferenceLog = invalidReferenceLog + referencedGroupId;
                LogInfo(invalidReferenceLog);
                return false;
            }

            resolvedGroups.Insert(referencedGroup);
        }

        array<ref S77MigrateScenarioConfig> runnableGroups = new array<ref S77MigrateScenarioConfig>();
        for (int groupIndex = 0; groupIndex < resolvedGroups.Count(); groupIndex++)
        {
            S77MigrateScenarioConfig group = resolvedGroups.Get(groupIndex);
            if (group.enabled == 1)
            {
                runnableGroups.Insert(group);
                continue;
            }

            string skippedLog = EVENT_LOG_PREFIX;
            skippedLog = skippedLog + " EVENT_RESOLVED eventId=";
            skippedLog = skippedLog + eventId;
            skippedLog = skippedLog + " groupId=";
            skippedLog = skippedLog + group.groupId;
            skippedLog = skippedLog + " result=DISABLED_SKIPPED";
            LogInfo(skippedLog);
        }

        string resolvedLog = EVENT_LOG_PREFIX;
        resolvedLog = resolvedLog + " EVENT_RESOLVED eventId=";
        resolvedLog = resolvedLog + eventId;
        resolvedLog = resolvedLog + " groups=";
        resolvedLog = resolvedLog + BuildGroupIdList(runnableGroups);
        LogInfo(resolvedLog);

        if (runnableGroups.Count() == 0)
        {
            string noRunnableLog = EVENT_LOG_PREFIX;
            noRunnableLog = noRunnableLog + " EVENT_START_REQUEST eventId=";
            noRunnableLog = noRunnableLog + eventId;
            noRunnableLog = noRunnableLog + " result=NO_RUNNABLE_GROUPS";
            LogInfo(noRunnableLog);
            return false;
        }

        return BeginLaunchBatch(runnableGroups, "EVENT", eventId);
    }

    protected bool BeginLaunchBatch(array<ref S77MigrateScenarioConfig> groups, string requestType, string requestId)
    {
        if (m_Stopped || m_Config.enabled != 1)
        {
            string disabledLog = EVENT_LOG_PREFIX;
            disabledLog = disabledLog + " LAUNCH_STARTED requestType=";
            disabledLog = disabledLog + requestType;
            disabledLog = disabledLog + " requestId=";
            disabledLog = disabledLog + requestId;
            disabledLog = disabledLog + " result=DISABLED";
            LogInfo(disabledLog);
            return false;
        }

        if (m_LaunchBusy)
        {
            string busyLog = EVENT_LOG_PREFIX;
            busyLog = busyLog + " LAUNCH_BUSY requestType=";
            busyLog = busyLog + requestType;
            busyLog = busyLog + " requestId=";
            busyLog = busyLog + requestId;
            LogInfo(busyLog);
            return false;
        }

        m_PendingGroups.Clear();
        for (int copyIndex = 0; copyIndex < groups.Count(); copyIndex++)
            m_PendingGroups.Insert(groups.Get(copyIndex));

        m_LaunchBusy = true;
        int delayMs = Math.Round(m_Config.eventDelaySeconds * 1000.0);
        int weatherRequestingGroups = CountWeatherRequestingGroups(m_PendingGroups);
        bool shouldStartWeatherTransition = false;
        string weatherDecision = "SKIP_GLOBAL_DISABLED";

        if (m_Config.weatherEnabled == 1)
        {
            weatherDecision = "SKIP_NO_GROUP_REQUEST";
            if (weatherRequestingGroups > 0)
            {
                shouldStartWeatherTransition = true;
                weatherDecision = "TRANSITION";
            }
        }

        string weatherLog = EVENT_LOG_PREFIX;
        weatherLog = weatherLog + " WEATHER_DECISION globalEnabled=";
        weatherLog = weatherLog + m_Config.weatherEnabled.ToString();
        weatherLog = weatherLog + " requestingGroups=";
        weatherLog = weatherLog + weatherRequestingGroups.ToString();
        weatherLog = weatherLog + " batchGroups=";
        weatherLog = weatherLog + m_PendingGroups.Count().ToString();
        weatherLog = weatherLog + " result=";
        weatherLog = weatherLog + weatherDecision;
        LogInfo(weatherLog);

        string startedLog = EVENT_LOG_PREFIX;
        startedLog = startedLog + " LAUNCH_STARTED requestType=";
        startedLog = startedLog + requestType;
        startedLog = startedLog + " requestId=";
        startedLog = startedLog + requestId;
        startedLog = startedLog + " groups=";
        startedLog = startedLog + BuildGroupIdList(m_PendingGroups);
        startedLog = startedLog + " delaySeconds=";
        startedLog = startedLog + m_Config.eventDelaySeconds.ToString();
        LogInfo(startedLog);

        if (shouldStartWeatherTransition)
        {
            m_WeatherStartScheduled = true;
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginWeatherTransition, delayMs, false);
        }
        else
        {
            m_StartGroupsScheduled = true;
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartPendingGroups, delayMs, false);
        }

        return true;
    }

    protected int CountWeatherRequestingGroups(array<ref S77MigrateScenarioConfig> groups)
    {
        int requestingGroups = 0;
        for (int groupIndex = 0; groupIndex < groups.Count(); groupIndex++)
        {
            S77MigrateScenarioConfig group = groups.Get(groupIndex);
            if (group && group.weatherChangeEnabled == 1)
                requestingGroups++;
        }

        return requestingGroups;
    }

    protected string BuildGroupIdList(array<ref S77MigrateScenarioConfig> groups)
    {
        string result = "[";
        for (int i = 0; i < groups.Count(); i++)
        {
            if (i > 0)
                result = result + ",";

            S77MigrateScenarioConfig group = groups.Get(i);
            if (group)
                result = result + group.groupId;
        }

        result = result + "]";
        return result;
    }

    protected void BeginWeatherTransition()
    {
        m_WeatherStartScheduled = false;
        if (m_Stopped || !m_LaunchBusy)
            return;

        Weather weather = GetGame().GetWeather();
        if (!weather)
        {
            LogError(EVENT_LOG_PREFIX + " ERROR: Weather API is unavailable; event start aborted");
            m_PendingGroups.Clear();
            m_LaunchBusy = false;
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
        weatherLog = weatherLog + " Weather transition started duration=";
        weatherLog = weatherLog + transitionSeconds.ToString();
        weatherLog = weatherLog + " overcast=";
        weatherLog = weatherLog + m_Config.weatherOvercast.ToString();
        weatherLog = weatherLog + " fog=";
        weatherLog = weatherLog + m_Config.weatherFog.ToString();
        weatherLog = weatherLog + " wind=";
        weatherLog = weatherLog + windSpeed.ToString();
        weatherLog = weatherLog + " rain=";
        weatherLog = weatherLog + m_Config.weatherRain.ToString();
        weatherLog = weatherLog + " stormFinalDensity=";
        weatherLog = weatherLog + m_Config.weatherStormDensity.ToString();
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
        string logMessage484 = EVENT_LOG_PREFIX;
        logMessage484 = logMessage484 + " Storm ramp scheduled after=";
        logMessage484 = logMessage484 + rampDelaySeconds.ToString();
        logMessage484 = logMessage484 + " seconds duration=";
        logMessage484 = logMessage484 + m_EffectiveStormRampSeconds.ToString();
        logMessage484 = logMessage484 + " step=";
        logMessage484 = logMessage484 + (STORM_RAMP_STEP_MS / 1000).ToString();
        logMessage484 = logMessage484 + " seconds";
        LogInfo(logMessage484);
    }

    protected void BeginStormRamp()
    {
        m_StormRampStartScheduled = false;
        if (m_Stopped || !m_WeatherControlActive || !m_LaunchBusy)
            return;

        m_StormRampStartTime = GetGame().GetTime();
        ApplyStormDensity(0.0, 0.0);
        m_StormRampUpdateScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateStormRamp, STORM_RAMP_STEP_MS, true);
    }

    protected void UpdateStormRamp()
    {
        bool launchInactive = m_Stopped || !m_WeatherControlActive || !m_LaunchBusy;
        if (launchInactive || m_EffectiveStormRampSeconds <= 0.0)
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
        string logMessage524 = EVENT_LOG_PREFIX;
        logMessage524 = logMessage524 + " Storm ramp progress=";
        logMessage524 = logMessage524 + progress.ToString();
        logMessage524 = logMessage524 + " density=";
        logMessage524 = logMessage524 + density.ToString();
        LogInfo(logMessage524);
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

        LogInfo(EVENT_LOG_PREFIX + " Weather transition completed; starting current migration batch");
        StartPendingGroups();
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
            string logMessage561 = EVENT_LOG_PREFIX;
            logMessage561 = logMessage561 + " Weather control released; previous MissionWeather state restored=";
            logMessage561 = logMessage561 + m_PreviousMissionWeather.ToString();
            LogInfo(logMessage561);
        }

        m_WeatherControlActive = false;
    }

    protected bool NeedsAIStimulus()
    {
        for (int i = 0; i < m_GroupConfigs.Count(); i++)
        {
            S77MigrateScenarioConfig group = m_GroupConfigs.Get(i);
            if (!group)
                continue;

            bool finalStimulusNeeded = group.finalActivationEnabled == 1;
            bool stuckStimulusNeeded = group.stuckRecoveryEnabled == 1;
            bool watchdogStimulusNeeded = group.routeProgressWatchdogEnabled == 1;
            bool routeStimulusNeeded = group.routeActivationEnabled == 1 && group.routePoints.Count() > 0;
            if (finalStimulusNeeded || stuckStimulusNeeded || watchdogStimulusNeeded || routeStimulusNeeded)
                return true;
        }

        return false;
    }

    protected void PrepareAIStimulus()
    {
        m_StimulusReady = false;

        if (!GetGame().ConfigIsExisting(AI_STIMULUS_NOISE_PATH))
        {
            string logMessage585 = EVENT_LOG_PREFIX;
            logMessage585 = logMessage585 + " ERROR: AI stimulus preset is unavailable: ";
            logMessage585 = logMessage585 + AI_STIMULUS_NOISE_PATH;
            logMessage585 = logMessage585 + "; migration and configured arrival behavior remain active without AI stimulus";
            LogError(logMessage585);
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
        string logMessage599 = EVENT_LOG_PREFIX;
        logMessage599 = logMessage599 + " AI stimulus ready preset=";
        logMessage599 = logMessage599 + AI_STIMULUS_NOISE_PATH;
        LogInfo(logMessage599);
    }

    protected void StartPendingGroups()
    {
        m_StartGroupsScheduled = false;
        if (m_Stopped || !m_LaunchBusy)
            return;

        int beforeCount = m_Units.Count();
        int batchGroupCount = m_PendingGroups.Count();
        for (int groupIndex = 0; groupIndex < m_PendingGroups.Count(); groupIndex++)
            StartGroupBlueprint(m_PendingGroups.Get(groupIndex));

        int batchSpawnedCount = m_Units.Count() - beforeCount;

        string logMessage618 = EVENT_LOG_PREFIX;
        logMessage618 = logMessage618 + " Current launch batch completed; groups=";
        logMessage618 = logMessage618 + batchGroupCount.ToString();
        logMessage618 = logMessage618 + " spawned=";
        logMessage618 = logMessage618 + batchSpawnedCount.ToString();
        LogInfo(logMessage618);
        m_PendingGroups.Clear();
        m_LaunchBusy = false;

        if (m_Units.Count() == 0)
            return;

        if (!m_RouteUpdateScheduled)
        {
            m_RouteUpdateScheduled = true;
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateRoutes, UPDATE_INTERVAL_MS, true);
        }

        if (m_Config.loggingEnabled == 1 && !m_LogScheduled)
        {
            m_LogScheduled = true;
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LogUnitStates, LOG_CHECK_INTERVAL_MS, true);
            LogUnitStates();
        }
    }

    protected void StartGroupBlueprint(S77MigrateScenarioConfig config)
    {
        vector spawnCenter = config.GetSpawnPosition();
        vector targetCenter = config.GetTargetPosition();
        TVectorArray routePoints = config.GetRoutePoints();
        m_NextRuntimeGroupSerial++;
        string runtimeGroupId = config.groupId + "_RUN_" + m_NextRuntimeGroupSerial.ToString();
        S77MigrateGroupState groupState = new S77MigrateGroupState(runtimeGroupId, config, routePoints);
        m_Groups.Insert(groupState);
        float spawnFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        float targetFormationRotation = Math.RandomFloatInclusive(0.0, 360.0);
        string scenarioPrefix = MIGRATION_LOG_PREFIX;
        scenarioPrefix = scenarioPrefix + " groupId=";
        scenarioPrefix = scenarioPrefix + config.groupId;
        scenarioPrefix = scenarioPrefix + " runtimeGroupId=";
        scenarioPrefix = scenarioPrefix + runtimeGroupId;

        string formationLog = scenarioPrefix;
        formationLog = formationLog + " Formation spawnSpacing=";
        formationLog = formationLog + config.spawnFormationSpacing.ToString();
        formationLog = formationLog + " spawnJitter=";
        formationLog = formationLog + config.spawnFormationJitter.ToString();
        formationLog = formationLog + " spawnRotation=";
        formationLog = formationLog + spawnFormationRotation.ToString();
        formationLog = formationLog + " targetSpacing=";
        formationLog = formationLog + config.targetFormationSpacing.ToString();
        formationLog = formationLog + " targetJitter=";
        formationLog = formationLog + config.targetFormationJitter.ToString();
        formationLog = formationLog + " targetRotation=";
        formationLog = formationLog + targetFormationRotation.ToString();
        formationLog = formationLog + " routePoints=";
        formationLog = formationLog + routePoints.Count().ToString();
        formationLog = formationLog + " lifetimeSeconds=";
        formationLog = formationLog + config.groupLifetimeSeconds.ToString();
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
        string logMessage666 = scenarioPrefix;
        logMessage666 = logMessage666 + " started; spawned=";
        logMessage666 = logMessage666 + spawnedCount.ToString();
        logMessage666 = logMessage666 + "/";
        logMessage666 = logMessage666 + config.infectedCount.ToString();
        LogInfo(logMessage666);
    }

    protected void SpawnMigrationInfected(S77MigrateScenarioConfig config, S77MigrateGroupState groupState, int index, string className, vector spawnPosition, vector finalTargetCenter, vector routeOffset, TVectorArray routePoints)
    {
        string scenarioPrefix = MIGRATION_LOG_PREFIX;
        scenarioPrefix = scenarioPrefix + " groupId=";
        scenarioPrefix = scenarioPrefix + config.groupId;
        scenarioPrefix = scenarioPrefix + " runtimeGroupId=";
        scenarioPrefix = scenarioPrefix + groupState.m_RuntimeGroupId;
        if (!GetGame().ConfigIsExisting("CfgVehicles " + className) || !GetGame().IsKindOf(className, "DayZInfected"))
        {
            string logMessage674 = scenarioPrefix;
            logMessage674 = logMessage674 + " ERROR: invalid infected class: ";
            logMessage674 = logMessage674 + className;
            LogError(logMessage674);
            return;
        }

        int createFlags = ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS;
        DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx(className, spawnPosition, createFlags));

        if (!infected)
        {
            string logMessage683 = scenarioPrefix;
            logMessage683 = logMessage683 + " ERROR: CreateObjectEx failed for class=";
            logMessage683 = logMessage683 + className;
            logMessage683 = logMessage683 + " position=";
            logMessage683 = logMessage683 + spawnPosition.ToString();
            LogError(logMessage683);
            return;
        }

        infected.SetLifetimeMax(config.groupLifetimeSeconds);
        infected.SetLifetime(config.groupLifetimeSeconds);

        string infectedId = "INF_" + (index + 1).ToString();
        int logIntervalMs = Math.Round(config.logIntervalSeconds * 1000.0);
        vector migrationTarget = finalTargetCenter + routeOffset;
        S77MigrateUnitState state = new S77MigrateUnitState(infected, infectedId, config.groupId, groupState.m_RuntimeGroupId, migrationTarget, finalTargetCenter, routeOffset, routePoints, config.routePointReachRadius, config, logIntervalMs);
        m_Units.Insert(state);
        groupState.m_Members.Insert(state);

        BuildPath(state);
        string logMessage695 = scenarioPrefix;
        logMessage695 = logMessage695 + " SPAWNED id=";
        logMessage695 = logMessage695 + infectedId;
        logMessage695 = logMessage695 + " class=";
        logMessage695 = logMessage695 + className;
        logMessage695 = logMessage695 + " position=";
        logMessage695 = logMessage695 + infected.GetPosition().ToString();
        logMessage695 = logMessage695 + " target=";
        logMessage695 = logMessage695 + migrationTarget.ToString();
        logMessage695 = logMessage695 + " routePoints=";
        logMessage695 = logMessage695 + routePoints.Count().ToString();
        LogInfo(logMessage695);
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
            string logMessage710 = MIGRATION_LOG_PREFIX;
            logMessage710 = logMessage710 + " groupId=";
            logMessage710 = logMessage710 + state.m_GroupId;
            logMessage710 = logMessage710 + " runtimeGroupId=";
            logMessage710 = logMessage710 + state.m_RuntimeGroupId;
            logMessage710 = logMessage710 + " id=";
            logMessage710 = logMessage710 + state.m_InfectedId;
            logMessage710 = logMessage710 + " ERROR: AIWorld is unavailable";
            LogError(logMessage710);
            return false;
        }

        vector sampledStart;
        vector sampledTarget;
        vector currentPosition = state.m_Infected.GetPosition();
        vector currentRouteTarget = GetCurrentRouteTarget(state);

        if (!aiWorld.SampleNavmeshPosition(currentPosition, 8.0, m_PathFilter, sampledStart))
        {
            string logMessage721 = MIGRATION_LOG_PREFIX;
            logMessage721 = logMessage721 + " PATH_ERROR groupId=";
            logMessage721 = logMessage721 + state.m_GroupId;
            logMessage721 = logMessage721 + " runtimeGroupId=";
            logMessage721 = logMessage721 + state.m_RuntimeGroupId;
            logMessage721 = logMessage721 + " id=";
            logMessage721 = logMessage721 + state.m_InfectedId;
            logMessage721 = logMessage721 + " reason=NO_NAVMESH_START position=";
            logMessage721 = logMessage721 + currentPosition.ToString();
            LogError(logMessage721);
            return false;
        }

        if (!aiWorld.SampleNavmeshPosition(currentRouteTarget, 15.0, m_PathFilter, sampledTarget))
        {
            string logMessage727 = MIGRATION_LOG_PREFIX;
            logMessage727 = logMessage727 + " PATH_ERROR groupId=";
            logMessage727 = logMessage727 + state.m_GroupId;
            logMessage727 = logMessage727 + " runtimeGroupId=";
            logMessage727 = logMessage727 + state.m_RuntimeGroupId;
            logMessage727 = logMessage727 + " id=";
            logMessage727 = logMessage727 + state.m_InfectedId;
            logMessage727 = logMessage727 + " reason=NO_NAVMESH_TARGET target=";
            logMessage727 = logMessage727 + currentRouteTarget.ToString();
            LogError(logMessage727);
            return false;
        }

        TVectorArray newWaypoints = new TVectorArray();
        if (!aiWorld.FindPath(sampledStart, sampledTarget, m_PathFilter, newWaypoints) || newWaypoints.Count() < 2)
        {
            string logMessage734 = MIGRATION_LOG_PREFIX;
            logMessage734 = logMessage734 + " PATH_ERROR groupId=";
            logMessage734 = logMessage734 + state.m_GroupId;
            logMessage734 = logMessage734 + " runtimeGroupId=";
            logMessage734 = logMessage734 + state.m_RuntimeGroupId;
            logMessage734 = logMessage734 + " id=";
            logMessage734 = logMessage734 + state.m_InfectedId;
            logMessage734 = logMessage734 + " reason=FIND_PATH_FAILED from=";
            logMessage734 = logMessage734 + sampledStart.ToString();
            logMessage734 = logMessage734 + " to=";
            logMessage734 = logMessage734 + sampledTarget.ToString();
            LogError(logMessage734);
            return false;
        }

        state.m_Waypoints = newWaypoints;
        state.m_WaypointIndex = 1;
        state.m_NextPathRetryTime = 0;
        ResetStuckSample(state, "BUILD_PATH");
        ScheduleRouteProgressBaseline(state, GetGame().GetTime());
        string logMessage743 = MIGRATION_LOG_PREFIX;
        logMessage743 = logMessage743 + " BUILD_PATH groupId=";
        logMessage743 = logMessage743 + state.m_GroupId;
        logMessage743 = logMessage743 + " runtimeGroupId=";
        logMessage743 = logMessage743 + state.m_RuntimeGroupId;
        logMessage743 = logMessage743 + " id=";
        logMessage743 = logMessage743 + state.m_InfectedId;
        logMessage743 = logMessage743 + " mode=";
        logMessage743 = logMessage743 + state.m_Mode;
        logMessage743 = logMessage743 + " routePoint=";
        logMessage743 = logMessage743 + state.m_RoutePointIndex.ToString();
        logMessage743 = logMessage743 + "/";
        logMessage743 = logMessage743 + state.m_RoutePoints.Count().ToString();
        logMessage743 = logMessage743 + " waypoints=";
        logMessage743 = logMessage743 + newWaypoints.Count().ToString();
        logMessage743 = logMessage743 + " target=";
        logMessage743 = logMessage743 + currentRouteTarget.ToString();
        LogInfo(logMessage743);
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
                    string logMessage778 = MIGRATION_LOG_PREFIX;
                    logMessage778 = logMessage778 + " DEATH groupId=";
                    logMessage778 = logMessage778 + state.m_GroupId;
                    logMessage778 = logMessage778 + " runtimeGroupId=";
                    logMessage778 = logMessage778 + state.m_RuntimeGroupId;
                    logMessage778 = logMessage778 + " id=";
                    logMessage778 = logMessage778 + state.m_InfectedId;
                    logMessage778 = logMessage778 + " mode=";
                    logMessage778 = logMessage778 + state.m_Mode;
                    logMessage778 = logMessage778 + " position=";
                    logMessage778 = logMessage778 + state.m_Infected.GetPosition().ToString();
                    LogInfo(logMessage778);
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

            if (IsVanillaBusy(controller))
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
        if (firstHandoff)
            state.m_RecoveryResumeMode = GetMovementIntent(state);
        state.m_Mode = "AGGRO";
        state.m_VanillaBusy = true;
        state.m_ResumeAfterTime = 0;
        ResetStuckSample(state, "AGGRO");
        SuspendRouteProgressWatchdog(state);

        if (firstHandoff)
        {
            string logMessage852 = MIGRATION_LOG_PREFIX;
            logMessage852 = logMessage852 + " VANILLA_AGGRO_HANDOFF groupId=";
            logMessage852 = logMessage852 + state.m_GroupId;
            logMessage852 = logMessage852 + " runtimeGroupId=";
            logMessage852 = logMessage852 + state.m_RuntimeGroupId;
            logMessage852 = logMessage852 + " id=";
            logMessage852 = logMessage852 + state.m_InfectedId;
            logMessage852 = logMessage852 + " mode=";
            logMessage852 = logMessage852 + state.m_Mode;
            logMessage852 = logMessage852 + " resumeMode=";
            logMessage852 = logMessage852 + state.m_RecoveryResumeMode;
            logMessage852 = logMessage852 + " position=";
            logMessage852 = logMessage852 + state.m_Infected.GetPosition().ToString();
            LogInfo(logMessage852);
        }
    }

    protected bool ResumeAfterVanillaBusy(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);

        if (IsVanillaBusy(controller))
        {
            state.m_ResumeAfterTime = 0;
            return false;
        }

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
        {
            string logMessage873 = MIGRATION_LOG_PREFIX;
            logMessage873 = logMessage873 + " VANILLA_AGGRO_RESUME groupId=";
            logMessage873 = logMessage873 + state.m_GroupId;
            logMessage873 = logMessage873 + " runtimeGroupId=";
            logMessage873 = logMessage873 + state.m_RuntimeGroupId;
            logMessage873 = logMessage873 + " id=";
            logMessage873 = logMessage873 + state.m_InfectedId;
            logMessage873 = logMessage873 + " mode=";
            logMessage873 = logMessage873 + state.m_Mode;
            logMessage873 = logMessage873 + " position=";
            logMessage873 = logMessage873 + state.m_Infected.GetPosition().ToString();
            logMessage873 = logMessage873 + " target=";
            logMessage873 = logMessage873 + GetCurrentRouteTarget(state).ToString();
            LogInfo(logMessage873);
        }

        return pathBuilt;
    }

    protected string GetMovementIntent(S77MigrateUnitState state)
    {
        if (state && state.m_Mode == "RETURN_TO_HOLD")
            return "RETURN_TO_HOLD";

        return "MIGRATION";
    }

    protected bool IsVanillaBusy(DayZInfectedInputController controller)
    {
        if (!controller)
            return true;

        EntityAI vanillaTarget = controller.GetTargetEntity();
        if (vanillaTarget)
            return true;

        int mindState = controller.GetMindState();
        return mindState != DayZInfectedConstants.MINDSTATE_CALM;
    }

    protected void UpdateStuckRecovery(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        if (state.m_RecoveryPhase == "REVERSE_ACTIVE")
        {
            UpdateReverseEscape(state, controller, now);
            return;
        }

        if (state.m_RecoveryPhase == "REVERSE_WAIT")
        {
            TryStartReverseEscape(state, controller, now);
            return;
        }

        if (state.m_RecoveryPhase == "POST_REVERSE_FREE")
        {
            UpdatePostReverseFree(state, controller, now);
            return;
        }

        ReleaseRouteControl(controller);

        if (now < state.m_RecoveryFreeUntilTime)
            return;

        if (now < state.m_NextRecoveryStatusCheckTime)
            return;

        state.m_NextRecoveryStatusCheckTime = now + Math.Round(state.m_StuckRecoveryStatusCheckSeconds * 1000.0);

        if (IsVanillaBusy(controller))
        {
            state.m_RecoveryCalmAfterTime = 0;
            LogRecoveryWaitBusy(state, "STUCK_RECOVERY_WAIT_BUSY", state.m_StuckRecoveryStatusCheckSeconds);
            return;
        }

        if (state.m_RecoveryCalmAfterTime == 0)
        {
            state.m_RecoveryCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            LogRecoveryCalm(state);
            return;
        }

        if (now < state.m_RecoveryCalmAfterTime)
            return;

        vector currentPosition = state.m_Infected.GetPosition();
        float movedDistance = HorizontalDistance(state.m_RecoveryStartPosition, currentPosition);
        string movementResult = "NO_MOVEMENT";
        if (movedDistance >= state.m_StuckMinMovementMeters)
            movementResult = "MOVED";

        string movementLog = MIGRATION_LOG_PREFIX;
        movementLog = movementLog + " STUCK_RECOVERY_MOVEMENT_CHECK groupId=";
        movementLog = movementLog + state.m_GroupId;
        movementLog = movementLog + " runtimeGroupId=";
        movementLog = movementLog + state.m_RuntimeGroupId;
        movementLog = movementLog + " id=";
        movementLog = movementLog + state.m_InfectedId;
        movementLog = movementLog + " startPosition=";
        movementLog = movementLog + state.m_RecoveryStartPosition.ToString();
        movementLog = movementLog + " currentPosition=";
        movementLog = movementLog + currentPosition.ToString();
        movementLog = movementLog + " movedXZ=";
        movementLog = movementLog + movedDistance.ToString();
        movementLog = movementLog + " threshold=";
        movementLog = movementLog + state.m_StuckMinMovementMeters.ToString();
        movementLog = movementLog + " result=";
        movementLog = movementLog + movementResult;
        LogInfo(movementLog);

        if (movedDistance >= state.m_StuckMinMovementMeters || state.m_StuckReverseEnabled != 1)
        {
            ResumeStuckRecovery(state, controller, "STUCK_RECOVERY_RESUME");
            return;
        }

        state.m_RecoveryPhase = "REVERSE_WAIT";
        state.m_RecoveryCalmAfterTime = now;
        state.m_NextReverseAttemptTime = now;
        TryStartReverseEscape(state, controller, now);
    }

    protected void TryStartReverseEscape(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);

        if (now < state.m_NextReverseAttemptTime)
            return;

        if (IsVanillaBusy(controller))
        {
            state.m_RecoveryCalmAfterTime = 0;
            state.m_NextReverseAttemptTime = now + Math.Round(state.m_StuckReverseRetrySeconds * 1000.0);
            LogRecoveryWaitBusy(state, "STUCK_REVERSE_WAIT_BUSY", state.m_StuckReverseRetrySeconds);
            return;
        }

        if (state.m_RecoveryCalmAfterTime == 0)
        {
            state.m_RecoveryCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            state.m_NextReverseAttemptTime = state.m_RecoveryCalmAfterTime;
            return;
        }

        if (now < state.m_RecoveryCalmAfterTime)
            return;

        vector currentPosition = state.m_Infected.GetPosition();
        vector forwardDirection = state.m_RecoveryForwardDirection;
        forwardDirection[1] = 0.0;
        if (forwardDirection.Length() <= 0.01)
        {
            vector logicalTarget = GetCurrentRouteTarget(state);
            forwardDirection = vector.Direction(currentPosition, logicalTarget);
            forwardDirection[1] = 0.0;
        }

        if (forwardDirection.Length() > 0.01)
            forwardDirection.Normalize();

        vector reverseDirection = forwardDirection * -1.0;
        vector reverseTarget = currentPosition + reverseDirection * state.m_StuckReverseDistanceMeters;
        reverseTarget[1] = currentPosition[1];

        state.m_RecoveryForwardDirection = forwardDirection;
        state.m_ReverseStartPosition = currentPosition;
        state.m_ReverseTarget = reverseTarget;
        state.m_ReverseStartedTime = now;
        state.m_NextReverseAttemptTime = 0;
        state.m_RecoveryPhase = "REVERSE_ACTIVE";

        float targetDistance = HorizontalDistance(currentPosition, reverseTarget);
        string startLog = MIGRATION_LOG_PREFIX;
        startLog = startLog + " STUCK_REVERSE_START groupId=";
        startLog = startLog + state.m_GroupId;
        startLog = startLog + " runtimeGroupId=";
        startLog = startLog + state.m_RuntimeGroupId;
        startLog = startLog + " id=";
        startLog = startLog + state.m_InfectedId;
        startLog = startLog + " position=";
        startLog = startLog + currentPosition.ToString();
        startLog = startLog + " forwardDirection=";
        startLog = startLog + forwardDirection.ToString();
        startLog = startLog + " reverseDirection=";
        startLog = startLog + reverseDirection.ToString();
        startLog = startLog + " reverseTarget=";
        startLog = startLog + reverseTarget.ToString();
        startLog = startLog + " maxSeconds=";
        startLog = startLog + state.m_StuckReverseMaxSeconds.ToString();
        startLog = startLog + " targetDistance=";
        startLog = startLog + targetDistance.ToString();
        LogInfo(startLog);
    }

    protected void UpdateReverseEscape(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        if (IsVanillaBusy(controller))
        {
            ReleaseRouteControl(controller);
            state.m_RecoveryPhase = "REVERSE_WAIT";
            state.m_RecoveryCalmAfterTime = 0;
            state.m_ReverseStartedTime = 0;
            state.m_NextReverseAttemptTime = now + Math.Round(state.m_StuckReverseRetrySeconds * 1000.0);
            LogRecoveryWaitBusy(state, "STUCK_REVERSE_WAIT_BUSY", state.m_StuckReverseRetrySeconds);
            return;
        }

        vector currentPosition = state.m_Infected.GetPosition();
        float movedDistance = HorizontalDistance(state.m_ReverseStartPosition, currentPosition);
        float elapsedSeconds = (now - state.m_ReverseStartedTime) / 1000.0;
        bool movedEnough = movedDistance >= state.m_StuckMinMovementMeters;
        bool timedOut = elapsedSeconds >= state.m_StuckReverseMaxSeconds;

        if (movedEnough || timedOut)
        {
            FinishReverseEscape(state, controller, now, movedEnough);
            return;
        }

        vector reverseDirection = vector.Direction(currentPosition, state.m_ReverseTarget);
        reverseDirection[1] = 0.0;
        if (reverseDirection.Length() <= 0.01)
            return;

        reverseDirection.Normalize();
        float headingRadians = reverseDirection.VectorToAngles()[0] * Math.DEG2RAD;
        controller.OverrideHeading(true, headingRadians);
        controller.OverrideMovementSpeed(true, MIGRATION_SPEED);
    }

    protected void FinishReverseEscape(S77MigrateUnitState state, DayZInfectedInputController controller, int now, bool movedEnough)
    {
        ReleaseRouteControl(controller);

        vector endPosition = state.m_Infected.GetPosition();
        float movedDistance = HorizontalDistance(state.m_ReverseStartPosition, endPosition);
        float elapsedSeconds = (now - state.m_ReverseStartedTime) / 1000.0;
        string result = "TIMEOUT";
        if (movedEnough)
            result = "MOVED";

        string endLog = MIGRATION_LOG_PREFIX;
        endLog = endLog + " STUCK_REVERSE_END groupId=";
        endLog = endLog + state.m_GroupId;
        endLog = endLog + " runtimeGroupId=";
        endLog = endLog + state.m_RuntimeGroupId;
        endLog = endLog + " id=";
        endLog = endLog + state.m_InfectedId;
        endLog = endLog + " startPosition=";
        endLog = endLog + state.m_ReverseStartPosition.ToString();
        endLog = endLog + " endPosition=";
        endLog = endLog + endPosition.ToString();
        endLog = endLog + " movedXZ=";
        endLog = endLog + movedDistance.ToString();
        endLog = endLog + " elapsedSeconds=";
        endLog = endLog + elapsedSeconds.ToString();
        endLog = endLog + " result=";
        endLog = endLog + result;
        LogInfo(endLog);

        state.m_RecoveryPhase = "POST_REVERSE_FREE";
        state.m_PostReverseFreeUntilTime = now + Math.Round(state.m_StuckPostReverseFreeSeconds * 1000.0);
        state.m_NextRecoveryStatusCheckTime = state.m_PostReverseFreeUntilTime;
        state.m_RecoveryCalmAfterTime = 0;
        state.m_ReverseStartedTime = 0;

        string freeLog = MIGRATION_LOG_PREFIX;
        freeLog = freeLog + " STUCK_POST_REVERSE_FREE groupId=";
        freeLog = freeLog + state.m_GroupId;
        freeLog = freeLog + " runtimeGroupId=";
        freeLog = freeLog + state.m_RuntimeGroupId;
        freeLog = freeLog + " id=";
        freeLog = freeLog + state.m_InfectedId;
        freeLog = freeLog + " freeSeconds=";
        freeLog = freeLog + state.m_StuckPostReverseFreeSeconds.ToString();
        LogInfo(freeLog);
    }

    protected void UpdatePostReverseFree(S77MigrateUnitState state, DayZInfectedInputController controller, int now)
    {
        ReleaseRouteControl(controller);

        if (now < state.m_PostReverseFreeUntilTime)
            return;

        if (now < state.m_NextRecoveryStatusCheckTime)
            return;

        state.m_NextRecoveryStatusCheckTime = now + Math.Round(state.m_StuckRecoveryStatusCheckSeconds * 1000.0);

        if (IsVanillaBusy(controller))
        {
            state.m_RecoveryCalmAfterTime = 0;
            LogRecoveryWaitBusy(state, "STUCK_RECOVERY_WAIT_BUSY", state.m_StuckRecoveryStatusCheckSeconds);
            return;
        }

        if (state.m_RecoveryCalmAfterTime == 0)
        {
            state.m_RecoveryCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            LogRecoveryCalm(state);
            return;
        }

        if (now < state.m_RecoveryCalmAfterTime)
            return;

        ResumeStuckRecovery(state, controller, "STUCK_REVERSE_RESUME");
    }

    protected bool ResumeStuckRecovery(S77MigrateUnitState state, DayZInfectedInputController controller, string eventName)
    {
        ReleaseRouteControl(controller);
        if (IsVanillaBusy(controller))
        {
            state.m_RecoveryCalmAfterTime = 0;
            return false;
        }

        state.m_Mode = state.m_RecoveryResumeMode;
        if (state.m_Mode != "RETURN_TO_HOLD")
            state.m_Mode = "MIGRATION";

        state.m_VanillaBusy = false;
        state.m_RecoveryFreeUntilTime = 0;
        state.m_NextRecoveryStatusCheckTime = 0;
        state.m_RecoveryCalmAfterTime = 0;
        state.m_RecoveryPhase = "NONE";
        state.m_ReverseStartedTime = 0;
        state.m_NextReverseAttemptTime = 0;
        state.m_PostReverseFreeUntilTime = 0;
        ResetStuckSample(state, eventName);

        bool pathBuilt = BuildPath(state);
        if (pathBuilt)
        {
            string resumeLog = MIGRATION_LOG_PREFIX;
            resumeLog = resumeLog + " ";
            resumeLog = resumeLog + eventName;
            resumeLog = resumeLog + " groupId=";
            resumeLog = resumeLog + state.m_GroupId;
            resumeLog = resumeLog + " runtimeGroupId=";
            resumeLog = resumeLog + state.m_RuntimeGroupId;
            resumeLog = resumeLog + " id=";
            resumeLog = resumeLog + state.m_InfectedId;
            resumeLog = resumeLog + " mode=";
            resumeLog = resumeLog + state.m_Mode;
            resumeLog = resumeLog + " position=";
            resumeLog = resumeLog + state.m_Infected.GetPosition().ToString();
            resumeLog = resumeLog + " target=";
            resumeLog = resumeLog + GetCurrentRouteTarget(state).ToString();
            LogInfo(resumeLog);
        }

        return pathBuilt;
    }

    protected void LogRecoveryWaitBusy(S77MigrateUnitState state, string eventName, float nextCheckSeconds)
    {
        string waitLog = MIGRATION_LOG_PREFIX;
        waitLog = waitLog + " ";
        waitLog = waitLog + eventName;
        waitLog = waitLog + " groupId=";
        waitLog = waitLog + state.m_GroupId;
        waitLog = waitLog + " runtimeGroupId=";
        waitLog = waitLog + state.m_RuntimeGroupId;
        waitLog = waitLog + " id=";
        waitLog = waitLog + state.m_InfectedId;
        waitLog = waitLog + " mode=";
        waitLog = waitLog + state.m_Mode;
        waitLog = waitLog + " phase=";
        waitLog = waitLog + state.m_RecoveryPhase;
        waitLog = waitLog + " nextCheckSeconds=";
        waitLog = waitLog + nextCheckSeconds.ToString();
        LogInfo(waitLog);
    }

    protected void LogRecoveryCalm(S77MigrateUnitState state)
    {
        string calmLog = MIGRATION_LOG_PREFIX;
        calmLog = calmLog + " STUCK_RECOVERY_CALM groupId=";
        calmLog = calmLog + state.m_GroupId;
        calmLog = calmLog + " runtimeGroupId=";
        calmLog = calmLog + state.m_RuntimeGroupId;
        calmLog = calmLog + " id=";
        calmLog = calmLog + state.m_InfectedId;
        calmLog = calmLog + " mode=";
        calmLog = calmLog + state.m_Mode;
        calmLog = calmLog + " phase=";
        calmLog = calmLog + state.m_RecoveryPhase;
        calmLog = calmLog + " cooldownMs=";
        calmLog = calmLog + AGGRO_COOLDOWN_MS.ToString();
        LogInfo(calmLog);
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

        if (IsVanillaBusy(controller))
        {
            state.m_HoldCalmAfterTime = 0;
            string logMessage956 = MIGRATION_LOG_PREFIX;
            logMessage956 = logMessage956 + " HOLD_RETURN_WAIT_BUSY groupId=";
            logMessage956 = logMessage956 + state.m_GroupId;
            logMessage956 = logMessage956 + " runtimeGroupId=";
            logMessage956 = logMessage956 + state.m_RuntimeGroupId;
            logMessage956 = logMessage956 + " id=";
            logMessage956 = logMessage956 + state.m_InfectedId;
            logMessage956 = logMessage956 + " mode=";
            logMessage956 = logMessage956 + state.m_Mode;
            logMessage956 = logMessage956 + " distance=";
            logMessage956 = logMessage956 + distance.ToString();
            logMessage956 = logMessage956 + " holdRadius=";
            logMessage956 = logMessage956 + state.m_FinalHoldRadius.ToString();
            LogInfo(logMessage956);
            return;
        }

        if (state.m_HoldCalmAfterTime == 0)
        {
            state.m_HoldCalmAfterTime = now + AGGRO_COOLDOWN_MS;
            string logMessage963 = MIGRATION_LOG_PREFIX;
            logMessage963 = logMessage963 + " HOLD_OUTSIDE groupId=";
            logMessage963 = logMessage963 + state.m_GroupId;
            logMessage963 = logMessage963 + " runtimeGroupId=";
            logMessage963 = logMessage963 + state.m_RuntimeGroupId;
            logMessage963 = logMessage963 + " id=";
            logMessage963 = logMessage963 + state.m_InfectedId;
            logMessage963 = logMessage963 + " mode=";
            logMessage963 = logMessage963 + state.m_Mode;
            logMessage963 = logMessage963 + " distance=";
            logMessage963 = logMessage963 + distance.ToString();
            logMessage963 = logMessage963 + " holdRadius=";
            logMessage963 = logMessage963 + state.m_FinalHoldRadius.ToString();
            logMessage963 = logMessage963 + " cooldownMs=";
            logMessage963 = logMessage963 + AGGRO_COOLDOWN_MS.ToString();
            LogInfo(logMessage963);
            return;
        }

        if (now < state.m_HoldCalmAfterTime)
            return;

        StartReturnToHold(state, controller);
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

        string logMessage1001 = MIGRATION_LOG_PREFIX;
        logMessage1001 = logMessage1001 + " STUCK_SAMPLE_CHECK groupId=";
        logMessage1001 = logMessage1001 + state.m_GroupId;
        logMessage1001 = logMessage1001 + " runtimeGroupId=";
        logMessage1001 = logMessage1001 + state.m_RuntimeGroupId;
        logMessage1001 = logMessage1001 + " id=";
        logMessage1001 = logMessage1001 + state.m_InfectedId;
        logMessage1001 = logMessage1001 + " mode=";
        logMessage1001 = logMessage1001 + state.m_Mode;
        logMessage1001 = logMessage1001 + " positionA=";
        logMessage1001 = logMessage1001 + state.m_StuckSamplePosition.ToString();
        logMessage1001 = logMessage1001 + " positionB=";
        logMessage1001 = logMessage1001 + position.ToString();
        logMessage1001 = logMessage1001 + " elapsedSeconds=";
        logMessage1001 = logMessage1001 + elapsedSeconds.ToString();
        logMessage1001 = logMessage1001 + " movedXZ=";
        logMessage1001 = logMessage1001 + movedDistance.ToString();
        logMessage1001 = logMessage1001 + " threshold=";
        logMessage1001 = logMessage1001 + state.m_StuckMinMovementMeters.ToString();
        logMessage1001 = logMessage1001 + " result=";
        logMessage1001 = logMessage1001 + sampleResult;
        LogStuckDebug(logMessage1001);

        if (movedDistance >= state.m_StuckMinMovementMeters)
        {
            SetStuckSample(state, now);
            return false;
        }

        string logMessage1009 = MIGRATION_LOG_PREFIX;
        logMessage1009 = logMessage1009 + " STUCK_DETECTED groupId=";
        logMessage1009 = logMessage1009 + state.m_GroupId;
        logMessage1009 = logMessage1009 + " runtimeGroupId=";
        logMessage1009 = logMessage1009 + state.m_RuntimeGroupId;
        logMessage1009 = logMessage1009 + " id=";
        logMessage1009 = logMessage1009 + state.m_InfectedId;
        logMessage1009 = logMessage1009 + " mode=";
        logMessage1009 = logMessage1009 + state.m_Mode;
        logMessage1009 = logMessage1009 + " positionA=";
        logMessage1009 = logMessage1009 + state.m_StuckSamplePosition.ToString();
        logMessage1009 = logMessage1009 + " positionB=";
        logMessage1009 = logMessage1009 + position.ToString();
        logMessage1009 = logMessage1009 + " elapsedSeconds=";
        logMessage1009 = logMessage1009 + elapsedSeconds.ToString();
        logMessage1009 = logMessage1009 + " movedXZ=";
        logMessage1009 = logMessage1009 + movedDistance.ToString();
        logMessage1009 = logMessage1009 + " threshold=";
        logMessage1009 = logMessage1009 + state.m_StuckMinMovementMeters.ToString();
        logMessage1009 = logMessage1009 + " target=";
        logMessage1009 = logMessage1009 + controlTarget.ToString();
        logMessage1009 = logMessage1009 + " progressDiagnostic=";
        logMessage1009 = logMessage1009 + progress.ToString();
        LogInfo(logMessage1009);
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

        string routeProgressLog = MIGRATION_LOG_PREFIX;
        routeProgressLog = routeProgressLog + " ROUTE_PROGRESS_LOST groupId=";
        routeProgressLog = routeProgressLog + state.m_GroupId;
        routeProgressLog = routeProgressLog + " runtimeGroupId=";
        routeProgressLog = routeProgressLog + state.m_RuntimeGroupId;
        routeProgressLog = routeProgressLog + " id=";
        routeProgressLog = routeProgressLog + state.m_InfectedId;
        routeProgressLog = routeProgressLog + " mode=";
        routeProgressLog = routeProgressLog + state.m_Mode;
        routeProgressLog = routeProgressLog + " logicalTarget=";
        routeProgressLog = routeProgressLog + logicalTarget.ToString();
        routeProgressLog = routeProgressLog + " previous=";
        routeProgressLog = routeProgressLog + previousDistance.ToString();
        routeProgressLog = routeProgressLog + " current=";
        routeProgressLog = routeProgressLog + currentDistance.ToString();
        routeProgressLog = routeProgressLog + " progress=";
        routeProgressLog = routeProgressLog + progress.ToString();
        routeProgressLog = routeProgressLog + " backtrack=";
        routeProgressLog = routeProgressLog + backtrack.ToString();
        routeProgressLog = routeProgressLog + " badChecks=";
        routeProgressLog = routeProgressLog + state.m_RouteProgressBadCheckCount.ToString();
        routeProgressLog = routeProgressLog + " reason=";
        routeProgressLog = routeProgressLog + reason;
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

        BeginStuckRecoveryState(state, controller, now, direction);

        if (direction.Length() <= 0.01)
        {
            string logMessage1114 = MIGRATION_LOG_PREFIX;
            logMessage1114 = logMessage1114 + " STUCK_RECOVERY_STIMULUS_ATTEMPT groupId=";
            logMessage1114 = logMessage1114 + state.m_GroupId;
            logMessage1114 = logMessage1114 + " runtimeGroupId=";
            logMessage1114 = logMessage1114 + state.m_RuntimeGroupId;
            logMessage1114 = logMessage1114 + " id=";
            logMessage1114 = logMessage1114 + state.m_InfectedId;
            logMessage1114 = logMessage1114 + " mode=";
            logMessage1114 = logMessage1114 + state.m_Mode;
            logMessage1114 = logMessage1114 + " cause=";
            logMessage1114 = logMessage1114 + cause;
            logMessage1114 = logMessage1114 + " position=";
            logMessage1114 = logMessage1114 + position.ToString();
            logMessage1114 = logMessage1114 + " target=";
            logMessage1114 = logMessage1114 + stimulusTarget.ToString();
            logMessage1114 = logMessage1114 + " stimulusPosition=UNAVAILABLE forwardDistance=";
            logMessage1114 = logMessage1114 + state.m_StuckStimulusForwardDistance.ToString();
            logMessage1114 = logMessage1114 + " lifetime=";
            logMessage1114 = logMessage1114 + state.m_StuckStimulusLifetimeSeconds.ToString();
            logMessage1114 = logMessage1114 + " strength=";
            logMessage1114 = logMessage1114 + state.m_StuckStimulusStrengthMultiplier.ToString();
            LogInfo(logMessage1114);
            string logMessage1115 = MIGRATION_LOG_PREFIX;
            logMessage1115 = logMessage1115 + " STUCK_RECOVERY_STIMULUS_SKIPPED groupId=";
            logMessage1115 = logMessage1115 + state.m_GroupId;
            logMessage1115 = logMessage1115 + " runtimeGroupId=";
            logMessage1115 = logMessage1115 + state.m_RuntimeGroupId;
            logMessage1115 = logMessage1115 + " id=";
            logMessage1115 = logMessage1115 + state.m_InfectedId;
            logMessage1115 = logMessage1115 + " mode=";
            logMessage1115 = logMessage1115 + state.m_Mode;
            logMessage1115 = logMessage1115 + " cause=";
            logMessage1115 = logMessage1115 + cause;
            logMessage1115 = logMessage1115 + " position=";
            logMessage1115 = logMessage1115 + position.ToString();
            logMessage1115 = logMessage1115 + " controlTarget=";
            logMessage1115 = logMessage1115 + controlTarget.ToString();
            logMessage1115 = logMessage1115 + " logicalTarget=";
            logMessage1115 = logMessage1115 + stimulusTarget.ToString();
            logMessage1115 = logMessage1115 + " reason=NO_USABLE_DIRECTION";
            LogInfo(logMessage1115);
            return;
        }

        direction.Normalize();
        vector stimulusPosition = position + direction * state.m_StuckStimulusForwardDistance;
        stimulusPosition[1] = position[1];
        string logMessage1122 = MIGRATION_LOG_PREFIX;
        logMessage1122 = logMessage1122 + " STUCK_RECOVERY_STIMULUS_ATTEMPT groupId=";
        logMessage1122 = logMessage1122 + state.m_GroupId;
        logMessage1122 = logMessage1122 + " runtimeGroupId=";
        logMessage1122 = logMessage1122 + state.m_RuntimeGroupId;
        logMessage1122 = logMessage1122 + " id=";
        logMessage1122 = logMessage1122 + state.m_InfectedId;
        logMessage1122 = logMessage1122 + " mode=";
        logMessage1122 = logMessage1122 + state.m_Mode;
        logMessage1122 = logMessage1122 + " cause=";
        logMessage1122 = logMessage1122 + cause;
        logMessage1122 = logMessage1122 + " position=";
        logMessage1122 = logMessage1122 + position.ToString();
        logMessage1122 = logMessage1122 + " target=";
        logMessage1122 = logMessage1122 + stimulusTarget.ToString();
        logMessage1122 = logMessage1122 + " stimulusPosition=";
        logMessage1122 = logMessage1122 + stimulusPosition.ToString();
        logMessage1122 = logMessage1122 + " forwardDistance=";
        logMessage1122 = logMessage1122 + state.m_StuckStimulusForwardDistance.ToString();
        logMessage1122 = logMessage1122 + " lifetime=";
        logMessage1122 = logMessage1122 + state.m_StuckStimulusLifetimeSeconds.ToString();
        logMessage1122 = logMessage1122 + " strength=";
        logMessage1122 = logMessage1122 + state.m_StuckStimulusStrengthMultiplier.ToString();
        LogInfo(logMessage1122);

        S77MigrateGroupState groupState = GetGroupState(state);
        if (!groupState)
        {
            string logMessage1127 = MIGRATION_LOG_PREFIX;
            logMessage1127 = logMessage1127 + " STUCK_RECOVERY_STIMULUS_SKIPPED groupId=";
            logMessage1127 = logMessage1127 + state.m_GroupId;
            logMessage1127 = logMessage1127 + " runtimeGroupId=";
            logMessage1127 = logMessage1127 + state.m_RuntimeGroupId;
            logMessage1127 = logMessage1127 + " id=";
            logMessage1127 = logMessage1127 + state.m_InfectedId;
            logMessage1127 = logMessage1127 + " mode=";
            logMessage1127 = logMessage1127 + state.m_Mode;
            logMessage1127 = logMessage1127 + " cause=";
            logMessage1127 = logMessage1127 + cause;
            logMessage1127 = logMessage1127 + " position=";
            logMessage1127 = logMessage1127 + position.ToString();
            logMessage1127 = logMessage1127 + " target=";
            logMessage1127 = logMessage1127 + stimulusTarget.ToString();
            logMessage1127 = logMessage1127 + " stimulusPosition=";
            logMessage1127 = logMessage1127 + stimulusPosition.ToString();
            logMessage1127 = logMessage1127 + " reason=GROUP_NOT_FOUND";
            LogInfo(logMessage1127);
            return;
        }

        bool stimulusCreated = EmitAIStimulus(groupState, stimulusPosition, state.m_StuckStimulusLifetimeSeconds, state.m_StuckStimulusStrengthMultiplier, "STUCK_RECOVERY");
        if (stimulusCreated)
        {
            string logMessage1133 = MIGRATION_LOG_PREFIX;
            logMessage1133 = logMessage1133 + " STUCK_RECOVERY_STIMULUS_OK groupId=";
            logMessage1133 = logMessage1133 + state.m_GroupId;
            logMessage1133 = logMessage1133 + " runtimeGroupId=";
            logMessage1133 = logMessage1133 + state.m_RuntimeGroupId;
            logMessage1133 = logMessage1133 + " id=";
            logMessage1133 = logMessage1133 + state.m_InfectedId;
            logMessage1133 = logMessage1133 + " mode=";
            logMessage1133 = logMessage1133 + state.m_Mode;
            logMessage1133 = logMessage1133 + " cause=";
            logMessage1133 = logMessage1133 + cause;
            logMessage1133 = logMessage1133 + " position=";
            logMessage1133 = logMessage1133 + position.ToString();
            logMessage1133 = logMessage1133 + " target=";
            logMessage1133 = logMessage1133 + stimulusTarget.ToString();
            logMessage1133 = logMessage1133 + " stimulusPosition=";
            logMessage1133 = logMessage1133 + stimulusPosition.ToString();
            LogInfo(logMessage1133);
        }
        else
        {
            string logMessage1135 = MIGRATION_LOG_PREFIX;
            logMessage1135 = logMessage1135 + " STUCK_RECOVERY_STIMULUS_FAILED groupId=";
            logMessage1135 = logMessage1135 + state.m_GroupId;
            logMessage1135 = logMessage1135 + " runtimeGroupId=";
            logMessage1135 = logMessage1135 + state.m_RuntimeGroupId;
            logMessage1135 = logMessage1135 + " id=";
            logMessage1135 = logMessage1135 + state.m_InfectedId;
            logMessage1135 = logMessage1135 + " mode=";
            logMessage1135 = logMessage1135 + state.m_Mode;
            logMessage1135 = logMessage1135 + " cause=";
            logMessage1135 = logMessage1135 + cause;
            logMessage1135 = logMessage1135 + " position=";
            logMessage1135 = logMessage1135 + position.ToString();
            logMessage1135 = logMessage1135 + " target=";
            logMessage1135 = logMessage1135 + stimulusTarget.ToString();
            logMessage1135 = logMessage1135 + " stimulusPosition=";
            logMessage1135 = logMessage1135 + stimulusPosition.ToString();
            LogInfo(logMessage1135);
        }
    }

    protected void BeginStuckRecoveryState(S77MigrateUnitState state, DayZInfectedInputController controller, int now, vector forwardDirection)
    {
        string resumeMode = GetMovementIntent(state);
        vector recoveryPosition = state.m_Infected.GetPosition();
        ReleaseRouteControl(controller);
        forwardDirection[1] = 0.0;
        if (forwardDirection.Length() > 0.01)
            forwardDirection.Normalize();

        state.m_RecoveryResumeMode = resumeMode;
        state.m_Mode = "STUCK_RECOVERY";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_RecoveryFreeUntilTime = now + Math.Round(state.m_StuckRecoveryFreeSeconds * 1000.0);
        state.m_NextRecoveryStatusCheckTime = state.m_RecoveryFreeUntilTime;
        state.m_RecoveryCalmAfterTime = 0;
        state.m_RecoveryPhase = "FIRST_FREE";
        state.m_RecoveryStartPosition = recoveryPosition;
        state.m_RecoveryForwardDirection = forwardDirection;
        state.m_ReverseStartPosition = Vector(0.0, 0.0, 0.0);
        state.m_ReverseTarget = Vector(0.0, 0.0, 0.0);
        state.m_ReverseStartedTime = 0;
        state.m_NextReverseAttemptTime = 0;
        state.m_PostReverseFreeUntilTime = 0;
        ResetStuckSample(state, "STUCK_RECOVERY");
        SuspendRouteProgressWatchdog(state);
        string logMessage1150 = MIGRATION_LOG_PREFIX;
        logMessage1150 = logMessage1150 + " STUCK_RECOVERY_RELEASE groupId=";
        logMessage1150 = logMessage1150 + state.m_GroupId;
        logMessage1150 = logMessage1150 + " runtimeGroupId=";
        logMessage1150 = logMessage1150 + state.m_RuntimeGroupId;
        logMessage1150 = logMessage1150 + " id=";
        logMessage1150 = logMessage1150 + state.m_InfectedId;
        logMessage1150 = logMessage1150 + " mode=";
        logMessage1150 = logMessage1150 + state.m_Mode;
        logMessage1150 = logMessage1150 + " resumeMode=";
        logMessage1150 = logMessage1150 + resumeMode;
        logMessage1150 = logMessage1150 + " position=";
        logMessage1150 = logMessage1150 + state.m_Infected.GetPosition().ToString();
        logMessage1150 = logMessage1150 + " freeSeconds=";
        logMessage1150 = logMessage1150 + state.m_StuckRecoveryFreeSeconds.ToString();
        LogInfo(logMessage1150);
    }

    protected void SetStuckSample(S77MigrateUnitState state, int now)
    {
        state.m_StuckSampleValid = true;
        state.m_StuckSamplePosition = state.m_Infected.GetPosition();
        state.m_StuckSampleDistance = HorizontalDistance(state.m_StuckSamplePosition, GetCurrentControlTarget(state));
        state.m_StuckSampleTime = now;
        string logMessage1159 = MIGRATION_LOG_PREFIX;
        logMessage1159 = logMessage1159 + " STUCK_SAMPLE_START groupId=";
        logMessage1159 = logMessage1159 + state.m_GroupId;
        logMessage1159 = logMessage1159 + " runtimeGroupId=";
        logMessage1159 = logMessage1159 + state.m_RuntimeGroupId;
        logMessage1159 = logMessage1159 + " id=";
        logMessage1159 = logMessage1159 + state.m_InfectedId;
        logMessage1159 = logMessage1159 + " mode=";
        logMessage1159 = logMessage1159 + state.m_Mode;
        logMessage1159 = logMessage1159 + " positionA=";
        logMessage1159 = logMessage1159 + state.m_StuckSamplePosition.ToString();
        logMessage1159 = logMessage1159 + " sampleTimeMs=";
        logMessage1159 = logMessage1159 + now.ToString();
        logMessage1159 = logMessage1159 + " detectionSeconds=";
        logMessage1159 = logMessage1159 + state.m_StuckDetectionSeconds.ToString();
        logMessage1159 = logMessage1159 + " threshold=";
        logMessage1159 = logMessage1159 + state.m_StuckMinMovementMeters.ToString();
        logMessage1159 = logMessage1159 + " target=";
        logMessage1159 = logMessage1159 + GetCurrentControlTarget(state).ToString();
        LogStuckDebug(logMessage1159);
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

            string logMessage1173 = MIGRATION_LOG_PREFIX;
            logMessage1173 = logMessage1173 + " STUCK_SAMPLE_RESET groupId=";
            logMessage1173 = logMessage1173 + state.m_GroupId;
            logMessage1173 = logMessage1173 + " runtimeGroupId=";
            logMessage1173 = logMessage1173 + state.m_RuntimeGroupId;
            logMessage1173 = logMessage1173 + " id=";
            logMessage1173 = logMessage1173 + state.m_InfectedId;
            logMessage1173 = logMessage1173 + " mode=";
            logMessage1173 = logMessage1173 + state.m_Mode;
            logMessage1173 = logMessage1173 + " reason=";
            logMessage1173 = logMessage1173 + reason;
            logMessage1173 = logMessage1173 + " positionA=";
            logMessage1173 = logMessage1173 + state.m_StuckSamplePosition.ToString();
            logMessage1173 = logMessage1173 + " sampleAgeMs=";
            logMessage1173 = logMessage1173 + sampleAgeMs.ToString();
            LogStuckDebug(logMessage1173);
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

            string activationLog = MIGRATION_LOG_PREFIX;
            activationLog = activationLog + " ROUTE_ACTIVATION groupId=";
            activationLog = activationLog + groupState.m_GroupId;
            activationLog = activationLog + " runtimeGroupId=";
            activationLog = activationLog + groupState.m_RuntimeGroupId;
            activationLog = activationLog + " point=";
            activationLog = activationLog + pointNumber.ToString();
            activationLog = activationLog + " alive=";
            activationLog = activationLog + aliveCount.ToString();
            activationLog = activationLog + " inside=";
            activationLog = activationLog + insideCount.ToString();
            activationLog = activationLog + " required=";
            activationLog = activationLog + requiredCount.ToString();
            activationLog = activationLog + " percent=";
            activationLog = activationLog + groupState.m_RouteActivationTriggerPercent.ToString();
            activationLog = activationLog + " radius=";
            activationLog = activationLog + radius.ToString();
            LogInfo(activationLog);

            EmitAIStimulus(groupState, pointCenter, groupState.m_RouteStimulusLifetimeSeconds, groupState.m_RouteStimulusStrengthMultiplier, "point=" + pointNumber.ToString());
            groupState.m_RouteActivationArmed.Set(pointIndex, false);
            return;
        }

        if (!armed)
        {
            groupState.m_RouteActivationArmed.Set(pointIndex, true);
            string logMessage1285 = MIGRATION_LOG_PREFIX;
            logMessage1285 = logMessage1285 + " ROUTE_ACTIVATION_REARMED groupId=";
            logMessage1285 = logMessage1285 + groupState.m_GroupId;
            logMessage1285 = logMessage1285 + " runtimeGroupId=";
            logMessage1285 = logMessage1285 + groupState.m_RuntimeGroupId;
            logMessage1285 = logMessage1285 + " point=";
            logMessage1285 = logMessage1285 + pointNumber.ToString();
            logMessage1285 = logMessage1285 + " inside=";
            logMessage1285 = logMessage1285 + insideCount.ToString();
            logMessage1285 = logMessage1285 + " required=";
            logMessage1285 = logMessage1285 + requiredCount.ToString();
            LogInfo(logMessage1285);
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

            string activationLog = MIGRATION_LOG_PREFIX;
            activationLog = activationLog + " FINAL_ACTIVATION groupId=";
            activationLog = activationLog + groupState.m_GroupId;
            activationLog = activationLog + " runtimeGroupId=";
            activationLog = activationLog + groupState.m_RuntimeGroupId;
            activationLog = activationLog + " alive=";
            activationLog = activationLog + aliveCount.ToString();
            activationLog = activationLog + " inside=";
            activationLog = activationLog + insideCount.ToString();
            activationLog = activationLog + " required=";
            activationLog = activationLog + requiredCount.ToString();
            activationLog = activationLog + " percent=";
            activationLog = activationLog + groupState.m_FinalActivationTriggerPercent.ToString();
            activationLog = activationLog + " radius=";
            activationLog = activationLog + groupState.m_FinalActivationDistance.ToString();
            LogInfo(activationLog);

            EmitAIStimulus(groupState, groupState.m_FinalTargetCenter, groupState.m_FinalStimulusLifetimeSeconds, groupState.m_FinalStimulusStrengthMultiplier, "FINAL");
            groupState.m_FinalActivationArmed = false;
            return;
        }

        if (!groupState.m_FinalActivationArmed)
        {
            groupState.m_FinalActivationArmed = true;
            string logMessage1316 = MIGRATION_LOG_PREFIX;
            logMessage1316 = logMessage1316 + " FINAL_REARMED groupId=";
            logMessage1316 = logMessage1316 + groupState.m_GroupId;
            logMessage1316 = logMessage1316 + " runtimeGroupId=";
            logMessage1316 = logMessage1316 + groupState.m_RuntimeGroupId;
            logMessage1316 = logMessage1316 + " inside=";
            logMessage1316 = logMessage1316 + insideCount.ToString();
            logMessage1316 = logMessage1316 + " required=";
            logMessage1316 = logMessage1316 + requiredCount.ToString();
            LogInfo(logMessage1316);
        }
    }

    protected bool EmitAIStimulus(S77MigrateGroupState groupState, vector position, float lifetime, float strengthMultiplier, string pointLabel)
    {
        if (!m_StimulusReady || !m_NoiseSystem || !m_StimulusParams)
        {
            if (!groupState.m_StimulusFailureLogged)
            {
                string logMessage1326 = MIGRATION_LOG_PREFIX;
                logMessage1326 = logMessage1326 + " WARNING: AI stimulus unavailable groupId=";
                logMessage1326 = logMessage1326 + groupState.m_GroupId;
                logMessage1326 = logMessage1326 + " runtimeGroupId=";
                logMessage1326 = logMessage1326 + groupState.m_RuntimeGroupId;
                logMessage1326 = logMessage1326 + "; migration route remains active";
                LogInfo(logMessage1326);
                groupState.m_StimulusFailureLogged = true;
            }
            return false;
        }

        m_NoiseSystem.AddNoiseTarget(position, lifetime, m_StimulusParams, strengthMultiplier);

        string stimulusType = "AI_STIMULUS";
        if (pointLabel == "FINAL")
            stimulusType = "FINAL_STIMULUS";

        string stimulusLog = MIGRATION_LOG_PREFIX;
        stimulusLog = stimulusLog + " ";
        stimulusLog = stimulusLog + stimulusType;
        stimulusLog = stimulusLog + " groupId=";
        stimulusLog = stimulusLog + groupState.m_GroupId;
        stimulusLog = stimulusLog + " runtimeGroupId=";
        stimulusLog = stimulusLog + groupState.m_RuntimeGroupId;
        stimulusLog = stimulusLog + " ";
        stimulusLog = stimulusLog + pointLabel;
        stimulusLog = stimulusLog + " position=";
        stimulusLog = stimulusLog + position.ToString();
        stimulusLog = stimulusLog + " lifetime=";
        stimulusLog = stimulusLog + lifetime.ToString();
        stimulusLog = stimulusLog + " strength=";
        stimulusLog = stimulusLog + strengthMultiplier.ToString();
        stimulusLog = stimulusLog + " preset=";
        stimulusLog = stimulusLog + AI_STIMULUS_NOISE_PATH;
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
        if (IsVanillaBusy(controller))
        {
            EnterVanillaBusy(state, controller);
            return;
        }

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
                string logMessage1426 = MIGRATION_LOG_PREFIX;
                logMessage1426 = logMessage1426 + " ROUTE_POINT_REACHED groupId=";
                logMessage1426 = logMessage1426 + state.m_GroupId;
                logMessage1426 = logMessage1426 + " runtimeGroupId=";
                logMessage1426 = logMessage1426 + state.m_RuntimeGroupId;
                logMessage1426 = logMessage1426 + " id=";
                logMessage1426 = logMessage1426 + state.m_InfectedId;
                logMessage1426 = logMessage1426 + " mode=";
                logMessage1426 = logMessage1426 + state.m_Mode;
                logMessage1426 = logMessage1426 + " reached=";
                logMessage1426 = logMessage1426 + state.m_RoutePointIndex.ToString();
                logMessage1426 = logMessage1426 + "/";
                logMessage1426 = logMessage1426 + state.m_RoutePoints.Count().ToString();
                LogInfo(logMessage1426);
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

        string logMessage1520 = MIGRATION_LOG_PREFIX;
        logMessage1520 = logMessage1520 + " ";
        logMessage1520 = logMessage1520 + eventName;
        logMessage1520 = logMessage1520 + " groupId=";
        logMessage1520 = logMessage1520 + state.m_GroupId;
        logMessage1520 = logMessage1520 + " runtimeGroupId=";
        logMessage1520 = logMessage1520 + state.m_RuntimeGroupId;
        logMessage1520 = logMessage1520 + " id=";
        logMessage1520 = logMessage1520 + state.m_InfectedId;
        logMessage1520 = logMessage1520 + " mode=";
        logMessage1520 = logMessage1520 + state.m_Mode;
        logMessage1520 = logMessage1520 + " reason=";
        logMessage1520 = logMessage1520 + reason;
        logMessage1520 = logMessage1520 + " position=";
        logMessage1520 = logMessage1520 + state.m_Infected.GetPosition().ToString();
        logMessage1520 = logMessage1520 + " holdRadius=";
        logMessage1520 = logMessage1520 + state.m_FinalHoldRadius.ToString();
        logMessage1520 = logMessage1520 + " returnRadius=";
        logMessage1520 = logMessage1520 + state.m_FinalHoldReturnRadius.ToString();
        LogInfo(logMessage1520);
    }

    protected void StartReturnToHold(S77MigrateUnitState state, DayZInfectedInputController controller)
    {
        if (!state || !state.m_Infected || !state.m_Infected.IsAlive())
            return;

        if (IsVanillaBusy(controller))
        {
            ReleaseRouteControl(controller);
            state.m_HoldCalmAfterTime = 0;
            return;
        }

        state.m_RoutePointIndex = state.m_RoutePoints.Count();
        state.m_Mode = "RETURN_TO_HOLD";
        state.m_RecoveryResumeMode = "RETURN_TO_HOLD";
        state.m_VanillaBusy = false;
        state.m_ResumeAfterTime = 0;
        state.m_HoldCalmAfterTime = 0;
        ResetStuckSample(state, "RETURN_TO_HOLD");
        SuspendRouteProgressWatchdog(state);

        if (BuildPath(state))
        {
            string logMessage1538 = MIGRATION_LOG_PREFIX;
            logMessage1538 = logMessage1538 + " HOLD_RETURN_START groupId=";
            logMessage1538 = logMessage1538 + state.m_GroupId;
            logMessage1538 = logMessage1538 + " runtimeGroupId=";
            logMessage1538 = logMessage1538 + state.m_RuntimeGroupId;
            logMessage1538 = logMessage1538 + " id=";
            logMessage1538 = logMessage1538 + state.m_InfectedId;
            logMessage1538 = logMessage1538 + " mode=";
            logMessage1538 = logMessage1538 + state.m_Mode;
            logMessage1538 = logMessage1538 + " position=";
            logMessage1538 = logMessage1538 + state.m_Infected.GetPosition().ToString();
            logMessage1538 = logMessage1538 + " target=";
            logMessage1538 = logMessage1538 + state.m_MigrationTarget.ToString();
            logMessage1538 = logMessage1538 + " returnRadius=";
            logMessage1538 = logMessage1538 + state.m_FinalHoldReturnRadius.ToString();
            LogInfo(logMessage1538);
        }
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
        string logMessage1562 = MIGRATION_LOG_PREFIX;
        logMessage1562 = logMessage1562 + " RELEASED groupId=";
        logMessage1562 = logMessage1562 + state.m_GroupId;
        logMessage1562 = logMessage1562 + " runtimeGroupId=";
        logMessage1562 = logMessage1562 + state.m_RuntimeGroupId;
        logMessage1562 = logMessage1562 + " id=";
        logMessage1562 = logMessage1562 + state.m_InfectedId;
        logMessage1562 = logMessage1562 + " mode=";
        logMessage1562 = logMessage1562 + state.m_Mode;
        logMessage1562 = logMessage1562 + " reason=";
        logMessage1562 = logMessage1562 + reason;
        logMessage1562 = logMessage1562 + " position=";
        logMessage1562 = logMessage1562 + state.m_Infected.GetPosition().ToString();
        LogInfo(logMessage1562);
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
            logLine = logLine + " groupId=";
            logLine = logLine + state.m_GroupId;
            logLine = logLine + " runtimeGroupId=";
            logLine = logLine + state.m_RuntimeGroupId;
            logLine = logLine + " id=";
            logLine = logLine + state.m_InfectedId;
            logLine = logLine + " class=";
            logLine = logLine + state.m_ClassName;
            logLine = logLine + " position=";
            logLine = logLine + position.ToString();
            logLine = logLine + " routePoint=";
            logLine = logLine + routePointStatus;
            logLine = logLine + " waypoint=";
            logLine = logLine + waypointStatus;
            logLine = logLine + " distance=";
            logLine = logLine + distanceToTarget.ToString();
            logLine = logLine + " target=";
            logLine = logLine + targetPresent;
            logLine = logLine + " mind=";
            logLine = logLine + MindStateToString(mindState);
            logLine = logLine + " mode=";
            logLine = logLine + state.m_Mode;

            LogInfo(logLine);
        }
    }

    protected void Stop()
    {
        m_Stopped = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartPendingGroups);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginWeatherTransition);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CompleteWeatherTransition);
        StopStormRampCallbacks();

        m_StartGroupsScheduled = false;
        m_WeatherStartScheduled = false;
        m_WeatherCompletionScheduled = false;
        m_LaunchBusy = false;
        m_PendingGroups.Clear();

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

        string logMessage1654 = EVENT_LOG_PREFIX;
        logMessage1654 = logMessage1654 + " SESSION_END groups=";
        logMessage1654 = logMessage1654 + m_Groups.Count().ToString();
        logMessage1654 = logMessage1654 + " units=";
        logMessage1654 = logMessage1654 + m_Units.Count().ToString();
        LogInfo(logMessage1654);
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
