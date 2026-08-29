const string S77_MIGRATE_PROFILE_DIR = "$profile:Silver_77_Migrate";
const string S77_MIGRATE_SCENARIO_001_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_001.json";

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
    float spawnDelaySeconds;
    float logIntervalSeconds;

    void S77MigrateScenarioConfig()
    {
        SetDefaults();
    }

    void SetDefaults()
    {
        enabled = 0;
        scenarioId = "MIGRATION_TEST_001";
        infectedCount = 5;

        infectedTypes = new array<string>();
        infectedTypes.Insert("ZmbF_JournalistNormal_Blue");
        infectedTypes.Insert("ZmbM_CitizenASkinny_Brown");
        infectedTypes.Insert("ZmbM_FarmerFat_Brown");
        infectedTypes.Insert("ZmbF_VillagerOld_Blue");
        infectedTypes.Insert("ZmbM_HikerSkinny_Blue");

        spawnPosition = new array<float>();
        spawnPosition.Insert(13223.644531);
        spawnPosition.Insert(79.554642);
        spawnPosition.Insert(13369.614258);

        targetPosition = new array<float>();
        targetPosition.Insert(13102.965820);
        targetPosition.Insert(115.809814);
        targetPosition.Insert(13094.113281);

        spawnFormationSpacing = 2.5;
        spawnFormationJitter = 0.20;
        targetFormationSpacing = 2.5;
        targetFormationJitter = 0.20;
        spawnDelaySeconds = 10.0;
        logIntervalSeconds = 10.0;
    }

    void Normalize()
    {
        if (scenarioId == "")
            scenarioId = "MIGRATION_TEST_001";

        if (infectedCount < 1)
            infectedCount = 5;

        if (!infectedTypes || infectedTypes.Count() == 0)
        {
            infectedTypes = new array<string>();
            infectedTypes.Insert("ZmbF_JournalistNormal_Blue");
            infectedTypes.Insert("ZmbM_CitizenASkinny_Brown");
            infectedTypes.Insert("ZmbM_FarmerFat_Brown");
            infectedTypes.Insert("ZmbF_VillagerOld_Blue");
            infectedTypes.Insert("ZmbM_HikerSkinny_Blue");
        }

        if (!spawnPosition || spawnPosition.Count() < 3)
        {
            spawnPosition = new array<float>();
            spawnPosition.Insert(13223.644531);
            spawnPosition.Insert(79.554642);
            spawnPosition.Insert(13369.614258);
        }

        if (!targetPosition || targetPosition.Count() < 3)
        {
            targetPosition = new array<float>();
            targetPosition.Insert(13102.965820);
            targetPosition.Insert(115.809814);
            targetPosition.Insert(13094.113281);
        }

        if (spawnFormationSpacing < 0.0)
            spawnFormationSpacing = 2.5;

        if (spawnFormationJitter < 0.0)
            spawnFormationJitter = 0.20;

        if (targetFormationSpacing < 0.0)
            targetFormationSpacing = 2.5;

        if (targetFormationJitter < 0.0)
            targetFormationJitter = 0.20;

        if (spawnDelaySeconds < 0.0)
            spawnDelaySeconds = 0.0;

        if (logIntervalSeconds < 1.0)
            logIntervalSeconds = 10.0;
    }

    vector GetSpawnPosition()
    {
        return Vector(spawnPosition.Get(0), spawnPosition.Get(1), spawnPosition.Get(2));
    }

    vector GetTargetPosition()
    {
        return Vector(targetPosition.Get(0), targetPosition.Get(1), targetPosition.Get(2));
    }
}

class S77MigrateConfigLoader
{
    static S77MigrateScenarioConfig LoadScenario001()
    {
        if (!FileExist(S77_MIGRATE_PROFILE_DIR))
            MakeDirectory(S77_MIGRATE_PROFILE_DIR);

        S77MigrateScenarioConfig config = new S77MigrateScenarioConfig();

        if (!FileExist(S77_MIGRATE_SCENARIO_001_CONFIG))
        {
            JsonFileLoader<S77MigrateScenarioConfig>.JsonSaveFile(S77_MIGRATE_SCENARIO_001_CONFIG, config);
            Print("[S77Migrate][TEST001] Default disabled config created: " + S77_MIGRATE_SCENARIO_001_CONFIG);
        }
        else
        {
            JsonFileLoader<S77MigrateScenarioConfig>.JsonLoadFile(S77_MIGRATE_SCENARIO_001_CONFIG, config);
        }

        config.Normalize();
        return config;
    }
}
