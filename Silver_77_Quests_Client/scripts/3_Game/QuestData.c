// ============================================================================
// QUEST DATA STRUCTURES
// ============================================================================

const int SILVER77_QUEST_RPC_CONFIG_REQUEST = 7700771;
const int SILVER77_QUEST_RPC_CONFIG_DATA = 7700772;
const int SILVER77_QUEST_RPC_PLAYER_DATA_REQUEST = 7700773;
const int SILVER77_QUEST_RPC_PLAYER_DATA = 7700774;
const int SILVER77_QUEST_RPC_ACCEPT = 7700775;
const int SILVER77_QUEST_RPC_COMPLETE = 7700776;

class Silver77_QuestItem
{
    string className;
    int quantity;
    bool spawnOnGround;
    bool setItemQuantity;
    float itemQuantity;
    
    void Silver77_QuestItem()
    {
        className = "";
        quantity = 1;
        spawnOnGround = false;
        setItemQuantity = false;
        itemQuantity = 0;
    }
}

class Silver77_QuestObjective
{
    string type;
    string className;
    int quantity;
    bool removeOnComplete;
    bool useItemQuantity;
    bool allowPartialTurnIn;
    
    void Silver77_QuestObjective()
    {
        type = "item";
        className = "";
        quantity = 1;
        removeOnComplete = true;
        useItemQuantity = false;
        allowPartialTurnIn = false;
    }
}

class Silver77_QuestTriggerAction
{
    string triggerId;
    string actionType;
    string dialogText;
    ref array<ref Silver77_QuestItem> rewards;
    
    void Silver77_QuestTriggerAction()
    {
        triggerId = "";
        actionType = "";
        dialogText = "";
        rewards = new array<ref Silver77_QuestItem>;
    }
}

class Silver77_Quest
{
    string id;
    string name;
    string description;
    bool repeatable;
    int cooldownSeconds;
    string requiresPrevious;
    ref array<string> requiredQuestIds;
    bool hideUntilRequirementsComplete;
    ref array<string> offerTriggerIds;
    ref array<string> completionTriggerIds;
    ref array<string> rewardTriggerIds;
    ref array<ref Silver77_QuestTriggerAction> triggerActions;
    ref array<ref Silver77_QuestItem> giveItems;
    ref array<ref Silver77_QuestObjective> objectives;
    ref array<ref Silver77_QuestItem> rewards;
    
    void Silver77_Quest()
    {
        id = "";
        name = "";
        description = "";
        repeatable = false;
        cooldownSeconds = 0;
        requiresPrevious = "";
        requiredQuestIds = new array<string>;
        hideUntilRequirementsComplete = false;
        offerTriggerIds = new array<string>;
        completionTriggerIds = new array<string>;
        rewardTriggerIds = new array<string>;
        triggerActions = new array<ref Silver77_QuestTriggerAction>;
        giveItems = new array<ref Silver77_QuestItem>;
        objectives = new array<ref Silver77_QuestObjective>;
        rewards = new array<ref Silver77_QuestItem>;
    }
}

class Silver77_NpcItem
{
    string className;
    string slot;
    int quantity;
    bool setItemQuantity;
    float itemQuantity;

    void Silver77_NpcItem()
    {
        className = "";
        slot = "";
        quantity = 1;
        setItemQuantity = false;
        itemQuantity = 0;
    }
}

class Silver77_NpcMagazine
{
    string className;
    int ammoCount;

    void Silver77_NpcMagazine()
    {
        className = "";
        ammoCount = 0;
    }
}

class Silver77_NpcContainer
{
    string className;
    string slot;
    ref array<ref Silver77_NpcItem> items;

    void Silver77_NpcContainer()
    {
        className = "";
        slot = "";
        items = new array<ref Silver77_NpcItem>;
    }
}

class Silver77_NpcWeapon
{
    string className;
    string target;
    ref array<ref Silver77_NpcItem> attachments;
    ref Silver77_NpcMagazine magazine;
    ref array<ref Silver77_NpcItem> ammo;

    void Silver77_NpcWeapon()
    {
        className = "";
        target = "";
        attachments = new array<ref Silver77_NpcItem>;
        magazine = new Silver77_NpcMagazine();
        ammo = new array<ref Silver77_NpcItem>;
    }
}

class Silver77_NpcEquipment
{
    ref array<ref Silver77_NpcItem> clothing;
    ref array<ref Silver77_NpcContainer> containers;
    ref Silver77_NpcItem hands;
    ref array<ref Silver77_NpcItem> backItems;
    ref array<ref Silver77_NpcWeapon> weapons;

    void Silver77_NpcEquipment()
    {
        clothing = new array<ref Silver77_NpcItem>;
        containers = new array<ref Silver77_NpcContainer>;
        hands = new Silver77_NpcItem();
        backItems = new array<ref Silver77_NpcItem>;
        weapons = new array<ref Silver77_NpcWeapon>;
    }
}

class Silver77_QuestTriggerConfig
{
    string id;
    ref array<float> position;
    float radius;
    float focusHeight;
    float focusRadius;
    ref array<string> questIds;
    string hintText;  // Текст подсказки
    bool spawnNpc;
    string npcClassName;
    ref array<float> npcPosition;
    ref array<float> npcOrientation;
    ref array<string> npcLoadout;
    string npcHandsItem;
    ref array<string> npcBackItems;
    string npcLoadoutPreset;
    ref Silver77_NpcEquipment npcEquipment;
    
    void Silver77_QuestTriggerConfig()
    {
        id = "";
        position = new array<float>;
        radius = 2.0;
        focusHeight = 1.2;
        focusRadius = 1.0;
        questIds = new array<string>;
        hintText = "[F] Открыть квесты";  // По умолчанию
        spawnNpc = false;
        npcClassName = "SurvivorM_Mirek";
        npcPosition = new array<float>;
        npcOrientation = new array<float>;
        npcLoadout = new array<string>;
        npcHandsItem = "";
        npcBackItems = new array<string>;
        npcLoadoutPreset = "";
        npcEquipment = new Silver77_NpcEquipment();
    }
}

class Silver77_QuestConfig
{
    int version;
    ref array<ref Silver77_Quest> quests;
    ref array<ref Silver77_QuestTriggerConfig> triggers;
    
    void Silver77_QuestConfig()
    {
        version = 1;
        quests = new array<ref Silver77_Quest>;
        triggers = new array<ref Silver77_QuestTriggerConfig>;
    }
}
