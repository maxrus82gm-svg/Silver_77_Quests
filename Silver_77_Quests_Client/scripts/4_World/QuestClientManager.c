// ============================================================================
// CLIENT-SIDE QUEST STATE AND RPC REQUESTS
// ============================================================================

ref Silver77_QuestConfig g_ClientQuestConfig;
ref map<string, ref PlayerQuestData> g_ClientPlayerQuestData;
int g_ClientQuestConfigRevision = 0;
int g_ClientQuestDataRevision = 0;

void Silver77_ClientSeedQuestRoles(Silver77_QuestConfig config)
{
    if (!config || !config.quests || !config.triggers)
        return;

    foreach (Silver77_Quest quest : config.quests)
    {
        if (!quest)
            continue;

        if (!quest.offerTriggerIds)
            quest.offerTriggerIds = new array<string>;

        if (!quest.completionTriggerIds)
            quest.completionTriggerIds = new array<string>;

        if (!quest.rewardTriggerIds)
            quest.rewardTriggerIds = new array<string>;

        if (!quest.triggerActions)
            quest.triggerActions = new array<ref Silver77_QuestTriggerAction>;

        array<string> assignedTriggerIds = new array<string>;
        foreach (Silver77_QuestTriggerConfig trigger : config.triggers)
        {
            if (!trigger || !trigger.questIds)
                continue;

            if (trigger.questIds.Find(quest.id) != -1 && assignedTriggerIds.Find(trigger.id) == -1)
                assignedTriggerIds.Insert(trigger.id);
        }

        Silver77_ClientNormalizeSingleTriggerRoleIds(quest.offerTriggerIds);
        Silver77_ClientNormalizeSingleTriggerRoleIds(quest.rewardTriggerIds);

        if (quest.offerTriggerIds.Count() == 0 && assignedTriggerIds.Count() > 0)
            quest.offerTriggerIds.Insert(assignedTriggerIds.Get(0));

        if (quest.rewardTriggerIds.Count() == 0 && assignedTriggerIds.Count() > 0)
            quest.rewardTriggerIds.Insert(assignedTriggerIds.Get(0));
    }
}

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

        if (!quest.requiredQuestIds)
            quest.requiredQuestIds = new array<string>;

        if (!quest.offerTriggerIds)
            quest.offerTriggerIds = new array<string>;

        if (!quest.completionTriggerIds)
            quest.completionTriggerIds = new array<string>;

        if (!quest.rewardTriggerIds)
            quest.rewardTriggerIds = new array<string>;

        if (!quest.triggerActions)
            quest.triggerActions = new array<ref Silver77_QuestTriggerAction>;

        Silver77_ClientNormalizeSingleTriggerRoleIds(quest.offerTriggerIds);
        Silver77_ClientNormalizeSingleTriggerRoleIds(quest.rewardTriggerIds);

        foreach (Silver77_QuestTriggerAction action : quest.triggerActions)
        {
            if (!action)
                continue;

            if (!action.rewards)
                action.rewards = new array<ref Silver77_QuestItem>;
        }
    }

    foreach (Silver77_QuestTriggerConfig trigger : config.triggers)
    {
        if (!trigger)
            continue;

        if (!trigger.position)
            trigger.position = new array<float>;

        if (!trigger.questIds)
            trigger.questIds = new array<string>;

        if (!trigger.npcPosition)
            trigger.npcPosition = new array<float>;

        if (!trigger.npcOrientation)
            trigger.npcOrientation = new array<float>;

        if (!trigger.npcLoadout)
            trigger.npcLoadout = new array<string>;

        if (!trigger.npcBackItems)
            trigger.npcBackItems = new array<string>;

        if (trigger.radius <= 0)
            trigger.radius = 2.0;

        if (trigger.focusHeight <= 0)
            trigger.focusHeight = 1.2;

        if (trigger.focusRadius <= 0)
            trigger.focusRadius = 1.0;
    }

    Silver77_ClientSeedQuestRoles(config);
}

