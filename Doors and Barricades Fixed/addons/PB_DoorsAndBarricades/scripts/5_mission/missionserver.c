modded class MissionServer
{
    private ref PBConfig m_PBConfig;

    void MissionServer()
    {
        GetRPCManager().AddRPC("PBConfig", "GetPBConfigCfg", this, SingeplayerExecutionType.Server);
    }

    override void OnInit()
    {
        super.OnInit();
        
        Print("[PBConfig] Server Cfg loading..."); 
       
        m_PBConfig = new PBConfig();
        g_Game.SetPBConfig( m_PBConfig );
    }

    void GetPBConfigCfg(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type == CallType.Server && sender != null) //Verify that sender is still around so we don't send a null RPC ( sending and RPC with null target will send it to all players in the session )
        {
            Print("[PBConfig] MissionServer --> Sending instance of cfg to client: " + sender.GetName());
            GetRPCManager().SendRPC("PBConfig", "PBConfigStoreConfig", new Param1<ref PBConfig>( m_PBConfig ), true, sender);
        }
    }

};
