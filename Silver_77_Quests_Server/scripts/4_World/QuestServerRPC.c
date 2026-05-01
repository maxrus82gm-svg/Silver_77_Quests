// ============================================================================
// SERVER-AUTHORITATIVE QUEST RPC
// ============================================================================

modded class PlayerBase
{
    override void EEInit()
    {
        super.EEInit();
        
        if (!GetGame().IsServer() || !GetIdentity())
            return;
        
        Print("[Silver_77_Quests] PlayerBase.EEInit bootstrap for quest server: " + GetIdentity().GetId());
        QuestServerManager.EnsureQuestNpcsSpawned();
    }
    
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);
        
        switch (rpc_type)
        {
            case SILVER77_QUEST_RPC_CONFIG_REQUEST:
                Silver77_HandleQuestConfigRequest(sender);
                break;
            
            case SILVER77_QUEST_RPC_PLAYER_DATA_REQUEST:
                Silver77_HandleQuestPlayerDataRequest(sender);
                break;
            
            case SILVER77_QUEST_RPC_ACCEPT:
                Silver77_HandleQuestAccept(sender, ctx);
                break;
            
            case SILVER77_QUEST_RPC_COMPLETE:
                Silver77_HandleQuestComplete(sender, ctx);
                break;
        }
    }
    
    private bool Silver77_IsValidQuestSender(PlayerIdentity sender)
    {
        if (!GetGame().IsServer())
            return false;
        
        if (!GetIdentity())
            return false;
        
        if (!sender)
        {
            Print("[Silver_77_Quests] Quest RPC sender is null, accepting because RPC target is player");
            return true;
        }
        
        return sender.GetId() == GetIdentity().GetId();
    }
    
    private void Silver77_HandleQuestConfigRequest(PlayerIdentity sender)
    {
        if (!Silver77_IsValidQuestSender(sender))
        {
            Print("[Silver_77_Quests] Rejected quest config request RPC");
            return;
        }
        
        Print("[Silver_77_Quests] Received quest config request RPC");
        QuestServerManager.SendConfigToClient(this);
    }
    
    private void Silver77_HandleQuestPlayerDataRequest(PlayerIdentity sender)
    {
        if (!Silver77_IsValidQuestSender(sender))
        {
            Print("[Silver_77_Quests] Rejected quest progress request RPC");
            return;
        }
        
        Print("[Silver_77_Quests] Received quest progress request RPC");
        QuestServerManager.SendPlayerDataToClient(this);
    }
    
    private void Silver77_HandleQuestAccept(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!Silver77_IsValidQuestSender(sender))
        {
            Print("[Silver_77_Quests] Rejected accept quest RPC");
            return;
        }
        
        Param2<string, string> data = new Param2<string, string>("", "");
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read accept quest RPC");
            return;
        }
        
        bool accepted = QuestServerManager.AcceptQuest(this, data.param1, data.param2);
        Print("[Silver_77_Quests] Accept quest RPC result for " + data.param1 + " via trigger " + data.param2 + ": " + accepted);
        QuestServerManager.SendPlayerDataToClient(this);
    }
    
    private void Silver77_HandleQuestComplete(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!Silver77_IsValidQuestSender(sender))
        {
            Print("[Silver_77_Quests] Rejected complete quest RPC");
            return;
        }
        
        Param2<string, string> data = new Param2<string, string>("", "");
        if (!ctx.Read(data))
        {
            Print("[Silver_77_Quests] ERROR: Failed to read complete quest RPC");
            return;
        }
        
        bool completed = QuestServerManager.CompleteQuest(this, data.param1, data.param2);
        Print("[Silver_77_Quests] Complete quest RPC result for " + data.param1 + " via trigger " + data.param2 + ": " + completed);
        QuestServerManager.SendPlayerDataToClient(this);
    }
}

