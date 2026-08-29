class CfgPatches
{
    class Silver_77_Migrate_server
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Characters_Zombies"
        };
    };
};

class CfgMods
{
    class Silver_77_Migrate_server
    {
        dir = "Silver_77_Migrate_server";
        name = "Silver 77 Migrate Server";
        type = "mod";
        dependencies[] =
        {
            "World",
            "Mission"
        };

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "Silver_77_Migrate_server/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] =
                {
                    "Silver_77_Migrate_server/scripts/5_Mission"
                };
            };
        };
    };
};
