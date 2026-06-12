// ============================================================================
// QUEST SERVER MANAGER
// ============================================================================

const int SILVER77_QUEST_CONFIG_VERSION = 3;
const int SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE = 512;
const string SILVER77_QUEST_CONFIG_DIR = "$profile:Silver_77_Quests";
const string SILVER77_QUEST_CONFIG_PATH = "$profile:Silver_77_Quests/Silver_77_Quests.json";

ref Silver77_QuestConfig g_ServerQuestConfig;
ref map<string, ref PlayerQuestData> g_ServerPlayerQuestData;
ref array<Object> g_ServerQuestNpcObjects;
int g_ServerQuestConfigRevision = 0;
int g_ServerQuestDataRevision = 0;

Silver77_QuestConfig CreateDefaultQuestConfig()
{
    Silver77_QuestConfig config = new Silver77_QuestConfig();
    config.version = SILVER77_QUEST_CONFIG_VERSION;

    // ========== QUEST 1: quest_hunter_1 ==========
    Silver77_Quest quest1 = new Silver77_Quest();
    quest1.id = "quest_hunter_1";
    quest1.name = "Картошечка с маслицем";
    quest1.description = "Слушай меня тут озадачил Маскимыч, говорит, жареху надо оформить. Где хочешь найди мне картошки 50 штук, 1 лисичку, 2 сироежки, 1 белый гриб, только чтоб свежее все! И это на тебе ножичек, что бы с корнями не вырывал!";
    quest1.repeatable = true;
    quest1.cooldownSeconds = 43200;
    quest1.requiresPrevious = "";
    quest1.hideUntilRequirementsComplete = false;
    quest1.offerTriggerIds.Insert("hunter_trigger");
    quest1.rewardTriggerIds.Insert("hunter_trigger");

    Silver77_QuestTriggerAction quest1Action1 = new Silver77_QuestTriggerAction();
    quest1Action1.triggerId = "hunter_trigger";
    quest1Action1.actionType = "offer";
    quest1Action1.dialogText = "Слушай, меня тут озадачил Маскимыч, говорит, жареху надо оформить, гости будут. Где хочешь найди мне картошки 120 штук, 2 лисички, 2 сыроежки, 2 белых гриба, только чтоб свежее все! И это, на тебе ножичек, что бы с корнями не вырывал! Я проверю потом, если Максимычу пожалуюсь, он такое решает быстро, еда это не шутка тут.";
    quest1.triggerActions.Insert(quest1Action1);

    Silver77_QuestTriggerAction quest1Action2 = new Silver77_QuestTriggerAction();
    quest1Action2.triggerId = "hunter_trigger";
    quest1Action2.actionType = "reward";
    quest1Action2.dialogText = "Вот это ты меня выручил, большое спасибо тебе! Держи патрики!";
    Silver77_QuestItem quest1Action2Reward1 = new Silver77_QuestItem();
    quest1Action2Reward1.className = "Ammo_12gaPellets";
    quest1Action2Reward1.quantity = 8;
    quest1Action2Reward1.spawnOnGround = false;
    quest1Action2.rewards.Insert(quest1Action2Reward1);
    quest1.triggerActions.Insert(quest1Action2);

    Silver77_QuestItem quest1GiveItem1 = new Silver77_QuestItem();
    quest1GiveItem1.className = "SteakKnife";
    quest1GiveItem1.quantity = 1;
    quest1GiveItem1.spawnOnGround = false;
    quest1.giveItems.Insert(quest1GiveItem1);

    Silver77_QuestObjective quest1Objective1 = new Silver77_QuestObjective();
    quest1Objective1.type = "item";
    quest1Objective1.className = "PotatoSeed";
    quest1Objective1.quantity = 120;
    quest1Objective1.removeOnComplete = true;
    quest1Objective1.useItemQuantity = false;
    quest1Objective1.allowPartialTurnIn = true;
    quest1.objectives.Insert(quest1Objective1);

    Silver77_QuestObjective quest1Objective2 = new Silver77_QuestObjective();
    quest1Objective2.type = "item";
    quest1Objective2.className = "PleurotusMushroom";
    quest1Objective2.quantity = 2;
    quest1Objective2.removeOnComplete = true;
    quest1Objective2.useItemQuantity = false;
    quest1Objective2.allowPartialTurnIn = true;
    quest1.objectives.Insert(quest1Objective2);

    Silver77_QuestObjective quest1Objective3 = new Silver77_QuestObjective();
    quest1Objective3.type = "item";
    quest1Objective3.className = "MacrolepiotaMushroom";
    quest1Objective3.quantity = 2;
    quest1Objective3.removeOnComplete = true;
    quest1Objective3.useItemQuantity = false;
    quest1Objective3.allowPartialTurnIn = true;
    quest1.objectives.Insert(quest1Objective3);

    Silver77_QuestObjective quest1Objective4 = new Silver77_QuestObjective();
    quest1Objective4.type = "item";
    quest1Objective4.className = "BoletusMushroom";
    quest1Objective4.quantity = 2;
    quest1Objective4.removeOnComplete = true;
    quest1Objective4.useItemQuantity = false;
    quest1Objective4.allowPartialTurnIn = true;
    quest1.objectives.Insert(quest1Objective4);

    Silver77_QuestItem quest1Reward1 = new Silver77_QuestItem();
    quest1Reward1.className = "Ammo_12gaPellets";
    quest1Reward1.quantity = 7;
    quest1Reward1.spawnOnGround = false;
    quest1.rewards.Insert(quest1Reward1);

    config.quests.Insert(quest1);

    // ========== QUEST 2: quest_fisherman_1 ==========
    Silver77_Quest quest2 = new Silver77_Quest();
    quest2.id = "quest_fisherman_1";
    quest2.name = "Рыба это вам не картошка!";
    quest2.description = "Слух, родной, принеси мне карпиков, штук 6, я тут застрял, на какое то время, с черного, привык к рыбке, а тут хер ее найдешь, вобщем неси, я не обижу. Где ловить!? Так на черном озеро есть!)";
    quest2.repeatable = true;
    quest2.cooldownSeconds = 43200;
    quest2.requiresPrevious = "";
    quest2.hideUntilRequirementsComplete = false;
    quest2.offerTriggerIds.Insert("fisherman_trigger");
    quest2.rewardTriggerIds.Insert("fisherman_trigger");

    Silver77_QuestTriggerAction quest2Action1 = new Silver77_QuestTriggerAction();
    quest2Action1.triggerId = "fisherman_trigger";
    quest2Action1.actionType = "offer";
    quest2Action1.dialogText = "Слух, родной, принеси мне карпиков, штук 6, я тут застрял, на какое то время, с черного, привык к рыбке, а тут хер ее найдешь, в общем неси, я не обижу. Где ловить!? Так на черном озеро есть!)";
    quest2.triggerActions.Insert(quest2Action1);

    Silver77_QuestTriggerAction quest2Action2 = new Silver77_QuestTriggerAction();
    quest2Action2.triggerId = "fisherman_trigger";
    quest2Action2.actionType = "reward";
    quest2Action2.dialogText = "";
    Silver77_QuestItem quest2Action2Reward1 = new Silver77_QuestItem();
    quest2Action2Reward1.className = "Ammo_12gaPellets";
    quest2Action2Reward1.quantity = 8;
    quest2Action2Reward1.spawnOnGround = false;
    quest2Action2.rewards.Insert(quest2Action2Reward1);
    quest2.triggerActions.Insert(quest2Action2);

    Silver77_QuestItem quest2GiveItem1 = new Silver77_QuestItem();
    quest2GiveItem1.className = "HuntingKnife";
    quest2GiveItem1.quantity = 1;
    quest2GiveItem1.spawnOnGround = false;
    quest2.giveItems.Insert(quest2GiveItem1);

    Silver77_QuestObjective quest2Objective1 = new Silver77_QuestObjective();
    quest2Objective1.type = "item";
    quest2Objective1.className = "Carp";
    quest2Objective1.quantity = 6;
    quest2Objective1.removeOnComplete = true;
    quest2Objective1.useItemQuantity = false;
    quest2Objective1.allowPartialTurnIn = true;
    quest2.objectives.Insert(quest2Objective1);

    Silver77_QuestItem quest2Reward1 = new Silver77_QuestItem();
    quest2Reward1.className = "Ammo_12gaPellets";
    quest2Reward1.quantity = 8;
    quest2Reward1.spawnOnGround = false;
    quest2.rewards.Insert(quest2Reward1);

    config.quests.Insert(quest2);

    // ========== QUEST 3: quest_Rasputin_1 ==========
    Silver77_Quest quest3 = new Silver77_Quest();
    quest3.id = "quest_Rasputin_1";
    quest3.name = "Взаимовыручка прежде всего!";
    quest3.description = "Здарова, я тут это, вспомнил про дружка своего, вот проведать решил. Если не сложно, заготовь мне 10 палок и дров 6 штук, я тебе патрон накину. Ну и это, выпей за дружбана моего. Ну и самое главное, вижу не сдешний. Тут 2 относительно безопасных поселений, хутор с лева от Ольши, и Черное Озеро. К озеру пойдёшь — гляди в оба, в лесу там заражённые шастают. А ежели в сторону Ольши двинешь, так с горки спустись метров на триста — сразу выйдешь куда надо, оно рядом. Только в саму Ольшу лишний раз не суйся пока… целее будешь.";
    quest3.repeatable = false;
    quest3.cooldownSeconds = 250000;
    quest3.requiresPrevious = "";
    quest3.hideUntilRequirementsComplete = false;
    quest3.offerTriggerIds.Insert("Rasputin_1_trigger");
    quest3.rewardTriggerIds.Insert("Rasputin_1_trigger");

    Silver77_QuestTriggerAction quest3Action1 = new Silver77_QuestTriggerAction();
    quest3Action1.triggerId = "Rasputin_1_trigger";
    quest3Action1.actionType = "offer";
    quest3Action1.dialogText = "Здарова, я тут это, вспомнил про дружка своего, вот проведать решил. Если не сложно, заготовь мне 10 палок и дров 6 штук, я тебе патрон накину. Ну и это, выпей за дружбана моего. Ну и самое главное, вижу не сдешний. Тут 2 относительно безопасных поселений, хутор с лева от Ольши, и Черное Озеро. К озеру пойдёшь — гляди в оба, в лесу там заражённые шастают. А ежели в сторону Ольши двинешь, так с горки спустись метров на триста — сразу выйдешь куда надо, оно рядом. Только в саму Ольшу лишний раз не суйся пока… целее будешь.";
    quest3.triggerActions.Insert(quest3Action1);

    Silver77_QuestTriggerAction quest3Action2 = new Silver77_QuestTriggerAction();
    quest3Action2.triggerId = "Rasputin_1_trigger";
    quest3Action2.actionType = "reward";
    quest3Action2.dialogText = "Спасибо чувак, не забуду, мне ту по грусти шевелиться даже не хочется, на выпей за братана, Васек этого достоин.";
    Silver77_QuestItem quest3Action2Reward1 = new Silver77_QuestItem();
    quest3Action2Reward1.className = "DisinfectantAlcohol";
    quest3Action2Reward1.quantity = 1;
    quest3Action2Reward1.spawnOnGround = false;
    quest3Action2.rewards.Insert(quest3Action2Reward1);
    quest3.triggerActions.Insert(quest3Action2);

    Silver77_QuestObjective quest3Objective1 = new Silver77_QuestObjective();
    quest3Objective1.type = "item";
    quest3Objective1.className = "WoodenStick";
    quest3Objective1.quantity = 10;
    quest3Objective1.removeOnComplete = true;
    quest3Objective1.useItemQuantity = true;
    quest3Objective1.allowPartialTurnIn = true;
    quest3.objectives.Insert(quest3Objective1);

    Silver77_QuestObjective quest3Objective2 = new Silver77_QuestObjective();
    quest3Objective2.type = "item";
    quest3Objective2.className = "Firewood";
    quest3Objective2.quantity = 6;
    quest3Objective2.removeOnComplete = true;
    quest3Objective2.useItemQuantity = true;
    quest3Objective2.allowPartialTurnIn = true;
    quest3.objectives.Insert(quest3Objective2);

    Silver77_QuestItem quest3Reward1 = new Silver77_QuestItem();
    quest3Reward1.className = "Ammo_12gaPellets";
    quest3Reward1.quantity = 3;
    quest3Reward1.spawnOnGround = false;
    quest3.rewards.Insert(quest3Reward1);

    config.quests.Insert(quest3);

    // ========== QUEST 4: quest_fisherman_2 ==========
    Silver77_Quest quest4 = new Silver77_Quest();
    quest4.id = "quest_fisherman_2";
    quest4.name = "Поставка медицины";
    quest4.description = "Дружище, с рыбой ты мне помог, огромное спасибо тебе. Но теперь, мне нужны бинты, 6 штук будет достаточно. У Максимыча, да и у всех барыг, дикие цены на них, оно и понятно, это же барыги, хоть и свои... Та и хер с ними. На тебе ножичек, пройдись по домам, насобирай тряпок побольше, может бухла найдешь, для дезинфекции, ничего не пропускай. А тут уже, обратно когда вернёшься, под лестницей стоит верстак. Там и инструкция есть, подскажет что использовать для изготовления бинтов. Ты главное в верстак все положи, что надо для изготовления бинтов, так удобней будет. Все жду с нетерпением.";
    quest4.repeatable = true;
    quest4.cooldownSeconds = 80000;
    quest4.requiresPrevious = "";
    quest4.hideUntilRequirementsComplete = false;
    quest4.offerTriggerIds.Insert("fisherman_trigger");
    quest4.rewardTriggerIds.Insert("fisherman_trigger");

    Silver77_QuestTriggerAction quest4Action1 = new Silver77_QuestTriggerAction();
    quest4Action1.triggerId = "fisherman_trigger";
    quest4Action1.actionType = "offer";
    quest4Action1.dialogText = "Дружище, с рыбой ты мне помог, огромное спасибо тебе. Но теперь, мне нужны бинты, 6 штук будет достаточно. У Максимыча, да и у всех барыг, дикие цены на них, оно и понятно, это же барыги, хоть и свои... Та и хер с ними. На тебе ножичек, пройдись по домам, насобирай тряпок побольше, может бухла найдешь, для дезинфекции, ничего не пропускай. А тут уже, обратно когда вернёшься, под лестницей стоит верстак. Там и инструкция есть, подскажет что использовать для изготовления бинтов. Ты главное в верстак все положи, что надо для изготовления бинтов, так удобней будет. Все жду с нетерпением.";
    quest4.triggerActions.Insert(quest4Action1);

    Silver77_QuestTriggerAction quest4Action2 = new Silver77_QuestTriggerAction();
    quest4Action2.triggerId = "fisherman_trigger";
    quest4Action2.actionType = "reward";
    quest4Action2.dialogText = "Вот это я понимаю,вот это я понимаю... Спасибо за исполнение просьбы. Держи 100 патронов, потратился наверно на зараженных.";
    Silver77_QuestItem quest4Action2Reward1 = new Silver77_QuestItem();
    quest4Action2Reward1.className = "Ammo_12gaPellets";
    quest4Action2Reward1.quantity = 10;
    quest4Action2Reward1.spawnOnGround = false;
    quest4Action2.rewards.Insert(quest4Action2Reward1);
    quest4.triggerActions.Insert(quest4Action2);

    Silver77_QuestItem quest4GiveItem1 = new Silver77_QuestItem();
    quest4GiveItem1.className = "SteakKnife";
    quest4GiveItem1.quantity = 1;
    quest4GiveItem1.spawnOnGround = false;
    quest4.giveItems.Insert(quest4GiveItem1);

    Silver77_QuestObjective quest4Objective1 = new Silver77_QuestObjective();
    quest4Objective1.type = "item";
    quest4Objective1.className = "BandageDressing";
    quest4Objective1.quantity = 6;
    quest4Objective1.removeOnComplete = true;
    quest4Objective1.useItemQuantity = false;
    quest4Objective1.allowPartialTurnIn = true;
    quest4.objectives.Insert(quest4Objective1);

    Silver77_QuestItem quest4Reward1 = new Silver77_QuestItem();
    quest4Reward1.className = "Ammo_12gaPellets";
    quest4Reward1.quantity = 1;
    quest4Reward1.spawnOnGround = false;
    quest4.rewards.Insert(quest4Reward1);

    config.quests.Insert(quest4);

    // ========== QUEST 5: quest_hunter_2 ==========
    Silver77_Quest quest5 = new Silver77_Quest();
    quest5.id = "quest_hunter_2";
    quest5.name = "Поставка провизии";
    quest5.description = "Здорово, ты наверно знаешь где распутин сейчас, я тут ему приготовил картошки, отнеси ему если не сложно.";
    quest5.repeatable = false;
    quest5.cooldownSeconds = 0;
    quest5.requiresPrevious = "";
    quest5.hideUntilRequirementsComplete = false;
    quest5.offerTriggerIds.Insert("hunter_trigger");
    quest5.completionTriggerIds.Insert("Rasputin_1_trigger");
    quest5.rewardTriggerIds.Insert("hunter_trigger");

    Silver77_QuestTriggerAction quest5Action1 = new Silver77_QuestTriggerAction();
    quest5Action1.triggerId = "hunter_trigger";
    quest5Action1.actionType = "offer";
    quest5Action1.dialogText = "Здорово, ты наверно знаешь где Федя,  распутин который, на горе поди опять, бухает... Мда, даже как то не ловко его так, они друзьями были с Васей, не разлей вода, а тут такое, мда, гребаные собаки, ты там аккуратно сам, ружье на готове всегда держи, а то бухать за тбе не кому будет )) В общем так, я тут ему приготовил картошки, отнеси если не сложно.";
    quest5.triggerActions.Insert(quest5Action1);

    Silver77_QuestTriggerAction quest5Action2 = new Silver77_QuestTriggerAction();
    quest5Action2.triggerId = "Rasputin_1_trigger";
    quest5Action2.actionType = "completion";
    quest5Action2.dialogText = "Ооо, спасибо тебе, и Ворону за заботу, скоро зайду к нему. Скажи что скоро занесу ему собакена интересного, псевдопес его зовут вроде. Ты если что их добудешь, барыгам местным не вздумай продавать, они тут совсем оборзели, не, ну а че за цены то! В общем на ВИБИЧ надо их нести, поднакопи, и вперед, одному ходить по началу не рекомендую, да и минимум с двухстволкой надо туда идти... А поготь, на тебе пару грибов, на жареху )";
    Silver77_QuestItem quest5Action2Reward1 = new Silver77_QuestItem();
    quest5Action2Reward1.className = "BoletusMushroom";
    quest5Action2Reward1.quantity = 2;
    quest5Action2Reward1.spawnOnGround = false;
    quest5Action2.rewards.Insert(quest5Action2Reward1);
    quest5.triggerActions.Insert(quest5Action2);

    Silver77_QuestTriggerAction quest5Action3 = new Silver77_QuestTriggerAction();
    quest5Action3.triggerId = "hunter_trigger";
    quest5Action3.actionType = "reward";
    quest5Action3.dialogText = "Ну что, отнес, как он там, пропился? Трезвый говоришь, ну и ладненько. На тебе патриков немного.";
    Silver77_QuestItem quest5Action3Reward1 = new Silver77_QuestItem();
    quest5Action3Reward1.className = "Ammo_12gaPellets";
    quest5Action3Reward1.quantity = 30;
    quest5Action3Reward1.spawnOnGround = false;
    quest5Action3.rewards.Insert(quest5Action3Reward1);
    quest5.triggerActions.Insert(quest5Action3);

    Silver77_QuestItem quest5GiveItem1 = new Silver77_QuestItem();
    quest5GiveItem1.className = "PotatoSeed";
    quest5GiveItem1.quantity = 12;
    quest5GiveItem1.spawnOnGround = false;
    quest5.giveItems.Insert(quest5GiveItem1);

    Silver77_QuestObjective quest5Objective1 = new Silver77_QuestObjective();
    quest5Objective1.type = "item";
    quest5Objective1.className = "PotatoSeed";
    quest5Objective1.quantity = 12;
    quest5Objective1.removeOnComplete = true;
    quest5Objective1.useItemQuantity = false;
    quest5Objective1.allowPartialTurnIn = true;
    quest5.objectives.Insert(quest5Objective1);

    config.quests.Insert(quest5);

    // ========== TRIGGERS ==========

    Silver77_QuestTriggerConfig trigger1 = new Silver77_QuestTriggerConfig();
    trigger1.id = "hunter_trigger";
    trigger1.position.Insert(13092.814453125);
    trigger1.position.Insert(117.00776672363281);
    trigger1.position.Insert(13084.4853515625);
    trigger1.radius = 2.0;
    trigger1.focusHeight = 1.2000000476837158;
    trigger1.focusRadius = 1.0;
    trigger1.questIds.Insert("quest_hunter_2");
    trigger1.questIds.Insert("quest_hunter_1");
    trigger1.hintText = "[F] Коля Ворон";
    trigger1.spawnNpc = true;
    trigger1.npcClassName = "SurvivorM_Mirek";
    trigger1.npcPosition.Insert(13092.814453125);
    trigger1.npcPosition.Insert(117.00776672363281);
    trigger1.npcPosition.Insert(13084.4853515625);
    trigger1.npcOrientation.Insert(380.0);
    trigger1.npcOrientation.Insert(0.0);
    trigger1.npcOrientation.Insert(0.0);
    trigger1.npcLoadout.Insert("FlatCap_BrownCheck");
    trigger1.npcLoadout.Insert("HuntingJacket_Brown");
    trigger1.npcLoadout.Insert("Jeans_Blue");
    trigger1.npcLoadout.Insert("WorkingGloves_Brown");
    trigger1.npcLoadout.Insert("HikingBootsLow_Black");
    trigger1.npcHandsItem = "FarmingHoe";
    trigger1.npcBackItems.Insert("HuntingBag");
    trigger1.npcBackItems.Insert("Izh43Shotgun");
    config.triggers.Insert(trigger1);

    Silver77_QuestTriggerConfig trigger2 = new Silver77_QuestTriggerConfig();
    trigger2.id = "fisherman_trigger";
    trigger2.position.Insert(13091.6630859375);
    trigger2.position.Insert(116.75563049316406);
    trigger2.position.Insert(13088.6376953125);
    trigger2.radius = 2.0;
    trigger2.focusHeight = 1.2000000476837158;
    trigger2.focusRadius = 1.0;
    trigger2.questIds.Insert("quest_fisherman_1");
    trigger2.questIds.Insert("quest_fisherman_2");
    trigger2.hintText = "[F] Рыбак Гаврила";
    trigger2.spawnNpc = true;
    trigger2.npcClassName = "SurvivorM_Boris";
    trigger2.npcPosition.Insert(13091.6630859375);
    trigger2.npcPosition.Insert(116.75563049316406);
    trigger2.npcPosition.Insert(13088.6376953125);
    trigger2.npcOrientation.Insert(75.0);
    trigger2.npcOrientation.Insert(0.0);
    trigger2.npcOrientation.Insert(0.0);
    trigger2.npcLoadout.Insert("BeanieHat_Green");
    trigger2.npcLoadout.Insert("Raincoat_Green");
    trigger2.npcLoadout.Insert("HunterPants_Summer");
    trigger2.npcLoadout.Insert("WorkingGloves_Black");
    trigger2.npcLoadout.Insert("Wellies_Green");
    trigger2.npcHandsItem = "FishingRod";
    trigger2.npcBackItems.Insert("DryBag_Green");
    trigger2.npcBackItems.Insert("Izh43Shotgun");
    config.triggers.Insert(trigger2);

    Silver77_QuestTriggerConfig trigger3 = new Silver77_QuestTriggerConfig();
    trigger3.id = "Rasputin_1_trigger";
    trigger3.position.Insert(12979.4);
    trigger3.position.Insert(212.68);
    trigger3.position.Insert(12761.7);
    trigger3.radius = 2.0;
    trigger3.focusHeight = 1.2;
    trigger3.focusRadius = 1.0;
    trigger3.questIds.Insert("quest_Rasputin_1");
    trigger3.questIds.Insert("quest_hunter_2");
    trigger3.hintText = "[F] Федор (Распутин)";
    trigger3.spawnNpc = true;
    trigger3.npcClassName = "SurvivorM_Oliver";
    trigger3.npcPosition.Insert(12979.4);
    trigger3.npcPosition.Insert(212.68);
    trigger3.npcPosition.Insert(12761.7);
    trigger3.npcOrientation.Insert(130.0);
    trigger3.npcOrientation.Insert(0.0);
    trigger3.npcOrientation.Insert(0.0);
    trigger3.npcLoadout.Insert("FS_ForestOldCap_ColorBase");
    trigger3.npcLoadout.Insert("FS_Forest_Jacket");
    trigger3.npcLoadout.Insert("FS_RusVKPO_pants");
    trigger3.npcLoadout.Insert("WorkingGloves_Beige");
    trigger3.npcLoadout.Insert("CombatBoots_Grey");
    trigger3.npcHandsItem = "VEPR12u";
    trigger3.npcBackItems.Insert("FS_Eco_Orange_Bag");
    config.triggers.Insert(trigger3);

    SeedDefaultQuestRoles(config);
    return config;
}

