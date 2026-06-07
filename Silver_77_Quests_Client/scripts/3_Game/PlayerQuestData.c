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

class QuestRewardItemProgress
{
    string className;
    int need;
    int given;
    bool spawnOnGround;
    bool setItemQuantity;
    float itemQuantity;
    string lastError;

    void QuestRewardItemProgress()
    {
        className = "";
        need = 0;
        given = 0;
        spawnOnGround = false;
        setItemQuantity = false;
        itemQuantity = 0;
        lastError = "";
    }
}

class QuestPendingRewardProgress
{
    string questId;
    int attemptId;
    string rewardId;
    string stage;
    string triggerId;
    string actionType;
    string status;
    ref array<ref QuestRewardItemProgress> items;
    int createdAt;
    int updatedAt;
    string lastError;

    void QuestPendingRewardProgress()
    {
        questId = "";
        attemptId = 0;
        rewardId = "";
        stage = "";
        triggerId = "";
        actionType = "";
        status = "pending";
        items = new array<ref QuestRewardItemProgress>;
        createdAt = 0;
        updatedAt = 0;
        lastError = "";
    }
}

class QuestDeliveredRewardProgress
{
    string questId;
    int attemptId;
    string rewardId;
    string stage;
    string triggerId;
    string actionType;
    int deliveredAt;

    void QuestDeliveredRewardProgress()
    {
        questId = "";
        attemptId = 0;
        rewardId = "";
        stage = "";
        triggerId = "";
        actionType = "";
        deliveredAt = 0;
    }
}

class PlayerQuestProgress
{
    string questId;
    string status;
    // UTC Unix time in seconds. Older saves with uptime milliseconds are treated as expired cooldowns.
    int lastCompletedTime;
    int currentAttemptId;
    int lastFinalizedAttemptId;
    ref array<ref PlayerQuestObjectiveProgress> objectiveProgress;
    ref array<string> completedCompletionTriggerIds;
    ref array<ref PlayerQuestStageVisit> stageVisits;
    ref array<ref QuestPendingRewardProgress> pendingRewards;
    ref array<ref QuestDeliveredRewardProgress> deliveredRewards;

    void PlayerQuestProgress()
    {
        questId = "";
        status = "available";
        lastCompletedTime = 0;
        currentAttemptId = 0;
        lastFinalizedAttemptId = 0;
        objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
        completedCompletionTriggerIds = new array<string>;
        stageVisits = new array<ref PlayerQuestStageVisit>;
        pendingRewards = new array<ref QuestPendingRewardProgress>;
        deliveredRewards = new array<ref QuestDeliveredRewardProgress>;
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
