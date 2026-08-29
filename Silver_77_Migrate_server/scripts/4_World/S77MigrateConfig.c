const string S77_MIGRATE_PROFILE_DIR = "$profile:Silver_77_Migrate";
const string S77_MIGRATE_SCENARIO_001_CONFIG = "$profile:Silver_77_Migrate/MigrationScenario_001.json";
const string S77_MIGRATE_SCENARIO_001_TEMP = "$profile:Silver_77_Migrate/MigrationScenario_001.schema.tmp";
const string S77_MIGRATE_SCENARIO_001_BACKUP = "$profile:Silver_77_Migrate/MigrationScenario_001.schema.backup";

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

    void S77MigrateScenarioConfig()
    {
        SetDefaults();
    }

    void SetDefaults()
    {
        enabled = 1;
        scenarioId = "MIGRATION_TEST_001";
        infectedCount = 10;

        infectedTypes = new array<string>();
        infectedTypes.Insert("ZmbF_JournalistNormal_Blue");
        infectedTypes.Insert("ZmbM_CitizenASkinny_Brown");
        infectedTypes.Insert("ZmbM_FarmerFat_Brown");
        infectedTypes.Insert("ZmbF_VillagerOld_Blue");
        infectedTypes.Insert("ZmbM_HikerSkinny_Blue");

        spawnPosition = new array<float>();
        spawnPosition.Insert(13223.64453125);
        spawnPosition.Insert(79.55464172363281);
        spawnPosition.Insert(13369.6142578125);

        targetPosition = new array<float>();
        targetPosition.Insert(13102.9658203125);
        targetPosition.Insert(115.809814453125);
        targetPosition.Insert(13094.11328125);

        spawnFormationSpacing = 4.5;
        spawnFormationJitter = 0.5;
        targetFormationSpacing = 4.5;
        targetFormationJitter = 0.5;
        spawnDelaySeconds = 30.0;
        logIntervalSeconds = 10.0;
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

    void Normalize()
    {
        if (scenarioId == "")
            scenarioId = "MIGRATION_TEST_001";

        if (infectedCount < 1)
            infectedCount = 10;

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
            spawnPosition.Insert(13223.64453125);
            spawnPosition.Insert(79.55464172363281);
            spawnPosition.Insert(13369.6142578125);
        }

        if (!targetPosition || targetPosition.Count() < 3)
        {
            targetPosition = new array<float>();
            targetPosition.Insert(13102.9658203125);
            targetPosition.Insert(115.809814453125);
            targetPosition.Insert(13094.11328125);
        }

        if (spawnFormationSpacing < 0.0)
            spawnFormationSpacing = 4.5;

        if (spawnFormationJitter < 0.0)
            spawnFormationJitter = 0.5;

        if (targetFormationSpacing < 0.0)
            targetFormationSpacing = 4.5;

        if (targetFormationJitter < 0.0)
            targetFormationJitter = 0.5;

        if (spawnDelaySeconds < 0.0)
            spawnDelaySeconds = 0.0;

        if (logIntervalSeconds < 1.0)
            logIntervalSeconds = 10.0;

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
    }

    protected float Clamp01(float value)
    {
        if (value < 0.0)
            return 0.0;

        if (value > 1.0)
            return 1.0;

        return value;
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
            string saveError;
            if (!JsonFileLoader<S77MigrateScenarioConfig>.SaveFile(S77_MIGRATE_SCENARIO_001_CONFIG, config, saveError))
            {
                Print("[S77Migrate][TEST001] ERROR: " + saveError);
                return null;
            }

            Print("[S77Migrate][TEST001] Default config created: " + S77_MIGRATE_SCENARIO_001_CONFIG);
        }
        else
        {
            if (!EnsureRuntimeSchema(config))
                return null;
        }

        string loadError;
        if (!JsonFileLoader<S77MigrateScenarioConfig>.LoadFile(S77_MIGRATE_SCENARIO_001_CONFIG, config, loadError))
        {
            Print("[S77Migrate][TEST001] ERROR: " + loadError);
            return null;
        }

        config.Normalize();
        return config;
    }

    protected static bool EnsureRuntimeSchema(S77MigrateScenarioConfig defaults)
    {
        string originalText;
        if (!ReadTextFile(S77_MIGRATE_SCENARIO_001_CONFIG, originalText))
        {
            Print("[S77Migrate][TEST001] ERROR: runtime config could not be read; schema update aborted");
            return false;
        }

        S77MigrateScenarioConfig validationConfig = new S77MigrateScenarioConfig();
        string validationError;
        if (!JsonFileLoader<S77MigrateScenarioConfig>.LoadData(originalText, validationConfig, validationError))
        {
            Print("[S77Migrate][TEST001] ERROR: malformed runtime config; file left unchanged: " + validationError);
            return false;
        }

        array<string> existingKeys;
        int rootCloseIndex;
        string scanError;
        if (!CollectTopLevelKeys(originalText, existingKeys, rootCloseIndex, scanError))
        {
            Print("[S77Migrate][TEST001] ERROR: runtime schema scan failed; file left unchanged: " + scanError);
            return false;
        }

        array<string> addOnlyKeys = GetAddOnlyWeatherKeys();
        array<string> missingKeys = new array<string>();

        for (int i = 0; i < addOnlyKeys.Count(); i++)
        {
            string key = addOnlyKeys.Get(i);
            if (existingKeys.Find(key) == -1)
                missingKeys.Insert(key);
        }

        if (missingKeys.Count() == 0)
            return true;

        string updatedText;
        if (!BuildUpdatedJson(originalText, rootCloseIndex, missingKeys, defaults, updatedText))
        {
            Print("[S77Migrate][TEST001] ERROR: runtime schema fragments could not be built; file left unchanged");
            return false;
        }

        S77MigrateScenarioConfig updatedValidation = new S77MigrateScenarioConfig();
        if (!JsonFileLoader<S77MigrateScenarioConfig>.LoadData(updatedText, updatedValidation, validationError))
        {
            Print("[S77Migrate][TEST001] ERROR: generated runtime schema is invalid; file left unchanged: " + validationError);
            return false;
        }

        if (FileExist(S77_MIGRATE_SCENARIO_001_BACKUP))
        {
            Print("[S77Migrate][TEST001] ERROR: retained schema backup already exists; automatic update stopped for manual recovery: " + S77_MIGRATE_SCENARIO_001_BACKUP);
            return false;
        }

        DeleteTemporaryFiles();

        if (!WriteTextFile(S77_MIGRATE_SCENARIO_001_TEMP, updatedText))
        {
            Print("[S77Migrate][TEST001] ERROR: schema temp file could not be written; runtime config left unchanged");
            return false;
        }

        string tempText;
        S77MigrateScenarioConfig tempValidation = new S77MigrateScenarioConfig();
        if (!ReadTextFile(S77_MIGRATE_SCENARIO_001_TEMP, tempText) || tempText != updatedText || !JsonFileLoader<S77MigrateScenarioConfig>.LoadFile(S77_MIGRATE_SCENARIO_001_TEMP, tempValidation, validationError))
        {
            Print("[S77Migrate][TEST001] ERROR: schema temp verification failed; runtime config left unchanged");
            DeleteTemporaryFiles();
            return false;
        }

        if (!CopyFile(S77_MIGRATE_SCENARIO_001_CONFIG, S77_MIGRATE_SCENARIO_001_BACKUP))
        {
            Print("[S77Migrate][TEST001] ERROR: runtime config backup could not be created; schema update aborted");
            DeleteTemporaryFiles();
            return false;
        }

        if (!WriteTextFile(S77_MIGRATE_SCENARIO_001_CONFIG, updatedText))
        {
            Print("[S77Migrate][TEST001] ERROR: runtime config could not be updated; restoring original text");
            RestoreOriginalText(originalText);
            DeleteFile(S77_MIGRATE_SCENARIO_001_TEMP);
            return false;
        }

        string writtenText;
        S77MigrateScenarioConfig writtenValidation = new S77MigrateScenarioConfig();
        bool writeVerified = ReadTextFile(S77_MIGRATE_SCENARIO_001_CONFIG, writtenText);
        writeVerified = writeVerified && writtenText == updatedText;
        writeVerified = writeVerified && JsonFileLoader<S77MigrateScenarioConfig>.LoadFile(S77_MIGRATE_SCENARIO_001_CONFIG, writtenValidation, validationError);

        if (!writeVerified)
        {
            Print("[S77Migrate][TEST001] ERROR: runtime schema update verification failed; restoring original text");
            RestoreOriginalText(originalText);
            DeleteFile(S77_MIGRATE_SCENARIO_001_TEMP);
            return false;
        }

        string addedKeys;
        for (int keyIndex = 0; keyIndex < missingKeys.Count(); keyIndex++)
        {
            if (keyIndex > 0)
                addedKeys = addedKeys + ",";

            addedKeys = addedKeys + missingKeys.Get(keyIndex);
        }

        Print("[S77Migrate][TEST001] Runtime schema add-only update completed; added=" + addedKeys);
        DeleteTemporaryFiles();
        DeleteFile(S77_MIGRATE_SCENARIO_001_BACKUP);
        return true;
    }

    protected static array<string> GetAddOnlyWeatherKeys()
    {
        array<string> keys = new array<string>();
        keys.Insert("weatherEnabled");
        keys.Insert("weatherTransitionSeconds");
        keys.Insert("weatherOvercast");
        keys.Insert("weatherFog");
        keys.Insert("weatherWindMagnitude");
        keys.Insert("weatherRain");
        keys.Insert("weatherStormEnabled");
        keys.Insert("weatherStormDensity");
        keys.Insert("weatherStormThreshold");
        keys.Insert("weatherStormTimeoutSeconds");
        return keys;
    }

    protected static bool BuildUpdatedJson(string originalText, int rootCloseIndex, array<string> missingKeys, S77MigrateScenarioConfig defaults, out string updatedText)
    {
        int lastContentIndex = rootCloseIndex - 1;
        while (lastContentIndex >= 0 && IsWhitespace(originalText.Get(lastContentIndex)))
            lastContentIndex--;

        if (lastContentIndex < 0)
            return false;

        bool rootWasEmpty = originalText.Get(lastContentIndex) == "{";
        string leadingText = originalText.Substring(0, lastContentIndex + 1);
        string existingWhitespace = originalText.Substring(lastContentIndex + 1, rootCloseIndex - lastContentIndex - 1);
        string trailingText = originalText.Substring(rootCloseIndex, originalText.Length() - rootCloseIndex);
        string newLine = "\n";

        if (originalText.IndexOf("\r\n") >= 0)
            newLine = "\r\n";

        updatedText = leadingText;
        if (!rootWasEmpty)
            updatedText = updatedText + ",";

        if (existingWhitespace == "")
            updatedText = updatedText + newLine;
        else
            updatedText = updatedText + existingWhitespace;

        for (int i = 0; i < missingKeys.Count(); i++)
        {
            string propertyText = BuildDefaultProperty(missingKeys.Get(i), defaults);
            if (propertyText == "")
                return false;

            updatedText = updatedText + "    " + propertyText;

            if (i < missingKeys.Count() - 1)
                updatedText = updatedText + ",";

            updatedText = updatedText + newLine;
        }

        updatedText = updatedText + trailingText;
        return true;
    }

    protected static string BuildDefaultProperty(string key, S77MigrateScenarioConfig defaults)
    {
        if (key == "weatherEnabled")
            return "\"weatherEnabled\": " + defaults.weatherEnabled.ToString();
        if (key == "weatherTransitionSeconds")
            return "\"weatherTransitionSeconds\": " + defaults.weatherTransitionSeconds.ToString();
        if (key == "weatherOvercast")
            return "\"weatherOvercast\": " + defaults.weatherOvercast.ToString();
        if (key == "weatherFog")
            return "\"weatherFog\": " + defaults.weatherFog.ToString();
        if (key == "weatherWindMagnitude")
            return "\"weatherWindMagnitude\": " + defaults.weatherWindMagnitude.ToString();
        if (key == "weatherRain")
            return "\"weatherRain\": " + defaults.weatherRain.ToString();
        if (key == "weatherStormEnabled")
            return "\"weatherStormEnabled\": " + defaults.weatherStormEnabled.ToString();
        if (key == "weatherStormDensity")
            return "\"weatherStormDensity\": " + defaults.weatherStormDensity.ToString();
        if (key == "weatherStormThreshold")
            return "\"weatherStormThreshold\": " + defaults.weatherStormThreshold.ToString();
        if (key == "weatherStormTimeoutSeconds")
            return "\"weatherStormTimeoutSeconds\": " + defaults.weatherStormTimeoutSeconds.ToString();

        return "";
    }

    protected static bool CollectTopLevelKeys(string jsonText, out array<string> keys, out int rootCloseIndex, out string error)
    {
        keys = new array<string>();
        rootCloseIndex = -1;

        int firstIndex = 0;
        while (firstIndex < jsonText.Length() && IsWhitespace(jsonText.Get(firstIndex)))
            firstIndex++;

        if (firstIndex >= jsonText.Length() || jsonText.Get(firstIndex) != "{")
        {
            error = "root JSON value is not an object";
            return false;
        }

        int objectDepth = 0;
        int arrayDepth = 0;
        bool expectingRootKey = false;

        for (int i = firstIndex; i < jsonText.Length(); i++)
        {
            string character = jsonText.Get(i);

            if (character == "\"")
            {
                string stringValue;
                int stringEndIndex;
                if (!ReadJsonString(jsonText, i, stringValue, stringEndIndex))
                {
                    error = "unterminated JSON string";
                    return false;
                }

                if (objectDepth == 1 && arrayDepth == 0 && expectingRootKey)
                {
                    int colonIndex = stringEndIndex + 1;
                    while (colonIndex < jsonText.Length() && IsWhitespace(jsonText.Get(colonIndex)))
                        colonIndex++;

                    if (colonIndex >= jsonText.Length() || jsonText.Get(colonIndex) != ":")
                    {
                        error = "top-level property has no colon";
                        return false;
                    }

                    keys.Insert(stringValue);
                    expectingRootKey = false;
                    i = colonIndex;
                    continue;
                }

                i = stringEndIndex;
                continue;
            }

            if (character == "{")
            {
                objectDepth++;
                if (objectDepth == 1)
                    expectingRootKey = true;
                continue;
            }

            if (character == "}")
            {
                if (objectDepth == 1 && arrayDepth == 0)
                {
                    rootCloseIndex = i;

                    for (int tailIndex = i + 1; tailIndex < jsonText.Length(); tailIndex++)
                    {
                        if (!IsWhitespace(jsonText.Get(tailIndex)))
                        {
                            error = "non-whitespace data follows the root object";
                            return false;
                        }
                    }

                    return true;
                }

                objectDepth--;
                if (objectDepth < 1)
                {
                    error = "unexpected object closing brace";
                    return false;
                }
                continue;
            }

            if (character == "[")
            {
                arrayDepth++;
                continue;
            }

            if (character == "]")
            {
                arrayDepth--;
                if (arrayDepth < 0)
                {
                    error = "unexpected array closing bracket";
                    return false;
                }
                continue;
            }

            if (character == "," && objectDepth == 1 && arrayDepth == 0)
                expectingRootKey = true;
        }

        error = "root object has no closing brace";
        return false;
    }

    protected static bool ReadJsonString(string jsonText, int quoteIndex, out string value, out int endIndex)
    {
        bool escaped = false;
        value = "";
        endIndex = -1;

        for (int i = quoteIndex + 1; i < jsonText.Length(); i++)
        {
            string character = jsonText.Get(i);

            if (escaped)
            {
                value = value + character;
                escaped = false;
                continue;
            }

            if (character == "\\")
            {
                escaped = true;
                continue;
            }

            if (character == "\"")
            {
                endIndex = i;
                return true;
            }

            value = value + character;
        }

        return false;
    }

    protected static bool IsWhitespace(string character)
    {
        return character == " " || character == "\t" || character == "\r" || character == "\n";
    }

    protected static bool ReadTextFile(string path, out string content)
    {
        FileHandle handle = OpenFile(path, FileMode.READ);
        if (handle == 0)
            return false;

        const int READ_CHUNK_SIZE = 16384;
        string chunk;
        content = "";

        int bytesRead = ReadFile(handle, chunk, READ_CHUNK_SIZE);
        while (bytesRead > 0)
        {
            content = content + chunk;
            chunk = "";
            bytesRead = ReadFile(handle, chunk, READ_CHUNK_SIZE);
        }

        CloseFile(handle);
        return true;
    }

    protected static bool WriteTextFile(string path, string content)
    {
        FileHandle handle = OpenFile(path, FileMode.WRITE);
        if (handle == 0)
            return false;

        FPrint(handle, content);
        CloseFile(handle);
        return true;
    }

    protected static bool RestoreOriginalText(string originalText)
    {
        if (!WriteTextFile(S77_MIGRATE_SCENARIO_001_CONFIG, originalText))
        {
            Print("[S77Migrate][TEST001] CRITICAL: runtime config restore failed; backup retained at " + S77_MIGRATE_SCENARIO_001_BACKUP);
            return false;
        }

        string restoredText;
        if (!ReadTextFile(S77_MIGRATE_SCENARIO_001_CONFIG, restoredText) || restoredText != originalText)
        {
            Print("[S77Migrate][TEST001] CRITICAL: restored runtime config differs from original; backup retained at " + S77_MIGRATE_SCENARIO_001_BACKUP);
            return false;
        }

        DeleteFile(S77_MIGRATE_SCENARIO_001_BACKUP);
        return true;
    }

    protected static void DeleteTemporaryFiles()
    {
        if (FileExist(S77_MIGRATE_SCENARIO_001_TEMP))
            DeleteFile(S77_MIGRATE_SCENARIO_001_TEMP);
    }
}
