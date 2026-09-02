const string S77_MIGRATE_PROFILE_DIR = "$profile:Silver_77_Migrate";
const string S77_MIGRATE_CONFIG = "$profile:Silver_77_Migrate/MigrationConfig.json";
const string S77_MIGRATE_CONFIG_TEMP = "$profile:Silver_77_Migrate/MigrationConfig.migration.tmp";
const string S77_MIGRATE_LOG_FILE = "$profile:Silver_77_Migrate/Migration.log";
const string S77_MIGRATE_LEGACY_EVENT_CONFIG = "$profile:Silver_77_Migrate/MigrationEvent.json";
const string S77_MIGRATE_LEGACY_SCENARIO_001_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_001.json";
const string S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_002.json";

class S77MigrateScenarioConfig
{
    int enabled;
    int weatherChangeEnabled;
    string scenarioId;
    int infectedCount;
    float groupLifetimeSeconds;
    ref array<string> infectedTypes;
    ref array<float> spawnPosition;
    ref array<float> targetPosition;
    ref array<ref array<float>> routePoints;
    float routePointReachRadius;
    int routeActivationEnabled;
    float routeActivationTriggerPercent;
    float routeActivationRadius;
    ref array<float> routeActivationRadii;
    float routeStimulusLifetimeSeconds;
    float routeStimulusStrengthMultiplier;
    float spawnFormationSpacing;
    float spawnFormationJitter;
    float targetFormationSpacing;
    float targetFormationJitter;
    float logIntervalSeconds;
    int finalActivationEnabled;
    float finalActivationTriggerPercent;
    float finalActivationDistance;
    float finalStimulusLifetimeSeconds;
    float finalStimulusStrengthMultiplier;
    int stuckRecoveryEnabled;
    float stuckDetectionSeconds;
    float stuckMinMovementMeters;
    int routeProgressWatchdogEnabled;
    float routeProgressCheckSeconds;
    float routeProgressMinProgressMeters;
    float routeProgressMaxBacktrackMeters;
    int routeProgressBadCheckLimit;
    float stuckRecoveryFreeSeconds;
    float stuckRecoveryStatusCheckSeconds;
    float stuckStimulusForwardDistance;
    float stuckStimulusShareRadius;
    float stuckStimulusRetrySeconds;
    float stuckStimulusLifetimeSeconds;
    float stuckStimulusStrengthMultiplier;
    int finalHoldEnabled;
    float finalHoldRadius;
    float finalHoldReturnRadius;
    float finalHoldCheckSeconds;

    void S77MigrateScenarioConfig()
    {
        SetBaseDefaults();
    }

    void SetBaseDefaults()
    {
        enabled = 1;
        weatherChangeEnabled = 1;
        scenarioId = "";
        infectedCount = 10;
        groupLifetimeSeconds = 14400.0;
        SetDefaultInfectedTypes();
        spawnPosition = new array<float>();
        targetPosition = new array<float>();
        routePoints = new array<ref array<float>>();
        routePointReachRadius = 6.0;
        routeActivationEnabled = 0;
        routeActivationTriggerPercent = 30.0;
        routeActivationRadius = 12.0;
        routeActivationRadii = new array<float>();
        routeStimulusLifetimeSeconds = 1.0;
        routeStimulusStrengthMultiplier = 1.0;
        spawnFormationSpacing = 4.5;
        spawnFormationJitter = 0.5;
        targetFormationSpacing = 4.5;
        targetFormationJitter = 0.5;
        logIntervalSeconds = 10.0;
        finalActivationEnabled = 0;
        finalActivationTriggerPercent = 30.0;
        finalActivationDistance = 12.0;
        finalStimulusLifetimeSeconds = 1.0;
        finalStimulusStrengthMultiplier = 1.0;
        stuckRecoveryEnabled = 1;
        stuckDetectionSeconds = 6.0;
        stuckMinMovementMeters = 2.0;
        routeProgressWatchdogEnabled = 1;
        routeProgressCheckSeconds = 30.0;
        routeProgressMinProgressMeters = 5.0;
        routeProgressMaxBacktrackMeters = 10.0;
        routeProgressBadCheckLimit = 2;
        stuckRecoveryFreeSeconds = 30.0;
        stuckRecoveryStatusCheckSeconds = 3.0;
        stuckStimulusForwardDistance = 10.0;
        stuckStimulusShareRadius = 20.0;
        stuckStimulusRetrySeconds = 5.0;
        stuckStimulusLifetimeSeconds = 1.0;
        stuckStimulusStrengthMultiplier = 1.0;
        finalHoldEnabled = 1;
        finalHoldRadius = 200.0;
        finalHoldReturnRadius = 30.0;
        finalHoldCheckSeconds = 5.0;
    }

