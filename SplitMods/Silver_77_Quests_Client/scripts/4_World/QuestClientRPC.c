// ============================================================================
// CLIENT-SIDE RPC HANDLERS FOR QUEST SYNC
// ============================================================================

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
        
        Param1<Silver77_QuestConfig> data = new Param1<Silver77_QuestConfig>(null);
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read quest config sync RPC");
            return;
        }
        
        QuestClientManager.ApplySyncedConfig(data.param1);
    }
    
    private void Silver77_HandleQuestPlayerData(ParamsReadContext ctx)
    {
        if (GetGame().IsDedicatedServer())
            return;
        
        Param1<PlayerQuestData> data = new Param1<PlayerQuestData>(null);
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read quest progress sync RPC");
            return;
        }
        
        QuestClientManager.ApplySyncedPlayerData(data.param1);
    }
}
