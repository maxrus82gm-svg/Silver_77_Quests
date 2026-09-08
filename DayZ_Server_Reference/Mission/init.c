
//Настройки спавна
#include "$CurrentDir:mpmissions\dayzOffline.chernarusplus\CustomSpawnPlayerConfig\init_mod.c"
//Настройки спавна


void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	// Date is not forced to a fixed winter window.
	// This lets cfggameplay.json monthly temperatures work across seasons.
	//Настройки спавна
	//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(My_Custom_Spawn_Parameters.Read_Update_Config, 120000, true); // Обновление настроек скрипта кастомных спавнов, 1 минута = 60000
	//Настройки спавна
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.45, 0.65 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		//Настройки спавна
		//pos = My_Custom_Spawn_Parameters.Load_And_Check_Spawnpoints(identity, pos);
		//characterName = My_Custom_Spawn_Parameters.Load_And_Check_SpawnSkin_Player(identity, characterName);
		//Настройки спавна
		
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "BandageDressing" );
			player.SetQuickBarEntityShortcut(itemEnt, 2);
			
			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt( 0, 4 );
			itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
			SetRandomHealth( itemEnt );
			player.SetQuickBarEntityShortcut(itemEnt, 1);

			rand = Math.RandomFloatInclusive( 0.0, 1.0 );
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory( "Apple" );
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory( "Pear" );
			else
				itemEnt = player.GetInventory().CreateInInventory( "Plum" );
			player.SetQuickBarEntityShortcut(itemEnt, 3);
			SetRandomHealth( itemEnt );
			
			
		}
		
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
			SetRandomHealth( itemClothing );
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
		
		//Настройки спавна
		//My_Custom_Spawn_Parameters.Load_And_Check_StartLoadout(player);
		//Настройки спавна
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
