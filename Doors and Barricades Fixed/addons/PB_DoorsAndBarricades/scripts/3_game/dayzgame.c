modded class DayZGame
{
    private ref PBConfig m_PBConfig;

    ref PBConfig GetPBConfig()
    {
        if (m_PBConfig)
        {
            return m_PBConfig;
        }

        Print("[PBConfig] DayZGame --> Cannot Locate PBConfig Config File...");
	    return null;
    }

    void SetPBConfig(ref PBConfig cfg)
	{
		m_PBConfig = cfg;
	}
}