    void SetDefaultsScenario001()
    {
        SetBaseDefaults();
        scenarioId = "MIGRATION_TEST_001";
        SetPosition(spawnPosition, 13203.203125, 82.482025, 13336.852539);
        AddRoutePoint(13162.564453, 104.073151, 13181.419922, 12.0);
        AddRoutePoint(13109.365234, 113.158279, 13134.160156, 12.0);
        SetPosition(targetPosition, 13101.915039, 116.453857, 13085.597656);
    }

    void SetDefaultsScenario002()
    {
        SetBaseDefaults();
        scenarioId = "MIGRATION_TEST_002";
        SetPosition(spawnPosition, 12841.188477, 133.146896, 13063.173828);
        AddRoutePoint(12975.204102, 126.648422, 13069.739258, 12.0);
        AddRoutePoint(12998.249023, 124.016357, 13076.784180, 12.0);
        AddRoutePoint(13069.861328, 126.232880, 13047.046875, 12.0);
        AddRoutePoint(13092.981445, 121.014259, 13051.743164, 12.0);
        SetPosition(targetPosition, 13099.694336, 116.208076, 13092.741211);
    }

    void Normalize()
    {
        if (enabled != 1)
            enabled = 0;

        if (weatherChangeEnabled != 1)
            weatherChangeEnabled = 0;

        if (infectedCount < 1)
            infectedCount = 10;

        if (groupLifetimeSeconds <= 0.0)
            groupLifetimeSeconds = 14400.0;

        if (!infectedTypes || infectedTypes.Count() == 0)
            SetDefaultInfectedTypes();

        if (!routePoints)
            routePoints = new array<ref array<float>>();

        if (routePointReachRadius <= 0.0)
            routePointReachRadius = 6.0;

        if (routeActivationEnabled != 1)
            routeActivationEnabled = 0;

        if (routeActivationTriggerPercent <= 0.0 || routeActivationTriggerPercent > 100.0)
            routeActivationTriggerPercent = 30.0;

        if (routeActivationRadius <= 0.0)
            routeActivationRadius = 12.0;

        if (!routeActivationRadii)
            routeActivationRadii = new array<float>();

        if (routeStimulusLifetimeSeconds <= 0.0)
            routeStimulusLifetimeSeconds = 1.0;

        if (routeStimulusStrengthMultiplier < 0.0)
            routeStimulusStrengthMultiplier = 1.0;

        if (spawnFormationSpacing < 0.0)
            spawnFormationSpacing = 4.5;

        if (spawnFormationJitter < 0.0)
            spawnFormationJitter = 0.5;

        if (targetFormationSpacing < 0.0)
            targetFormationSpacing = 4.5;

        if (targetFormationJitter < 0.0)
            targetFormationJitter = 0.5;

        if (logIntervalSeconds < 1.0)
            logIntervalSeconds = 10.0;

        if (finalActivationEnabled != 1)
            finalActivationEnabled = 0;

        if (finalActivationTriggerPercent <= 0.0 || finalActivationTriggerPercent > 100.0)
            finalActivationTriggerPercent = 30.0;

        if (finalActivationDistance <= 0.0)
            finalActivationDistance = 12.0;

        if (finalStimulusLifetimeSeconds <= 0.0)
            finalStimulusLifetimeSeconds = 1.0;

        if (finalStimulusStrengthMultiplier < 0.0)
            finalStimulusStrengthMultiplier = 1.0;

        if (stuckRecoveryEnabled != 1)
            stuckRecoveryEnabled = 0;

        if (stuckDetectionSeconds <= 0.0)
            stuckDetectionSeconds = 6.0;

        if (stuckMinMovementMeters <= 0.0)
            stuckMinMovementMeters = 2.0;

        if (routeProgressWatchdogEnabled != 1)
            routeProgressWatchdogEnabled = 0;

        if (routeProgressCheckSeconds <= 0.0)
            routeProgressCheckSeconds = 30.0;

        if (routeProgressMinProgressMeters <= 0.0)
            routeProgressMinProgressMeters = 5.0;

        if (routeProgressMaxBacktrackMeters <= 0.0)
            routeProgressMaxBacktrackMeters = 10.0;

        if (routeProgressBadCheckLimit < 1)
            routeProgressBadCheckLimit = 2;

        if (stuckRecoveryFreeSeconds <= 0.0)
            stuckRecoveryFreeSeconds = 30.0;

        if (stuckRecoveryStatusCheckSeconds <= 0.0)
            stuckRecoveryStatusCheckSeconds = 3.0;

        if (stuckStimulusForwardDistance <= 0.0)
            stuckStimulusForwardDistance = 10.0;

        if (stuckStimulusShareRadius <= 0.0)
            stuckStimulusShareRadius = 20.0;

        if (stuckStimulusRetrySeconds <= 0.0)
            stuckStimulusRetrySeconds = 5.0;

        if (stuckStimulusLifetimeSeconds <= 0.0)
            stuckStimulusLifetimeSeconds = 1.0;

        if (stuckStimulusStrengthMultiplier < 0.0)
            stuckStimulusStrengthMultiplier = 1.0;

        if (finalHoldEnabled != 1)
            finalHoldEnabled = 0;

        if (finalHoldRadius <= 0.0)
            finalHoldRadius = 200.0;

        if (finalHoldReturnRadius <= 0.0)
            finalHoldReturnRadius = 30.0;

        if (finalHoldCheckSeconds <= 0.0)
            finalHoldCheckSeconds = 5.0;
    }