void Silver77_ServerAppendUniqueTriggerId(array<string> target, string triggerId)
{
    if (!target || triggerId == "" || target.Find(triggerId) != -1)
        return;

    target.Insert(triggerId);
}

void Silver77_ServerAppendUniqueTriggerIds(array<string> source, array<string> target)
{
    if (!source || !target)
        return;

    foreach (string triggerId : source)
    {
        Silver77_ServerAppendUniqueTriggerId(target, triggerId);
    }
}

void Silver77_ServerNormalizeSingleTriggerRoleIds(array<string> triggerIds)
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

void SeedDefaultQuestRoles(Silver77_QuestConfig config)
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

            if (trigger.questIds.Find(quest.id) != -1)
                Silver77_ServerAppendUniqueTriggerId(assignedTriggerIds, trigger.id);
        }

        Silver77_ServerNormalizeSingleTriggerRoleIds(quest.offerTriggerIds);
        Silver77_ServerNormalizeSingleTriggerRoleIds(quest.rewardTriggerIds);

        if (quest.offerTriggerIds.Count() == 0 && assignedTriggerIds.Count() > 0)
            Silver77_ServerAppendUniqueTriggerId(quest.offerTriggerIds, assignedTriggerIds.Get(0));

        if (quest.rewardTriggerIds.Count() == 0 && assignedTriggerIds.Count() > 0)
            Silver77_ServerAppendUniqueTriggerId(quest.rewardTriggerIds, assignedTriggerIds.Get(0));
    }
}

