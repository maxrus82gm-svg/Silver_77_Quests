const string S77_MIGRATE_PROFILE_DIR = "$profile:Silver_77_Migrate";
const string S77_MIGRATE_CONFIG = "$profile:Silver_77_Migrate/MigrationConfig.json";
const string S77_MIGRATE_CONFIG_TEMP = "$profile:Silver_77_Migrate/MigrationConfig.migration.tmp";
const string S77_MIGRATE_LEGACY_EVENT_CONFIG = "$profile:Silver_77_Migrate/MigrationEvent.json";
const string S77_MIGRATE_LEGACY_SCENARIO_001_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_001.json";
const string S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_002.json";

class S77MigrateScenarioConfig
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

    void S77MigrateScenarioConfig()
    {
        SetBaseDefaults();
    }

    void SetBaseDefaults()
    {
        enabled = 1;
        scenarioId = "";
        infectedCount = 10;
        SetDefaultInfectedTypes();
        spawnPosition = new array<float>();
        targetPosition = new array<float>();
        spawnFormationSpacing = 4.5;
        spawnFormationJitter = 0.5;
        targetFormationSpacing = 4.5;
        targetFormationJitter = 0.5;
        logIntervalSeconds = 10.0;
    }

    void SetDefaultsScenario001()
    {
        SetBaseDefaults();
        scenarioId = "MIGRATION_TEST_001";
        SetPosition(spawnPosition, 13223.64453125, 79.55464172363281, 13369.6142578125);
        SetPosition(targetPosition, 13102.9658203125, 115.809814453125, 13094.11328125);
    }

    void SetDefaultsScenario002()
    {
        SetBaseDefaults();
        scenarioId = "MIGRATION_TEST_002";
        SetPosition(spawnPosition, 12874.942383, 130.418289, 13074.552734);
        SetPosition(targetPosition, 13091.167969, 121.727158, 13051.032227);
    }

    void Normalize()
    {
        if (enabled != 1)
            enabled = 0;

        if (infectedCount < 1)
            infectedCount = 10;

        if (!infectedTypes || infectedTypes.Count() == 0)
            SetDefaultInfectedTypes();

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
        return scenarioId != "" && infectedTypes && infectedTypes.Count() > 0 && spawnPosition && spawnPosition.Count() >= 3 && targetPosition && targetPosition.Count() >= 3;
    }

    vector GetSpawnPosition()
    {
        return Vector(spawnPosition.Get(0), spawnPosition.Get(1), spawnPosition.Get(2));
    }

    vector GetTargetPosition()
    {
        return Vector(targetPosition.Get(0), targetPosition.Get(1), targetPosition.Get(2));
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
}

class S77MigrateConfig
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
    ref array<ref S77MigrateScenarioConfig> scenarios;

    void S77MigrateConfig()
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
        }
        S77MigrateScenarioConfig migratedScenario001 = CopyLegacyScenario(legacyScenario001);
        migratedScenario001.NormalizeLegacy("MIGRATION_TEST_001", Vector(13223.64453125, 79.55464172363281, 13369.6142578125), Vector(13102.9658203125, 115.809814453125, 13094.11328125));
        migratedScenarios.Insert(migratedScenario001);

        S77MigrateLegacyScenarioConfig legacyScenario002 = new S77MigrateLegacyScenarioConfig();
        legacyScenario002.SetDefaultsScenario002();
        if (FileExist(S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG))
        {
            string scenario002Error;
            if (!JsonFileLoader<S77MigrateLegacyScenarioConfig>.LoadFile(S77_MIGRATE_LEGACY_SCENARIO_002_CONFIG, legacyScenario002, scenario002Error))
            {
                Print("[S77Migrate][EVENT] ERROR: legacy MigrationScenario_002.json is malformed; unified config was not created: " + scenario002Error);
                return false;
            }

            legacyFound = true;
        }
        S77MigrateScenarioConfig migratedScenario002 = CopyLegacyScenario(legacyScenario002);
        migratedScenario002.NormalizeLegacy("MIGRATION_TEST_002", Vector(12874.942383, 130.418289, 13074.552734), Vector(13091.167969, 121.727158, 13051.032227));
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