    void NormalizeLegacy(string fallbackScenarioId, vector fallbackSpawn, vector fallbackTarget)
    {
        if (scenarioId == "")
            scenarioId = fallbackScenarioId;

        if (!spawnPosition || spawnPosition.Count() < 3)
        {
            spawnPosition = new array<float>();
            SetPosition(spawnPosition, fallbackSpawn[0], fallbackSpawn[1], fallbackSpawn[2]);
        }

        if (!targetPosition || targetPosition.Count() < 3)
        {
            targetPosition = new array<float>();
            SetPosition(targetPosition, fallbackTarget[0], fallbackTarget[1], fallbackTarget[2]);
        }

        Normalize();
    }

    bool IsValid()
    {
        if (scenarioId == "" || !infectedTypes || infectedTypes.Count() == 0 || !spawnPosition || spawnPosition.Count() < 3 || !targetPosition || targetPosition.Count() < 3 || !routePoints)
            return false;

        for (int i = 0; i < routePoints.Count(); i++)
        {
            array<float> point = routePoints.Get(i);
            if (!point || point.Count() != 3)
                return false;
        }

        return true;
    }

    vector GetSpawnPosition()
    {
        return Vector(spawnPosition.Get(0), spawnPosition.Get(1), spawnPosition.Get(2));
    }

