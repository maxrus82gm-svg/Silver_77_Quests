// ============================================================================
// UI ДЛЯ ЖУРНАЛА АКТИВНЫХ КВЕСТОВ
// ============================================================================

class QuestJournalUIMenu extends UIScriptedMenu
{
    private TextListboxWidget m_QuestList;
    private MultilineTextWidget m_QuestDescription;
    private ButtonWidget m_CloseButton;
    private PlayerBase m_Player;
    private string m_SelectedQuestId;
    private ref array<string> m_VisibleQuestIds;
    private int m_LastConfigRevision;
    private int m_LastDataRevision;
    
    void QuestJournalUIMenu()
    {
        m_SelectedQuestId = "";
        m_VisibleQuestIds = new array<string>;
        m_LastConfigRevision = -1;
        m_LastDataRevision = -1;
    }
    
    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Silver_77_Quests/gui/QuestJournal.layout");
        if (!layoutRoot)
        {
            Print("[Silver_77_Quests] ERROR: Failed to create QuestJournal.layout");
            return null;
        }
        
        m_QuestList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("QuestListbox"));
        m_QuestDescription = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("DescriptionText"));
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
        
        return layoutRoot;
    }
    
    override void Update(float timeslice)
    {
        super.Update(timeslice);
        
        if (m_LastConfigRevision != g_ClientQuestConfigRevision || m_LastDataRevision != g_ClientQuestDataRevision)
        {
            m_LastConfigRevision = g_ClientQuestConfigRevision;
            m_LastDataRevision = g_ClientQuestDataRevision;
            RefreshQuestList();
            UpdateQuestDetails();
            return;
        }
        
        if (SyncSelectedQuestFromList(false))
        {
            UpdateQuestDetails();
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
        {
            m_SelectedQuestId = "";
            return;
        }
        
        foreach (Silver77_Quest quest : g_ClientQuestConfig.quests)
        {
            if (!quest)
                continue;
            
            if (QuestClientManager.GetQuestStatus(m_Player, quest.id) != "active")
                continue;
            
            int row = m_QuestList.AddItem(quest.name, null, 0);
            m_VisibleQuestIds.Insert(quest.id);
            
            if (quest.id == previousQuestId)
                selectedRow = row;
        }
        
        if (selectedRow < 0 && m_VisibleQuestIds.Count() > 0)
            selectedRow = 0;
        
        if (selectedRow >= 0)
        {
            m_QuestList.SelectRow(selectedRow);
            m_SelectedQuestId = m_VisibleQuestIds.Get(selectedRow);
        }
        else
        {
            m_SelectedQuestId = "";
        }
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
        return true;
    }
    
    bool SelectQuestOffset(int offset)
    {
        if (!m_QuestList || !m_VisibleQuestIds || m_VisibleQuestIds.Count() == 0)
            return false;
        
        int selectedRow = m_QuestList.GetSelectedRow();
        if (selectedRow < 0 || selectedRow >= m_VisibleQuestIds.Count())
            selectedRow = 0;
        
        int newRow = selectedRow + offset;
        if (newRow < 0)
            newRow = m_VisibleQuestIds.Count() - 1;
        else if (newRow >= m_VisibleQuestIds.Count())
            newRow = 0;
        
        m_QuestList.SelectRow(newRow);
        m_SelectedQuestId = m_VisibleQuestIds.Get(newRow);
        UpdateQuestDetails();
        return true;
    }
    
    void UpdateQuestDetails()
    {
        if (!m_QuestDescription)
            return;
        
        if (m_SelectedQuestId == "")
        {
            if (m_VisibleQuestIds && m_VisibleQuestIds.Count() == 0)
                m_QuestDescription.SetText("Активных квестов нет.");
            else
                m_QuestDescription.SetText("Выберите квест из списка.");
            
            return;
        }
        
        Silver77_Quest quest = QuestClientManager.GetQuestById(m_SelectedQuestId);
        if (!quest)
        {
            m_QuestDescription.SetText("Квест не найден.");
            return;
        }
        
        bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, quest.id);
        bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, quest.id);
        
        string desc = quest.description + "\n\n";
        desc += "Статус: взят\n";
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
            if (!obj || obj.type != "item")
                continue;
            
            bool ready = QuestClientManager.PlayerHasItems(m_Player, obj.className, obj.quantity, obj.useItemQuantity);
            if (obj.allowPartialTurnIn)
            {
                float deposited = QuestClientManager.GetQuestObjectiveDeposited(m_Player, quest.id, i, obj.className);
                float remaining = obj.quantity - deposited;
                if (remaining < 0)
                    remaining = 0;
                
                ready = remaining <= 0 || QuestClientManager.PlayerHasItemAmount(m_Player, obj.className, remaining, obj.useItemQuantity);
                desc += "- Принести: " + obj.className + " x" + obj.quantity + "\n";
                desc += "  Сдано: " + deposited + " / " + obj.quantity + "\n";
                if (ready)
                    desc += "  [готово]\n";
                else
                    desc += "  [не готово]\n";
                
                continue;
            }
            
            desc += "- Принести: " + obj.className + " x" + obj.quantity;
            if (ready)
                desc += " [готово]\n";
            else
                desc += " [не готово]\n";
            
            if (!obj.removeOnComplete)
                desc += "  Предмет останется у игрока\n";
        }
        
        if (quest.rewards.Count() > 0)
        {
            desc += "\nНаграда:\n";
            foreach (Silver77_QuestItem reward : quest.rewards)
            {
                desc += "- " + reward.className + " x" + reward.quantity + "\n";
            }
        }
        
        desc += "\nСдать квест можно у NPC, который его выдал.";
        m_QuestDescription.SetText(desc);
    }
    
    override bool OnClick(Widget w, int x, int y, int button)
    {
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
            return true;
        }
        
        return false;
    }
    
    override bool OnMouseWheel(Widget w, int x, int y, int wheel)
    {
        if (wheel > 0)
            return SelectQuestOffset(-1);
        
        if (wheel < 0)
            return SelectQuestOffset(1);
        
        return false;
    }
    
    override void OnShow()
    {
        super.OnShow();
        // Журнал можно читать на ходу, поэтому не забираем управление персонажем.
        // GetGame().GetInput().ChangeGameFocus(1);
        // GetGame().GetUIManager().ShowUICursor(true);
        
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
            mission.OnQuestMenuOpened();
        
        if (m_Player)
            QuestClientManager.RequestPlayerDataSync(m_Player);
    }
    
    override void OnHide()
    {
        super.OnHide();
        // Фокус не меняли при открытии, поэтому не возвращаем его при закрытии.
        // GetGame().GetInput().ChangeGameFocus(-1);
        // GetGame().GetUIManager().ShowUICursor(false);
        
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
            mission.OnQuestMenuClosed();
    }
}

