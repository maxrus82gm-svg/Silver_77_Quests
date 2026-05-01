// ============================================================================
// ДАННЫЕ ПРОГРЕССА ИГРОКА
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

class PlayerQuestProgress
{
    string questId;
    string status;
    // UTC Unix time in seconds. Older saves with uptime milliseconds are treated as expired cooldowns.
    int lastCompletedTime;
    ref array<ref PlayerQuestObjectiveProgress> objectiveProgress;
    
    void PlayerQuestProgress()
    {
        questId = "";
        status = "available";
        lastCompletedTime = 0;
        objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
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
