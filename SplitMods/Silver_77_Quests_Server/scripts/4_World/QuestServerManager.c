// ============================================================================
// МЕНЕДЖЕР КВЕСТОВ
// ============================================================================

ref Silver77_QuestConfig g_ServerQuestConfig;
ref map<string, ref PlayerQuestData> g_ServerPlayerQuestData;
int g_ServerQuestConfigRevision = 0;
int g_ServerQuestDataRevision = 0;

Silver77_QuestConfig CreateDefaultQuestConfig()
{
    Silver77_QuestConfig config = new Silver77_QuestConfig();
    config.version = 2;
    
    // ========== КВЕСТ 1: Коля Ворон ==========
    Silver77_Quest hunterQuest = new Silver77_Quest();
    hunterQuest.id = "quest_hunter_1";
    hunterQuest.name = "Картошечка с маслицем";
    hunterQuest.description = "Слушай меня тут озадачил Маскимыч, говорит, жареху надо оформить. Где хочешь найди мне картошки 50 штук, 1 лисичку, 2 сироежки, 1 белый гриб, только чтоб свежее все! И это на тебе ножичек, что бы с корнями не вырывал!";
    hunterQuest.repeatable = true;
    hunterQuest.cooldownSeconds = 43200;
    hunterQuest.requiresPrevious = "";
    
    Silver77_QuestItem giveHunterKnife = new Silver77_QuestItem();
    giveHunterKnife.className = "SteakKnife";
    giveHunterKnife.quantity = 1;
    giveHunterKnife.spawnOnGround = false;
    hunterQuest.giveItems.Insert(giveHunterKnife);
    
    Silver77_QuestObjective objPotato = new Silver77_QuestObjective();
    objPotato.type = "item";
    objPotato.className = "PotatoSeed";
    objPotato.quantity = 50;
    objPotato.removeOnComplete = true;
    objPotato.useItemQuantity = false;
    objPotato.allowPartialTurnIn = true;
    hunterQuest.objectives.Insert(objPotato);
    
    Silver77_QuestObjective objPleurotus = new Silver77_QuestObjective();
    objPleurotus.type = "item";
    objPleurotus.className = "PleurotusMushroom";
    objPleurotus.quantity = 1;
    objPleurotus.removeOnComplete = true;
    objPleurotus.useItemQuantity = false;
    objPleurotus.allowPartialTurnIn = true;
    hunterQuest.objectives.Insert(objPleurotus);
    
    Silver77_QuestObjective objMacrolepiota = new Silver77_QuestObjective();
    objMacrolepiota.type = "item";
    objMacrolepiota.className = "MacrolepiotaMushroom";
    objMacrolepiota.quantity = 2;
    objMacrolepiota.removeOnComplete = true;
    objMacrolepiota.useItemQuantity = false;
    objMacrolepiota.allowPartialTurnIn = true;
    hunterQuest.objectives.Insert(objMacrolepiota);
    
    Silver77_QuestObjective objBoletus = new Silver77_QuestObjective();
    objBoletus.type = "item";
    objBoletus.className = "BoletusMushroom";
    objBoletus.quantity = 1;
    objBoletus.removeOnComplete = true;
    objBoletus.useItemQuantity = false;
    objBoletus.allowPartialTurnIn = true;
    hunterQuest.objectives.Insert(objBoletus);
    
    Silver77_QuestItem rewardHunterShells = new Silver77_QuestItem();
    rewardHunterShells.className = "Ammo_12gaPellets";
    rewardHunterShells.quantity = 7;
    rewardHunterShells.spawnOnGround = false;
    hunterQuest.rewards.Insert(rewardHunterShells);
    
    config.quests.Insert(hunterQuest);
    
    // ========== КВЕСТ 2: Рыбак Гаврила ==========
    Silver77_Quest fishermanQuest = new Silver77_Quest();
    fishermanQuest.id = "quest_fisherman_1";
    fishermanQuest.name = "Рыба это вам не картошка!";
    fishermanQuest.description = "Слух, родной, принеси мне карпиков, штук 6, я тут застрял, на какое то время, с черного, привык к рыбке, а тут хер ее найдешь, вобщем неси, я не обижу. Где ловить!? Так на черном озеро есть!)";
    fishermanQuest.repeatable = true;
    fishermanQuest.cooldownSeconds = 43200;
    fishermanQuest.requiresPrevious = "";
    
    Silver77_QuestItem giveFishingKnife = new Silver77_QuestItem();
    giveFishingKnife.className = "HuntingKnife";
    giveFishingKnife.quantity = 1;
    giveFishingKnife.spawnOnGround = false;
    fishermanQuest.giveItems.Insert(giveFishingKnife);
    
    Silver77_QuestObjective objCarp = new Silver77_QuestObjective();
    objCarp.type = "item";
    objCarp.className = "Carp";
    objCarp.quantity = 6;
    objCarp.removeOnComplete = true;
    objCarp.useItemQuantity = false;
    objCarp.allowPartialTurnIn = true;
    fishermanQuest.objectives.Insert(objCarp);
    
    Silver77_QuestItem rewardFishShells = new Silver77_QuestItem();
    rewardFishShells.className = "Ammo_12gaPellets";
    rewardFishShells.quantity = 0;
    rewardFishShells.spawnOnGround = false;
    fishermanQuest.rewards.Insert(rewardFishShells);
    
    config.quests.Insert(fishermanQuest);
    
    // ========== КВЕСТ 3: Федор Распутин ==========
    Silver77_Quest rasputinQuest = new Silver77_Quest();
    rasputinQuest.id = "quest_Rasputin_1";
    rasputinQuest.name = "Взаимовыручка прежде всего!";
    rasputinQuest.description = "Здорова, я тут это, вспомнил про дружка своего, вот проведать решил. Если не сложно, заготовь мне 10 палок и дров 6 штук, я тебе патрон накину. Ну и это, выпей за дружбана моего. Ну и самое главное, вижу не здешний. Тут 2 относительно безопасных поселений, хутор с лева от Ольши, и Черное Озеро. К озеру пойдёшь — гляди в оба, в лесу там заражённые шастают. А ежели в сторону Ольши двинешь, так с горки спустись метров на триста — сразу выйдешь куда надо, оно рядом. Только в саму Ольшу лишний раз не суйся пока… целее будешь.";
    rasputinQuest.repeatable = false;
    rasputinQuest.cooldownSeconds = 250000;
    rasputinQuest.requiresPrevious = "";
    
    Silver77_QuestItem giveAlcohol = new Silver77_QuestItem();
    giveAlcohol.className = "DisinfectantAlcohol";
    giveAlcohol.quantity = 1;
    giveAlcohol.spawnOnGround = false;
    rasputinQuest.giveItems.Insert(giveAlcohol);
    
    Silver77_QuestObjective objStick = new Silver77_QuestObjective();
    objStick.type = "item";
    objStick.className = "WoodenStick";
    objStick.quantity = 2;
    objStick.removeOnComplete = true;
    objStick.useItemQuantity = false;
    objStick.allowPartialTurnIn = true;
    rasputinQuest.objectives.Insert(objStick);
    
    Silver77_QuestObjective objFirewood = new Silver77_QuestObjective();
    objFirewood.type = "item";
    objFirewood.className = "Firewood";
    objFirewood.quantity = 2;
    objFirewood.removeOnComplete = true;
    objFirewood.useItemQuantity = false;
    objFirewood.allowPartialTurnIn = true;
    rasputinQuest.objectives.Insert(objFirewood);
    
    Silver77_QuestItem rewardRasputinShells = new Silver77_QuestItem();
    rewardRasputinShells.className = "Ammo_12gaPellets";
    rewardRasputinShells.quantity = 3;
    rewardRasputinShells.spawnOnGround = false;
    rasputinQuest.rewards.Insert(rewardRasputinShells);
    
    config.quests.Insert(rasputinQuest);
    
    // ========== КВЕСТ 4: Поставка медицины ==========
    Silver77_Quest fishermanQuest2 = new Silver77_Quest();
    fishermanQuest2.id = "quest_fisherman_2";
    fishermanQuest2.name = "Поставка медицины";
    fishermanQuest2.description = "Дружище, с рыбой ты мне помог, огромное спасибо тебе. Но теперь, мне нужны бинты, 6 штук будет достаточно. У Максимыча, да и у всех барыг, дикие цены на них, оно и понятно, это же барыги, хоть и свои... Та и хер с ними. На тебе ножичек, пройдись по домам, насобирай тряпок побольше, может бухла найдешь, для дезинфекции, ничего не пропускай. А тут уже, обратно когда вернёшься, под лестницей стоит верстак. Там и инструкция есть, подскажет что использовать для изготовления бинтов. Ты главное в верстак все положи, что надо для изготовления бинтов, так удобней будет. Все жду с нетерпением.";
    fishermanQuest2.repeatable = true;
    fishermanQuest2.cooldownSeconds = 80000;
    fishermanQuest2.requiresPrevious = "";
    
    Silver77_QuestItem giveMedicKnife = new Silver77_QuestItem();
    giveMedicKnife.className = "SteakKnife";
    giveMedicKnife.quantity = 1;
    giveMedicKnife.spawnOnGround = false;
    fishermanQuest2.giveItems.Insert(giveMedicKnife);
    
    Silver77_QuestObjective objBandage = new Silver77_QuestObjective();
    objBandage.type = "item";
    objBandage.className = "BandageDressing";
    objBandage.quantity = 1;
    objBandage.removeOnComplete = true;
    objBandage.useItemQuantity = false;
    objBandage.allowPartialTurnIn = true;
    fishermanQuest2.objectives.Insert(objBandage);
    
    Silver77_QuestItem rewardMedicShells = new Silver77_QuestItem();
    rewardMedicShells.className = "Ammo_12gaPellets";
    rewardMedicShells.quantity = 1;
    rewardMedicShells.spawnOnGround = false;
    fishermanQuest2.rewards.Insert(rewardMedicShells);
    
    config.quests.Insert(fishermanQuest2);
    
    // ========== ТРИГГЕРЫ ==========
    Silver77_QuestTriggerConfig hunterTrigger = new Silver77_QuestTriggerConfig();
    hunterTrigger.id = "hunter_trigger";
    hunterTrigger.position.Insert(13092.814453);
    hunterTrigger.position.Insert(117.007767);
    hunterTrigger.position.Insert(13084.485352);
    hunterTrigger.radius = 2.0;
    hunterTrigger.questIds.Insert("quest_hunter_1");
    hunterTrigger.hintText = "[F] Коля Ворон";
    config.triggers.Insert(hunterTrigger);
    
    Silver77_QuestTriggerConfig fishermanTrigger = new Silver77_QuestTriggerConfig();
    fishermanTrigger.id = "fisherman_trigger";
    fishermanTrigger.position.Insert(13091.663086);
    fishermanTrigger.position.Insert(116.755630);
    fishermanTrigger.position.Insert(13088.637695);
    fishermanTrigger.radius = 2.0;
    fishermanTrigger.questIds.Insert("quest_fisherman_2");
    fishermanTrigger.questIds.Insert("quest_fisherman_1");
    fishermanTrigger.hintText = "[F] Рыбак Гаврила";
    config.triggers.Insert(fishermanTrigger);
    
    Silver77_QuestTriggerConfig rasputinTrigger = new Silver77_QuestTriggerConfig();
    rasputinTrigger.id = "Rasputin_1_trigger";
    rasputinTrigger.position.Insert(12979.4);
    rasputinTrigger.position.Insert(212.68);
    rasputinTrigger.position.Insert(12761.7);
    rasputinTrigger.radius = 2.0;
    rasputinTrigger.questIds.Insert("quest_Rasputin_1");
    rasputinTrigger.hintText = "[F] Федор (Распутин)";
    config.triggers.Insert(rasputinTrigger);
    
    return config;
}

