// ============================================================================
// CLIENT-SIDE QUEST STATE AND RPC REQUESTS
// ============================================================================

ref Silver77_QuestConfig g_ClientQuestConfig;
ref map<string, ref PlayerQuestData> g_ClientPlayerQuestData;
int g_ClientQuestConfigRevision = 0;
int g_ClientQuestDataRevision = 0;

void Silver77_ClientNormalizeQuestConfig(Silver77_QuestConfig config)
{
    if (!config)
        return;
    
    if (!config.quests)
        config.quests = new array<ref Silver77_Quest>;
    
    if (!config.triggers)
        config.triggers = new array<ref Silver77_QuestTriggerConfig>;
    
    foreach (Silver77_Quest quest : config.quests)
    {
        if (!quest)
            continue;
        
        if (!quest.giveItems)
            quest.giveItems = new array<ref Silver77_QuestItem>;
        
        if (!quest.objectives)
            quest.objectives = new array<ref Silver77_QuestObjective>;
        
        if (!quest.rewards)
            quest.rewards = new array<ref Silver77_QuestItem>;
    }
    
    foreach (Silver77_QuestTriggerConfig trigger : config.triggers)
    {
        if (!trigger)
            continue;
        
        if (!trigger.position)
            trigger.position = new array<float>;
        
        if (!trigger.questIds)
            trigger.questIds = new array<string>;
    }
}

class QuestClientManager
{
    static void Init()
    {
        g_ClientQuestConfig = new Silver77_QuestConfig();
        Silver77_ClientNormalizeQuestConfig(g_ClientQuestConfig);
        g_ClientPlayerQuestData = new map<string, ref PlayerQuestData>;
        g_ClientQuestConfigRevision++;
        g_ClientQuestDataRevision++;
        Print("[Silver_77_Quests] Client quest manager initialized");
    }
    
    static Silver77_Quest GetQuestById(string questId)
    {
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.quests)
            return null;
        
        foreach (Silver77_Quest quest : g_ClientQuestConfig.quests)
        {
            if (!quest)
                continue;
            
            if (quest.id == questId)
                return quest;
        }
        
