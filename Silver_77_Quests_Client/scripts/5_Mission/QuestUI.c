// ============================================================================
// QUEST UI
// ============================================================================

const int MENU_QUEST_UI = 77777;
const int MENU_QUEST_JOURNAL_UI = 77778;
const int QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE = 72;
const int QUEST_UI_DIALOG_MAX_CHARS_PER_LINE = 74;

class QuestUIMenu extends UIScriptedMenu
{
    private TextWidget m_Title;
    private TextListboxWidget m_QuestList;
    private TextListboxWidget m_TriggerRouteList;
    private TextListboxWidget m_QuestDescription;
    private TextListboxWidget m_DialogText;
    private ButtonWidget m_AcceptButton;
    private ButtonWidget m_CompleteButton;
    private ButtonWidget m_CloseButton;
    private TextWidget m_AcceptButtonText;
    private TextWidget m_CompleteButtonText;
    private TextWidget m_CloseButtonText;
    private PlayerBase m_Player;
    private string m_SelectedQuestId;
    private ref array<string> m_VisibleQuestIds;
    private ref array<string> m_VisibleRouteKeys;
    private string m_SelectedRouteKey;
    private int m_LastConfigRevision;
    private int m_LastDataRevision;
    private bool m_WaitingForServer;
    private float m_ServerWaitTimer;
    