    vector GetTargetPosition()
    {
        return Vector(targetPosition.Get(0), targetPosition.Get(1), targetPosition.Get(2));
    }

    TVectorArray GetRoutePoints()
    {
        TVectorArray points = new TVectorArray();
        for (int i = 0; i < routePoints.Count(); i++)
        {
            array<float> point = routePoints.Get(i);
            points.Insert(Vector(point.Get(0), point.Get(1), point.Get(2)));
        }

        return points;
    }

    float GetRouteActivationRadius(int index)
    {
        if (routeActivationRadii && index >= 0 && index < routeActivationRadii.Count())
        {
            float pointRadius = routeActivationRadii.Get(index);
            if (pointRadius > 0.0)
                return pointRadius;
        }

        return routeActivationRadius;
    }

    protected void SetDefaultInfectedTypes()
    {
        infectedTypes = new array<string>();
        infectedTypes.Insert("ZmbF_JournalistNormal_Blue");
        infectedTypes.Insert("ZmbM_CitizenASkinny_Brown");
        infectedTypes.Insert("ZmbM_FarmerFat_Brown");
        infectedTypes.Insert("ZmbF_VillagerOld_Blue");
        infectedTypes.Insert("ZmbM_HikerSkinny_Blue");
    }

    protected void SetPosition(array<float> values, float x, float y, float z)
    {
        values.Clear();
        values.Insert(x);
        values.Insert(y);
        values.Insert(z);
    }

    protected void AddRoutePoint(float x, float y, float z, float activationRadius)
    {
        array<float> point = new array<float>();
        point.Insert(x);
        point.Insert(y);
        point.Insert(z);
        routePoints.Insert(point);
        routeActivationRadii.Insert(activationRadius);
    }
}

class S77MigrateConfig
{
    int enabled;
    int loggingEnabled;
    int stuckDebugLoggingEnabled;
    float eventDelaySeconds;
    int weatherEnabled;
    float weatherTransitionSeconds;
    float weatherOvercast;
    float weatherFog;
    float weatherWindMagnitude;
    float weatherRain;
    int weatherStormEnabled;
    float weatherStormDensity;
    float weatherStormThreshold;
    float weatherStormTimeoutSeconds;
    float weatherStormRampSeconds;
    ref array<ref S77MigrateScenarioConfig> scenarios;

    void S77MigrateConfig()
    {
        enabled = 1;
        loggingEnabled = 1;
        stuckDebugLoggingEnabled = 0;
        eventDelaySeconds = 30.0;
        weatherEnabled = 1;
        weatherTransitionSeconds = 180.0;
        weatherOvercast = 1.0;
        weatherFog = 0.8;
        weatherWindMagnitude = 0.0;
        weatherRain = 0.0;
        weatherStormEnabled = 1;
        weatherStormDensity = 0.8;
        weatherStormThreshold = 0.8;
        weatherStormTimeoutSeconds = 30.0;
        weatherStormRampSeconds = 60.0;

        scenarios = new array<ref S77MigrateScenarioConfig>();

        S77MigrateScenarioConfig scenario001 = new S77MigrateScenarioConfig();
        scenario001.SetDefaultsScenario001();
        scenarios.Insert(scenario001);

        S77MigrateScenarioConfig scenario002 = new S77MigrateScenarioConfig();
        scenario002.SetDefaultsScenario002();
        scenarios.Insert(scenario002);
    }

