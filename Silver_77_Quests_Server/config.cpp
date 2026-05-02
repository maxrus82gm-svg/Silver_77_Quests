class CfgPatches
{
    class Silver_77_Quests_Server
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts", "Silver_77_Quests_Client"};
    };
};

class CfgMods
{
    class Silver_77_Quests_Server
    {
        dir = "Silver_77_Quests_Server";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Silver 77 Quests Server";
        credits = "";
        author = "Silver_77";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";
        
        dependencies[] = {"World", "Mission"};
        
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"Silver_77_Quests/scripts/3_Game"};
            };
            
            class worldScriptModule
            {
                value = "";
                files[] = {"Silver_77_Quests_Server/scripts/4_World"};
            };
            
            class missionScriptModule
            {
                value = "";
                files[] = {"Silver_77_Quests_Server/scripts/5_Mission"};
            };
        };
    };
};