void Silver77_NormalizeNpcItem(Silver77_NpcItem item)
{
    if (!item)
        return;

    if (item.quantity <= 0)
        item.quantity = 1;
}

void Silver77_NormalizeNpcContainer(Silver77_NpcContainer container)
{
    if (!container)
        return;

    if (!container.items)
        container.items = new array<ref Silver77_NpcItem>;

    foreach (Silver77_NpcItem item : container.items)
    {
        Silver77_NormalizeNpcItem(item);
    }
}

void Silver77_NormalizeNpcWeapon(Silver77_NpcWeapon weapon)
{
    if (!weapon)
        return;

    if (!weapon.attachments)
        weapon.attachments = new array<ref Silver77_NpcItem>;

    if (!weapon.magazine)
        weapon.magazine = new Silver77_NpcMagazine();

    if (!weapon.ammo)
        weapon.ammo = new array<ref Silver77_NpcItem>;

    foreach (Silver77_NpcItem attachment : weapon.attachments)
    {
        Silver77_NormalizeNpcItem(attachment);
    }

    foreach (Silver77_NpcItem ammoItem : weapon.ammo)
    {
        Silver77_NormalizeNpcItem(ammoItem);
    }
}

void Silver77_NormalizeNpcEquipment(Silver77_NpcEquipment equipment)
{
    if (!equipment)
        return;

    if (!equipment.clothing)
        equipment.clothing = new array<ref Silver77_NpcItem>;

    if (!equipment.containers)
        equipment.containers = new array<ref Silver77_NpcContainer>;

    if (!equipment.hands)
        equipment.hands = new Silver77_NpcItem();

    if (!equipment.backItems)
        equipment.backItems = new array<ref Silver77_NpcItem>;

    if (!equipment.weapons)
        equipment.weapons = new array<ref Silver77_NpcWeapon>;

    foreach (Silver77_NpcItem clothingItem : equipment.clothing)
    {
        Silver77_NormalizeNpcItem(clothingItem);
    }

    foreach (Silver77_NpcContainer container : equipment.containers)
    {
        Silver77_NormalizeNpcContainer(container);
    }

    Silver77_NormalizeNpcItem(equipment.hands);

    foreach (Silver77_NpcItem backItem : equipment.backItems)
    {
        Silver77_NormalizeNpcItem(backItem);
    }

    foreach (Silver77_NpcWeapon weapon : equipment.weapons)
    {
        Silver77_NormalizeNpcWeapon(weapon);
    }
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

        Silver77_ServerNormalizeSingleTriggerRoleIds(quest.offerTriggerIds);
        Silver77_ServerNormalizeSingleTriggerRoleIds(quest.rewardTriggerIds);

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

        if (!trigger.npcEquipment)
            trigger.npcEquipment = new Silver77_NpcEquipment();

        Silver77_NormalizeNpcEquipment(trigger.npcEquipment);

        if (trigger.radius <= 0)
            trigger.radius = 2.0;

        if (trigger.focusHeight <= 0)
            trigger.focusHeight = 1.2;

        if (trigger.focusRadius <= 0)
            trigger.focusRadius = 1.0;
    }

    SeedDefaultQuestRoles(config);
}

Silver77_QuestTriggerConfig Silver77_FindTriggerInConfig(Silver77_QuestConfig config, string triggerId)
{
    if (!config || !config.triggers)
        return null;

    foreach (Silver77_QuestTriggerConfig trigger : config.triggers)
    {
        if (!trigger)
            continue;

        if (trigger.id == triggerId)
            return trigger;
    }

    return null;
}

void Silver77_CopyFloatArray(array<float> source, array<float> target)
{
    if (!source || !target)
        return;

    target.Clear();
    foreach (float value : source)
    {
        target.Insert(value);
    }
}

void Silver77_CopyStringArray(array<string> source, array<string> target)
{
    if (!source || !target)
        return;

    target.Clear();
    foreach (string value : source)
    {
        target.Insert(value);
    }
}

void Silver77_ApplyDefaultTriggerNpcSettings(Silver77_QuestTriggerConfig trigger, Silver77_QuestTriggerConfig defaultTrigger)
{
    if (!trigger || !defaultTrigger)
        return;

    trigger.spawnNpc = defaultTrigger.spawnNpc;
    trigger.npcClassName = defaultTrigger.npcClassName;
    Silver77_CopyFloatArray(defaultTrigger.npcPosition, trigger.npcPosition);
    Silver77_CopyFloatArray(defaultTrigger.npcOrientation, trigger.npcOrientation);
    Silver77_CopyStringArray(defaultTrigger.npcLoadout, trigger.npcLoadout);
    trigger.npcHandsItem = defaultTrigger.npcHandsItem;
    Silver77_CopyStringArray(defaultTrigger.npcBackItems, trigger.npcBackItems);
}

bool Silver77_MigrateQuestConfig(Silver77_QuestConfig config)
{
    if (!config)
        return false;

    if (config.version >= SILVER77_QUEST_CONFIG_VERSION)
        return false;

    Silver77_QuestConfig defaultConfig = CreateDefaultQuestConfig();
    NormalizeQuestConfig(defaultConfig);

    Silver77_ApplyDefaultTriggerNpcSettings(Silver77_FindTriggerInConfig(config, "fisherman_trigger"), Silver77_FindTriggerInConfig(defaultConfig, "fisherman_trigger"));
    Silver77_ApplyDefaultTriggerNpcSettings(Silver77_FindTriggerInConfig(config, "hunter_trigger"), Silver77_FindTriggerInConfig(defaultConfig, "hunter_trigger"));

    config.version = SILVER77_QUEST_CONFIG_VERSION;
    Print("[Silver_77_Quests] Migrated quest config to version " + SILVER77_QUEST_CONFIG_VERSION);
    return true;
}

void Silver77_EnsureQuestConfigDirectory()
{
    if (!FileExist(SILVER77_QUEST_CONFIG_DIR))
    {
        MakeDirectory(SILVER77_QUEST_CONFIG_DIR);
        Print("[Silver_77_Quests] Created config directory: " + SILVER77_QUEST_CONFIG_DIR);
    }
}

bool Silver77_SaveQuestConfigFile(string configPath, Silver77_QuestConfig config)
{
    if (!config)
        return false;

    NormalizeQuestConfig(config);
    Silver77_EnsureQuestConfigDirectory();
    Print("[Silver_77_Quests] Saving quest config to: " + configPath);
    JsonFileLoader<Silver77_QuestConfig>.JsonSaveFile(configPath, config);

    if (FileExist(configPath))
    {
        Print("[Silver_77_Quests] Quest config saved via JsonSaveFile: " + configPath);
        return true;
    }

    Print("[Silver_77_Quests] WARNING: JsonSaveFile did not create quest config, trying serializer fallback...");

    string fileContent;
    JsonSerializer serializer = new JsonSerializer();
    if (!serializer.WriteToString(config, true, fileContent))
    {
        Print("[Silver_77_Quests] ERROR: Failed to serialize quest config in fallback writer");
        return false;
    }

    if (fileContent == "")
    {
        Print("[Silver_77_Quests] ERROR: Quest config serializer fallback returned empty content");
        return false;
    }

    FileHandle handle = OpenFile(configPath, FileMode.WRITE);
    if (handle == 0)
    {
        Print("[Silver_77_Quests] ERROR: OpenFile WRITE failed for quest config in fallback writer: " + configPath);
        return false;
    }

    FPrint(handle, fileContent);
    CloseFile(handle);

    if (!FileExist(configPath))
    {
        Print("[Silver_77_Quests] ERROR: Failed to save quest config at: " + configPath);
        return false;
    }

    Print("[Silver_77_Quests] Quest config saved via serializer fallback: " + configPath);
    return true;
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

    string configPath = SILVER77_QUEST_CONFIG_PATH;
    Print("[Silver_77_Quests] Loading quest config from: " + configPath);
    Silver77_EnsureQuestConfigDirectory();

    if (!FileExist(configPath))
    {
        Print("[Silver_77_Quests] Config not found, creating default...");
        g_ServerQuestConfig = CreateDefaultQuestConfig();
        NormalizeQuestConfig(g_ServerQuestConfig);
        if (Silver77_SaveQuestConfigFile(configPath, g_ServerQuestConfig))
            Print("[Silver_77_Quests] Config created at: " + configPath);
    }
    else
    {
        g_ServerQuestConfig = new Silver77_QuestConfig();
        JsonFileLoader<Silver77_QuestConfig>.JsonLoadFile(configPath, g_ServerQuestConfig);
        NormalizeQuestConfig(g_ServerQuestConfig);
        if (Silver77_MigrateQuestConfig(g_ServerQuestConfig))
        {
            NormalizeQuestConfig(g_ServerQuestConfig);
            if (Silver77_SaveQuestConfigFile(configPath, g_ServerQuestConfig))
                Print("[Silver_77_Quests] Config migrated and saved: " + configPath);
        }
        Print("[Silver_77_Quests] Loaded " + g_ServerQuestConfig.quests.Count() + " quests and " + g_ServerQuestConfig.triggers.Count() + " triggers");
    }
}

class QuestServerManager
{
    static void EnsureServerQuestConfigLoaded()
    {
        if (!GetGame().IsServer())
            return;

        if (!g_ServerQuestConfig)
        {
            Print("[Silver_77_Quests] Quest config was not initialized yet, loading now");
            LoadQuestConfig();
            SpawnConfiguredNpcs();
            return;
        }

        if (!FileExist(SILVER77_QUEST_CONFIG_PATH))
        {
            Print("[Silver_77_Quests] Quest config object exists, but file is missing. Saving default/current config...");
            Silver77_SaveQuestConfigFile(SILVER77_QUEST_CONFIG_PATH, g_ServerQuestConfig);
        }
    }

    static void EnsureQuestNpcsSpawned()
    {
        if (!GetGame().IsServer())
            return;

        EnsureServerQuestConfigLoaded();

        if (!g_ServerQuestNpcObjects || g_ServerQuestNpcObjects.Count() == 0)
        {
            Print("[Silver_77_Quests] Quest NPC cache is empty, spawning configured NPCs now");
            SpawnConfiguredNpcs();
        }
    }