    void Normalize()
    {
        if (enabled != 1)
            enabled = 0;

        if (loggingEnabled != 1)
            loggingEnabled = 0;

        if (stuckDebugLoggingEnabled != 1)
            stuckDebugLoggingEnabled = 0;

        if (eventDelaySeconds < 0.0)
            eventDelaySeconds = 0.0;

        if (weatherEnabled != 1)
            weatherEnabled = 0;

        if (weatherTransitionSeconds < 0.0)
            weatherTransitionSeconds = 0.0;

        weatherOvercast = Clamp01(weatherOvercast);
        weatherFog = Clamp01(weatherFog);

        if (weatherWindMagnitude < 0.0)
            weatherWindMagnitude = 0.0;

        weatherRain = Clamp01(weatherRain);

        if (weatherStormEnabled != 1)
            weatherStormEnabled = 0;

        weatherStormDensity = Clamp01(weatherStormDensity);
        weatherStormThreshold = Clamp01(weatherStormThreshold);

        if (weatherStormTimeoutSeconds <= 0.0)
            weatherStormTimeoutSeconds = 30.0;

        if (weatherStormRampSeconds < 0.0)
            weatherStormRampSeconds = 0.0;

        if (!scenarios)
            scenarios = new array<ref S77MigrateScenarioConfig>();

        for (int i = 0; i < scenarios.Count(); i++)
        {
            S77MigrateScenarioConfig scenario = scenarios.Get(i);
            if (scenario)
                scenario.Normalize();
        }
    }

    protected float Clamp01(float value)
    {
        if (value < 0.0)
            return 0.0;

        if (value > 1.0)
            return 1.0;

        return value;
    }
}

class S77MigrateLegacyEventConfig
{
    int enabled;
    float eventDelaySeconds;
    int weatherEnabled;
    float weatherTransitionSeconds;
    float weatherOvercast;
    float weatherFog;
    float weatherWindMagnitude;
    float weatherRain;
    int weatherStormEnabled;
    float weatherStormDensity;
    float weatherStormThreshold;
    float weatherStormTimeoutSeconds;
    float weatherStormRampSeconds;

    void S77MigrateLegacyEventConfig()
    {
        enabled = 1;
        eventDelaySeconds = 30.0;
        weatherEnabled = 1;
        weatherTransitionSeconds = 180.0;
        weatherOvercast = 1.0;
        weatherFog = 0.8;
        weatherWindMagnitude = 0.0;
        weatherRain = 0.0;
        weatherStormEnabled = 1;
        weatherStormDensity = 0.8;
        weatherStormThreshold = 0.8;
        weatherStormTimeoutSeconds = 30.0;
        weatherStormRampSeconds = 60.0;
    }
}

class S77MigrateLegacyScenarioConfig
{
    int enabled;
    string scenarioId;
    int infectedCount;
    ref array<string> infectedTypes;
    ref array<float> spawnPosition;
    ref array<float> targetPosition;
    float spawnFormationSpacing;
    float spawnFormationJitter;
    float targetFormationSpacing;
    float targetFormationJitter;
    float logIntervalSeconds;
    float spawnDelaySeconds;
    int weatherEnabled;
    float weatherTransitionSeconds;
    float weatherOvercast;
    float weatherFog;
    float weatherWindMagnitude;
    float weatherRain;
    int weatherStormEnabled;
    float weatherStormDensity;
    float weatherStormThreshold;
    float weatherStormTimeoutSeconds;

    void S77MigrateLegacyScenarioConfig()
    {
        SetDefaultsScenario001();
    }

    void SetDefaultsScenario001()
    {
        enabled = 1;
        scenarioId = "MIGRATION_TEST_001";
        infectedCount = 10;
        SetDefaultInfectedTypes();
        SetPositionDefaults(13223.64453125, 79.55464172363281, 13369.6142578125, 13102.9658203125, 115.809814453125, 13094.11328125);
        SetCommonDefaults();
    }

    void SetDefaultsScenario002()
    {
        enabled = 1;
        scenarioId = "MIGRATION_TEST_002";
        infectedCount = 10;
        SetDefaultInfectedTypes();
        SetPositionDefaults(12874.942383, 130.418289, 13074.552734, 13091.167969, 121.727158, 13051.032227);
        SetCommonDefaults();
    }

