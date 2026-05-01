// ============================================================================
// CLIENT-SIDE RPC HANDLERS FOR QUEST SYNC
// ============================================================================

const string SILVER77_QUEST_CLIENT_SYNC_DIR = "$profile:Silver_77_Quests";
const string SILVER77_QUEST_CLIENT_CONFIG_SYNC_PATH = "$profile:Silver_77_Quests/client_synced_quest_config.json";
const string SILVER77_QUEST_CLIENT_PLAYER_SYNC_PATH = "$profile:Silver_77_Quests/client_synced_player_data.json";
ref map<int, string> g_Silver77_QuestConfigSyncChunks;
int g_Silver77_QuestConfigSyncExpectedChunkCount = 0;

void Silver77_ResetQuestConfigSyncBuffer()
{
    g_Silver77_QuestConfigSyncChunks = new map<int, string>;
    g_Silver77_QuestConfigSyncExpectedChunkCount = 0;
}

string Silver77_StoreQuestConfigSyncChunk(int chunkIndex, int totalChunks, string chunkPayload)
{
    if (chunkIndex < 0 || totalChunks <= 0 || chunkIndex >= totalChunks || chunkPayload == "")
    {
        Print("[Silver_77_Quests] ERROR: Invalid quest config chunk received");
        Silver77_ResetQuestConfigSyncBuffer();
        return "";
    }
    
    if (!g_Silver77_QuestConfigSyncChunks || g_Silver77_QuestConfigSyncExpectedChunkCount != totalChunks || (chunkIndex == 0 && g_Silver77_QuestConfigSyncChunks.Count() > 0))
    {
        Silver77_ResetQuestConfigSyncBuffer();
        g_Silver77_QuestConfigSyncExpectedChunkCount = totalChunks;
    }
    
    g_Silver77_QuestConfigSyncChunks.Set(chunkIndex, chunkPayload);
    Print("[Silver_77_Quests] Received quest config chunk " + (chunkIndex + 1) + "/" + totalChunks + " (" + chunkPayload.Length() + " bytes)");
    
    if (g_Silver77_QuestConfigSyncChunks.Count() < g_Silver77_QuestConfigSyncExpectedChunkCount)
        return "";
    
    string payload = "";
    for (int i = 0; i < g_Silver77_QuestConfigSyncExpectedChunkCount; i++)
    {
        if (!g_Silver77_QuestConfigSyncChunks.Contains(i))
        {
            Print("[Silver_77_Quests] ERROR: Missing quest config chunk " + i);
            Silver77_ResetQuestConfigSyncBuffer();
            return "";
        }
        
        payload += g_Silver77_QuestConfigSyncChunks.Get(i);
    }
    
    Silver77_ResetQuestConfigSyncBuffer();
    return payload;
}

modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);
        
        switch (rpc_type)
        {
            case SILVER77_QUEST_RPC_CONFIG_DATA:
                Silver77_HandleQuestConfigData(ctx);
                break;
            
            case SILVER77_QUEST_RPC_PLAYER_DATA:
                Silver77_HandleQuestPlayerData(ctx);
                break;
        }
    }
    
    private void Silver77_HandleQuestConfigData(ParamsReadContext ctx)
    {
        if (GetGame().IsDedicatedServer())
            return;
        
        Param3<int, int, string> data = new Param3<int, int, string>(0, 0, "");
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read quest config sync RPC");
            return;
        }
        
        string payload = Silver77_StoreQuestConfigSyncChunk(data.param1, data.param2, data.param3);
        if (payload == "")
            return;
        
        Print("[Silver_77_Quests] Reassembled quest config sync payload (" + payload.Length() + " bytes)");
        Silver77_QuestConfig config = Silver77_LoadQuestConfigFromJson(payload);
        if (!config)
        {
            Print("[Silver_77_Quests] ERROR: Failed to decode quest config sync payload");
            return;
        }
        
        QuestClientManager.ApplySyncedConfig(config);
    }
    
    private void Silver77_HandleQuestPlayerData(ParamsReadContext ctx)
    {
        if (GetGame().IsDedicatedServer())
            return;
        
        Param1<string> data = new Param1<string>("");
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read quest progress sync RPC");
            return;
        }

        Print("[Silver_77_Quests] Received quest progress sync payload (" + data.param1.Length() + " bytes)");
        
        PlayerQuestData playerData = Silver77_LoadPlayerDataFromJson(data.param1);
        if (!playerData)
        {
            Print("[Silver_77_Quests] ERROR: Failed to decode quest progress sync payload");
            return;
        }

        if (playerData.steamId == "" && GetIdentity())
        {
            playerData.steamId = GetIdentity().GetId();
            Print("[Silver_77_Quests] WARNING: Synced player progress had empty steamId, applied local identity fallback: " + playerData.steamId);
        }

        int progressCount = 0;
        if (playerData.progress)
            progressCount = playerData.progress.Count();
        Print("[Silver_77_Quests] Decoded quest progress sync for " + playerData.steamId + " with " + progressCount + " quest entries");
        
        QuestClientManager.ApplySyncedPlayerData(playerData);
    }
    
    private void Silver77_EnsureClientSyncDirectory()
    {
        if (!FileExist(SILVER77_QUEST_CLIENT_SYNC_DIR))
            MakeDirectory(SILVER77_QUEST_CLIENT_SYNC_DIR);
    }
    
    private bool Silver77_WriteSyncJsonFile(string filePath, string jsonPayload)
    {
        if (filePath == "" || jsonPayload == "")
            return false;
        
        Silver77_EnsureClientSyncDirectory();
        
        FileHandle handle = OpenFile(filePath, FileMode.WRITE);
        if (handle == 0)
        {
            Print("[Silver_77_Quests] ERROR: OpenFile WRITE failed for sync payload: " + filePath);
            return false;
        }
        
        FPrint(handle, jsonPayload);
        CloseFile(handle);
        return true;
    }
    
    private Silver77_QuestConfig Silver77_LoadQuestConfigFromJson(string jsonPayload)
    {
        if (!Silver77_WriteSyncJsonFile(SILVER77_QUEST_CLIENT_CONFIG_SYNC_PATH, jsonPayload))
            return null;
        
        Silver77_QuestConfig config = new Silver77_QuestConfig();
        JsonFileLoader<Silver77_QuestConfig>.JsonLoadFile(SILVER77_QUEST_CLIENT_CONFIG_SYNC_PATH, config);
        return config;
    }
    
    private PlayerQuestData Silver77_LoadPlayerDataFromJson(string jsonPayload)
    {
        if (!Silver77_WriteSyncJsonFile(SILVER77_QUEST_CLIENT_PLAYER_SYNC_PATH, jsonPayload))
            return null;
        
        PlayerQuestData data = new PlayerQuestData();
        JsonFileLoader<PlayerQuestData>.JsonLoadFile(SILVER77_QUEST_CLIENT_PLAYER_SYNC_PATH, data);
        return data;
    }
}