    static void Init()
    {
        LoadQuestConfig();
        g_ServerPlayerQuestData = new map<string, ref PlayerQuestData>;
        EnsureQuestNpcsSpawned();
        g_ServerQuestConfigRevision++;
        g_ServerQuestDataRevision++;
        Print("[Silver_77_Quests] QuestServerManager initialized");
    }

    static void CleanupQuestNpcs()
    {
        if (!g_ServerQuestNpcObjects)
        {
            g_ServerQuestNpcObjects = new array<Object>;
            return;
        }

        foreach (Object npcObject : g_ServerQuestNpcObjects)
        {
            if (npcObject)
                GetGame().ObjectDelete(npcObject);
        }

        g_ServerQuestNpcObjects.Clear();
    }

    static void SpawnConfiguredNpcs()
    {
        if (!GetGame().IsServer())
            return;

        CleanupQuestNpcs();

        if (!g_ServerQuestConfig || !g_ServerQuestConfig.triggers)
            return;

        foreach (Silver77_QuestTriggerConfig trigger : g_ServerQuestConfig.triggers)
        {
            SpawnNpcForTrigger(trigger);
        }
    }

    static void SpawnNpcForTrigger(Silver77_QuestTriggerConfig trigger)
    {
        if (!trigger || !trigger.spawnNpc)
            return;

        if (trigger.npcClassName == "")
        {
            Print("[Silver_77_Quests] NPC class is empty for trigger: " + trigger.id);
            return;
        }

        if (!Silver77_HasVector(trigger.position) && !Silver77_HasVector(trigger.npcPosition))
        {
            Print("[Silver_77_Quests] NPC position is invalid for trigger: " + trigger.id);
            return;
        }

        vector triggerPosition = Silver77_VectorFromArray(trigger.position, vector.Zero);
        vector npcPosition = Silver77_VectorFromArray(trigger.npcPosition, triggerPosition);
        vector npcOrientation = Silver77_VectorFromArray(trigger.npcOrientation, vector.Zero);

        Object npcObject = GetGame().CreateObjectEx(trigger.npcClassName, npcPosition, ECE_PLACE_ON_SURFACE);
        if (!npcObject)
        {
            Print("[Silver_77_Quests] Failed to spawn NPC " + trigger.npcClassName + " for trigger: " + trigger.id);
            return;
        }

        g_ServerQuestNpcObjects.Insert(npcObject);
        npcObject.SetPosition(npcPosition);
        npcObject.SetOrientation(npcOrientation);

        EntityAI npcEntity = EntityAI.Cast(npcObject);
        if (npcEntity)
            npcEntity.SetAllowDamage(false);

        PlayerBase npcPlayer = PlayerBase.Cast(npcObject);
        if (npcPlayer)
        {
            ApplyNpcLoadout(npcPlayer, trigger);
            npcPlayer.PlaceOnSurface();
        }
        else
        {
            Print("[Silver_77_Quests] Spawned NPC object is not PlayerBase: " + trigger.npcClassName);
        }

        Print("[Silver_77_Quests] Spawned quest NPC " + trigger.npcClassName + " for trigger " + trigger.id + " at " + npcPosition.ToString());
    }

    static bool Silver77_HasVector(array<float> values)
    {
        if (!values)
            return false;

        return values.Count() >= 3;
    }

    static vector Silver77_VectorFromArray(array<float> values, vector fallback)
    {
        if (Silver77_HasVector(values))
            return Vector(values[0], values[1], values[2]);

        return fallback;
    }

    static void ApplyNpcLoadout(PlayerBase npcPlayer, Silver77_QuestTriggerConfig trigger)
    {
        if (!npcPlayer || !trigger)
            return;

        if (HasNpcEquipment(trigger.npcEquipment))
        {
            ApplyNpcEquipment(npcPlayer, trigger);
            return;
        }

        Print("[Silver_77_Quests] NPC equipment is empty, using legacy NPC loadout for trigger: " + trigger.id);
        ApplyLegacyNpcLoadout(npcPlayer, trigger);
    }

    static void ApplyLegacyNpcLoadout(PlayerBase npcPlayer, Silver77_QuestTriggerConfig trigger)
    {
        if (!npcPlayer || !trigger)
            return;

        if (trigger.npcLoadout)
        {
            foreach (string loadoutItem : trigger.npcLoadout)
            {
                AddNpcAttachment(npcPlayer, loadoutItem, trigger.id);
            }
        }

        if (trigger.npcBackItems)
        {
            foreach (string backItem : trigger.npcBackItems)
            {
                AddNpcAttachment(npcPlayer, backItem, trigger.id);
            }
        }

        if (trigger.npcHandsItem != "")
        {
            EntityAI handsItem = npcPlayer.GetHumanInventory().CreateInHands(trigger.npcHandsItem);
            if (!handsItem)
                Print("[Silver_77_Quests] Failed to put NPC item in hands: " + trigger.npcHandsItem + " for trigger: " + trigger.id);
        }
    }

    static bool HasNpcItem(Silver77_NpcItem item)
    {
        if (!item)
            return false;

        return item.className != "";
    }

    static bool HasNpcItemArray(array<ref Silver77_NpcItem> items)
    {
        if (!items)
            return false;

        foreach (Silver77_NpcItem item : items)
        {
            if (HasNpcItem(item))
                return true;
        }

        return false;
    }

    static bool HasNpcContainerArray(array<ref Silver77_NpcContainer> containers)
    {
        if (!containers)
            return false;

        foreach (Silver77_NpcContainer container : containers)
        {
            if (!container)
                continue;

            if (container.className != "" || HasNpcItemArray(container.items))
                return true;
        }

        return false;
    }

    static bool HasNpcWeaponArray(array<ref Silver77_NpcWeapon> weapons)
    {
        if (!weapons)
            return false;

        foreach (Silver77_NpcWeapon weapon : weapons)
        {
            if (!weapon)
                continue;

            if (weapon.className != "")
                return true;

            if (HasNpcItemArray(weapon.attachments) || HasNpcItemArray(weapon.ammo))
                return true;

            if (weapon.magazine && weapon.magazine.className != "")
                return true;
        }

        return false;
    }

    static bool HasNpcEquipment(Silver77_NpcEquipment equipment)
    {
        if (!equipment)
            return false;

        return HasNpcItemArray(equipment.clothing) || HasNpcContainerArray(equipment.containers) || HasNpcItem(equipment.hands) || HasNpcItemArray(equipment.backItems) || HasNpcWeaponArray(equipment.weapons);
    }

    static void ApplyNpcEquipment(PlayerBase npcPlayer, Silver77_QuestTriggerConfig trigger)
    {
        if (!npcPlayer || !trigger || !trigger.npcEquipment)
            return;

        Silver77_NpcEquipment equipment = trigger.npcEquipment;

        if (equipment.clothing)
        {
            foreach (Silver77_NpcItem clothingItem : equipment.clothing)
            {
                CreateNpcAttachmentCopies(npcPlayer, clothingItem, trigger.id, "clothing");
            }
        }

        if (equipment.containers)
        {
            foreach (Silver77_NpcContainer container : equipment.containers)
            {
                ApplyNpcContainer(npcPlayer, container, trigger.id);
            }
        }

        if (HasNpcItem(equipment.hands))
            CreateNpcHandsItem(npcPlayer, equipment.hands, trigger.id, "hands");

        if (equipment.backItems)
        {
            foreach (Silver77_NpcItem backItem : equipment.backItems)
            {
                CreateNpcAttachmentCopies(npcPlayer, backItem, trigger.id, "backItems");
            }
        }

        if (equipment.weapons)
        {
            foreach (Silver77_NpcWeapon weapon : equipment.weapons)
            {
                ApplyNpcWeapon(npcPlayer, weapon, trigger.id);
            }
        }

        Print("[Silver_77_Quests] Applied npcEquipment for trigger: " + trigger.id);
    }

    static int GetNpcItemQuantity(Silver77_NpcItem item)
    {
        if (!item || item.quantity <= 0)
            return 1;

        return item.quantity;
    }

    static bool ApplyNpcItemQuantity(EntityAI createdItem, Silver77_NpcItem item, string triggerId, string context)
    {
        if (!createdItem || !item || !item.setItemQuantity)
            return true;

        string itemQuantityError = "";
        string itemQuantityMode = "";
        float actualItemQuantity = 0;
        if (!ApplyRewardItemQuantitySafe(createdItem, item.className, item.itemQuantity, itemQuantityError, itemQuantityMode, actualItemQuantity))
        {
            createdItem.Delete();
            Print("[Silver_77_Quests] NPC itemQuantity failed: trigger=" + triggerId + " context=" + context + " item=" + item.className + " error=" + itemQuantityError);
            return false;
        }

        Print("[Silver_77_Quests] NPC itemQuantity applied: trigger=" + triggerId + " context=" + context + " item=" + item.className + " mode=" + itemQuantityMode + " requested=" + item.itemQuantity.ToString() + " actual=" + actualItemQuantity.ToString());
        return true;
    }

    static EntityAI CreateNpcAttachment(EntityAI parent, string className, string slot, string triggerId, string context)
    {
        if (!parent || className == "")
            return null;

        EntityAI attachment = null;
        if (slot != "")
        {
            int slotId = InventorySlots.GetSlotIdFromString(slot);
            if (slotId >= 0)
                attachment = parent.GetInventory().CreateAttachmentEx(className, slotId);

            if (!attachment)
                Print("[Silver_77_Quests] NPC attachment slot failed: trigger=" + triggerId + " context=" + context + " item=" + className + " slot=" + slot);
        }

        if (!attachment)
            attachment = parent.GetInventory().CreateAttachment(className);

        if (!attachment)
            Print("[Silver_77_Quests] Failed to attach NPC item: trigger=" + triggerId + " context=" + context + " item=" + className + " slot=" + slot);

        return attachment;
    }

    static void CreateNpcAttachmentCopies(EntityAI parent, Silver77_NpcItem item, string triggerId, string context)
    {
        if (!parent || !HasNpcItem(item))
            return;

        int quantity = GetNpcItemQuantity(item);
        for (int i = 0; i < quantity; i++)
        {
            EntityAI attachment = CreateNpcAttachment(parent, item.className, item.slot, triggerId, context);
            if (attachment)
                ApplyNpcItemQuantity(attachment, item, triggerId, context);
        }
    }

    static EntityAI CreateNpcInventoryItem(EntityAI owner, PlayerBase npcPlayer, Silver77_NpcItem item, string triggerId, string context, bool allowPlayerFallback = true)
    {
        if (!owner || !HasNpcItem(item))
            return null;

        EntityAI createdItem = owner.GetInventory().CreateInInventory(item.className);
        if (!createdItem && allowPlayerFallback && npcPlayer)
        {
            createdItem = npcPlayer.GetInventory().CreateInInventory(item.className);
            if (createdItem)
                Print("[Silver_77_Quests] NPC item fallback to player inventory: trigger=" + triggerId + " context=" + context + " item=" + item.className);
        }

        if (!createdItem)
        {
            Print("[Silver_77_Quests] Failed to create NPC inventory item: trigger=" + triggerId + " context=" + context + " item=" + item.className);
            return null;
        }

        if (!ApplyNpcItemQuantity(createdItem, item, triggerId, context))
            return null;

        return createdItem;
    }

    static void CreateNpcInventoryItemCopies(EntityAI owner, PlayerBase npcPlayer, Silver77_NpcItem item, string triggerId, string context, bool allowPlayerFallback = true)
    {
        if (!owner || !HasNpcItem(item))
            return;

        int quantity = GetNpcItemQuantity(item);
        for (int i = 0; i < quantity; i++)
        {
            CreateNpcInventoryItem(owner, npcPlayer, item, triggerId, context, allowPlayerFallback);
        }
    }

    static void ApplyNpcContainer(PlayerBase npcPlayer, Silver77_NpcContainer container, string triggerId)
    {
        if (!npcPlayer || !container)
            return;

        if (container.className == "")
        {
            Print("[Silver_77_Quests] NPC container class is empty for trigger: " + triggerId);
            return;
        }

        EntityAI containerEntity = CreateNpcAttachment(npcPlayer, container.className, container.slot, triggerId, "container");
        if (!containerEntity)
            return;

        if (container.items)
        {
            foreach (Silver77_NpcItem item : container.items)
            {
                CreateNpcInventoryItemCopies(containerEntity, npcPlayer, item, triggerId, "container.items", true);
            }
        }
    }