    protected void SetCommonDefaults()
    {
        spawnFormationSpacing = 4.5;
        spawnFormationJitter = 0.5;
        targetFormationSpacing = 4.5;
        targetFormationJitter = 0.5;
        logIntervalSeconds = 10.0;
        spawnDelaySeconds = 30.0;
        weatherEnabled = 1;
        weatherTransitionSeconds = 180.0;
        weatherOvercast = 1.0;
        weatherFog = 0.8;
        weatherWindMagnitude = 0.0;
        weatherRain = 0.0;
        weatherStormEnabled = 0;
        weatherStormDensity = 0.0;
        weatherStormThreshold = 0.8;
        weatherStormTimeoutSeconds = 30.0;
    }

    protected void SetDefaultInfectedTypes()
    {
        infectedTypes = new array<string>();
        infectedTypes.Insert("ZmbF_JournalistNormal_Blue");
        infectedTypes.Insert("ZmbM_CitizenASkinny_Brown");
        infectedTypes.Insert("ZmbM_FarmerFat_Brown");
        infectedTypes.Insert("ZmbF_VillagerOld_Blue");
        infectedTypes.Insert("ZmbM_HikerSkinny_Blue");
    }

    protected void SetPositionDefaults(float spawnX, float spawnY, float spawnZ, float targetX, float targetY, float targetZ)
    {
        spawnPosition = new array<float>();
        spawnPosition.Insert(spawnX);
        spawnPosition.Insert(spawnY);
        spawnPosition.Insert(spawnZ);

        targetPosition = new array<float>();
        targetPosition.Insert(targetX);
        targetPosition.Insert(targetY);
        targetPosition.Insert(targetZ);
    }
}

class S77MigrateConfigLoader
{
    static S77MigrateConfig Load()
    {
        EnsureProfileDirectory();

        if (!FileExist(S77_MIGRATE_CONFIG) && !CreateUnifiedConfig())
            return null;

        S77MigrateConfig config = new S77MigrateConfig();
        string loadError;
        if (!JsonFileLoader<S77MigrateConfig>.LoadFile(S77_MIGRATE_CONFIG, config, loadError))
        {
            Print("[S77Migrate][EVENT] ERROR: malformed unified runtime config left unchanged: " + loadError);
            return null;
        }

        config.Normalize();

        if (FileExist(S77_MIGRATE_LEGACY_EVENT_CONFIG) || FileExist(S77_MIGRATE_LEGACY_SCENARIO_001_CONFIG) || FileExist(S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG))
            Print("[S77Migrate][EVENT] Unified config is active; legacy runtime JSON files are ignored and left unchanged");

        return config;
    }

