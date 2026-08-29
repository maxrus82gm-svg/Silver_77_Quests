modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        Print("[S77Migrate][TEST001] MissionServer.OnInit");
        S77MigrateManager.Init();
    }

    override void OnMissionFinish()
    {
        S77MigrateManager.Shutdown();
        super.OnMissionFinish();
    }
}