    static EntityAI CreateNpcHandsItem(PlayerBase npcPlayer, Silver77_NpcItem item, string triggerId, string context)
    {
        if (!npcPlayer || !HasNpcItem(item))
            return null;

        EntityAI itemInHands = npcPlayer.GetHumanInventory().GetEntityInHands();
        if (itemInHands)
        {
            Print("[Silver_77_Quests] NPC hands already occupied: trigger=" + triggerId + " context=" + context + " item=" + item.className);
            return null;
        }

        EntityAI handsItem = npcPlayer.GetHumanInventory().CreateInHands(item.className);
        if (!handsItem)
        {
            Print("[Silver_77_Quests] Failed to put NPC item in hands: trigger=" + triggerId + " context=" + context + " item=" + item.className);
            return null;
        }

        if (!ApplyNpcItemQuantity(handsItem, item, triggerId, context))
            return null;

        int quantity = GetNpcItemQuantity(item);
        for (int i = 1; i < quantity; i++)
        {
            CreateNpcInventoryItem(npcPlayer, npcPlayer, item, triggerId, context + ".extra", false);
        }

        return handsItem;
    }

    static void ApplyNpcWeapon(PlayerBase npcPlayer, Silver77_NpcWeapon weapon, string triggerId)
    {
        if (!npcPlayer || !weapon)
            return;

        if (weapon.className == "")
        {
            Print("[Silver_77_Quests] NPC weapon class is empty for trigger: " + triggerId);
            return;
        }

        EntityAI weaponEntity = CreateNpcWeaponEntity(npcPlayer, weapon, triggerId);
        if (!weaponEntity)
            return;

        if (weapon.attachments)
        {
            foreach (Silver77_NpcItem attachment : weapon.attachments)
            {
                CreateNpcAttachmentCopies(weaponEntity, attachment, triggerId, "weapon.attachments");
            }
        }

        if (weapon.magazine && weapon.magazine.className != "")
            ApplyNpcWeaponMagazine(weaponEntity, weapon.magazine, triggerId);

        if (weapon.ammo)
        {
            foreach (Silver77_NpcItem ammoItem : weapon.ammo)
            {
                CreateNpcInventoryItemCopies(npcPlayer, npcPlayer, ammoItem, triggerId, "weapon.ammo", false);
            }
        }
    }

    static EntityAI CreateNpcWeaponEntity(PlayerBase npcPlayer, Silver77_NpcWeapon weapon, string triggerId)
    {
        if (!npcPlayer || !weapon || weapon.className == "")
            return null;

        string target = weapon.target;
        target.ToLower();

        EntityAI weaponEntity = null;
        if (target == "hands")
        {
            EntityAI itemInHands = npcPlayer.GetHumanInventory().GetEntityInHands();
            if (itemInHands)
            {
                Print("[Silver_77_Quests] NPC hands already occupied for weapon: trigger=" + triggerId + " item=" + weapon.className);
                return null;
            }

            weaponEntity = npcPlayer.GetHumanInventory().CreateInHands(weapon.className);
            if (!weaponEntity)
                Print("[Silver_77_Quests] Failed to put NPC weapon in hands: trigger=" + triggerId + " item=" + weapon.className);
        }
        else if (target == "back" || target == "shoulder")
        {
            weaponEntity = CreateNpcAttachment(npcPlayer, weapon.className, "", triggerId, "weapon." + target);
        }
        else
        {
            weaponEntity = CreateNpcAttachment(npcPlayer, weapon.className, "", triggerId, "weapon");
            if (!weaponEntity)
            {
                Silver77_NpcItem weaponItem = new Silver77_NpcItem();
                weaponItem.className = weapon.className;
                weaponItem.quantity = 1;
                weaponEntity = CreateNpcInventoryItem(npcPlayer, npcPlayer, weaponItem, triggerId, "weapon.inventory", false);
            }
        }

        return weaponEntity;
    }

    static void ApplyNpcWeaponMagazine(EntityAI weaponEntity, Silver77_NpcMagazine magazineConfig, string triggerId)
    {
        if (!weaponEntity || !magazineConfig || magazineConfig.className == "")
            return;

        EntityAI magazineEntity = CreateNpcAttachment(weaponEntity, magazineConfig.className, "", triggerId, "weapon.magazine");
        if (!magazineEntity)
            return;

        if (magazineConfig.ammoCount <= 0)
            return;

        Magazine magazine = Magazine.Cast(magazineEntity);
        if (!magazine)
        {
            Print("[Silver_77_Quests] NPC magazine item is not Magazine: trigger=" + triggerId + " item=" + magazineConfig.className);
            return;
        }

        int ammoMax = magazine.GetAmmoMax();
        if (ammoMax > 0 && magazineConfig.ammoCount > ammoMax)
        {
            Print("[Silver_77_Quests] NPC magazine ammoCount exceeds max: trigger=" + triggerId + " item=" + magazineConfig.className + " requested=" + magazineConfig.ammoCount.ToString() + " max=" + ammoMax.ToString());
            return;
        }

        magazine.ServerSetAmmoCount(magazineConfig.ammoCount);
        int actualAmmoCount = magazine.GetAmmoCount();
        if (actualAmmoCount != magazineConfig.ammoCount)
            Print("[Silver_77_Quests] NPC magazine ammoCount mismatch: trigger=" + triggerId + " item=" + magazineConfig.className + " requested=" + magazineConfig.ammoCount.ToString() + " actual=" + actualAmmoCount.ToString());
        else
            Print("[Silver_77_Quests] NPC magazine ammoCount applied: trigger=" + triggerId + " item=" + magazineConfig.className + " ammoCount=" + actualAmmoCount.ToString());
    }

    static void AddNpcAttachment(PlayerBase npcPlayer, string className, string triggerId)
    {
        if (!npcPlayer || className == "")
            return;

        EntityAI attachment = npcPlayer.GetInventory().CreateAttachment(className);
        if (!attachment)
            Print("[Silver_77_Quests] Failed to attach NPC item: " + className + " for trigger: " + triggerId);
    }