void NormalizeQuestConfig(Silver77_QuestConfig config)
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

void LoadQuestConfig()
{
    if (!GetGame().IsServer())
    {
        g_ServerQuestConfig = CreateDefaultQuestConfig();
        NormalizeQuestConfig(g_ServerQuestConfig);
        Print("[Silver_77_Quests] Client using in-memory default config until server sync arrives");
        return;
    }
    
    string configPath = "$profile:Silver_77_Quests/Silver_77_Quests.json";
    if (!FileExist(configPath))
    {
        Print("[Silver_77_Quests] Config not found, creating default...");
        if (!FileExist("$profile:Silver_77_Quests"))
        {
            MakeDirectory("$profile:Silver_77_Quests");
        }
        g_ServerQuestConfig = CreateDefaultQuestConfig();
        NormalizeQuestConfig(g_ServerQuestConfig);
        JsonFileLoader<Silver77_QuestConfig>.JsonSaveFile(configPath, g_ServerQuestConfig);
        Print("[Silver_77_Quests] Config created at: " + configPath);
    }
    else
    {
        g_ServerQuestConfig = new Silver77_QuestConfig();
        JsonFileLoader<Silver77_QuestConfig>.JsonLoadFile(configPath, g_ServerQuestConfig);
        NormalizeQuestConfig(g_ServerQuestConfig);
        Print("[Silver_77_Quests] Loaded " + g_ServerQuestConfig.quests.Count() + " quests and " + g_ServerQuestConfig.triggers.Count() + " triggers");
    }
}