    protected static bool CreateUnifiedConfig()
    {
        S77MigrateConfig config = new S77MigrateConfig();
        bool legacyFound = false;
        bool legacyEventLoaded = false;

        if (FileExist(S77_MIGRATE_LEGACY_EVENT_CONFIG))
        {
            S77MigrateLegacyEventConfig legacyEvent = new S77MigrateLegacyEventConfig();
            string eventError;
            if (!JsonFileLoader<S77MigrateLegacyEventConfig>.LoadFile(S77_MIGRATE_LEGACY_EVENT_CONFIG, legacyEvent, eventError))
            {
                Print("[S77Migrate][EVENT] ERROR: legacy MigrationEvent.json is malformed; unified config was not created: " + eventError);
                return false;
            }

            CopyLegacyEvent(legacyEvent, config);
            legacyFound = true;
            legacyEventLoaded = true;
        }

        array<ref S77MigrateScenarioConfig> migratedScenarios = new array<ref S77MigrateScenarioConfig>();

        S77MigrateLegacyScenarioConfig legacyScenario001 = new S77MigrateLegacyScenarioConfig();
        legacyScenario001.SetDefaultsScenario001();
        S77MigrateScenarioConfig migratedScenario001;
        if (FileExist(S77_MIGRATE_LEGACY_SCENARIO_001_CONFIG))
        {
            string scenario001Error;
            if (!JsonFileLoader<S77MigrateLegacyScenarioConfig>.LoadFile(S77_MIGRATE_LEGACY_SCENARIO_001_CONFIG, legacyScenario001, scenario001Error))
            {
                Print("[S77Migrate][EVENT] ERROR: legacy MigrationScenario_001.json is malformed; unified config was not created: " + scenario001Error);
                return false;
            }

            legacyFound = true;
            if (!legacyEventLoaded)
                CopyLegacyScenarioWeather(legacyScenario001, config);

            migratedScenario001 = CopyLegacyScenario(legacyScenario001);
            migratedScenario001.NormalizeLegacy("MIGRATION_TEST_001", Vector(13223.64453125, 79.55464172363281, 13369.6142578125), Vector(13102.9658203125, 115.809814453125, 13094.11328125));
        }
        else
        {
            migratedScenario001 = new S77MigrateScenarioConfig();
            migratedScenario001.SetDefaultsScenario001();
        }
        migratedScenarios.Insert(migratedScenario001);

        S77MigrateLegacyScenarioConfig legacyScenario002 = new S77MigrateLegacyScenarioConfig();
        legacyScenario002.SetDefaultsScenario002();
        S77MigrateScenarioConfig migratedScenario002;
        if (FileExist(S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG))
        {
            string scenario002Error;
            if (!JsonFileLoader<S77MigrateLegacyScenarioConfig>.LoadFile(S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG, legacyScenario002, scenario002Error))
            {
                Print("[S77Migrate][EVENT] ERROR: legacy MigrationScenario_002.json is malformed; unified config was not created: " + scenario002Error);
                return false;
            }

            legacyFound = true;

            migratedScenario002 = CopyLegacyScenario(legacyScenario002);
            migratedScenario002.NormalizeLegacy("MIGRATION_TEST_002", Vector(12874.942383, 130.418289, 13074.552734), Vector(13091.167969, 121.727158, 13051.032227));
        }
        else
        {
            migratedScenario002 = new S77MigrateScenarioConfig();
            migratedScenario002.SetDefaultsScenario002();
        }
        migratedScenarios.Insert(migratedScenario002);

        config.scenarios = migratedScenarios;
        config.Normalize();

        if (FileExist(S77_MIGRATE_CONFIG_TEMP))
            DeleteFile(S77_MIGRATE_CONFIG_TEMP);

        string saveError;
        if (!JsonFileLoader<S77MigrateConfig>.SaveFile(S77_MIGRATE_CONFIG_TEMP, config, saveError))
        {
            Print("[S77Migrate][EVENT] ERROR: unified config temp file could not be written: " + saveError);
            return false;
        }

        S77MigrateConfig tempValidation = new S77MigrateConfig();
        string validationError;
        if (!JsonFileLoader<S77MigrateConfig>.LoadFile(S77_MIGRATE_CONFIG_TEMP, tempValidation, validationError))
        {
            Print("[S77Migrate][EVENT] ERROR: unified config temp validation failed: " + validationError);
            DeleteFile(S77_MIGRATE_CONFIG_TEMP);
            return false;
        }

        if (FileExist(S77_MIGRATE_CONFIG) || !CopyFile(S77_MIGRATE_CONFIG_TEMP, S77_MIGRATE_CONFIG))
        {
            Print("[S77Migrate][EVENT] ERROR: validated unified config could not be installed");
            DeleteFile(S77_MIGRATE_CONFIG_TEMP);
            return false;
        }

        S77MigrateConfig finalValidation = new S77MigrateConfig();
        if (!JsonFileLoader<S77MigrateConfig>.LoadFile(S77_MIGRATE_CONFIG, finalValidation, validationError))
        {
            Print("[S77Migrate][EVENT] ERROR: installed unified config failed validation and was removed: " + validationError);
            DeleteFile(S77_MIGRATE_CONFIG);
            DeleteFile(S77_MIGRATE_CONFIG_TEMP);
            return false;
        }

        DeleteFile(S77_MIGRATE_CONFIG_TEMP);

        if (legacyFound)
            Print("[S77Migrate][EVENT] Unified config created by safe migration; legacy runtime JSON files were left unchanged");
        else
            Print("[S77Migrate][EVENT] Default unified config created: " + S77_MIGRATE_CONFIG);

        return true;
    }

