// ============================================================================
// UI ДЛЯ КВЕСТОВ
// ============================================================================

const int MENU_QUEST_UI = 77777;
const int MENU_QUEST_JOURNAL_UI = 77778;

class QuestUIMenu extends UIScriptedMenu
{
    private TextWidget m_Title;
    private TextListboxWidget m_QuestList;
    private MultilineTextWidget m_QuestDescription;
    private ButtonWidget m_AcceptButton;
    private ButtonWidget m_CompleteButton;
    private ButtonWidget m_CloseButton;
    private PlayerBase m_Player;
    private string m_SelectedQuestId;
    private ref array<string> m_VisibleQuestIds;
    private int m_LastConfigRevision;
    private int m_LastDataRevision;
    private bool m_WaitingForServer;
    private float m_ServerWaitTimer;
    
    void QuestUIMenu()
    {
        m_SelectedQuestId = "";
        m_VisibleQuestIds = new array<string>;
        m_LastConfigRevision = -1;
        m_LastDataRevision = -1;
        m_WaitingForServer = false;
        m_ServerWaitTimer = 0;
    }
    
    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Silver_77_Quests/gui/QuestMenu.layout");
        if (!layoutRoot)
        {
            Print("[Silver_77_Quests] ERROR: Failed to create QuestMenu.layout");
            return null;
        }
        
