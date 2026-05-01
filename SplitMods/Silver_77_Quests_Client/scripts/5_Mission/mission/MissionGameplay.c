// ============================================================================
// ИНТЕГРАЦИЯ В МИССИЮ (КЛИЕНТ)
// ============================================================================

modded class MissionGameplay
{
    private QuestUIMenu m_QuestMenu;
    private QuestJournalUIMenu m_QuestJournalMenu;
    private bool m_PlayerInQuestZone = false;
    private string m_CurrentHintText = "";
    private float m_TriggerCheckTimer = 0;
    private Widget m_QuestHintWidget;
    private TextWidget m_QuestHintText;
    private bool m_QuestHintWidgetFailed = false;
    private bool m_QuestInitialSyncRequested = false;
    private int m_LastQuestConfigRevision = -1;
    private ref array<string> m_CurrentQuestIds;
    
    override void OnMissionStart()
    {
        super.OnMissionStart();
        Print("[Silver_77_Quests] MissionGameplay.OnMissionStart called");
        
        // Клиент загружает конфиг, а потом синхронизируется с сервером.
        if (GetGame().IsClient())
        {
            Print("[Silver_77_Quests] Client loading config");
            QuestClientManager.Init();
            QuestTriggerManager.Init();
            m_LastQuestConfigRevision = g_ClientQuestConfigRevision;
        }
    }
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == MENU_QUEST_UI)
        {
            QuestUIMenu menu = new QuestUIMenu();
            menu.SetID(id);
            return menu;
        }
        
        if (id == MENU_QUEST_JOURNAL_UI)
        {
            QuestJournalUIMenu journalMenu = new QuestJournalUIMenu();
            journalMenu.SetID(id);
            return journalMenu;
        }
        
        return super.CreateScriptedMenu(id);
    }
    
    void CreateHintWidget()
    {
        if (m_QuestHintWidgetFailed)
        {
            return;
        }
        
        // Проверка, что виджет еще не создан.
        if (m_QuestHintWidget)
        {
            Print("[Silver_77_Quests] Widget already exists, skipping creation");
            return;
        }
        
        Print("[Silver_77_Quests] Attempting to create hint widget...");
        
        m_QuestHintWidget = GetGame().GetWorkspace().CreateWidgets("Silver_77_Quests/gui/layouts/QuestHint.layout");
        
        if (m_QuestHintWidget)
        {
            Print("[Silver_77_Quests] Widget created successfully");
            m_QuestHintText = TextWidget.Cast(m_QuestHintWidget.FindAnyWidget("QuestHintAction"));
            
            if (m_QuestHintText)
            {
                Print("[Silver_77_Quests] Text widget found");
                m_QuestHintWidget.Show(false);
                Print("[Silver_77_Quests] Quest hint widget ready");
            }
            else
            {
                Print("[Silver_77_Quests] ERROR: Text widget 'QuestHintAction' not found!");
                DestroyHintWidget();
                m_QuestHintWidgetFailed = true;
            }
        }
        else
        {
            Print("[Silver_77_Quests] ERROR: Failed to create widget from layout!");
            m_QuestHintWidgetFailed = true;
        }
    }
    
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        // Только на клиенте.
        if (GetGame().IsClient())
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player)
            {
                if (!m_QuestInitialSyncRequested)
                {
                    RequestInitialQuestSync(player);
                }
                
                if (m_LastQuestConfigRevision != g_ClientQuestConfigRevision)
                {
                    Print("[Silver_77_Quests] Quest config revision changed, rebuilding client triggers");
                    QuestTriggerManager.Init();
                    m_LastQuestConfigRevision = g_ClientQuestConfigRevision;
                }
                
                // Проверяем триггеры каждые 0.5 секунды.
                m_TriggerCheckTimer += timeslice;
                if (m_TriggerCheckTimer >= 0.5)
                {
                    m_TriggerCheckTimer = 0;
                    CheckPlayerInTrigger(player);
                }
                
            }
        }
    }
    
    void RequestInitialQuestSync(PlayerBase player)
    {
        m_QuestInitialSyncRequested = true;
        Print("[Silver_77_Quests] Requesting initial quest sync from server");
        QuestClientManager.RequestConfigSync(player);
        QuestClientManager.RequestPlayerDataSync(player);
    }
    
    override void OnKeyPress(int key)
    {
        if (key == KeyCode.KC_ESCAPE && GetGame().GetUIManager().IsMenuOpen(MENU_QUEST_JOURNAL_UI))
        {
            if (m_QuestJournalMenu)
            {
                Print("[Silver_77_Quests] ESC pressed, closing quest journal");
                CloseQuestJournal();
                return;
            }
            
            Print("[Silver_77_Quests] ESC pressed, quest journal menu is open but local reference is null");
        }
        
        super.OnKeyPress(key);
        
        if (key == KeyCode.KC_J)
        {
            if (GetGame().GetUIManager().GetMenu())
            {
                Print("[Silver_77_Quests] J pressed, but another menu is already open");
                return;
            }
            
            Print("[Silver_77_Quests] J pressed, opening quest journal");
            RequestOpenQuestJournal();
            return;
        }
        
        if (key == KeyCode.KC_F && m_PlayerInQuestZone)
        {
            if (GetGame().GetUIManager().GetMenu())
            {
                Print("[Silver_77_Quests] F pressed in quest zone, but another menu is already open");
                return;
            }
            
            Print("[Silver_77_Quests] F pressed in quest zone");
            RequestOpenQuestMenu();
        }
    }
    
    override void OnMissionFinish()
    {
        DestroyHintWidget();
        super.OnMissionFinish();
    }
    
    void CheckPlayerInTrigger(PlayerBase player)
    {
        QuestTrigger trigger = QuestTriggerManager.GetTriggerForPlayer(player);
        
        if (trigger && !m_PlayerInQuestZone)
        {
            // Вошли в зону.
            m_PlayerInQuestZone = true;
            m_CurrentHintText = trigger.GetHintText();
            SetCurrentQuestIds(trigger.GetAvailableQuests());
            ShowQuestHint(m_CurrentHintText);
            Print("[Silver_77_Quests] Client: Player entered quest zone at " + trigger.GetPosition().ToString());
            Print("[Silver_77_Quests] Client: Hint text = " + m_CurrentHintText);
        }
        else if (trigger && m_PlayerInQuestZone)
        {
            // Все еще в зоне: обновляем текст, если он изменился.
            SetCurrentQuestIds(trigger.GetAvailableQuests());
            string newHint = trigger.GetHintText();
            if (newHint != m_CurrentHintText)
            {
                m_CurrentHintText = newHint;
                ShowQuestHint(m_CurrentHintText);
            }
        }
        else if (!trigger && m_PlayerInQuestZone)
        {
            // Вышли из зоны: сразу скрываем подсказку.
            m_PlayerInQuestZone = false;
            m_CurrentHintText = "";
            ClearCurrentQuestIds();
            HideQuestHint();
            Print("[Silver_77_Quests] Client: Player left quest zone");
        }
    }
    
    void SetCurrentQuestIds(array<string> questIds)
    {
        if (!m_CurrentQuestIds)
            m_CurrentQuestIds = new array<string>;
        
        m_CurrentQuestIds.Clear();
        
        if (!questIds)
            return;
        
        foreach (string questId : questIds)
        {
            m_CurrentQuestIds.Insert(questId);
        }
    }
    
    void ClearCurrentQuestIds()
    {
        if (m_CurrentQuestIds)
            m_CurrentQuestIds.Clear();
    }
    
    array<string> GetCurrentQuestIds()
    {
        if (!m_CurrentQuestIds)
            m_CurrentQuestIds = new array<string>;
        
        return m_CurrentQuestIds;
    }
    
    void ShowQuestHint(string hintText)
    {
        Print("[Silver_77_Quests] Showing hint: " + hintText);
        
        if (!m_QuestHintWidget && !m_QuestHintWidgetFailed)
        {
            CreateHintWidget();
        }
        
        if (m_QuestHintWidget && m_QuestHintText)
        {
            m_QuestHintText.SetText(hintText);
            m_QuestHintWidget.Show(true);
        }
        else
        {
            ShowQuestChatHint(hintText);
        }
    }
    
    void HideQuestHint()
    {
        Print("[Silver_77_Quests] Hiding hint");
        
        if (m_QuestHintWidget)
        {
            m_QuestHintWidget.Show(false);
        }
    }
    
    void ShowQuestChatHint(string hintText)
    {
        if (m_Chat)
        {
            m_Chat.Add(new ChatMessageEventParams(CCSystem, "Silver_77_Quests", hintText, ""));
        }
    }
    
    void DestroyHintWidget()
    {
        if (m_QuestHintWidget)
        {
            delete m_QuestHintWidget;
            m_QuestHintWidget = null;
            m_QuestHintText = null;
        }
    }
    
    void RequestOpenQuestMenu()
    {
        Print("[Silver_77_Quests] Opening quest menu");
        
        if (GetGame().GetUIManager().IsMenuOpen(MENU_QUEST_UI))
        {
            Print("[Silver_77_Quests] Quest menu already open");
            return;
        }
        
        HideQuestHint();
        m_QuestMenu = QuestUIMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_QUEST_UI, null));
        if (m_QuestMenu)
        {
            Print("[Silver_77_Quests] Quest menu opened");
        }
        else
        {
            Print("[Silver_77_Quests] ERROR: Failed to open quest menu");
        }
    }
    
    void RequestOpenQuestJournal()
    {
        Print("[Silver_77_Quests] Opening quest journal");
        
        if (GetGame().GetUIManager().IsMenuOpen(MENU_QUEST_JOURNAL_UI))
        {
            Print("[Silver_77_Quests] Quest journal already open");
            return;
        }
        
        HideQuestHint();
        m_QuestJournalMenu = QuestJournalUIMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_QUEST_JOURNAL_UI, null));
        if (m_QuestJournalMenu)
        {
            Print("[Silver_77_Quests] Quest journal opened");
        }
        else
        {
            Print("[Silver_77_Quests] ERROR: Failed to open quest journal");
        }
    }
    
    void OnQuestMenuOpened()
    {
        HideQuestHint();
    }
    
    void OnQuestMenuClosed()
    {
        m_QuestMenu = null;
        m_QuestJournalMenu = null;
        
        if (m_PlayerInQuestZone && m_CurrentHintText != "")
        {
            ShowQuestHint(m_CurrentHintText);
        }
    }
    
    void CloseQuestMenu()
    {
        if (m_QuestMenu)
        {
            m_QuestMenu.Close();
            m_QuestMenu = null;
        }
        
        if (m_QuestJournalMenu)
        {
            m_QuestJournalMenu.Close();
            m_QuestJournalMenu = null;
        }
    }
    
    void CloseQuestJournal()
    {
        if (m_QuestJournalMenu)
        {
            m_QuestJournalMenu.Close();
            m_QuestJournalMenu = null;
        }
    }
}