void Silver77_ClientNormalizeSingleTriggerRoleIds(array<string> triggerIds)
{
    if (!triggerIds)
        return;

    array<string> normalizedTriggerIds = new array<string>;
    foreach (string triggerId : triggerIds)
    {
        if (triggerId == "")
            continue;

        normalizedTriggerIds.Insert(triggerId);
        break;
    }

    triggerIds.Clear();
    foreach (string normalizedTriggerId : normalizedTriggerIds)
    {
        triggerIds.Insert(normalizedTriggerId);
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

    static Silver77_QuestTriggerConfig GetTriggerById(string triggerId)
    {
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.triggers || triggerId == "")
            return null;

        foreach (Silver77_QuestTriggerConfig trigger : g_ClientQuestConfig.triggers)
        {
            if (!trigger)
                continue;

            if (trigger.id == triggerId)
                return trigger;
        }

        return null;
    }

    static void AppendUniqueTriggerId(array<string> target, string triggerId)
    {
        if (!target || triggerId == "" || target.Find(triggerId) != -1)
            return;

        target.Insert(triggerId);
    }

    static void AppendUniqueTriggerIds(array<string> source, array<string> target)
    {
        if (!source || !target)
            return;

        foreach (string triggerId : source)
        {
            AppendUniqueTriggerId(target, triggerId);
        }
    }

    static void AppendFirstValidTriggerId(array<string> source, array<string> target)
    {
        if (!source || !target)
            return;

        foreach (string triggerId : source)
        {
            if (triggerId == "")
                continue;

            AppendUniqueTriggerId(target, triggerId);
            return;
        }
    }

    static bool IsQuestAssignedToTrigger(string questId, string triggerId)
    {
        if (questId == "" || triggerId == "")
            return false;

        Silver77_QuestTriggerConfig trigger = GetTriggerById(triggerId);
        if (!trigger || !trigger.questIds)
            return false;

        return trigger.questIds.Find(questId) != -1;
    }

    static array<string> GetAssignedTriggerIdsForQuest(string questId)
    {
        array<string> triggerIds = new array<string>;
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.triggers || questId == "")
            return triggerIds;

        foreach (Silver77_QuestTriggerConfig trigger : g_ClientQuestConfig.triggers)
        {
            if (!trigger || !trigger.questIds)
                continue;

            if (trigger.questIds.Find(questId) != -1)
                AppendUniqueTriggerId(triggerIds, trigger.id);
        }

        return triggerIds;
    }

    static array<string> GetQuestOfferTriggerIds(Silver77_Quest quest)
    {
        array<string> triggerIds = new array<string>;
        if (!quest)
            return triggerIds;

        AppendFirstValidTriggerId(quest.offerTriggerIds, triggerIds);
        return triggerIds;
    }

    static array<string> GetQuestCompletionTriggerIds(Silver77_Quest quest)
    {
        array<string> triggerIds = new array<string>;
        if (!quest)
            return triggerIds;

        AppendUniqueTriggerIds(quest.completionTriggerIds, triggerIds);
        return triggerIds;
    }

    static array<string> GetQuestRewardTriggerIds(Silver77_Quest quest)
    {
        array<string> triggerIds = new array<string>;
        if (!quest)
            return triggerIds;

        AppendFirstValidTriggerId(quest.rewardTriggerIds, triggerIds);
        return triggerIds;
    }

    static string GetTriggerDisplayText(string triggerId)
    {
        if (triggerId == "")
            return "";

        Silver77_QuestTriggerConfig trigger = GetTriggerById(triggerId);
        if (!trigger)
            return triggerId;

        if (trigger.hintText != "")
            return trigger.hintText;

        return trigger.id;
    }

    static string GetNextQuestTriggerId(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return "";

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return "";

        string status = GetQuestStatus(player, questId);
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);

        if (status == "active")
        {
            array<string> completionTriggerIds = GetQuestCompletionTriggerIds(quest);
            foreach (string completionTriggerId : completionTriggerIds)
            {
                if (completionTriggerId == "")
                    continue;

                if (!IsCompletionTriggerDone(progress, completionTriggerId))
                    return completionTriggerId;
            }

            array<string> rewardTriggerIds = GetQuestRewardTriggerIds(quest);
            if (rewardTriggerIds.Count() > 0)
                return rewardTriggerIds.Get(0);

            return "";
        }

        if (status == "reward_pending")
        {
            array<string> pendingRewardTriggerIds = GetQuestRewardTriggerIds(quest);
            if (pendingRewardTriggerIds.Count() > 0)
                return pendingRewardTriggerIds.Get(0);
        }

        if (status == "available" || status == "completed")
        {
            array<string> offerTriggerIds = GetQuestOfferTriggerIds(quest);
            if (offerTriggerIds.Count() > 0)
                return offerTriggerIds.Get(0);
        }

        return "";
    }

    static Silver77_QuestTriggerAction GetQuestTriggerAction(Silver77_Quest quest, string triggerId, string actionType)
    {
        if (!quest || !quest.triggerActions || triggerId == "" || actionType == "")
            return null;

        foreach (Silver77_QuestTriggerAction action : quest.triggerActions)
        {
            if (!action)
                continue;

            if (action.triggerId == triggerId && action.actionType == actionType)
                return action;
        }

        return null;
    }

    static string GetQuestTriggerActionDialogText(Silver77_Quest quest, string triggerId, string actionType)
    {
        Silver77_QuestTriggerAction action = GetQuestTriggerAction(quest, triggerId, actionType);
        if (!action)
            return "";

        return action.dialogText;
    }

    static array<ref Silver77_QuestItem> GetQuestActionRewards(Silver77_Quest quest, string triggerId, string actionType)
    {
        Silver77_QuestTriggerAction action = GetQuestTriggerAction(quest, triggerId, actionType);
        if (action && action.rewards && action.rewards.Count() > 0)
            return action.rewards;

        if (actionType != "reward" || !quest || !quest.rewards)
            return null;

        return quest.rewards;
    }

    static bool IsQuestTriggerAllowed(Silver77_Quest quest, string triggerId, array<string> allowedTriggerIds)
    {
        if (!quest || triggerId == "")
            return false;

        if (!IsQuestAssignedToTrigger(quest.id, triggerId))
            return false;

        if (!allowedTriggerIds || allowedTriggerIds.Count() == 0)
            return true;

        return allowedTriggerIds.Find(triggerId) != -1;
    }

    static bool QuestRoleContains(array<string> triggerIds, string triggerId)
    {
        if (!triggerIds || triggerId == "")
            return false;

        return triggerIds.Find(triggerId) != -1;
    }

    static bool QuestHasRoleTriggers(array<string> triggerIds)
    {
        return triggerIds && triggerIds.Count() > 0;
    }

    static bool IsCompletionTriggerDone(PlayerQuestProgress progress, string triggerId)
    {
        if (!progress || !progress.completedCompletionTriggerIds || triggerId == "")
            return false;

        return progress.completedCompletionTriggerIds.Find(triggerId) != -1;
    }

    static bool AreAllCompletionTriggersDone(Silver77_Quest quest, PlayerQuestProgress progress)
    {
        if (!quest || !quest.completionTriggerIds || quest.completionTriggerIds.Count() == 0)
            return true;

        foreach (string triggerId : quest.completionTriggerIds)
        {
            if (triggerId == "")
                continue;

            if (!IsCompletionTriggerDone(progress, triggerId))
                return false;
        }

        return true;
    }

    static bool IsQuestVisibleForTrigger(PlayerBase player, string questId, string triggerId)
    {
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        if (triggerId == "")
            return false;

        string status = GetQuestStatus(player, questId);

        if (!IsQuestAssignedToTrigger(quest.id, triggerId))
            return false;

        if (status == "reward_pending")
            return true;

        if (status == "active")
            return true;

        if (status == "completed")
            return CanAcceptQuest(player, questId, triggerId);

        if (quest.hideUntilRequirementsComplete && !AreQuestRequirementsCompleted(player, quest))
            return false;

        return IsQuestTriggerAllowed(quest, triggerId, GetQuestOfferTriggerIds(quest));
    }

    static bool CanClaimReward(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        string status = GetQuestStatus(player, questId);
        if (status != "active" && status != "reward_pending")
            return false;

        if (triggerId != "" && !IsQuestTriggerAllowed(quest, triggerId, GetQuestRewardTriggerIds(quest)))
            return false;

        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (!AreAllCompletionTriggersDone(quest, progress))
            return false;

        if (QuestHasRoleTriggers(quest.completionTriggerIds))
            return true;

        return AreQuestObjectivesReadyForCompletion(player, quest, progress);
    }

    static bool ShouldHighlightQuestAsReward(PlayerBase player, string questId, string triggerId)
    {
        if (!player || questId == "" || triggerId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        if (!IsQuestTriggerAllowed(quest, triggerId, GetQuestRewardTriggerIds(quest)))
            return false;

        string status = GetQuestStatus(player, questId);
        if (status == "reward_pending")
            return true;

        if (status != "active")
            return false;

        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        return AreAllCompletionTriggersDone(quest, progress);
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

    static bool HasSyncedPlayerData(PlayerBase player)
    {
        string steamId = GetPlayerSteamId(player);
        if (steamId == "")
            return false;

        if (!g_ClientPlayerQuestData)
        {
            Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] HasSyncedPlayerData: g_ClientPlayerQuestData is null for steamId=" + steamId);
            return false;
        }

        bool contains = g_ClientPlayerQuestData.Contains(steamId);
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] HasSyncedPlayerData: steamId=" + steamId + " contains=" + contains);
        return contains;
    }

    static PlayerQuestData GetPlayerData(PlayerBase player)
    {
        string steamId = GetPlayerSteamId(player);
        if (steamId == "")
            return null;

        if (!g_ClientPlayerQuestData)
            g_ClientPlayerQuestData = new map<string, ref PlayerQuestData>;

        bool existsBeforeCreate = g_ClientPlayerQuestData.Contains(steamId);
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] GetPlayerData: steamId=" + steamId + " existsBeforeCreate=" + existsBeforeCreate);

        if (!existsBeforeCreate)
        {
            Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] GetPlayerData: WARNING - creating new PlayerQuestData for steamId=" + steamId);
            g_ClientPlayerQuestData.Insert(steamId, CreatePlayerData(steamId));
        }

        PlayerQuestData data = g_ClientPlayerQuestData.Get(steamId);
        EnsurePlayerProgress(data);

        int progressCount = 0;
        if (data && data.progress)
            progressCount = data.progress.Count();
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] GetPlayerData: returning data with progressCount=" + progressCount);

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

            if (progress && !progress.completedCompletionTriggerIds)
            {
                progress.completedCompletionTriggerIds = new array<string>;
                changed = true;
            }

            if (progress && !progress.stageVisits)
            {
                progress.stageVisits = new array<ref PlayerQuestStageVisit>;
                changed = true;
            }

            if (progress && EnsureRewardProgressState(progress))
                changed = true;
        }

        return changed;
    }

    static bool EnsureRewardProgressState(PlayerQuestProgress progress)
    {
        if (!progress)
            return false;

        bool changed = false;

        if (!progress.pendingRewards)
        {
            progress.pendingRewards = new array<ref QuestPendingRewardProgress>;
            changed = true;
        }

        if (!progress.deliveredRewards)
        {
            progress.deliveredRewards = new array<ref QuestDeliveredRewardProgress>;
            changed = true;
        }

        if (progress.pendingRewards)
        {
            foreach (QuestPendingRewardProgress pendingReward : progress.pendingRewards)
            {
                if (pendingReward && !pendingReward.items)
                {
                    pendingReward.items = new array<ref QuestRewardItemProgress>;
                    changed = true;
                }
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

    static array<ref PlayerQuestStageVisit> GetQuestStageVisits(PlayerBase player, string questId)
    {
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (!progress || !progress.stageVisits)
            return null;

        return progress.stageVisits;
    }

    static bool QuestStageVisitMatches(PlayerQuestStageVisit stageVisit, string triggerId, string actionType)
    {
        if (!stageVisit || triggerId == "" || actionType == "")
            return false;

        return stageVisit.triggerId == triggerId && stageVisit.actionType == actionType;
    }

    static array<ref PlayerQuestStageVisit> GetQuestStageVisitsSorted(PlayerBase player, string questId)
    {
        array<ref PlayerQuestStageVisit> sourceVisits = GetQuestStageVisits(player, questId);
        array<ref PlayerQuestStageVisit> sortedVisits = new array<ref PlayerQuestStageVisit>;
        if (!sourceVisits)
            return sortedVisits;

        foreach (PlayerQuestStageVisit sourceVisit : sourceVisits)
        {
            if (!sourceVisit)
                continue;

            sortedVisits.Insert(sourceVisit);
        }

        for (int i = 0; i < sortedVisits.Count(); i++)
        {
            for (int j = i + 1; j < sortedVisits.Count(); j++)
            {
                PlayerQuestStageVisit leftVisit = sortedVisits.Get(i);
                PlayerQuestStageVisit rightVisit = sortedVisits.Get(j);
                if (!leftVisit || !rightVisit)
                    continue;

                if (rightVisit.firstActivatedTime < leftVisit.firstActivatedTime)
                {
                    sortedVisits.Set(i, rightVisit);
                    sortedVisits.Set(j, leftVisit);
                }
            }
        }

        return sortedVisits;
    }

    static string GetQuestStatus(PlayerBase player, string questId)
    {
        string steamId = GetPlayerSteamId(player);
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);

        if (progress)
        {
            Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] GetQuestStatus: questId=" + questId + " steamId=" + steamId + " found progress, status=" + progress.status);
            return progress.status;
        }

        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] GetQuestStatus: WARNING - questId=" + questId + " steamId=" + steamId + " progress NOT found, returning available");
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

    static bool AreQuestRequirementsCompleted(PlayerBase player, Silver77_Quest quest)
    {
        if (!player || !quest)
            return false;

        if (quest.requiresPrevious != "")
        {
            if (!IsQuestRequirementCompleted(player, quest.requiresPrevious))
                return false;
        }

        if (quest.requiredQuestIds)
        {
            foreach (string requiredQuestId : quest.requiredQuestIds)
            {
                if (requiredQuestId == "")
                    continue;

                if (!IsQuestRequirementCompleted(player, requiredQuestId))
                    return false;
            }
        }

        return true;
    }

    static bool IsQuestRequirementCompleted(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;

        if (GetQuestStatus(player, questId) == "completed")
            return true;

        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (progress && progress.lastCompletedTime > 0)
            return true;

        return false;
    }

    static bool CanAcceptQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        string status = GetQuestStatus(player, questId);
        if (status == "active")
            return false;

        if (status == "reward_pending")
            return false;

        if (status == "completed")
        {
            if (!quest.repeatable)
                return false;
            if (IsQuestOnCooldown(player, questId))
                return false;
        }

        if (!AreQuestRequirementsCompleted(player, quest))
            return false;

        if (triggerId != "" && !IsQuestTriggerAllowed(quest, triggerId, GetQuestOfferTriggerIds(quest)))
            return false;

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

    static bool AreQuestObjectivesReadyForCompletion(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress)
    {
        if (!player || !quest || !quest.objectives)
            return false;

        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective || objective.type != "item")
                continue;

            if (objective.allowPartialTurnIn)
            {
                float deposited = GetDepositedObjectiveQuantity(progress, i, objective.className);
                if (deposited < objective.quantity)
                    return false;

                continue;
            }

            float completed = GetDepositedObjectiveQuantity(progress, i, objective.className);
            if (completed >= objective.quantity)
                continue;

            if (!PlayerHasItemAmount(player, objective.className, objective.quantity, objective.useItemQuantity))
                return false;
        }

        return true;
    }

    static bool CanCompleteQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        if (GetQuestStatus(player, questId) != "active")
            return false;

        if (triggerId != "" && !IsQuestTriggerAllowed(quest, triggerId, GetQuestCompletionTriggerIds(quest)))
            return false;

        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (triggerId != "" && IsCompletionTriggerDone(progress, triggerId))
            return false;

        return AreQuestObjectivesReadyForCompletion(player, quest, progress);
    }

    static bool CanDepositQuestItems(PlayerBase player, string questId, string triggerId = "")
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
        bool canDepositHere = false;

        if (triggerId != "")
        {
            if (IsQuestTriggerAllowed(quest, triggerId, GetQuestCompletionTriggerIds(quest)) && !IsCompletionTriggerDone(progress, triggerId))
                canDepositHere = true;
            else if (!QuestHasRoleTriggers(quest.completionTriggerIds) && IsQuestTriggerAllowed(quest, triggerId, GetQuestRewardTriggerIds(quest)))
                canDepositHere = true;
        }

        if (!canDepositHere)
            return false;

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

    static bool RequestAcceptQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Print("[Silver_77_Quests] Sending accept quest RPC: " + questId + " via trigger " + triggerId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_ACCEPT, new Param2<string, string>(questId, triggerId), true);
        return true;
    }

    static bool RequestCompleteQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Print("[Silver_77_Quests] Sending complete quest RPC: " + questId + " via trigger " + triggerId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_COMPLETE, new Param2<string, string>(questId, triggerId), true);
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

        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: START steamId=" + data.steamId);

        int progressCountBefore = 0;
        if (data.progress)
            progressCountBefore = data.progress.Count();
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: progressCountBefore=" + progressCountBefore);

        if (data.progress)
        {
            foreach (PlayerQuestProgress debugProgress : data.progress)
            {
                if (debugProgress)
                    Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: BEFORE EnsurePlayerProgress questId=" + debugProgress.questId + " status=" + debugProgress.status);
            }
        }

        if (!g_ClientPlayerQuestData)
            g_ClientPlayerQuestData = new map<string, ref PlayerQuestData>;

        EnsurePlayerProgress(data);

        int progressCountAfter = 0;
        if (data.progress)
            progressCountAfter = data.progress.Count();
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: progressCountAfter=" + progressCountAfter);

        if (data.progress)
        {
            foreach (PlayerQuestProgress debugProgress2 : data.progress)
            {
                if (debugProgress2)
                    Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: AFTER EnsurePlayerProgress questId=" + debugProgress2.questId + " status=" + debugProgress2.status);
            }
        }

        g_ClientPlayerQuestData.Set(data.steamId, data);
        g_ClientQuestDataRevision++;

        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] ApplySyncedPlayerData: stored in g_ClientPlayerQuestData, new g_ClientQuestDataRevision=" + g_ClientQuestDataRevision);

        int nonZeroObjectiveProgressCount = 0;
        int stageVisitCount = 0;
        if (data.progress)
        {
            foreach (PlayerQuestProgress progress : data.progress)
            {
                if (!progress)
                    continue;

                if (progress.objectiveProgress)
                {
                    foreach (PlayerQuestObjectiveProgress objectiveProgress : progress.objectiveProgress)
                    {
                        if (objectiveProgress && objectiveProgress.depositedQuantity > 0)
                            nonZeroObjectiveProgressCount++;
                    }
                }

                if (progress.stageVisits)
                    stageVisitCount += progress.stageVisits.Count();
            }
        }

        Print("[Silver_77_Quests] Applied synced player progress: " + data.steamId + ", quests=" + progressCountAfter + ", nonZeroObjectiveProgress=" + nonZeroObjectiveProgressCount + ", stageVisits=" + stageVisitCount);
    }
}