class QuestServerManager
{
    static void Init()
    {
        LoadQuestConfig();
        g_ServerPlayerQuestData = new map<string, ref PlayerQuestData>;
        g_ServerQuestConfigRevision++;
        g_ServerQuestDataRevision++;
        Print("[Silver_77_Quests] QuestServerManager initialized");
    }
    
    static Silver77_Quest GetQuestById(string questId)
    {
        if (!g_ServerQuestConfig || !g_ServerQuestConfig.quests)
            return null;
        
        foreach (Silver77_Quest quest : g_ServerQuestConfig.quests)
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
    
    static PlayerQuestData GetPlayerData(PlayerBase player)
    {
        string steamId = GetPlayerSteamId(player);
        if (steamId == "")
            return null;
        
        if (!g_ServerPlayerQuestData)
            g_ServerPlayerQuestData = new map<string, ref PlayerQuestData>;
        
        if (!g_ServerPlayerQuestData.Contains(steamId))
        {
            PlayerQuestData data = null;
            if (GetGame().IsServer())
                data = LoadPlayerData(steamId);
            else
                data = CreatePlayerData(steamId);
            
            g_ServerPlayerQuestData.Insert(steamId, data);
        }
        
        PlayerQuestData playerData = g_ServerPlayerQuestData.Get(steamId);
        if (EnsurePlayerProgress(playerData) && GetGame().IsServer())
            SavePlayerData(playerData);
        
        return playerData;
    }
    
    static PlayerQuestData CreatePlayerData(string steamId)
    {
        PlayerQuestData data = new PlayerQuestData();
        data.steamId = steamId;
        EnsurePlayerProgress(data);
        return data;
    }
    
    static PlayerQuestData LoadPlayerData(string steamId)
    {
        PlayerQuestData data = new PlayerQuestData();
        string filePath = "$profile:Silver_77_Quests/players/" + steamId + ".json";
        
        if (FileExist(filePath))
        {
            JsonFileLoader<PlayerQuestData>.JsonLoadFile(filePath, data);
            if (data.steamId == "")
                data.steamId = steamId;
            
            if (EnsurePlayerProgress(data))
                SavePlayerData(data);
            
            return data;
        }
        
        data = CreatePlayerData(steamId);
        SavePlayerData(data);
        return data;
    }
    
    static void SavePlayerData(PlayerQuestData data)
    {
        if (!GetGame().IsServer() || !data || data.steamId == "")
            return;
        
        if (!FileExist("$profile:Silver_77_Quests"))
            MakeDirectory("$profile:Silver_77_Quests");
        
        if (!FileExist("$profile:Silver_77_Quests/players"))
            MakeDirectory("$profile:Silver_77_Quests/players");
        
        string filePath = "$profile:Silver_77_Quests/players/" + data.steamId + ".json";
        JsonFileLoader<PlayerQuestData>.JsonSaveFile(filePath, data);
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
        
        if (!g_ServerQuestConfig || !g_ServerQuestConfig.quests)
            return changed;
        
        foreach (Silver77_Quest quest : g_ServerQuestConfig.quests)
        {
            if (!quest)
                continue;
            
            if (!FindProgress(data, quest.id))
            {
                PlayerQuestProgress progress = new PlayerQuestProgress();
                progress.questId = quest.id;
                progress.status = "available";
                data.progress.Insert(progress);
                changed = true;
            }
            else
            {
                PlayerQuestProgress existingProgress = FindProgress(data, quest.id);
                if (existingProgress && !existingProgress.objectiveProgress)
                {
                    existingProgress.objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
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
    
    static PlayerQuestProgress GetOrCreateProgress(PlayerQuestData data, string questId)
    {
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (progress)
            return progress;
        
        if (!data)
            return null;
        
        if (!data.progress)
            data.progress = new array<ref PlayerQuestProgress>;
        
        progress = new PlayerQuestProgress();
        progress.questId = questId;
        progress.status = "available";
        data.progress.Insert(progress);
        return progress;
    }
    
    static string GetQuestStatus(PlayerBase player, string questId)
    {
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (progress)
            return progress.status;
        
        return "available";
    }
    
    static void SetQuestStatus(PlayerBase player, string questId, string status)
    {
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = GetOrCreateProgress(data, questId);
        if (!progress)
            return;
        
        progress.status = status;
        SavePlayerData(data);
        g_ServerQuestDataRevision++;
    }
    
    static void ClearObjectiveProgress(PlayerQuestProgress progress)
    {
        if (!progress)
            return;
        
        if (!progress.objectiveProgress)
            progress.objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
        
        progress.objectiveProgress.Clear();
    }
    
    static PlayerQuestObjectiveProgress GetOrCreateObjectiveProgress(PlayerQuestProgress progress, int objectiveIndex, string className)
    {
        if (!progress)
            return null;
        
        if (!progress.objectiveProgress)
            progress.objectiveProgress = new array<ref PlayerQuestObjectiveProgress>;
        
        foreach (PlayerQuestObjectiveProgress objectiveProgress : progress.objectiveProgress)
        {
            if (!objectiveProgress)
                continue;
            
            if (objectiveProgress.objectiveIndex == objectiveIndex && objectiveProgress.className == className)
                return objectiveProgress;
        }
        
        PlayerQuestObjectiveProgress newProgress = new PlayerQuestObjectiveProgress();
        newProgress.objectiveIndex = objectiveIndex;
        newProgress.className = className;
        progress.objectiveProgress.Insert(newProgress);
        return newProgress;
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
    
    static bool AcceptQuest(PlayerBase player, string questId)
    {
        if (!GetGame().IsServer() || !CanAcceptQuest(player, questId))
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        foreach (Silver77_QuestItem giveItem : quest.giveItems)
        {
            SpawnQuestItem(player, giveItem);
        }
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = GetOrCreateProgress(data, questId);
        if (progress)
        {
            progress.status = "active";
            ClearObjectiveProgress(progress);
        }
        
        SavePlayerData(data);
        g_ServerQuestDataRevision++;
        Print("[Silver_77_Quests] Player accepted quest: " + questId);
        return true;
    }
    
    static bool CanCompleteQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        string status = GetQuestStatus(player, questId);
        if (status != "active")
            return false;
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = FindProgress(data, questId);
        
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective)
                continue;
            
            if (objective.type == "item")
            {
                float deposited = 0;
                if (objective.allowPartialTurnIn)
                    deposited = GetDepositedObjectiveQuantity(progress, i, objective.className);
                
                float remaining = objective.quantity - deposited;
                if (remaining <= 0)
                    continue;
                
                if (!PlayerHasItemAmount(player, objective.className, remaining, objective.useItemQuantity))
                    return false;
            }
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
    
    static bool CompleteQuest(PlayerBase player, string questId)
    {
        if (!GetGame().IsServer() || !player || questId == "")
            return false;
        
        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;
        
        if (GetQuestStatus(player, questId) != "active")
            return false;
        
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = GetOrCreateProgress(data, questId);
        bool depositedSomething = DepositPartialQuestItems(player, quest, progress);
        if (depositedSomething)
        {
            SavePlayerData(data);
            g_ServerQuestDataRevision++;
        }
        
        if (!CanCompleteQuest(player, questId))
            return depositedSomething;
        
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective)
                continue;
            
            if (objective.type == "item" && objective.removeOnComplete)
            {
                if (!objective.allowPartialTurnIn)
                    RemoveItemsFromPlayer(player, objective.className, objective.quantity, objective.useItemQuantity);
            }
        }
        
        foreach (Silver77_QuestItem reward : quest.rewards)
        {
            SpawnQuestItem(player, reward);
        }
        
        if (progress)
        {
            progress.lastCompletedTime = GetCurrentUnixTimeUTC();
            progress.status = "completed";
            ClearObjectiveProgress(progress);
        }
        
        SavePlayerData(data);
        g_ServerQuestDataRevision++;
        
        Print("[Silver_77_Quests] Player completed quest: " + questId);
        return true;
    }
    
    static bool DepositPartialQuestItems(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress)
    {
        if (!player || !quest || !progress || !quest.objectives)
            return false;
        
        bool depositedSomething = false;
        
        for (int i = 0; i < quest.objectives.Count(); i++)
        {
            Silver77_QuestObjective objective = quest.objectives.Get(i);
            if (!objective || objective.type != "item" || !objective.allowPartialTurnIn)
                continue;
            
            PlayerQuestObjectiveProgress objectiveProgress = GetOrCreateObjectiveProgress(progress, i, objective.className);
            if (!objectiveProgress)
                continue;
            
            float remaining = objective.quantity - objectiveProgress.depositedQuantity;
            if (remaining <= 0)
                continue;
            
            float available = CountPlayerItems(player, objective.className, objective.useItemQuantity);
            if (available <= 0)
                continue;
            
            float toDeposit = available;
            if (toDeposit > remaining)
                toDeposit = remaining;
            
            if (toDeposit <= 0)
                continue;
            
            RemoveItemsFromPlayer(player, objective.className, toDeposit, objective.useItemQuantity);
            objectiveProgress.depositedQuantity += toDeposit;
            if (objectiveProgress.depositedQuantity > objective.quantity)
                objectiveProgress.depositedQuantity = objective.quantity;
            
            depositedSomething = true;
            Print("[Silver_77_Quests] Deposited partial objective for " + quest.id + ": " + objective.className + " +" + toDeposit + " (" + objectiveProgress.depositedQuantity + "/" + objective.quantity + ")");
        }
        
        return depositedSomething;
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
    
    static void RemoveItemsFromPlayer(PlayerBase player, string className, float quantity, bool useItemQuantity = false)
    {
        if (!player || className == "" || quantity <= 0)
            return;
        
        float remaining = quantity;
        array<EntityAI> items = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        foreach (EntityAI item : items)
        {
            if (remaining <= 0)
                break;
            
            if (item && item.IsKindOf(className))
            {
                if (useItemQuantity && item.HasQuantity())
                {
                    float itemQty = item.GetQuantity();
                    if (itemQty <= remaining)
                    {
                        remaining -= itemQty;
                        item.Delete();
                    }
                    else
                    {
                        item.SetQuantity(itemQty - remaining);
                        remaining = 0;
                    }
                }
                else
                {
                    item.Delete();
                    remaining -= 1;
                }
            }
        }
    }
    
    static void SpawnQuestItem(PlayerBase player, Silver77_QuestItem questItem)
    {
        if (!GetGame().IsServer() || !player || !questItem || questItem.className == "")
            return;
        
        for (int i = 0; i < questItem.quantity; i++)
        {
            if (questItem.spawnOnGround)
            {
                GetGame().CreateObjectEx(questItem.className, player.GetPosition(), ECE_PLACE_ON_SURFACE | ECE_KEEPHEIGHT);
            }
            else
            {
                EntityAI item = player.GetInventory().CreateInInventory(questItem.className);
                if (!item)
                {
                    GetGame().CreateObjectEx(questItem.className, player.GetPosition(), ECE_PLACE_ON_SURFACE | ECE_KEEPHEIGHT);
                }
            }
        }
    }
    
    static bool RequestAcceptQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        if (GetGame().IsServer())
            return AcceptQuest(player, questId);
        
        Print("[Silver_77_Quests] Sending accept quest RPC: " + questId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_ACCEPT, new Param1<string>(questId), true);
        return true;
    }
    
    static bool RequestCompleteQuest(PlayerBase player, string questId)
    {
        if (!player || questId == "")
            return false;
        
        if (GetGame().IsServer())
            return CompleteQuest(player, questId);
        
        Print("[Silver_77_Quests] Sending complete quest RPC: " + questId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_COMPLETE, new Param1<string>(questId), true);
        return true;
    }
    
    static void RequestConfigSync(PlayerBase player)
    {
        if (!player)
            return;
        
        if (GetGame().IsServer())
        {
            SendConfigToClient(player);
            return;
        }
        
        Print("[Silver_77_Quests] Sending quest config request RPC");
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_CONFIG_REQUEST, new Param1<bool>(true), true);
    }
    
    static void RequestPlayerDataSync(PlayerBase player)
    {
        if (!player)
            return;
        
        if (GetGame().IsServer())
        {
            SendPlayerDataToClient(player);
            return;
        }
        
        Print("[Silver_77_Quests] Sending quest progress request RPC");
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_PLAYER_DATA_REQUEST, new Param1<bool>(true), true);
    }
    
    static void SendConfigToClient(PlayerBase player)
    {
        if (!GetGame().IsServer() || !player || !player.GetIdentity() || !g_ServerQuestConfig)
            return;
        
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_CONFIG_DATA, new Param1<Silver77_QuestConfig>(g_ServerQuestConfig), true, player.GetIdentity());
        Print("[Silver_77_Quests] Sent quest config to client");
    }
    