    static Silver77_Quest GetQuestById(string questId)
    {
        EnsureServerQuestConfigLoaded();

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
        EnsureServerQuestConfigLoaded();

        string steamId = GetPlayerSteamId(player);
        if (steamId == "")
            return null;

        if (!g_ServerPlayerQuestData)
            g_ServerPlayerQuestData = new map<string, ref PlayerQuestData>;

        if (!g_ServerPlayerQuestData.Contains(steamId))
        {
            Print("[Silver_77_Quests][PROGRESS_DEBUG] GetPlayerData: steamId not in memory, calling LoadPlayerData for " + steamId);
            PlayerQuestData data = null;
            if (GetGame().IsServer())
                data = LoadPlayerData(steamId);
            else
                data = CreatePlayerData(steamId);

            g_ServerPlayerQuestData.Insert(steamId, data);
        }
        else
        {
            PlayerQuestData existingData = g_ServerPlayerQuestData.Get(steamId);
            int progressCount = 0;
            if (existingData && existingData.progress)
                progressCount = existingData.progress.Count();
            Print("[Silver_77_Quests][PROGRESS_DEBUG] GetPlayerData: steamId found in memory for " + steamId + " with " + progressCount + " progress entries");
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

        Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: steamId=" + steamId);
        Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: filePath=" + filePath);

        bool fileExists = FileExist(filePath);
        Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: FileExist=" + fileExists);

        if (fileExists)
        {
            JsonFileLoader<PlayerQuestData>.JsonLoadFile(filePath, data);

            int loadedProgressCount = 0;
            if (data.progress)
                loadedProgressCount = data.progress.Count();

            Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: JsonLoadFile completed, loaded " + loadedProgressCount + " progress entries");

            if (data.progress)
            {
                foreach (PlayerQuestProgress progress : data.progress)
                {
                    if (progress)
                        Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: loaded questId=" + progress.questId + " status=" + progress.status);
                }
            }

            if (data.steamId == "")
                data.steamId = steamId;

            if (EnsurePlayerProgress(data))
                SavePlayerData(data);

            return data;
        }

        Print("[Silver_77_Quests][PROGRESS_DEBUG] LoadPlayerData: file not found, creating new PlayerQuestData");
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

        int progressCount = 0;
        if (data.progress)
            progressCount = data.progress.Count();

        Print("[Silver_77_Quests][PROGRESS_DEBUG] SavePlayerData: steamId=" + data.steamId + " filePath=" + filePath + " progressCount=" + progressCount);

        if (data.progress)
        {
            foreach (PlayerQuestProgress progress : data.progress)
            {
                if (progress)
                    Print("[Silver_77_Quests][PROGRESS_DEBUG] SavePlayerData: saving questId=" + progress.questId + " status=" + progress.status);
            }
        }

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

                if (existingProgress && !existingProgress.completedCompletionTriggerIds)
                {
                    existingProgress.completedCompletionTriggerIds = new array<string>;
                    changed = true;
                }

                if (existingProgress && !existingProgress.stageVisits)
                {
                    existingProgress.stageVisits = new array<ref PlayerQuestStageVisit>;
                    changed = true;
                }

                if (existingProgress && EnsureRewardProgressState(existingProgress))
                    changed = true;
            }
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

    static Silver77_QuestTriggerConfig GetTriggerById(string triggerId)
    {
        EnsureServerQuestConfigLoaded();

        if (!g_ServerQuestConfig || !g_ServerQuestConfig.triggers || triggerId == "")
            return null;

        foreach (Silver77_QuestTriggerConfig trigger : g_ServerQuestConfig.triggers)
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
        EnsureServerQuestConfigLoaded();

        if (!g_ServerQuestConfig || !g_ServerQuestConfig.triggers || questId == "")
            return triggerIds;

        foreach (Silver77_QuestTriggerConfig trigger : g_ServerQuestConfig.triggers)
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

    static array<ref Silver77_QuestItem> ResolveQuestRewardItemsForTrigger(Silver77_Quest quest, string triggerId, bool rewardPhase)
    {
        if (!quest)
            return null;

        Silver77_QuestTriggerAction action;
        if (rewardPhase)
        {
            action = GetQuestTriggerAction(quest, triggerId, "reward");
        }
        else
        {
            action = GetQuestTriggerAction(quest, triggerId, "completion");
        }

        if (action && action.rewards && action.rewards.Count() > 0)
            return action.rewards;

        if (!rewardPhase || !quest.rewards)
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

    static PlayerQuestProgress GetOrCreateProgress(PlayerQuestData data, string questId)
    {
        PlayerQuestProgress progress = FindProgress(data, questId);
        if (progress)
        {
            EnsureRewardProgressState(progress);
            return progress;
        }

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

    static string BuildRewardId(string questId, int attemptId, string stage, string triggerId, string actionType, int index)
    {
        return questId + ":" + attemptId.ToString() + ":" + stage + ":" + triggerId + ":" + actionType + ":" + index.ToString();
    }

    static QuestPendingRewardProgress FindPendingReward(PlayerQuestProgress progress, string rewardId, int attemptId)
    {
        if (!progress || !progress.pendingRewards || rewardId == "" || attemptId <= 0)
            return null;

        foreach (QuestPendingRewardProgress pendingReward : progress.pendingRewards)
        {
            if (!pendingReward)
                continue;

            if (pendingReward.rewardId == rewardId && pendingReward.attemptId == attemptId)
                return pendingReward;
        }

        return null;
    }

    static bool HasDeliveredReward(PlayerQuestProgress progress, string rewardId, int attemptId)
    {
        if (!progress || !progress.deliveredRewards || rewardId == "" || attemptId <= 0)
            return false;

        foreach (QuestDeliveredRewardProgress deliveredReward : progress.deliveredRewards)
        {
            if (!deliveredReward)
                continue;

            if (deliveredReward.rewardId == rewardId && deliveredReward.attemptId == attemptId)
                return true;
        }

        return false;
    }

    static bool AddPendingReward(PlayerQuestProgress progress, QuestPendingRewardProgress reward)
    {
        if (!progress || !reward || reward.rewardId == "" || reward.attemptId <= 0)
            return false;

        EnsureRewardProgressState(progress);

        if (FindPendingReward(progress, reward.rewardId, reward.attemptId))
            return false;

        if (reward.questId == "")
            reward.questId = progress.questId;

        if (reward.status == "")
            reward.status = "pending";

        if (!reward.items)
            reward.items = new array<ref QuestRewardItemProgress>;

        if (reward.createdAt <= 0)
            reward.createdAt = GetCurrentUnixTimeUTC();

        reward.updatedAt = GetCurrentUnixTimeUTC();
        progress.pendingRewards.Insert(reward);
        return true;
    }

    static bool MarkPendingRewardDelivered(PlayerQuestProgress progress, string rewardId, int attemptId)
    {
        QuestPendingRewardProgress pendingReward = FindPendingReward(progress, rewardId, attemptId);
        if (!pendingReward)
            return false;

        EnsureRewardProgressState(progress);

        pendingReward.status = "delivered";
        pendingReward.updatedAt = GetCurrentUnixTimeUTC();

        if (!HasDeliveredReward(progress, rewardId, attemptId))
        {
            QuestDeliveredRewardProgress deliveredReward = new QuestDeliveredRewardProgress();
            deliveredReward.questId = pendingReward.questId;
            deliveredReward.attemptId = pendingReward.attemptId;
            deliveredReward.rewardId = pendingReward.rewardId;
            deliveredReward.stage = pendingReward.stage;
            deliveredReward.triggerId = pendingReward.triggerId;
            deliveredReward.actionType = pendingReward.actionType;
            deliveredReward.deliveredAt = pendingReward.updatedAt;
            progress.deliveredRewards.Insert(deliveredReward);
        }

        return true;
    }

    static bool HasOpenPendingRewardsForAttempt(PlayerQuestProgress progress, int attemptId)
    {
        if (!progress || !progress.pendingRewards || attemptId <= 0)
            return false;

        foreach (QuestPendingRewardProgress pendingReward : progress.pendingRewards)
        {
            if (!pendingReward)
                continue;

            if (pendingReward.attemptId == attemptId && pendingReward.status != "delivered")
                return true;
        }

        return false;
    }

    static bool IsAttemptFullyFinalized(PlayerQuestProgress progress, int attemptId)
    {
        if (!progress || attemptId <= 0)
            return false;

        if (progress.lastFinalizedAttemptId < attemptId)
            return false;

        return !HasOpenPendingRewardsForAttempt(progress, attemptId);
    }

    static int EnsureCurrentAttemptId(PlayerQuestProgress progress)
    {
        if (!progress)
            return 0;

        EnsureRewardProgressState(progress);

        if (progress.currentAttemptId <= 0 || progress.currentAttemptId <= progress.lastFinalizedAttemptId)
        {
            progress.currentAttemptId = progress.lastFinalizedAttemptId + 1;
            if (progress.currentAttemptId <= 0)
                progress.currentAttemptId = 1;
        }

        return progress.currentAttemptId;
    }

    static void MarkCurrentAttemptFinalized(PlayerQuestProgress progress)
    {
        if (!progress)
            return;

        int attemptId = progress.currentAttemptId;
        if (attemptId <= 0)
            attemptId = EnsureCurrentAttemptId(progress);

        if (attemptId > 0 && progress.lastFinalizedAttemptId < attemptId)
            progress.lastFinalizedAttemptId = attemptId;
    }

    static EntityAI TryCreateRewardItemSafe(PlayerBase player, string className, bool spawnOnGround)
    {
        if (!GetGame().IsServer() || !player || className == "")
            return null;

        EntityAI item = null;

        if (!spawnOnGround)
            item = player.GetInventory().CreateInInventory(className);

        if (!item)
            item = EntityAI.Cast(GetGame().CreateObjectEx(className, player.GetPosition(), ECE_PLACE_ON_SURFACE | ECE_KEEPHEIGHT));

        return item;
    }

    static bool ApplyRewardItemQuantitySafe(EntityAI item, string className, float requestedQuantity, out string errorText, out string quantityMode, out float actualQuantity)
    {
        errorText = "";
        quantityMode = "";
        actualQuantity = 0;

        if (!item)
        {
            errorText = "Created item is null for " + className;
            return false;
        }

        if (requestedQuantity <= 0)
        {
            errorText = "itemQuantity must be > 0 for " + className;
            return false;
        }

        Magazine magazine = Magazine.Cast(item);
        if (magazine)
        {
            quantityMode = "magazine";
            int requestedAmmoCount = Math.Round(requestedQuantity);
            float requestedAmmoAsFloat = requestedAmmoCount;
            if (Math.AbsFloat(requestedQuantity - requestedAmmoAsFloat) > 0.01)
            {
                errorText = "Magazine itemQuantity must be a whole number for " + className + " requested=" + requestedQuantity.ToString();
                return false;
            }

            if (requestedAmmoCount <= 0)
            {
                errorText = "Magazine itemQuantity must round to > 0 for " + className + " requested=" + requestedQuantity.ToString();
                return false;
            }

            int ammoMax = magazine.GetAmmoMax();
            if (ammoMax > 0 && requestedAmmoCount > ammoMax)
            {
                errorText = "Magazine itemQuantity exceeds ammo max for " + className + " requested=" + requestedAmmoCount.ToString() + " max=" + ammoMax.ToString();
                return false;
            }

            magazine.ServerSetAmmoCount(requestedAmmoCount);
            actualQuantity = magazine.GetAmmoCount();
            if (actualQuantity != requestedAmmoAsFloat)
            {
                errorText = "Magazine itemQuantity mismatch for " + className + " requested=" + requestedAmmoCount.ToString() + " actual=" + actualQuantity.ToString();
                return false;
            }

            return true;
        }

        if (!item.HasQuantity())
        {
            errorText = "Item does not support quantity for " + className;
            return false;
        }

        quantityMode = "quantity";
        item.SetQuantity(requestedQuantity);
        actualQuantity = item.GetQuantity();
        if (Math.AbsFloat(actualQuantity - requestedQuantity) > 0.01)
        {
            errorText = "Item quantity mismatch for " + className + " requested=" + requestedQuantity.ToString() + " actual=" + actualQuantity.ToString();
            return false;
        }

        return true;
    }

    static QuestPendingRewardProgress CreatePendingRewardBatch(PlayerQuestProgress progress, string rewardId, int attemptId, string stage, string triggerId, string actionType, array<ref Silver77_QuestItem> rewardItems)
    {
        if (!progress || rewardId == "" || attemptId <= 0)
            return null;

        QuestPendingRewardProgress pendingReward = new QuestPendingRewardProgress();
        pendingReward.questId = progress.questId;
        pendingReward.attemptId = attemptId;
        pendingReward.rewardId = rewardId;
        pendingReward.stage = stage;
        pendingReward.triggerId = triggerId;
        pendingReward.actionType = actionType;
        pendingReward.status = "pending";
        pendingReward.createdAt = GetCurrentUnixTimeUTC();
        pendingReward.updatedAt = pendingReward.createdAt;

        if (rewardItems)
        {
            foreach (Silver77_QuestItem rewardItem : rewardItems)
            {
                if (!rewardItem || rewardItem.className == "" || rewardItem.quantity <= 0)
                    continue;

                QuestRewardItemProgress itemProgress = new QuestRewardItemProgress();
                itemProgress.className = rewardItem.className;
                itemProgress.need = rewardItem.quantity;
                itemProgress.given = 0;
                itemProgress.spawnOnGround = rewardItem.spawnOnGround;
                itemProgress.setItemQuantity = rewardItem.setItemQuantity;
                itemProgress.itemQuantity = rewardItem.itemQuantity;
                pendingReward.items.Insert(itemProgress);
            }
        }

        return pendingReward;
    }

    static bool IsPendingRewardFullyDelivered(QuestPendingRewardProgress pendingReward)
    {
        if (!pendingReward || !pendingReward.items)
            return true;

        foreach (QuestRewardItemProgress itemProgress : pendingReward.items)
        {
            if (!itemProgress)
                continue;

            if (itemProgress.given < itemProgress.need)
                return false;
        }

        return true;
    }

    static bool HasAnyPendingRewardItemGiven(QuestPendingRewardProgress pendingReward)
    {
        if (!pendingReward || !pendingReward.items)
            return false;

        foreach (QuestRewardItemProgress itemProgress : pendingReward.items)
        {
            if (itemProgress && itemProgress.given > 0)
                return true;
        }

        return false;
    }

    static bool TryDeliverPendingRewardItemsSafe(PlayerBase player, PlayerQuestProgress progress, QuestPendingRewardProgress pendingReward)
    {
        if (!player || !progress || !pendingReward)
            return false;

        EnsureRewardProgressState(progress);

        if (!pendingReward.items)
            pendingReward.items = new array<ref QuestRewardItemProgress>;

        int deliveredThisCall = 0;
        int now = GetCurrentUnixTimeUTC();
        pendingReward.lastError = "";

        for (int i = 0; i < pendingReward.items.Count(); i++)
        {
            QuestRewardItemProgress itemProgress = pendingReward.items.Get(i);
            if (!itemProgress || itemProgress.className == "")
                continue;

            int remaining = itemProgress.need - itemProgress.given;
            while (remaining > 0)
            {
                EntityAI createdItem = TryCreateRewardItemSafe(player, itemProgress.className, itemProgress.spawnOnGround);
                if (!createdItem)
                {
                    itemProgress.lastError = "CreateInInventory/CreateObjectEx returned null for " + itemProgress.className;
                    pendingReward.lastError = itemProgress.lastError;
                    pendingReward.updatedAt = now;

                    if (deliveredThisCall > 0 || HasAnyPendingRewardItemGiven(pendingReward))
                        pendingReward.status = "partial";
                    else
                        pendingReward.status = "failed";

                    Print("[Silver_77_Quests] Safe reward delivery failed: questId=" + progress.questId + " rewardId=" + pendingReward.rewardId + " attemptId=" + pendingReward.attemptId.ToString() + " item=" + itemProgress.className);
                    return false;
                }

                if (itemProgress.setItemQuantity)
                {
                    string itemQuantityError = "";
                    string itemQuantityMode = "";
                    float actualItemQuantity = 0;
                    if (!ApplyRewardItemQuantitySafe(createdItem, itemProgress.className, itemProgress.itemQuantity, itemQuantityError, itemQuantityMode, actualItemQuantity))
                    {
                        createdItem.Delete();
                        itemProgress.lastError = itemQuantityError;
                        pendingReward.lastError = itemQuantityError;
                        pendingReward.updatedAt = now;

                        if (deliveredThisCall > 0 || HasAnyPendingRewardItemGiven(pendingReward))
                            pendingReward.status = "partial";
                        else
                            pendingReward.status = "failed";

                        Print("[Silver_77_Quests] Safe reward itemQuantity failed: questId=" + progress.questId + " rewardId=" + pendingReward.rewardId + " attemptId=" + pendingReward.attemptId.ToString() + " item=" + itemProgress.className + " error=" + itemQuantityError);
                        return false;
                    }

                    Print("[Silver_77_Quests] Safe reward itemQuantity applied: questId=" + progress.questId + " rewardId=" + pendingReward.rewardId + " stage=" + pendingReward.stage + " item=" + itemProgress.className + " mode=" + itemQuantityMode + " requested=" + itemProgress.itemQuantity.ToString() + " actual=" + actualItemQuantity.ToString());
                }

                itemProgress.given++;
                itemProgress.lastError = "";
                deliveredThisCall++;
                remaining--;
                pendingReward.updatedAt = GetCurrentUnixTimeUTC();
            }
        }

        if (IsPendingRewardFullyDelivered(pendingReward))
        {
            pendingReward.status = "delivered";
            pendingReward.lastError = "";
            MarkPendingRewardDelivered(progress, pendingReward.rewardId, pendingReward.attemptId);
            return true;
        }

        if (deliveredThisCall > 0)
            pendingReward.status = "partial";
        else if (pendingReward.status == "")
            pendingReward.status = "pending";

        pendingReward.updatedAt = GetCurrentUnixTimeUTC();
        return false;
    }

    static bool TryDeliverQuestItemsBatchSafe(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress, string stage, string triggerId, string actionType, int index, array<ref Silver77_QuestItem> rewardItems)
    {
        if (!player || !quest || !progress)
            return false;

        if (!rewardItems || rewardItems.Count() == 0)
            return true;

        int attemptId = EnsureCurrentAttemptId(progress);
        if (attemptId <= 0)
            return false;

        string rewardId = BuildRewardId(quest.id, attemptId, stage, triggerId, actionType, index);
        if (HasDeliveredReward(progress, rewardId, attemptId))
        {
            Print("[Silver_77_Quests] Safe delivery skipped already delivered reward: questId=" + quest.id + " stage=" + stage + " triggerId=" + triggerId + " rewardId=" + rewardId + " attemptId=" + attemptId.ToString());
            return true;
        }

        QuestPendingRewardProgress pendingReward = FindPendingReward(progress, rewardId, attemptId);
        if (!pendingReward)
        {
            pendingReward = CreatePendingRewardBatch(progress, rewardId, attemptId, stage, triggerId, actionType, rewardItems);
            AddPendingReward(progress, pendingReward);
            pendingReward = FindPendingReward(progress, rewardId, attemptId);
            Print("[Silver_77_Quests] Safe delivery pending reward created: questId=" + quest.id + " stage=" + stage + " triggerId=" + triggerId + " rewardId=" + rewardId + " attemptId=" + attemptId.ToString());
        }
        else if (pendingReward.status != "delivered")
        {
            Print("[Silver_77_Quests] Safe delivery retry pending reward: questId=" + quest.id + " stage=" + stage + " triggerId=" + triggerId + " rewardId=" + rewardId + " attemptId=" + attemptId.ToString() + " status=" + pendingReward.status);
        }

        if (!pendingReward)
            return false;

        bool delivered = TryDeliverPendingRewardItemsSafe(player, progress, pendingReward);
        if (delivered)
            Print("[Silver_77_Quests] Safe delivery delivered reward: questId=" + quest.id + " stage=" + stage + " triggerId=" + triggerId + " rewardId=" + rewardId + " attemptId=" + attemptId.ToString());
        else
            Print("[Silver_77_Quests] Safe delivery pending/failed reward: questId=" + quest.id + " stage=" + stage + " triggerId=" + triggerId + " rewardId=" + rewardId + " attemptId=" + attemptId.ToString() + " status=" + pendingReward.status);

        return delivered;
    }

    static bool TryDeliverOfferGiveItemsSafe(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress, string triggerId)
    {
        if (!quest)
            return false;

        return TryDeliverQuestItemsBatchSafe(player, quest, progress, "offer", triggerId, "giveItems", 0, quest.giveItems);
    }

    static bool TryDeliverCompletionRewardBatchSafe(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress, string triggerId)
    {
        return TryDeliverQuestItemsBatchSafe(player, quest, progress, "completion", triggerId, "completion", 0, ResolveQuestRewardItemsForTrigger(quest, triggerId, false));
    }

    static bool TryDeliverFinalRewardBatchSafe(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress, string triggerId, array<ref Silver77_QuestItem> rewardItems)
    {
        return TryDeliverQuestItemsBatchSafe(player, quest, progress, "reward", triggerId, "reward", 0, rewardItems);
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

    static void ClearCompletionProgress(PlayerQuestProgress progress)
    {
        if (!progress)
            return;

        if (!progress.completedCompletionTriggerIds)
            progress.completedCompletionTriggerIds = new array<string>;

        progress.completedCompletionTriggerIds.Clear();
    }

    static void ClearStageVisits(PlayerQuestProgress progress)
    {
        if (!progress)
            return;

        if (!progress.stageVisits)
            progress.stageVisits = new array<ref PlayerQuestStageVisit>;

        progress.stageVisits.Clear();
    }

    static bool HasStageVisit(PlayerQuestProgress progress, string triggerId, string actionType)
    {
        if (!progress || !progress.stageVisits || triggerId == "" || actionType == "")
            return false;

        foreach (PlayerQuestStageVisit stageVisit : progress.stageVisits)
        {
            if (!stageVisit)
                continue;

            if (stageVisit.triggerId == triggerId && stageVisit.actionType == actionType)
                return true;
        }

        return false;
    }

    static bool RecordStageVisit(PlayerQuestProgress progress, string triggerId, string actionType)
    {
        if (!progress || triggerId == "" || actionType == "")
            return false;

        if (!progress.stageVisits)
            progress.stageVisits = new array<ref PlayerQuestStageVisit>;

        if (HasStageVisit(progress, triggerId, actionType))
            return false;

        PlayerQuestStageVisit stageVisit = new PlayerQuestStageVisit();
        stageVisit.triggerId = triggerId;
        stageVisit.actionType = actionType;
        stageVisit.firstActivatedTime = GetCurrentUnixTimeUTC();
        progress.stageVisits.Insert(stageVisit);
        return true;
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

    static bool MarkCompletionTriggerDone(PlayerQuestProgress progress, string triggerId)
    {
        if (!progress || triggerId == "")
            return false;

        if (!progress.completedCompletionTriggerIds)
            progress.completedCompletionTriggerIds = new array<string>;

        if (progress.completedCompletionTriggerIds.Find(triggerId) != -1)
            return false;

        progress.completedCompletionTriggerIds.Insert(triggerId);
        return true;
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

    static bool AcceptQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!GetGame().IsServer() || !CanAcceptQuest(player, questId, triggerId))
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        string steamId = GetPlayerSteamId(player);
        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = GetOrCreateProgress(data, questId);

        string statusBefore = "";
        if (progress)
            statusBefore = progress.status;

        Print("[Silver_77_Quests][PROGRESS_DEBUG] AcceptQuest: steamId=" + steamId + " questId=" + questId + " triggerId=" + triggerId + " statusBefore=" + statusBefore);

        if (progress)
        {
            EnsureRewardProgressState(progress);
            EnsureCurrentAttemptId(progress);
            ClearObjectiveProgress(progress);
            ClearCompletionProgress(progress);
            ClearStageVisits(progress);
        }

        bool offerGiveItemsDelivered = TryDeliverOfferGiveItemsSafe(player, quest, progress, triggerId);
        if (!offerGiveItemsDelivered)
        {
            if (progress)
                progress.status = "available";

            SavePlayerData(data);
            g_ServerQuestDataRevision++;
            Print("[Silver_77_Quests] Offer giveItems delivery pending/failed for quest: " + questId + " via trigger " + triggerId);
            return false;
        }

        if (progress)
        {
            progress.status = "active";
            RecordStageVisit(progress, triggerId, "offer");
        }

        string statusAfter = "";
        if (progress)
            statusAfter = progress.status;

        Print("[Silver_77_Quests][PROGRESS_DEBUG] AcceptQuest: statusAfter=" + statusAfter + " calling SavePlayerData");

        SavePlayerData(data);
        g_ServerQuestDataRevision++;
        Print("[Silver_77_Quests] Player accepted quest: " + questId + " via trigger " + triggerId);
        return true;
    }

    static bool CanCompleteQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        string status = GetQuestStatus(player, questId);
        if (status != "active")
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

    static void FinalizeQuestReward(PlayerBase player, array<ref Silver77_QuestItem> rewardItems)
    {
        if (!player || !rewardItems)
            return;

        foreach (Silver77_QuestItem reward : rewardItems)
        {
            if (!reward)
                continue;

            SpawnQuestItem(player, reward);
        }
    }

    static void MarkQuestAsRewardPending(PlayerQuestProgress progress)
    {
        if (!progress)
            return;

        progress.status = "reward_pending";
    }

    static void MarkQuestAsCompleted(PlayerQuestProgress progress)
    {
        if (!progress)
            return;

        progress.lastCompletedTime = GetCurrentUnixTimeUTC();
        progress.status = "completed";
        ClearObjectiveProgress(progress);
        ClearCompletionProgress(progress);
    }

    static bool CompleteQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!GetGame().IsServer() || !player || questId == "")
            return false;

        Silver77_Quest quest = GetQuestById(questId);
        if (!quest)
            return false;

        string status = GetQuestStatus(player, questId);
        if (status != "active" && status != "reward_pending")
            return false;

        PlayerQuestData data = GetPlayerData(player);
        PlayerQuestProgress progress = GetOrCreateProgress(data, questId);

        if (status == "active" && QuestRoleContains(quest.completionTriggerIds, triggerId) && !IsCompletionTriggerDone(progress, triggerId))
        {
            bool depositedSomething = DepositPartialQuestItems(player, quest, progress);
            if (depositedSomething)
            {
                SavePlayerData(data);
                g_ServerQuestDataRevision++;
            }

            if (!AreQuestObjectivesReadyForCompletion(player, quest, progress))
                return depositedSomething;

            if (!ProcessQuestObjectivesForFinalReward(player, quest, progress))
            {
                Print("[Silver_77_Quests] Reward blocked because objectives are not safely processed for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            bool completionRewardDelivered = TryDeliverCompletionRewardBatchSafe(player, quest, progress, triggerId);
            if (!completionRewardDelivered)
            {
                SavePlayerData(data);
                g_ServerQuestDataRevision++;
                Print("[Silver_77_Quests] Completion reward delivery pending/failed for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            MarkCompletionTriggerDone(progress, triggerId);
            RecordStageVisit(progress, triggerId, "completion");

            if (QuestHasRoleTriggers(quest.rewardTriggerIds) && AreAllCompletionTriggersDone(quest, progress))
                MarkQuestAsRewardPending(progress);

            if (!QuestHasRoleTriggers(quest.rewardTriggerIds) && AreAllCompletionTriggersDone(quest, progress))
            {
                bool finalRewardDeliveredInCompletion = TryDeliverFinalRewardBatchSafe(player, quest, progress, triggerId, ResolveQuestRewardItemsForTrigger(quest, triggerId, true));
                if (!finalRewardDeliveredInCompletion)
                {
                    MarkQuestAsRewardPending(progress);
                    SavePlayerData(data);
                    g_ServerQuestDataRevision++;
                    Print("[Silver_77_Quests] Final reward delivery pending for quest: " + questId + " via trigger " + triggerId);
                    return false;
                }

                RecordStageVisit(progress, triggerId, "reward");
                MarkCurrentAttemptFinalized(progress);
                MarkQuestAsCompleted(progress);
            }

            SavePlayerData(data);
            g_ServerQuestDataRevision++;
            Print("[Silver_77_Quests] Player completed quest stage: " + questId + " via trigger " + triggerId);
            return true;
        }

        if (status == "reward_pending" && !QuestHasRoleTriggers(quest.rewardTriggerIds) && QuestRoleContains(quest.completionTriggerIds, triggerId))
        {
            if (!AreAllCompletionTriggersDone(quest, progress))
            {
                Print("[Silver_77_Quests] Final reward retry blocked, completion chain is not done for quest: " + questId);
                return false;
            }

            if (!ProcessQuestObjectivesForFinalReward(player, quest, progress))
            {
                Print("[Silver_77_Quests] Final reward retry blocked because objectives are not safely processed for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            bool finalRewardRetryDelivered = TryDeliverFinalRewardBatchSafe(player, quest, progress, triggerId, ResolveQuestRewardItemsForTrigger(quest, triggerId, true));
            if (!finalRewardRetryDelivered)
            {
                MarkQuestAsRewardPending(progress);
                SavePlayerData(data);
                g_ServerQuestDataRevision++;
                Print("[Silver_77_Quests] Final reward retry still pending for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            RecordStageVisit(progress, triggerId, "reward");
            MarkCurrentAttemptFinalized(progress);
            MarkQuestAsCompleted(progress);

            SavePlayerData(data);
            g_ServerQuestDataRevision++;
            Print("[Silver_77_Quests] Player received final reward and completed quest: " + questId + " via trigger " + triggerId);
            return true;
        }

        if (QuestRoleContains(quest.rewardTriggerIds, triggerId))
        {
            if (!AreAllCompletionTriggersDone(quest, progress))
            {
                Print("[Silver_77_Quests] Reward blocked, completion chain is not done for quest: " + questId);
                return false;
            }

            bool rewardDepositedSomething = false;
            if (!QuestHasRoleTriggers(quest.completionTriggerIds))
            {
                rewardDepositedSomething = DepositPartialQuestItems(player, quest, progress);
                if (rewardDepositedSomething)
                {
                    SavePlayerData(data);
                    g_ServerQuestDataRevision++;
                }

                if (!AreQuestObjectivesReadyForCompletion(player, quest, progress))
                    return rewardDepositedSomething;

            }

            if (!ProcessQuestObjectivesForFinalReward(player, quest, progress))
            {
                Print("[Silver_77_Quests] Reward blocked because objectives are not safely processed for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            bool finalRewardDelivered = TryDeliverFinalRewardBatchSafe(player, quest, progress, triggerId, ResolveQuestRewardItemsForTrigger(quest, triggerId, true));
            if (!finalRewardDelivered)
            {
                MarkQuestAsRewardPending(progress);
                SavePlayerData(data);
                g_ServerQuestDataRevision++;
                Print("[Silver_77_Quests] Final reward delivery pending for quest: " + questId + " via trigger " + triggerId);
                return false;
            }

            RecordStageVisit(progress, triggerId, "reward");
            MarkCurrentAttemptFinalized(progress);
            MarkQuestAsCompleted(progress);

            SavePlayerData(data);
            g_ServerQuestDataRevision++;
            Print("[Silver_77_Quests] Player received reward and completed quest: " + questId + " via trigger " + triggerId);
            return true;
        }

        return false;
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

            float removedAmount = RemoveItemsFromPlayerCount(player, objective.className, toDeposit, objective.useItemQuantity);
            if (removedAmount <= 0)
            {
                Print("[Silver_77_Quests] Objective remove failed for " + quest.id + ": " + objective.className + " needed " + toDeposit + ", removed 0");
                continue;
            }

            objectiveProgress.depositedQuantity += removedAmount;
            if (objectiveProgress.depositedQuantity > objective.quantity)
                objectiveProgress.depositedQuantity = objective.quantity;

            depositedSomething = true;
            if (removedAmount < toDeposit)
                Print("[Silver_77_Quests] Objective remove incomplete for " + quest.id + ": " + objective.className + " needed " + toDeposit + ", removed " + removedAmount);

            Print("[Silver_77_Quests] Deposited partial objective for " + quest.id + ": " + objective.className + " +" + removedAmount + " (" + objectiveProgress.depositedQuantity + "/" + objective.quantity + ")");
        }

        return depositedSomething;
    }

    static bool ProcessQuestObjectivesForFinalReward(PlayerBase player, Silver77_Quest quest, PlayerQuestProgress progress)
    {
        if (!player || !quest || !progress || !quest.objectives)
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
                {
                    Print("[Silver_77_Quests] Deposited quantity incomplete for " + quest.id + ": " + objective.className + " " + deposited + "/" + objective.quantity);
                    return false;
                }

                continue;
            }

            float completed = GetDepositedObjectiveQuantity(progress, i, objective.className);
            if (completed >= objective.quantity)
                continue;

            if (objective.removeOnComplete)
            {
                float available = CountPlayerItems(player, objective.className, objective.useItemQuantity);
                if (available < objective.quantity)
                {
                    Print("[Silver_77_Quests] Required quantity missing for " + quest.id + ": " + objective.className + " available " + available + "/" + objective.quantity);
                    return false;
                }

                float removedAmount = RemoveItemsFromPlayerCount(player, objective.className, objective.quantity, objective.useItemQuantity);
                if (removedAmount < objective.quantity)
                {
                    Print("[Silver_77_Quests] Objective remove failed for " + quest.id + ": " + objective.className + " needed " + objective.quantity + ", removed " + removedAmount);
                    return false;
                }
            }
            else
            {
                if (!PlayerHasItemAmount(player, objective.className, objective.quantity, objective.useItemQuantity))
                {
                    Print("[Silver_77_Quests] Required quantity missing for non-removable objective " + quest.id + ": " + objective.className + " need " + objective.quantity);
                    return false;
                }
            }

            PlayerQuestObjectiveProgress objectiveProgress = GetOrCreateObjectiveProgress(progress, i, objective.className);
            if (objectiveProgress)
                objectiveProgress.depositedQuantity = objective.quantity;
        }

        return true;
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

    static float RemoveItemsFromPlayerCount(PlayerBase player, string className, float quantity, bool useItemQuantity = false)
    {
        if (!player || className == "" || quantity <= 0)
            return 0;

        float before = CountPlayerItems(player, className, useItemQuantity);
        float remaining = quantity;
        float removedAmount = 0;
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
                    if (itemQty <= 0)
                        continue;

                    if (itemQty <= remaining)
                    {
                        removedAmount += itemQty;
                        remaining -= itemQty;
                        item.Delete();
                    }
                    else
                    {
                        removedAmount += remaining;
                        item.SetQuantity(itemQty - remaining);
                        remaining = 0;
                    }
                }
                else
                {
                    removedAmount += 1;
                    item.Delete();
                    remaining -= 1;
                }
            }
        }

        if (removedAmount > quantity)
            removedAmount = quantity;

        float after = CountPlayerItems(player, className, useItemQuantity);
        float observedRemoved = before - after;
        if (observedRemoved < 0)
            observedRemoved = 0;

        if (observedRemoved != removedAmount)
            Print("[Silver_77_Quests] Objective remove count check for " + className + ": processed=" + removedAmount + ", observed=" + observedRemoved + ", requested=" + quantity);

        return removedAmount;
    }

    static void RemoveItemsFromPlayer(PlayerBase player, string className, float quantity, bool useItemQuantity = false)
    {
        RemoveItemsFromPlayerCount(player, className, quantity, useItemQuantity);
    }

    static void SpawnQuestItem(PlayerBase player, Silver77_QuestItem questItem)
    {
        if (!GetGame().IsServer() || !player || !questItem || questItem.className == "")
            return;

        for (int i = 0; i < questItem.quantity; i++)
        {
            EntityAI createdItem = null;
            if (questItem.spawnOnGround)
            {
                createdItem = EntityAI.Cast(GetGame().CreateObjectEx(questItem.className, player.GetPosition(), ECE_PLACE_ON_SURFACE | ECE_KEEPHEIGHT));
            }
            else
            {
                createdItem = player.GetInventory().CreateInInventory(questItem.className);
                if (!createdItem)
                    createdItem = EntityAI.Cast(GetGame().CreateObjectEx(questItem.className, player.GetPosition(), ECE_PLACE_ON_SURFACE | ECE_KEEPHEIGHT));
            }

            if (createdItem && questItem.setItemQuantity)
            {
                string itemQuantityError = "";
                string itemQuantityMode = "";
                float actualItemQuantity = 0;
                if (!ApplyRewardItemQuantitySafe(createdItem, questItem.className, questItem.itemQuantity, itemQuantityError, itemQuantityMode, actualItemQuantity))
                {
                    createdItem.Delete();
                    Print("[Silver_77_Quests] Legacy quest itemQuantity failed: item=" + questItem.className + " error=" + itemQuantityError);
                    continue;
                }

                Print("[Silver_77_Quests] Legacy quest itemQuantity applied: item=" + questItem.className + " mode=" + itemQuantityMode + " requested=" + questItem.itemQuantity.ToString() + " actual=" + actualItemQuantity.ToString());
            }
        }
    }

    static bool RequestAcceptQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        if (GetGame().IsServer())
            return AcceptQuest(player, questId, triggerId);

        Print("[Silver_77_Quests] Sending accept quest RPC: " + questId + " via trigger " + triggerId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_ACCEPT, new Param2<string, string>(questId, triggerId), true);
        return true;
    }

    static bool RequestCompleteQuest(PlayerBase player, string questId, string triggerId = "")
    {
        if (!player || questId == "")
            return false;

        if (GetGame().IsServer())
            return CompleteQuest(player, questId, triggerId);

        Print("[Silver_77_Quests] Sending complete quest RPC: " + questId + " via trigger " + triggerId);
        GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_COMPLETE, new Param2<string, string>(questId, triggerId), true);
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
        EnsureServerQuestConfigLoaded();

        if (!GetGame().IsServer() || !player || !player.GetIdentity() || !g_ServerQuestConfig)
            return;

        string payload = SerializeQuestConfigForClient();
        if (payload == "")
            return;

        SendQuestConfigPayloadChunks(player, payload);
    }

    static void SendPlayerDataToClient(PlayerBase player)
    {
        if (!GetGame().IsServer() || !player || !player.GetIdentity())
            return;

        PlayerQuestData data = GetPlayerData(player);
        if (!data)
            return;

        int progressCount = 0;
        if (data.progress)
            progressCount = data.progress.Count();

        Print("[Silver_77_Quests][PROGRESS_DEBUG] SendPlayerDataToClient: steamId=" + data.steamId + " progressCount=" + progressCount.ToString());

        if (data.progress)
        {
            foreach (PlayerQuestProgress progress : data.progress)
            {
                if (progress)
                    Print("[Silver_77_Quests][PROGRESS_DEBUG] SendPlayerDataToClient: sending questId=" + progress.questId + " status=" + progress.status);
            }
        }

        string payload = SerializePlayerDataForClient(data);
        if (payload == "")
            return;

        Print("[Silver_77_Quests][VERSION_CHECK] SERVER TASK_053 PLAYER_DATA_RPC_FORMAT=PARAM3");
        Print("[Silver_77_Quests][VERSION_CHECK] SERVER ABOUT_TO_SEND_PARAM3 payloadLength=" + payload.Length().ToString());
        Print("[Silver_77_Quests][VERSION_CHECK] SERVER RPC_ID_PLAYER_DATA=" + SILVER77_QUEST_RPC_PLAYER_DATA.ToString());

        SendPlayerDataPayloadChunks(player, payload);
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

    private static string SerializeQuestConfigForClient()
    {
        if (!g_ServerQuestConfig)
            return "";

        string payload;
        JsonSerializer serializer = new JsonSerializer();
        if (!serializer.WriteToString(g_ServerQuestConfig, false, payload))
        {
            Print("[Silver_77_Quests] ERROR: Failed to serialize quest config for client sync");
            return "";
        }

        if (payload == "")
        {
            Print("[Silver_77_Quests] ERROR: Quest config sync payload is empty");
            return "";
        }

        return payload;
    }

    private static void SendQuestConfigPayloadChunks(PlayerBase player, string payload)
    {
        if (!player || !player.GetIdentity() || payload == "")
            return;

        int payloadLength = payload.Length();
        int totalChunks = payloadLength / SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;
        if ((payloadLength % SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE) != 0)
            totalChunks++;

        if (totalChunks <= 0)
            totalChunks = 1;

        for (int chunkIndex = 0; chunkIndex < totalChunks; chunkIndex++)
        {
            int offset = chunkIndex * SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;
            int chunkLength = payloadLength - offset;
            if (chunkLength > SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE)
                chunkLength = SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;

            string chunkPayload = payload.Substring(offset, chunkLength);
            GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_CONFIG_DATA, new Param3<int, int, string>(chunkIndex, totalChunks, chunkPayload), true, player.GetIdentity());
        }

        Print("[Silver_77_Quests] Sent quest config to client in " + totalChunks + " chunks (" + payloadLength + " bytes)");
    }

    private static void SendPlayerDataPayloadChunks(PlayerBase player, string payload)
    {
        if (!player || !player.GetIdentity() || payload == "")
            return;

        int payloadLength = payload.Length();
        int totalChunks = payloadLength / SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;
        if ((payloadLength % SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE) != 0)
            totalChunks++;

        if (totalChunks <= 0)
            totalChunks = 1;

        for (int chunkIndex = 0; chunkIndex < totalChunks; chunkIndex++)
        {
            int offset = chunkIndex * SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;
            int chunkLength = payloadLength - offset;
            if (chunkLength > SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE)
                chunkLength = SILVER77_QUEST_CONFIG_SYNC_CHUNK_SIZE;

            string chunkPayload = payload.Substring(offset, chunkLength);
            GetGame().RPCSingleParam(player, SILVER77_QUEST_RPC_PLAYER_DATA, new Param3<int, int, string>(chunkIndex, totalChunks, chunkPayload), true, player.GetIdentity());
        }

        Print("[Silver_77_Quests] Sent quest progress to client in " + totalChunks.ToString() + " chunks (" + payloadLength.ToString() + " bytes)");
    }

    private static string SerializePlayerDataForClient(PlayerQuestData data)
    {
        if (!data)
            return "";

        string payload;
        JsonSerializer serializer = new JsonSerializer();
        if (!serializer.WriteToString(data, false, payload))
        {
            Print("[Silver_77_Quests] ERROR: Failed to serialize player quest data for client sync");
            return "";
        }

        if (payload == "")
        {
            Print("[Silver_77_Quests] ERROR: Player quest sync payload is empty");
            return "";
        }

        return payload;
    }
}