        return null;
    }
    
    static string GetPlayerSteamId(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return "";
        
        return player.GetIdentity().GetId();
    }
    
    static PlayerQuestData CreatePlayerData(string steamId)
    {
        PlayerQuestData data = new PlayerQuestData();
        data.steamId = steamId;
        EnsurePlayerProgress(data);
        return data;
    }
    
    static PlayerQuestData GetPlayerData(PlayerBase player)
    {
        string steamId = GetPlayerSteamId(player);
        if (steamId == "")
            return null;
        
        if (!g_ClientPlayerQuestData)
            g_ClientPlayerQuestData = new map<string, ref PlayerQuestData>;
        
        if (!g_ClientPlayerQuestData.Contains(steamId))
            g_ClientPlayerQuestData.Insert(steamId, CreatePlayerData(steamId));
        
        PlayerQuestData data = g_ClientPlayerQuestData.Get(steamId);
        EnsurePlayerProgress(data);
        return data;
    }
    
    static bool EnsurePlayerProgress(PlayerQuestData data)
    {
        if (!data)
            return false;
        
        bool changed = false;
        
        if (!data.progress)
        {
            data.progress = new array<ref PlayerQuestProgress>;
            changed = true;
        }
        
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.quests)
            return changed;
        
        foreach (Silver77_Quest quest : g_ClientQuestConfig.quests)
        {
            if (!quest)
                continue;
            
            PlayerQuestProgress progress = FindProgress(data, quest.id);
            if (!progress)
            {
                progress = new PlayerQuestProgress();
                progress.questId = quest.id;
                progress.status = "available";
                data.progress.Insert(progress);
                changed = true;
            }
            
            if (progress && !progress.objectiveProgress)
            {
                progress.objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
                changed = true;
            }
        }
        
        return changed;
    }
    
    static PlayerQuestProgress FindProgress(PlayerQuestData data, string questId)
    {
        if (!data || !data.progress)
            return null;
        
        foreach (PlayerQuestProgress progress : data.progress)
        {
            if (progress.questId == questId)
                return progress;
        }
        
        return null;
    }
    
    static string GetQuestStatus(PlayerBase player, string questId)
    {
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (progress)
            return progress.status;
        
        return "available";
    }
    
    static int GetCurrentUnixTimeUTC()
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;
        
        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);
        
        if (year < 1970)
            return GetGame().GetTime() / 1000;
        
        int days = 0;
        for (int y = 1970; y < year; y++)
        {
            if (IsLeapYear(y))
                days += 366;
            else
                days += 365;
        }
        
        for (int m = 1; m < month; m++)
        {
            days += GetDaysInMonth(year, m);
        }
        
        days += day - 1;
        return days * 86400 + hour * 3600 + minute * 60 + second;
    }
    
    static bool IsLeapYear(int year)
    {
        if (year % 400 == 0)
            return true;
        
        if (year % 100 == 0)
            return false;
        
        return year % 4 == 0;
    }
    
    static int GetDaysInMonth(int year, int month)
    {
        switch (month)
        {
            case 1:
                return 31;
            case 2:
                if (IsLeapYear(year))
                    return 29;
                return 28;
            case 3:
                return 31;
            case 4:
                return 30;
            case 5:
                return 31;
            case 6:
                return 30;
            case 7:
                return 31;
            case 8:
                return 31;
            case 9:
                return 30;
            case 10:
                return 31;
            case 11:
                return 30;
            case 12:
                return 31;
        }
        
        return 30;
    }
    
    static bool IsQuestOnCooldown(PlayerBase player, string questId)
    {
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest || !quest.repeatable || quest.cooldownSeconds <= 0)
            return false;
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (!progress || progress.lastCompletedTime == 0)
            return false;
        
        int currentTime = GetCurrentUnixTimeUTC();
        if (progress.lastCompletedTime > currentTime)
            return false;
        
        int timePassed = currentTime - progress.lastCompletedTime;
        return timePassed < quest.cooldownSeconds;
    }
    
    static bool CanAcceptQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        string status = GetQuestStatus(player, questId);
        if (status == "active")
            return false;
        
        if (status == "completed")
        {
            if (!quest.repeatable)
                return false;
            if (IsQuestOnCooldown(player, questId))
                return false;
        }
        
        if (quest.requiresPrevious != "")
        {
            string prevStatus = GetQuestStatus(player, quest.requiresPrevious);
            if (prevStatus != "completed")
                return false;
        }
        
        return true;
    }
    
    static float GetDepositedObjectiveQuantity(PlayerQuestProgress progress, int objectiveIndex, string className)
    {
        if (!progress || !progress.objectiveProgress)
            return 0;
        
        foreach (PlayerQuestObjectiveProgress objectiveProgress : progress.objectiveProgress)
        {
            if (!objectiveProgress)
                continue;
            
            if (objectiveProgress.objectiveIndex == objectiveIndex && objectiveProgress.className == className)
                return objectiveProgress.depositedQuantity;
        }
        
        return 0;
    }
    
    static float GetQuestObjectiveDeposited(PlayerBase player, string questId, int objectiveIndex, string className)
    {
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        return GetDepositedObjectiveQuantity(progress, objectiveIndex, className);
    }
    
    static bool CanCompleteQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        if (GetQuestStatus(player, questId) != "active")
            return false;
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective || objective.type != "item")
                continue;
            
            float deposited = 0;
            if (objective.allowPartialTurnIn)
                deposited = GetDepositedObjectiveQuantity(progress, i, objective.className);
            
            float remaining = objective.quantity - deposited;
            if (remaining <= 0)
                continue;
            
            if (!PlayerHasItemAmount(player, objective.className, remaining, objective.useItemQuantity))
                return false;
        }
        
        return true;
    }
    
    static bool CanDepositQuestItems(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        if (GetQuestStatus(player, questId) != "active")
            return false;
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective || objective.type != "item" || !objective.allowPartialTurnIn)
                continue;
            
            float deposited = GetDepositedObjectiveQuantity(progress, i, objective.className);
            if (deposited >= objective.quantity)
                continue;
            
            if (CountPlayerItems(player, objective.className, objective.useItemQuantity) > 0)
                return true;
        }
        
        return false;
    }
    
    static bool PlayerHasItems(PlayerBase player, string className, int quantity, bool useItemQuantity = false)
    {
        return PlayerHasItemAmount(player, className, quantity, useItemQuantity);
    }
    
    static bool PlayerHasItemAmount(PlayerBase player, string className, float quantity, bool useItemQuantity = false)
    {
        if (!player || className == "" || quantity <= 0)
            return false;
        
        return CountPlayerItems(player, className, useItemQuantity) >= quantity;
    }
    
    static float CountPlayerItems(PlayerBase player, string className, bool useItemQuantity = false)
    {
        if (!player || className == "")
            return 0;
        
        float count = 0;
        array<EntityAI> items = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        foreach (EntityAI item : items)
        {
            if (item && item.IsKindOf(className))
            {
                if (useItemQuantity && item.HasQuantity())
                    count += item.GetQuantity();
                else
                    count++;
            }
        }
        
        return count;
    }
    
    static bool RequestAcceptQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Print("[Silver_77_Quests] Sending accept quest RPC: " + questId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_ACCEPT, new Param1<string>(questId), true);
        return true;
    }
    
    static bool RequestCompleteQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Print("[Silver_77_Quests] Sending complete quest RPC: " + questId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_COMPLETE, new Param1<string>(questId), true);
        return true;
    }
    
    static void RequestConfigSync(PlayerBase player)
    {
        if (!player)
            return;
        
        Print("[Silver_77_Quests] Sending quest config request RPC");
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_CONFIG_REQUEST, new Param1<bool>(true), true);
    }
    
    static void RequestPlayerDataSync(PlayerBase player)
    {
        if (!player)
            return;
        
        Print("[Silver_77_Quests] Sending quest progress request RPC");
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_PLAYER_DATA_REQUEST, new Param1<bool>(true), true);
    }
    
    static void ApplySyncedConfig(Silver77_QuestConfig config)
    {
        if (!config)
            return;
        
        Silver77_ClientNormalizeQuestConfig(config);
        g_ClientQuestConfig = config;
        g_ClientQuestConfigRevision++;
        
        int questCount = 0;
        int triggerCount = 0;
        if (g_ClientQuestConfig.quests)
            questCount = g_ClientQuestConfig.quests.Count();
        if (g_ClientQuestConfig.triggers)
            triggerCount = g_ClientQuestConfig.triggers.Count();
        
        Print("[Silver_77_Quests] Applied synced quest config: " + questCount + " quests, " + triggerCount + " triggers");
    }
    
    static void ApplySyncedPlayerData(PlayerQuestData data)
    {
        if (!data || data.steamId == "")
            return;
        
        if (!g_ClientPlayerQuestData)
            g_ClientPlayerQuestData = new map<string, ref PlayerQuestData>;
        
        EnsurePlayerProgress(data);
        g_ClientPlayerQuestData.Set(data.steamId, data);
        g_ClientQuestDataRevision++;
        Print("[Silver_77_Quests] Applied synced player progress: " + data.steamId);
    }
}
