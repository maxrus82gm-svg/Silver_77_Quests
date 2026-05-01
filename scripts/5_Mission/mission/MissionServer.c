// ============================================================================
// ИНИЦИАЛИЗАЦИЯ И ЛОГИКА НА СЕРВЕРЕ
// ============================================================================

modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        
        Print("[Silver_77_Quests] MissionServer.OnInit called");
        QuestManager.Init();
        QuestTriggerManager.Init();
        Print("[Silver_77_Quests] Initialization complete");
    }
}