        m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("TitleText"));
        m_QuestList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("QuestListbox"));
        m_QuestDescription = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("DescriptionText"));
        m_AcceptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("AcceptButton"));
        m_CompleteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CompleteButton"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
        
        m_Player = PlayerBase.Cast(GetGame().GetPlayer());
        if (m_Player)
        {
            QuestClientManager.RequestConfigSync(m_Player);
            QuestClientManager.RequestPlayerDataSync(m_Player);
        }
        
        m_LastConfigRevision = g_ClientQuestConfigRevision;
        m_LastDataRevision = g_ClientQuestDataRevision;
        RefreshQuestList();
        UpdateQuestDetails();
        UpdateButtons();
        
        return layoutRoot;
    }
    
    override void Update(float timeslice)
    {
        super.Update(timeslice);
        
        if (m_WaitingForServer)
        {
            m_ServerWaitTimer += timeslice;
            if (m_ServerWaitTimer >= 5.0)
            {
                Print("[Silver_77_Quests] Quest UI server response timeout, requesting progress again");
                m_WaitingForServer = false;
                m_ServerWaitTimer = 0;
                if (m_Player)
                    QuestClientManager.RequestPlayerDataSync(m_Player);
                
                UpdateQuestDetails();
                UpdateButtons();
            }
        }
        
        if (m_LastConfigRevision != g_ClientQuestConfigRevision)
        {
            m_LastConfigRevision = g_ClientQuestConfigRevision;
            m_LastDataRevision = g_ClientQuestDataRevision;
            m_WaitingForServer = false;
            m_ServerWaitTimer = 0;
            RefreshQuestList();
            UpdateQuestDetails();
            UpdateButtons();
            return;
        }
        
        if (m_LastDataRevision != g_ClientQuestDataRevision)
        {
            m_LastDataRevision = g_ClientQuestDataRevision;
            m_WaitingForServer = false;
            m_ServerWaitTimer = 0;
            RefreshQuestList();
            UpdateQuestDetails();
            UpdateButtons();
        }
        
        if (SyncSelectedQuestFromList(false))
        {
            UpdateQuestDetails();
            UpdateButtons();
        }
    }
    
    void RefreshQuestList()
    {
        if (!m_QuestList)
            return;
        
        string previousQuestId = m_SelectedQuestId;
        int selectedRow = -1;
        
        m_QuestList.ClearItems();
        m_VisibleQuestIds.Clear();
        
        if (!g_ClientQuestConfig || !g_ClientQuestConfig.quests)
            return;
            
        foreach (Silver77_Quest quest : g_ClientQuestConfig.quests)
        {
            if (!quest)
                continue;
            
            if (!IsQuestVisibleForCurrentNpc(quest.id))
                continue;
            
            string status = QuestClientManager.GetQuestStatus(m_Player, quest.id);
            string displayName = quest.name;
            
            if (status == "active")
                displayName = "[Активен] " + displayName;
            else if (status == "completed")
                displayName = "[Выполнен] " + displayName;
                
            int row = m_QuestList.AddItem(displayName, null, 0);
            m_VisibleQuestIds.Insert(quest.id);
            
            if (quest.id == previousQuestId)
                selectedRow = row;
        }
        
        if (selectedRow < 0 && m_VisibleQuestIds.Count() > 0)
        {
            selectedRow = 0;
        }
        
        if (selectedRow >= 0)
        {
            m_QuestList.SelectRow(selectedRow);
            m_SelectedQuestId = m_VisibleQuestIds.Get(selectedRow);
            Print("[Silver_77_Quests] Selected quest after refresh: " + m_SelectedQuestId + ", status: " + QuestClientManager.GetQuestStatus(m_Player, m_SelectedQuestId));
        }
        else
        {
            m_SelectedQuestId = "";
        }
    }
    
    bool IsQuestVisibleForCurrentNpc(string questId)
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission)
            return true;
        
        array<string> currentQuestIds = mission.GetCurrentQuestIds();
        if (!currentQuestIds || currentQuestIds.Count() == 0)
            return true;
        
        return currentQuestIds.Find(questId) != -1;
    }
    
    bool SyncSelectedQuestFromList(bool selectFirstIfNone)
    {
        if (!m_QuestList || !m_VisibleQuestIds)
            return false;
        
        int selectedRow = m_QuestList.GetSelectedRow();
        if ((selectedRow < 0 || selectedRow >= m_VisibleQuestIds.Count()) && selectFirstIfNone && m_VisibleQuestIds.Count() > 0)
        {
            selectedRow = 0;
            m_QuestList.SelectRow(selectedRow);
        }
        else if (selectedRow < 0 || selectedRow >= m_VisibleQuestIds.Count())
        {
            return false;
        }
        
        string questId = m_VisibleQuestIds.Get(selectedRow);
        
        if (questId == m_SelectedQuestId)
            return false;
        
        m_SelectedQuestId = questId;
        m_WaitingForServer = false;
        m_ServerWaitTimer = 0;
        
        if (m_SelectedQuestId != "")
        {
            Print("[Silver_77_Quests] Selected quest synced: " + m_SelectedQuestId + ", status: " + QuestClientManager.GetQuestStatus(m_Player, m_SelectedQuestId));
        }
        
        return true;
    }
    
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_AcceptButton)
        {
            SyncSelectedQuestFromList(true);
            if (m_SelectedQuestId != "")
            {
                Print("[Silver_77_Quests] Accept button clicked: " + m_SelectedQuestId);
                if (QuestClientManager.RequestAcceptQuest(m_Player, m_SelectedQuestId))
                {
                    m_WaitingForServer = true;
                    m_ServerWaitTimer = 0;
                    UpdateQuestDetails();
                    UpdateButtons();
                }
            }
            return true;
        }
        
        if (w == m_CompleteButton)
        {
            SyncSelectedQuestFromList(true);
            if (m_SelectedQuestId != "")
            {
                Print("[Silver_77_Quests] Complete button clicked: " + m_SelectedQuestId);
                if (QuestClientManager.RequestCompleteQuest(m_Player, m_SelectedQuestId))
                {
                    m_WaitingForServer = true;
                    m_ServerWaitTimer = 0;
                    UpdateQuestDetails();
                    UpdateButtons();
                }
            }
            return true;
        }
        
        if (w == m_CloseButton)
        {
            Close();
            return true;
        }
        
        return false;
    }
    
    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w == m_QuestList)
        {
            SyncSelectedQuestFromList(false);
            UpdateQuestDetails();
            UpdateButtons();
            return true;
        }
        return false;
    }
    
    void UpdateQuestDetails()
    {
        if (!m_QuestDescription)
            return;
        
        if (m_SelectedQuestId == "")
        {
            m_QuestDescription.SetText("Выберите квест из списка");
            return;
        }
        
        Silver77_Quest quest = QuestClientManager.GetQuestById(m_SelectedQuestId);
        if (!quest)
        {
            m_QuestDescription.SetText("Квест не найден");
            return;
        }
        
        bool canAccept = QuestClientManager.CanAcceptQuest(m_Player, quest.id);
        bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, quest.id);
        bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, quest.id);
        
        string desc = quest.description + "\n\n";
        desc += "Статус: " + QuestClientManager.GetQuestStatus(m_Player, quest.id) + "\n";
        if (canAccept)
            desc += "Можно взять: да\n";
        else
            desc += "Можно взять: нет\n";
        
        if (canComplete)
            desc += "Можно сдать: да\n\n";
        else if (canDeposit)
            desc += "Можно внести часть предметов: да\n\n";
        else
            desc += "Можно сдать: нет\n\n";
        
        desc += "Цели:\n";
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective obj = quest.objectives.Get(i);
            if (!obj)
                continue;
            
            if (obj.type == "item")
            {
                desc += "- Принести: " + obj.className + " x" + obj.quantity + "\n";
                if (obj.allowPartialTurnIn)
                {
                    float deposited = QuestClientManager.GetQuestObjectiveDeposited(m_Player, quest.id, i, obj.className);
                    desc += "  Сдано: " + deposited + " / " + obj.quantity + "\n";
                    desc += "  Можно сдавать частями\n";
                }
                
                if (!obj.removeOnComplete)
                    desc += "  Предмет останется у игрока\n";
            }
        }
        
        if (quest.rewards.Count() > 0)
        {
            desc += "\nНаграда:\n";
            foreach (Silver77_QuestItem reward : quest.rewards)
            {
                desc += "- " + reward.className + " x" + reward.quantity + "\n";
            }
        }
        
        if (m_WaitingForServer)
            desc += "\nЗапрос отправлен на сервер...";
        
        m_QuestDescription.SetText(desc);
    }
    
    void UpdateButtons()
    {
        if (!m_AcceptButton || !m_CompleteButton)
            return;
        
        if (m_SelectedQuestId == "" || m_WaitingForServer)
        {
            m_AcceptButton.Enable(false);
            m_CompleteButton.Enable(false);
            return;
        }
        
        bool canAccept = QuestClientManager.CanAcceptQuest(m_Player, m_SelectedQuestId);
        bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, m_SelectedQuestId);
        bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, m_SelectedQuestId);
        
        m_AcceptButton.Enable(canAccept);
        m_CompleteButton.Enable(canComplete || canDeposit);
    }
    
    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
        
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
            mission.OnQuestMenuOpened();
        
        if (m_Player)
        {
            QuestClientManager.RequestPlayerDataSync(m_Player);
        }
    }
    
    override void OnHide()
    {
        super.OnHide();
        GetGame().GetInput().ChangeGameFocus(-1);
        GetGame().GetUIManager().ShowUICursor(false);
        
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
            mission.OnQuestMenuClosed();
    }
}



