// ============================================================================
// PLAYER QUEST PROGRESS DATA
// ============================================================================

class PlayerQuestObjectiveProgress
{
    int objectiveIndex;
    string className;
    float depositedQuantity;
    
    void PlayerQuestObjectiveProgress()
    {
        objectiveIndex = -1;
        className = "";
        depositedQuantity = 0;
    }
}

class PlayerQuestStageVisit
{
    string triggerId;
    string actionType;
    int firstActivatedTime;
    
    void PlayerQuestStageVisit()
    {
        triggerId = "";
        actionType = "";
        firstActivatedTime = 0;
    }
}

class PlayerQuestProgress
{
    string questId;
    string status;
    // UTC Unix time in seconds. Older saves with uptime milliseconds are treated as expired cooldowns.
    int lastCompletedTime;
    ref array<ref PlayerQuestObjectiveProgress> objectiveProgress;
    ref array<string> completedCompletionTriggerIds;
    ref array<ref PlayerQuestStageVisit> stageVisits;
    
    void PlayerQuestProgress()
    {
        questId = "";
        status = "available";
        lastCompletedTime = 0;
        objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
        completedCompletionTriggerIds = new array<string>;
        stageVisits = new array<ref PlayerQuestStageVisit>;
    }
}

class PlayerQuestData
{
    string steamId;
    ref array<ref PlayerQuestProgress> progress;
    
    void PlayerQuestData()
    {
        steamId = "";
        progress = new array<ref PlayerQuestProgress>;
    }
}