    protected static void CopyLegacyEvent(S77MigrateLegacyEventConfig source, S77MigrateConfig target)
    {
        target.enabled = source.enabled;
        target.eventDelaySeconds = source.eventDelaySeconds;
        target.weatherEnabled = source.weatherEnabled;
        target.weatherTransitionSeconds = source.weatherTransitionSeconds;
        target.weatherOvercast = source.weatherOvercast;
        target.weatherFog = source.weatherFog;
        target.weatherWindMagnitude = source.weatherWindMagnitude;
        target.weatherRain = source.weatherRain;
        target.weatherStormEnabled = source.weatherStormEnabled;
        target.weatherStormDensity = source.weatherStormDensity;
        target.weatherStormThreshold = source.weatherStormThreshold;
        target.weatherStormTimeoutSeconds = source.weatherStormTimeoutSeconds;
        target.weatherStormRampSeconds = source.weatherStormRampSeconds;
    }

    protected static void CopyLegacyScenarioWeather(S77MigrateLegacyScenarioConfig source, S77MigrateConfig target)
    {
        target.enabled = source.enabled;
        target.eventDelaySeconds = source.spawnDelaySeconds;
        target.weatherEnabled = source.weatherEnabled;
        target.weatherTransitionSeconds = source.weatherTransitionSeconds;
        target.weatherOvercast = source.weatherOvercast;
        target.weatherFog = source.weatherFog;
        target.weatherWindMagnitude = source.weatherWindMagnitude;
        target.weatherRain = source.weatherRain;
        target.weatherStormEnabled = source.weatherStormEnabled;
        target.weatherStormDensity = source.weatherStormDensity;
        target.weatherStormThreshold = source.weatherStormThreshold;
        target.weatherStormTimeoutSeconds = source.weatherStormTimeoutSeconds;
    }

    protected static S77MigrateScenarioConfig CopyLegacyScenario(S77MigrateLegacyScenarioConfig source)
    {
        S77MigrateScenarioConfig target = new S77MigrateScenarioConfig();
        target.enabled = source.enabled;
        target.scenarioId = source.scenarioId;
        target.infectedCount = source.infectedCount;
        target.infectedTypes = CopyStringArray(source.infectedTypes);
        target.spawnPosition = CopyFloatArray(source.spawnPosition);
        target.targetPosition = CopyFloatArray(source.targetPosition);
        target.spawnFormationSpacing = source.spawnFormationSpacing;
        target.spawnFormationJitter = source.spawnFormationJitter;
        target.targetFormationSpacing = source.targetFormationSpacing;
        target.targetFormationJitter = source.targetFormationJitter;
        target.logIntervalSeconds = source.logIntervalSeconds;
        return target;
    }

    protected static array<string> CopyStringArray(array<string> source)
    {
        array<string> result = new array<string>();
        if (!source)
            return result;

        for (int i = 0; i < source.Count(); i++)
            result.Insert(source.Get(i));

        return result;
    }

    protected static array<float> CopyFloatArray(array<float> source)
    {
        array<float> result = new array<float>();
        if (!source)
            return result;

        for (int i = 0; i < source.Count(); i++)
            result.Insert(source.Get(i));

        return result;
    }

    protected static void EnsureProfileDirectory()
    {
        if (!FileExist(S77_MIGRATE_PROFILE_DIR))
            MakeDirectory(S77_MIGRATE_PROFILE_DIR);
    }
}
