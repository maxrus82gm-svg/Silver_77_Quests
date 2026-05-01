// ============================================================================
// ИНИЦИАЛИЗАЦИЯ И ЛОГИКА НА СЕРВЕРЕ
// ============================================================================

modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        
        Print("[Silver_77_Quests] MissionServer.OnInit called");
        QuestServerManager.Init();
        Print("[Silver_77_Quests] Initialization complete");
    }
}

