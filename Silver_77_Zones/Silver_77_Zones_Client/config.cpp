class CfgPatches
{
    class Silver_77_Zones_Client
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
    };
};

class CfgMods
{
    class Silver_77_Zones_Client
    {
        dir = "Silver_77_Zones_Client";
        name = "Silver 77 Zones Client";
        author = "Silver_77";
        version = "0.1";
        type = "mod";
        dependencies[] = {"Mission"};

        class defs
        {
            class missionScriptModule
            {
                value = "";
                files[] = {"Silver_77_Zones_Client/scripts/5_Mission"};
            };
        };
    };
};