    static void SendPlayerDataToClient(PlayerBase player)
    {
        if (!GetGame().IsServer() || !player || !player.GetIdentity())
            return;
        
        PlayerQuestData data = GetPlayerData(player);
        if (!data)
            return;
        
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_PLAYER_DATA, new Param1<PlayerQuestData>(data), true, player.GetIdentity());
        Print("[Silver_77_Quests] Sent quest progress to client: " + data.steamId);
    }
    
    static void ApplySyncedConfig(Silver77_QuestConfig config)
    {
        if (!config)
            return;
        
        NormalizeQuestConfig(config);
        g_ServerQuestConfig = config;
        g_ServerQuestConfigRevision++;
        
        int questCount = 0;
        int triggerCount = 0;
        if (g_ServerQuestConfig.quests)
            questCount = g_ServerQuestConfig.quests.Count();
        if (g_ServerQuestConfig.triggers)
            triggerCount = g_ServerQuestConfig.triggers.Count();
        
        Print("[Silver_77_Quests] Applied synced quest config: " + questCount + " quests, " + triggerCount + " triggers");
    }
    
    static void ApplySyncedPlayerData(PlayerQuestData data)
    {
        if (!data || data.steamId == "")
            return;
        
        if (!g_ServerPlayerQuestData)
            g_ServerPlayerQuestData = new map<string, ref PlayerQuestData>;
        
        EnsurePlayerProgress(data);
        g_ServerPlayerQuestData.Set(data.steamId, data);
        g_ServerQuestDataRevision++;
        Print("[Silver_77_Quests] Applied synced player progress: " + data.steamId);
    }
}



