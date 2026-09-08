class StartSetsPlayersConfig
{
    void StartSetsPlayer(PlayerBase player, int use_set_id)
    {
        EntityAI itemCreated;
        EntityAI itemCreated1;
        float rand;
        
        switch(use_set_id)
        {
            // Ваши кастомные сеты (1, 2, 3...)
            case 1: 
            {
                // ... (ваш код для сета 1)
                break;
            }
            
            case 2:
            {
                // ... (ваш код для сета 2)
                break;
            }
            
            // Дефолтный спавн (все новые игроки)
            default: 
            {
                RemoveAllItems(player); // Очистка стандартного лута
                
                // === Одежда ===
				player.GetInventory().CreateInInventory("WoolGlovesFingerless_Tan");  // Перчатки
                player.GetInventory().CreateInInventory("HikingBoots_Black");    // Обувь
                player.GetInventory().CreateInInventory("TrackSuitPants_Blue");    // Штаны
                player.GetInventory().CreateInInventory("courierBag");  // Перчатки
				player.GetInventory().CreateInInventory("RageVirusCure");  // Антивирус
				player.GetInventory().CreateInInventory("RageVirusCure");  // Антивирус
                
                // === Оружие ===
                itemCreated = player.GetInventory().CreateInInventory("Sawedoffizh18Shotgun");
                player.GetInventory().CreateInInventory("Ammo_12gaPellets"); // 10 патронов
				player.GetInventory().CreateInInventory("Ammo_12gaPellets"); // 10 патронов
				player.GetInventory().CreateInInventory("Ammo_12gaPellets"); // 10 патронов
                player.GetInventory().CreateInInventory("HuntingKnife"); // 10 патронов
                // === Еда/медикаменты ===
                player.GetInventory().CreateInInventory("SpaghettiCan");  // Тушёнка
				 player.GetInventory().CreateInInventory("CatFoodCan");  // Тушёнка
				player.GetInventory().CreateInInventory("CatFoodCan");  // Тушёнка
				player.GetInventory().CreateInInventory("CatFoodCan");
                player.GetInventory().CreateInInventory("SodaCan_cola");         // Кола
				player.GetInventory().CreateInInventory("SodaCan_cola");         // Кола
                
                // === Инструменты ===
				 player.GetInventory().CreateInInventory("ItemPDA");          // Планшет
				 player.GetInventory().CreateInInventory("Battery9V");          // Планшет
                player.GetInventory().CreateInInventory("BloodTestKit");          // Нож
				  player.GetInventory().CreateInInventory("BloodTestKit");          // Нож
                player.GetInventory().CreateInInventory("Roadflare");             // Фальшфейер
                
                // === Установка позиции ===
                array<vector> spawnPoints = {
                    "12926.992969 215.892012 12758.522406"  // Координаты у Чёрного озера
                   // "12800.0 210.0 12700.0"                  // Резервная точка
                };
                vector spawnPos = spawnPoints.GetRandomElement();
                player.SetPosition(spawnPos);
                
                // Для отладки (проверьте в server_console.log)
                Print("[StartSets] Spawned player at: " + spawnPos);
                break;
            }
        }
    }
    
    // Очистка инвентаря
    void RemoveAllItems(PlayerBase player)
    {
        array<EntityAI> items = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        foreach(EntityAI item : items)
        {
            if (item && !item.IsInherited(SurvivorBase))
            {
                player.GetInventory().LocalDestroyEntity(item);
            }
        }
    }
}