    void QuestUIMenu()
    {
        m_SelectedQuestId = "";
        m_VisibleQuestIds = new array<string>;
        m_VisibleRouteKeys = new array<string>;
        m_SelectedRouteKey = "";
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
        m_TriggerRouteList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("TriggerRouteListbox"));
        m_QuestDescription = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("DescriptionText"));
        m_DialogText = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("DialogText"));
        m_AcceptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("AcceptButton"));
        m_CompleteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CompleteButton"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
        m_AcceptButtonText = TextWidget.Cast(layoutRoot.FindAnyWidget("AcceptButtonText"));
        m_CompleteButtonText = TextWidget.Cast(layoutRoot.FindAnyWidget("CompleteButtonText"));
        m_CloseButtonText = TextWidget.Cast(layoutRoot.FindAnyWidget("CloseButtonText"));
        
        m_Player = PlayerBase.Cast(GetGame().GetPlayer());
        if (m_Player)
        {
            QuestClientManager.RequestConfigSync(m_Player);
            QuestClientManager.RequestPlayerDataSync(m_Player);
        }
        
        m_LastConfigRevision = g_ClientQuestConfigRevision;
        m_LastDataRevision = g_ClientQuestDataRevision;
        
        if (m_Player && QuestClientManager.HasSyncedPlayerData(m_Player))
        {
            RefreshQuestList();
            UpdateQuestDetails();
            UpdateButtons();
        }
        else
        {
            if (m_QuestDescription)
                SetDescriptionText("Загрузка данных квестов...");
            SetDialogText("");
        }
        
        return layoutRoot;
    }
    
    override void Update(float timeslice)
    {
        super.Update(timeslice);
        
        if (!m_Player || !QuestClientManager.HasSyncedPlayerData(m_Player))
        {
            if (m_QuestDescription)
                SetDescriptionText("Загрузка данных квестов...");
            SetDialogText("");
            return;
        }
        
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

    float GetQuestUiCharWeight(string character)
    {
        if (character == " " || character == "\t")
            return 0.35;

        if (character == "." || character == "," || character == ":" || character == ";" || character == "!" || character == "?" || character == "'" || character == "\"" || character == "`")
            return 0.45;

        if (character == "(" || character == ")" || character == "[" || character == "]" || character == "{" || character == "}" || character == "|" || character == "/")
            return 0.55;

        if (character == "-" || character == "_" || character == "+" || character == "=")
            return 0.65;

        if (character == "i" || character == "l" || character == "I" || character == "1")
            return 0.55;

        if (character == "m" || character == "w" || character == "M" || character == "W" || character == "@")
            return 1.2;

        return 1.0;
    }

    float GetQuestUiTextWeight(string text)
    {
        float totalWeight = 0.0;
        for (int characterIndex = 0; characterIndex < text.Length(); characterIndex++)
        {
            totalWeight += GetQuestUiCharWeight(text.Substring(characterIndex, 1));
        }

        return totalWeight;
    }

    ref array<string> SplitQuestUiWordByWeight(string word, int maxCharsPerLine)
    {
        ref array<string> chunks = new array<string>;
        string currentChunk = "";

        for (int characterIndex = 0; characterIndex < word.Length(); characterIndex++)
        {
            string character = word.Substring(characterIndex, 1);
            string candidateChunk = currentChunk + character;
            if (currentChunk != "" && GetQuestUiTextWeight(candidateChunk) > maxCharsPerLine)
            {
                chunks.Insert(currentChunk);
                currentChunk = character;
            }
            else
            {
                currentChunk = candidateChunk;
            }
        }

        if (currentChunk != "")
            chunks.Insert(currentChunk);

        if (chunks.Count() == 0)
            chunks.Insert(word);

        return chunks;
    }

    ref array<string> BuildWrappedTextLines(string text, int maxCharsPerLine)
    {
        ref array<string> wrappedLines = new array<string>;

        if (text == "")
        {
            wrappedLines.Insert("");
            return wrappedLines;
        }

        TStringArray sourceLines = new TStringArray;
        text.Split("\n", sourceLines);

        foreach (string sourceLine : sourceLines)
        {
            if (sourceLine == "")
            {
                wrappedLines.Insert("");
                continue;
            }

            TStringArray words = new TStringArray;
            sourceLine.Split(" ", words);

            string currentLine = "";
            bool hadWord = false;

            foreach (string word : words)
            {
                if (word == "")
                    continue;

                hadWord = true;
                if (currentLine == "")
                {
                    if (GetQuestUiTextWeight(word) <= maxCharsPerLine)
                    {
                        currentLine = word;
                    }
                    else
                    {
                        ref array<string> directChunks = SplitQuestUiWordByWeight(word, maxCharsPerLine);
                        for (int directChunkIndex = 0; directChunkIndex < directChunks.Count(); directChunkIndex++)
                        {
                            string directChunk = directChunks.Get(directChunkIndex);
                            if (directChunkIndex < directChunks.Count() - 1)
                                wrappedLines.Insert(directChunk);
                            else
                                currentLine = directChunk;
                        }
                    }
                    continue;
                }

                string candidateLine = currentLine + " " + word;
                if (GetQuestUiTextWeight(candidateLine) <= maxCharsPerLine)
                {
                    currentLine = candidateLine;
                }
                else
                {
                    wrappedLines.Insert(currentLine);
                    currentLine = "";

                    if (GetQuestUiTextWeight(word) <= maxCharsPerLine)
                    {
                        currentLine = word;
                    }
                    else
                    {
                        ref array<string> wrappedChunks = SplitQuestUiWordByWeight(word, maxCharsPerLine);
                        for (int wrappedChunkIndex = 0; wrappedChunkIndex < wrappedChunks.Count(); wrappedChunkIndex++)
                        {
                            string wrappedChunk = wrappedChunks.Get(wrappedChunkIndex);
                            if (wrappedChunkIndex < wrappedChunks.Count() - 1)
                                wrappedLines.Insert(wrappedChunk);
                            else
                                currentLine = wrappedChunk;
                        }
                    }
                }
            }

            if (!hadWord)
            {
                wrappedLines.Insert("");
                continue;
            }

            if (currentLine != "")
                wrappedLines.Insert(currentLine);
        }

        if (wrappedLines.Count() == 0)
            wrappedLines.Insert("");

        return wrappedLines;
    }

    void FillScrollableTextList(TextListboxWidget listWidget, string text, int maxCharsPerLine, int rowColor)
    {
        if (!listWidget)
            return;

        listWidget.ClearItems();

        ref array<string> wrappedLines = BuildWrappedTextLines(text, maxCharsPerLine);
        foreach (string wrappedLine : wrappedLines)
        {
            string displayLine = wrappedLine;
            if (displayLine == "")
                displayLine = " ";

            int row = listWidget.AddItem(displayLine, null, 0);
            listWidget.SetItemColor(row, 0, rowColor);
        }
    }

    void SetDescriptionText(string text)
    {
        // Description keeps source line boundaries so quest items/objectives stay separated.
        FillScrollableTextList(m_QuestDescription, text, QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE, 0xFFF0EADB);
    }

    void SetDialogText(string text)
    {
        FillScrollableTextList(m_DialogText, text, QUEST_UI_DIALOG_MAX_CHARS_PER_LINE, 0xFFFFD54F);
    }

    string SanitizeQuestUiLabel(string text)
    {
        string normalized = text;
        normalized.Replace("[F] ", "");
        normalized.Replace("[F]", "");
        normalized.Trim();

        while (normalized.IndexOf("  ") != -1)
            normalized.Replace("  ", " ");

        return normalized;
    }
    
    void RefreshQuestList()
    {
        if (!m_QuestList)
            return;
        
        bool hasSynced = false;
        if (m_Player)
            hasSynced = QuestClientManager.HasSyncedPlayerData(m_Player);
        
        Print("[Silver_77_Quests][CLIENT_PROGRESS_DEBUG] QuestUI.RefreshQuestList: called, g_ClientQuestDataRevision=" + g_ClientQuestDataRevision + " HasSyncedPlayerData=" + hasSynced);
        
        string previousQuestId = m_SelectedQuestId;
        int selectedRow = -1;
        string currentTriggerId = GetCurrentTriggerId();
        
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
            bool highlightAsReward = QuestClientManager.ShouldHighlightQuestAsReward(m_Player, quest.id, currentTriggerId);
            bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, quest.id, currentTriggerId);
            bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, quest.id, currentTriggerId);
            bool canClaimReward = QuestClientManager.CanClaimReward(m_Player, quest.id, currentTriggerId);
            string nextTriggerId = QuestClientManager.GetNextQuestTriggerId(m_Player, quest.id);
            bool isCurrentStageHere = (currentTriggerId != "" && currentTriggerId == nextTriggerId);
            string displayName = quest.name;
            int rowColor = 0xFFF0EADB;

            if (highlightAsReward || canClaimReward)
                rowColor = 0xFFFFD54F;
            else if (canComplete)
                rowColor = 0xFFA5D6A7;
            else if (canDeposit || isCurrentStageHere)
                rowColor = 0xFF90CAF9;
            else if (status == "reward_pending")
                rowColor = 0xFFFFE082;
            else if (status == "completed")
                rowColor = 0xFFB0BEC5;
            else if (status == "active")
                rowColor = 0xFFECEFF1;

            int row = m_QuestList.AddItem(displayName, null, 0);
            m_QuestList.SetItemColor(row, 0, rowColor);
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
        
        if (currentQuestIds.Find(questId) == -1)
            return false;
        
        return QuestClientManager.IsQuestVisibleForTrigger(m_Player, questId, mission.GetCurrentTriggerId());
    }
    
    string GetCurrentTriggerId()
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission)
            return "";
        
        return mission.GetCurrentTriggerId();
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
        m_SelectedRouteKey = "";
        m_WaitingForServer = false;
        m_ServerWaitTimer = 0;
        
        if (m_SelectedQuestId != "")
        {
            Print("[Silver_77_Quests] Selected quest synced: " + m_SelectedQuestId + ", status: " + QuestClientManager.GetQuestStatus(m_Player, m_SelectedQuestId));
        }
        
        return true;
    }

    string BuildRouteKey(string triggerId, string actionType)
    {
        if (triggerId == "" || actionType == "")
            return "";

        return triggerId + "|" + actionType;
    }

    void AddVisibleRouteKey(string routeKey)
    {
        if (!m_VisibleRouteKeys || routeKey == "" || m_VisibleRouteKeys.Find(routeKey) != -1)
            return;

        m_VisibleRouteKeys.Insert(routeKey);
    }

    bool HasQuestStageVisitForRoute(PlayerQuestStageVisit stageVisit, string routeKey)
    {
        if (!stageVisit || routeKey == "")
            return false;

        return BuildRouteKey(stageVisit.triggerId, stageVisit.actionType) == routeKey;
    }

    bool HasQuestStageVisitForRouteKey(array<ref PlayerQuestStageVisit> stageVisits, string routeKey)
    {
        if (!stageVisits || routeKey == "")
            return false;

        foreach (PlayerQuestStageVisit stageVisit : stageVisits)
        {
            if (HasQuestStageVisitForRoute(stageVisit, routeKey))
                return true;
        }

        return false;
    }

    string BuildRouteLabel(string triggerId, string actionType)
    {
        string triggerText = SanitizeQuestUiLabel(QuestClientManager.GetTriggerDisplayText(triggerId));
        string actionLabel = BuildQuestDialogActionLabel(actionType);
        if (actionLabel == "")
            return triggerText;

        return actionLabel + ": " + triggerText;
    }

    void RefreshTriggerRouteList(Silver77_Quest quest, string status)
    {
        if (!m_TriggerRouteList || !m_VisibleRouteKeys)
            return;

        m_TriggerRouteList.ClearItems();
        m_VisibleRouteKeys.Clear();

        if (!quest)
        {
            m_SelectedRouteKey = "";
            return;
        }

        array<ref PlayerQuestStageVisit> stageVisits = QuestClientManager.GetQuestStageVisitsSorted(m_Player, quest.id);
        string currentTriggerId = GetCurrentTriggerId();
        string currentActionType = ResolveCurrentDialogActionType(quest, status, currentTriggerId, QuestClientManager.CanAcceptQuest(m_Player, quest.id, currentTriggerId), QuestClientManager.CanCompleteQuest(m_Player, quest.id, currentTriggerId), QuestClientManager.CanClaimReward(m_Player, quest.id, currentTriggerId));
        string currentRouteKey = BuildRouteKey(currentTriggerId, currentActionType);

        array<string> offerTriggerIds = QuestClientManager.GetQuestOfferTriggerIds(quest);
        foreach (string offerTriggerId : offerTriggerIds)
        {
            AddVisibleRouteKey(BuildRouteKey(offerTriggerId, "offer"));
        }

        array<string> completionTriggerIds = QuestClientManager.GetQuestCompletionTriggerIds(quest);
        foreach (string completionTriggerId : completionTriggerIds)
        {
            AddVisibleRouteKey(BuildRouteKey(completionTriggerId, "completion"));
        }

        array<string> rewardTriggerIds = QuestClientManager.GetQuestRewardTriggerIds(quest);
        foreach (string rewardTriggerId : rewardTriggerIds)
        {
            AddVisibleRouteKey(BuildRouteKey(rewardTriggerId, "reward"));
        }

        if (stageVisits)
        {
            foreach (PlayerQuestStageVisit stageVisit : stageVisits)
            {
                if (!stageVisit)
                    continue;

                AddVisibleRouteKey(BuildRouteKey(stageVisit.triggerId, stageVisit.actionType));
            }
        }

        int selectedRow = -1;
        for (int routeIndex = 0; routeIndex < m_VisibleRouteKeys.Count(); routeIndex++)
        {
            string routeKey = m_VisibleRouteKeys.Get(routeIndex);
            TStringArray routeParts = new TStringArray;
            routeKey.Split("|", routeParts);
            if (routeParts.Count() < 2)
                continue;

            string triggerId = routeParts.Get(0);
            string actionType = routeParts.Get(1);
            string label = BuildRouteLabel(triggerId, actionType);
            int row = m_TriggerRouteList.AddItem(label, null, 0);

            bool visited = HasQuestStageVisitForRouteKey(stageVisits, routeKey);
            bool isCurrent = (currentRouteKey != "" && routeKey == currentRouteKey);

            if (visited)
                m_TriggerRouteList.SetItemColor(row, 0, 0xFFFFD54F);
            else if (isCurrent)
                m_TriggerRouteList.SetItemColor(row, 0, 0xFF90CAF9);

            if (m_SelectedRouteKey != "" && routeKey == m_SelectedRouteKey)
                selectedRow = row;
            else if (selectedRow < 0 && isCurrent)
                selectedRow = row;
        }

        if (selectedRow < 0 && m_VisibleRouteKeys.Count() > 0)
            selectedRow = 0;

        if (selectedRow >= 0 && selectedRow < m_VisibleRouteKeys.Count())
        {
            m_TriggerRouteList.SelectRow(selectedRow);
            m_SelectedRouteKey = m_VisibleRouteKeys.Get(selectedRow);
        }
        else
        {
            m_SelectedRouteKey = "";
        }
    }

    bool SyncSelectedRouteFromList(bool selectFirstIfNone)
    {
        if (!m_TriggerRouteList || !m_VisibleRouteKeys)
            return false;

        int selectedRow = m_TriggerRouteList.GetSelectedRow();
        if ((selectedRow < 0 || selectedRow >= m_VisibleRouteKeys.Count()) && selectFirstIfNone && m_VisibleRouteKeys.Count() > 0)
        {
            selectedRow = 0;
            m_TriggerRouteList.SelectRow(selectedRow);
        }
        else if (selectedRow < 0 || selectedRow >= m_VisibleRouteKeys.Count())
        {
            return false;
        }

        string routeKey = m_VisibleRouteKeys.Get(selectedRow);
        if (routeKey == m_SelectedRouteKey)
            return false;

        m_SelectedRouteKey = routeKey;
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
                if (QuestClientManager.RequestAcceptQuest(m_Player, m_SelectedQuestId, GetCurrentTriggerId()))
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
                if (QuestClientManager.RequestCompleteQuest(m_Player, m_SelectedQuestId, GetCurrentTriggerId()))
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
        if (w == m_TriggerRouteList)
        {
            SyncSelectedRouteFromList(false);
            UpdateQuestDetails();
            return true;
        }
        return false;
    }

    override bool OnMouseWheel(Widget w, int x, int y, int wheel)
    {
        return super.OnMouseWheel(w, x, y, wheel);
    }
    
    void UpdateQuestDetails()
    {
        if (!m_QuestDescription)
            return;
        
        if (m_SelectedQuestId == "")
        {
            SetDescriptionText("Выберите квест из списка");
            RefreshTriggerRouteList(null, "");
            SetDialogText("");
            return;
        }
        
        Silver77_Quest quest = QuestClientManager.GetQuestById(m_SelectedQuestId);
        if (!quest)
        {
            SetDescriptionText("Квест не найден");
            RefreshTriggerRouteList(null, "");
            SetDialogText("");
            return;
        }
        
        string status = QuestClientManager.GetQuestStatus(m_Player, quest.id);
        string currentTriggerId = GetCurrentTriggerId();
        bool canAccept = QuestClientManager.CanAcceptQuest(m_Player, quest.id, currentTriggerId);
        bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, quest.id, currentTriggerId);
        bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, quest.id, currentTriggerId);
        bool canClaimReward = QuestClientManager.CanClaimReward(m_Player, quest.id, currentTriggerId);
        
        string desc = "";
        if (quest.description != "")
            desc = quest.description + "\n\n";
        
        desc += "Статус: " + BuildQuestStatusText(status) + "\n";
        if (canAccept)
            desc += "Можно взять: да\n";
        else
            desc += "Можно взять: нет\n";
        
        desc += BuildQuestRequirementsText(quest);
        
        if (canClaimReward)
            desc += "Можно получить награду: да\n";
        else if (status == "reward_pending")
            desc += "Награда здесь недоступна\n";
        
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
        
        array<ref Silver77_QuestItem> visibleRewards = GetVisibleRewardItems(quest, status, currentTriggerId, canComplete, canClaimReward);
        if (visibleRewards && visibleRewards.Count() > 0)
        {
            desc += "\nНаграда:\n";
            foreach (Silver77_QuestItem reward : visibleRewards)
            {
                if (!reward)
                    continue;
                
                desc += "- " + reward.className + " x" + reward.quantity + "\n";
            }
        }
        if (m_WaitingForServer)
            desc += "\nЗапрос отправлен. Ждем ответ сервера...";
        
        SetDescriptionText(desc);
        
        RefreshTriggerRouteList(quest, status);
        
        string dialogText = BuildQuestDialogText(quest, status, currentTriggerId, canAccept, canComplete, canClaimReward);
        SetDialogText(dialogText);
    }
    
    string BuildQuestRequirementsText(Silver77_Quest quest)
    {
        if (!quest)
            return "";
        
        string text = "";
        if (quest.requiresPrevious != "")
            text += BuildQuestRequirementLine(quest.requiresPrevious);
        
        if (quest.requiredQuestIds)
        {
            foreach (string requiredQuestId : quest.requiredQuestIds)
            {
                if (requiredQuestId == "" || requiredQuestId == quest.requiresPrevious)
                    continue;
                
                text += BuildQuestRequirementLine(requiredQuestId);
            }
        }
        
        if (text == "")
            return "";
        
        return "Нужно выполнить:\n" + text + "\n";
    }
    
    string BuildQuestRequirementLine(string questId)
    {
        string requirementStatus = "нет";
        if (QuestClientManager.IsQuestRequirementCompleted(m_Player, questId))
            requirementStatus = "да";
        
        Silver77_Quest requiredQuest = QuestClientManager.GetQuestById(questId);
        if (requiredQuest && requiredQuest.name != "")
            return "- " + requiredQuest.name + " (" + questId + "): " + requirementStatus + "\n";
        
        return "- " + questId + ": " + requirementStatus + "\n";
    }
    
    string BuildQuestStatusText(string status)
    {
        if (status == "active")
            return "взят";
        
        if (status == "reward_pending")
            return "награда ожидает выдачи";
        
        if (status == "completed")
            return "выполнен";
        
        return status;
    }
    
    string BuildQuestStageRoleLabel(bool isActiveStage, string roleText, string triggerText)
    {
        string prefix = "[ ] ";
        if (isActiveStage)
            prefix = "[>] ";

        if (triggerText != "")
            return prefix + roleText + ": " + triggerText;

        return prefix + roleText;
    }

    string BuildQuestStageLineText(Silver77_Quest quest, string status)
    {
        if (!quest)
            return "";

        string nextTriggerId = QuestClientManager.GetNextQuestTriggerId(m_Player, quest.id);
        array<string> offerTriggerIds = QuestClientManager.GetQuestOfferTriggerIds(quest);
        array<string> completionTriggerIds = QuestClientManager.GetQuestCompletionTriggerIds(quest);
        array<string> rewardTriggerIds = QuestClientManager.GetQuestRewardTriggerIds(quest);

        string text = "Линия квеста:\n";

        if (offerTriggerIds && offerTriggerIds.Count() > 0)
        {
            string offerTriggerText = QuestClientManager.GetTriggerDisplayText(offerTriggerIds.Get(0));
            bool offerActive = (status == "available" || status == "completed") && nextTriggerId == offerTriggerIds.Get(0);
            text += BuildQuestStageRoleLabel(offerActive, "Старт", offerTriggerText) + "\n";
        }

        if (completionTriggerIds)
        {
            foreach (string completionTriggerId : completionTriggerIds)
            {
                if (completionTriggerId == "")
                    continue;

                string completionTriggerText = QuestClientManager.GetTriggerDisplayText(completionTriggerId);
                bool completionActive = (status == "active") && nextTriggerId == completionTriggerId;
                text += BuildQuestStageRoleLabel(completionActive, "Промежуточный этап", completionTriggerText) + "\n";
            }
        }

        if (rewardTriggerIds && rewardTriggerIds.Count() > 0)
        {
            string rewardTriggerText = QuestClientManager.GetTriggerDisplayText(rewardTriggerIds.Get(0));
            bool rewardActive = (status == "reward_pending") || ((status == "active") && nextTriggerId == rewardTriggerIds.Get(0) && !QuestClientManager.QuestHasRoleTriggers(quest.completionTriggerIds));
            text += BuildQuestStageRoleLabel(rewardActive, "Завершение", rewardTriggerText) + "\n";
        }

        return text + "\n";
    }

    string BuildQuestNpcContextText(Silver77_Quest quest, string status, string currentTriggerId, bool canAccept, bool canComplete, bool canDeposit, bool canClaimReward)
    {
        if (!quest)
            return "";

        string nextTriggerId = QuestClientManager.GetNextQuestTriggerId(m_Player, quest.id);
        string nextTriggerText = QuestClientManager.GetTriggerDisplayText(nextTriggerId);
        string contextText = "";

        if (canAccept)
            contextText = "Этот NPC выдает квест.";
        else if (canClaimReward)
            contextText = "У этого NPC можно забрать награду прямо сейчас.";
        else if (canComplete)
            contextText = "У этого NPC можно завершить текущий этап.";
        else if (canDeposit)
            contextText = "У этого NPC можно передать часть нужных предметов.";
        else if ((status == "active" || status == "reward_pending") && nextTriggerText != "")
            contextText = "Квест уже в работе. Следующий шаг по цепочке: " + nextTriggerText + ".";
        else if (status == "active" || status == "reward_pending")
            contextText = "Квест уже в работе. Открой журнал, если нужно сверить текущий этап.";

        if (contextText == "")
            return "";

        return "Контекст NPC:\n" + contextText + "\n\n";
    }

    array<ref Silver77_QuestItem> GetVisibleRewardItems(Silver77_Quest quest, string status, string currentTriggerId, bool canComplete, bool canClaimReward)
    {
        if (!quest)
            return null;
        
        if (canClaimReward)
            return QuestClientManager.GetQuestActionRewards(quest, currentTriggerId, "reward");
        
        if (status == "active")
        {
            if (canClaimReward)
                return QuestClientManager.GetQuestActionRewards(quest, currentTriggerId, "reward");
            
            if (canComplete)
                return QuestClientManager.GetQuestActionRewards(quest, currentTriggerId, "completion");
            
            return null;
        }
        
        return quest.rewards;
    }
    
    string BuildQuestDialogText(Silver77_Quest quest, string status, string currentTriggerId, bool canAccept, bool canComplete, bool canClaimReward)
    {
        if (!quest)
            return "";

        string focusedTriggerId = "";
        string focusedActionType = "";
        string focusedRouteKey = m_SelectedRouteKey;
        if (focusedRouteKey != "")
        {
            TStringArray routeParts = new TStringArray;
            focusedRouteKey.Split("|", routeParts);
            if (routeParts.Count() >= 2)
            {
                focusedTriggerId = routeParts.Get(0);
                focusedActionType = routeParts.Get(1);
            }
        }

        if (focusedTriggerId == "" || focusedActionType == "")
        {
            string currentActionType = ResolveCurrentDialogActionType(quest, status, currentTriggerId, canAccept, canComplete, canClaimReward);
            if (currentTriggerId != "" && currentActionType != "")
            {
                focusedTriggerId = currentTriggerId;
                focusedActionType = currentActionType;
            }
        }

        if (focusedTriggerId != "" && focusedActionType != "")
        {
            string focusedDialogText = QuestClientManager.GetQuestTriggerActionDialogText(quest, focusedTriggerId, focusedActionType);
            if (focusedDialogText != "")
                return focusedDialogText;
        }

        return "";
    }

    string BuildQuestDialogJournalText(Silver77_Quest quest, string status, string currentTriggerId, bool canAccept, bool canComplete, bool canClaimReward)
    {
        if (!quest)
            return "";

        string text = "Диалоговый журнал:\n";
        bool hasAnyEntry = false;

        array<ref PlayerQuestStageVisit> stageVisits = QuestClientManager.GetQuestStageVisitsSorted(m_Player, quest.id);
        string currentActionType = ResolveCurrentDialogActionType(quest, status, currentTriggerId, canAccept, canComplete, canClaimReward);
        string currentRouteKey = BuildRouteKey(currentTriggerId, currentActionType);
        string focusedRouteKey = m_SelectedRouteKey;
        if (focusedRouteKey == "")
            focusedRouteKey = currentRouteKey;

        string currentStageBlock = "";
        if (currentTriggerId != "" && currentActionType != "")
            currentStageBlock = BuildCurrentQuestDialogEntry(quest, currentTriggerId, currentActionType, focusedRouteKey == currentRouteKey);

        if (currentStageBlock != "")
        {
            text += currentStageBlock;
            hasAnyEntry = true;
        }

        if (stageVisits)
        {
            for (int stageVisitIndex = stageVisits.Count() - 1; stageVisitIndex >= 0; stageVisitIndex--)
            {
                PlayerQuestStageVisit stageVisit = stageVisits.Get(stageVisitIndex);
                string stageBlock = BuildQuestDialogHistoryEntry(quest, stageVisit);
                if (stageBlock == "")
                    continue;

                string stageRouteKey = BuildRouteKey(stageVisit.triggerId, stageVisit.actionType);
                bool isFocused = (focusedRouteKey != "" && stageRouteKey == focusedRouteKey);
                if (isFocused)
                    stageBlock = ">> " + stageBlock;

                if (currentRouteKey != "" && stageRouteKey == currentRouteKey)
                    continue;

                if (hasAnyEntry)
                    text += "\n";

                text += stageBlock;
                hasAnyEntry = true;
            }
        }

        if (!hasAnyEntry)
            return "";

        return text;
    }

    string ResolveCurrentDialogActionType(Silver77_Quest quest, string status, string currentTriggerId, bool canAccept, bool canComplete, bool canClaimReward)
    {
        if (!quest || currentTriggerId == "")
            return "";

        if (canAccept)
            return "offer";

        if (canClaimReward)
            return "reward";

        if (status == "active")
        {
            string completionDialogText = QuestClientManager.GetQuestTriggerActionDialogText(quest, currentTriggerId, "completion");
            if (completionDialogText != "")
                return "completion";
        }

        return "";
    }

    string BuildQuestDialogHistoryEntry(Silver77_Quest quest, PlayerQuestStageVisit stageVisit)
    {
        if (!quest || !stageVisit)
            return "";

        string dialogText = QuestClientManager.GetQuestTriggerActionDialogText(quest, stageVisit.triggerId, stageVisit.actionType);
        if (dialogText == "")
            return "";

        string triggerText = QuestClientManager.GetTriggerDisplayText(stageVisit.triggerId);
        string actionLabel = BuildQuestDialogActionLabel(stageVisit.actionType);
        string header = triggerText;
        if (actionLabel != "")
            header = header + " - " + actionLabel;

        return "[" + header + "]\n" + dialogText;
    }

    string BuildCurrentQuestDialogEntry(Silver77_Quest quest, string triggerId, string actionType, bool isFocused = false)
    {
        if (!quest || triggerId == "" || actionType == "")
            return "";

        string dialogText = QuestClientManager.GetQuestTriggerActionDialogText(quest, triggerId, actionType);
        if (dialogText == "")
            return "";

        string triggerText = QuestClientManager.GetTriggerDisplayText(triggerId);
        string actionLabel = BuildQuestDialogActionLabel(actionType);
        string header = triggerText;
        if (actionLabel != "")
            header = header + " - " + actionLabel;

        if (isFocused)
            header = ">> " + header;

        return "[" + header + "]\n" + dialogText;
    }

    string BuildQuestDialogActionLabel(string actionType)
    {
        if (actionType == "offer")
            return "Старт";

        if (actionType == "completion")
            return "Этап";

        if (actionType == "reward")
            return "Награда";

        return actionType;
    }
    
    void UpdateButtons()
    {
        if (!m_AcceptButton || !m_CompleteButton)
            return;
        
        if (m_SelectedQuestId == "" || m_WaitingForServer)
        {
            if (m_AcceptButtonText)
                m_AcceptButtonText.SetText("ВЗЯТЬ КВЕСТ");
            if (m_CompleteButtonText)
                m_CompleteButtonText.SetText("СДАТЬ КВЕСТ");
            m_AcceptButton.Enable(false);
            m_CompleteButton.Enable(false);
            return;
        }
        
        string currentTriggerId = GetCurrentTriggerId();
        string status = QuestClientManager.GetQuestStatus(m_Player, m_SelectedQuestId);
        bool canAccept = QuestClientManager.CanAcceptQuest(m_Player, m_SelectedQuestId, currentTriggerId);
        bool canComplete = QuestClientManager.CanCompleteQuest(m_Player, m_SelectedQuestId, currentTriggerId);
        bool canDeposit = QuestClientManager.CanDepositQuestItems(m_Player, m_SelectedQuestId, currentTriggerId);
        bool canClaimReward = QuestClientManager.CanClaimReward(m_Player, m_SelectedQuestId, currentTriggerId);

        if (m_AcceptButtonText)
        {
            if (canAccept)
                m_AcceptButtonText.SetText("ВЗЯТЬ КВЕСТ");
            else if (status == "active" || status == "reward_pending")
                m_AcceptButtonText.SetText("УЖЕ АКТИВЕН");
            else if (status == "completed")
                m_AcceptButtonText.SetText("ВЫПОЛНЕН");
            else
                m_AcceptButtonText.SetText("НЕДОСТУПНО");
        }

        if (m_CompleteButtonText)
        {
            if (canClaimReward)
                m_CompleteButtonText.SetText("ПОЛУЧИТЬ НАГРАДУ");
            else if (canComplete)
                m_CompleteButtonText.SetText("ЗАВЕРШИТЬ ЭТАП");
            else if (canDeposit)
                m_CompleteButtonText.SetText("ПЕРЕДАТЬ ПРЕДМЕТЫ");
            else
                m_CompleteButtonText.SetText("ЭТАП НЕДОСТУПЕН");
        }
        
        m_AcceptButton.Enable(canAccept);
        m_CompleteButton.Enable(canComplete || canDeposit || canClaimReward);
        
        if (m_CloseButton)
            m_CloseButton.Enable(true);
        
        if (m_CloseButtonText)
            m_CloseButtonText.SetText("Закрыть рот и уйти");
    }
    
    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
        if (GetGame().GetMission() && GetGame().GetMission().GetHud())
            GetGame().GetMission().GetHud().Show(false);
        
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
        if (GetGame().GetMission() && GetGame().GetMission().GetHud())
            GetGame().GetMission().GetHud().Show(true);
        
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
            mission.OnQuestMenuClosed();
    }
}
