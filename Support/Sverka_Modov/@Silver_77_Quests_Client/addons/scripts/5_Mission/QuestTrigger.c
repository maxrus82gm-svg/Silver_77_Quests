// ============================================================================
// QUEST TRIGGER
// ============================================================================

const float SILVER77_QUEST_DEFAULT_FOCUS_HEIGHT = 1.2;
const float SILVER77_QUEST_DEFAULT_FOCUS_RADIUS = 1.0;

class QuestTrigger
{
    private vector m_Position;
    private float m_Radius;
    private float m_FocusHeight;
    private float m_FocusRadius;
    private string m_TriggerId;
    private ref array<string> m_AvailableQuests;
    private string m_HintText;
    
    void QuestTrigger()
    {
        m_AvailableQuests = new array<string>;
        m_FocusHeight = SILVER77_QUEST_DEFAULT_FOCUS_HEIGHT;
        m_FocusRadius = SILVER77_QUEST_DEFAULT_FOCUS_RADIUS;
        m_HintText = "[F] Открыть квесты";
    }
    
    void SetupTrigger(vector position, float radius, float focusHeight, float focusRadius, string triggerId, array<string> questIds, string hintText = "")
    {
        m_Position = position;
        m_Radius = radius;
        m_FocusHeight = focusHeight;
        m_FocusRadius = focusRadius;
        m_TriggerId = triggerId;
        m_AvailableQuests = questIds;
        
        if (m_FocusHeight <= 0)
            m_FocusHeight = SILVER77_QUEST_DEFAULT_FOCUS_HEIGHT;
        
        if (m_FocusRadius <= 0)
            m_FocusRadius = SILVER77_QUEST_DEFAULT_FOCUS_RADIUS;
        
        if (hintText != "")
            m_HintText = hintText;
    }
    
    bool IsPlayerInside(PlayerBase player)
    {
        if (!player)
            return false;
        
        vector playerPos = player.GetPosition();
        float distance = vector.Distance(playerPos, m_Position);
        
        Print("[Silver_77_Quests] Distance check: " + distance + " <= " + m_Radius + " ? Player: " + playerPos.ToString() + " Trigger: " + m_Position.ToString());
        
        return distance <= m_Radius;
    }
    
    bool IsPlayerLookingAt(PlayerBase player)
    {
        if (!IsPlayerInside(player))
            return false;
        
        vector cameraPos = GetGame().GetCurrentCameraPosition();
        vector cameraDir = GetGame().GetCurrentCameraDirection();
        vector focusPos = m_Position + Vector(0, m_FocusHeight, 0);
        vector focusOffset = focusPos - cameraPos;
        float projectedDistance = vector.Dot(focusOffset, cameraDir);
        if (projectedDistance <= 0)
            return false;
        
        vector closestPoint = cameraPos + (cameraDir * projectedDistance);
        float focusMissDistance = vector.Distance(closestPoint, focusPos);
        
        Print("[Silver_77_Quests] Focus check: " + focusMissDistance + " <= " + m_FocusRadius);
        return focusMissDistance <= m_FocusRadius;
    }
    
    vector GetPosition()
    {
        return m_Position;
    }
    
    string GetTriggerId()
    {
        return m_TriggerId;
    }
    
    array<string> GetAvailableQuests()
    {
        return m_AvailableQuests;
    }
    
    string GetHintText()
    {
        return m_HintText;
    }
}

// ============================================================================
// QUEST TRIGGER MANAGER
// ============================================================================

class QuestTriggerManager
{
    private static ref array<ref QuestTrigger> m_Triggers;
    
    static void Init()
    {
        Print("[Silver_77_Quests] QuestTriggerManager.Init called");
        
        m_Triggers = new array<ref QuestTrigger>;
        CreateTriggersFromConfig();
        Print("[Silver_77_Quests] QuestTriggerManager initialized with " + m_Triggers.Count() + " triggers");
    }
    
    static void CreateTriggersFromConfig()
    {
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.triggers)
        {
            Print("[Silver_77_Quests] No triggers found in config!");
            return;
        }
        
        foreach (Silver77_QuestTriggerConfig triggerConfig : g_ClientQuestConfig.triggers)
        {
            if (!triggerConfig)
                continue;
            
            if (triggerConfig.position.Count() < 3)
            {
                Print("[Silver_77_Quests] Invalid position for trigger: " + triggerConfig.id);
                continue;
            }
            
            vector pos = Vector(triggerConfig.position[0], triggerConfig.position[1], triggerConfig.position[2]);
            if (triggerConfig.spawnNpc && triggerConfig.npcPosition && triggerConfig.npcPosition.Count() >= 3)
                pos = Vector(triggerConfig.npcPosition[0], triggerConfig.npcPosition[1], triggerConfig.npcPosition[2]);
            
            QuestTrigger trigger = new QuestTrigger();
            trigger.SetupTrigger(pos, triggerConfig.radius, triggerConfig.focusHeight, triggerConfig.focusRadius, triggerConfig.id, triggerConfig.questIds, triggerConfig.hintText);
            m_Triggers.Insert(trigger);
            
            Print("[Silver_77_Quests] Created trigger: " + triggerConfig.id + " at " + pos.ToString());
        }
    }
    
    static QuestTrigger GetTriggerForPlayer(PlayerBase player)
    {
        if (!player)
        {
            Print("[Silver_77_Quests] GetTriggerForPlayer: player is null");
            return null;
        }
        
        if (!m_Triggers)
        {
            Print("[Silver_77_Quests] GetTriggerForPlayer: m_Triggers is null");
            return null;
        }
        
        vector playerPos = player.GetPosition();
        Print("[Silver_77_Quests] Checking player position: " + playerPos.ToString() + " against " + m_Triggers.Count() + " triggers");
        
        foreach (QuestTrigger trigger : m_Triggers)
        {
            if (trigger.IsPlayerInside(player))
            {
                Print("[Silver_77_Quests] Player IS inside trigger at " + trigger.GetPosition().ToString());
                return trigger;
            }
        }
        return null;
    }
    
    static QuestTrigger GetFocusedTriggerForPlayer(PlayerBase player)
    {
        if (!player || !m_Triggers)
            return null;
        
        foreach (QuestTrigger trigger : m_Triggers)
        {
            if (trigger && trigger.IsPlayerLookingAt(player))
                return trigger;
        }
        
        return null;
    }
}


