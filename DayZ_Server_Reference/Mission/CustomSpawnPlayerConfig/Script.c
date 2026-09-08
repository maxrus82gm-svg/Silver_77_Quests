class CustomSpawnPlayerConfig
{
	private string Location_Directory_config = "$profile:"; 
	private string Location_filename_config = "CustomSpawnPlayerConfig.txt"; 
	private string default_log_block_name = "[CustomSpawnPlayerConfig] "; 
	private string disable_read_parameter = "0";	
	private string block_split_parameters_config = "|";	
	private string block_split_multi_parameters_config = ";";	
	
	private bool enabled_loading_custom_spawnpoints = false;
	private bool enabled_loading_custom_sets = true; 
	private bool enabled_loading_custom_skins = false; 
    
	// ------------------------------------------------------------
	// ----------------    NEXT CODE DON'T EDIT   -----------------
	// ------------------------------------------------------------
    ref StartSetsPlayersConfig StartSetsPlayers;
	//
	ref map<string, ref array<vector>> players_spawnpoints = new map<string, ref array<vector> >;
	ref map<string, ref array<int>> players_sets = new map<string, ref array<int> >;
	ref map<string, string> players_skins = new map<string, string>;
	private bool enabled_loaded_successfull_config = false;
	string Log_CustomSpawnPlayerConfig_GetDateTime()
	{
		private int year, month, day, hour, minute, second;		
		GetYearMonthDay(year, month, day);		
		GetHourMinuteSecond(hour, minute, second);
		string returned_message = "[" + day.ToStringLen(2) + "." + month.ToStringLen(2) + "." + year.ToStringLen(2) + " - " + hour.ToStringLen(2) + "." + minute.ToStringLen(2) + "." + second.ToStringLen(2) + "] ";
		return returned_message;
	}
	bool Check_coords_disable(vector coord)
	{
		private bool ret_zn = false;
		private float pos_x = coord[0];
		private float pos_y = coord[1];
		private float pos_z = coord[2];	
		if ((pos_x == 0.0) & (pos_y == 0.0) & (pos_z == 0.0))
		{
			ret_zn = true;
		}
		return ret_zn;
	}	
	vector Set_Read_coords_disable()
	{
		return Vector(0.0, 0.0, 0.0);
	}
	void CustomSpawnPlayerConfig()
	{
        StartSetsPlayers = new StartSetsPlayersConfig();
		Read_Update_Config();
	}
	
	void ~CustomSpawnPlayerConfig()
	{
		Print(Log_CustomSpawnPlayerConfig_GetDateTime() + default_log_block_name + "CustomSpawnPlayerConfig work end, class closed!");
	}
	
	void Read_Update_Config()
	{
		private string name_block_work = "[ReadAndUpdateSettings] ";
		private array<string> readed_lines_config = ReadFileConfig();
		private bool check_normal_read = AnaliseFileConfig(readed_lines_config);
		if (check_normal_read)
		{
			LoadFileConfig(readed_lines_config);
			enabled_loaded_successfull_config = true;
		} else
		{
			enabled_loaded_successfull_config = false;
		}
	}
	
	array<string> ReadFileConfig()
	{
		private string name_block_work = "[ReadFileConfig] ";
		private array<string> readed_lines_config = new array<string>;
		readed_lines_config.Clear();
		if (FileExist ( (Location_Directory_config + Location_filename_config) ) ) 
		{			
			private string line_content; 
			FileHandle file = OpenFile((Location_Directory_config + Location_filename_config), FileMode.READ);
			if (file != 0)
			{ 
				while ( FGets( file,  line_content ) > 0 )    
				{
					readed_lines_config.Insert( line_content);
				}
				CloseFile(file);
			}
			//readed_lines_config.Debug();
		}
		return readed_lines_config;
	}
	
	bool AnaliseFileConfig(array<string> readed_lines_config)
	{
		private bool return_zn = true;
		private string name_block_work = "[AnaliseFileConfig] ";
		if (readed_lines_config.Count() > 0)
		{
			foreach (string line : readed_lines_config)
			{
				if (line.Contains(block_split_parameters_config))
				{
					private array<string> splited_line = new array<string>;
					line.Split( block_split_parameters_config, splited_line );
					if (splited_line.Count() == 4)
					{
						private string UID_player = splited_line.Get(0);
						private string default_skin_player = splited_line.Get(1);
						private string sets_numbers_player = splited_line.Get(2);
						private string points_spawn_player = splited_line.Get(3);
						if ((UID_player == "") || (default_skin_player == "") || (sets_numbers_player == "") || (points_spawn_player == ""))
						{
							return_zn = false;
							break;
						}
					} else
					{
						return_zn = false;
						break;
					}
				} else
				{
					return_zn = false;
					break;
				}
			}
		} else
		{
			return_zn = false;
		}
		return return_zn;
	}
	
	void LoadFileConfig(array<string> readed_lines_config)
	{
		private string name_block_work = "[LoadFileConfig] ";
		players_spawnpoints.Clear();
		players_sets.Clear();
		players_skins.Clear();	
		foreach (string line : readed_lines_config)
		{
			private array<string> splited_line = new array<string>; splited_line.Clear();
			line.Split( block_split_parameters_config, splited_line );
			private string UID_player = splited_line.Get(0);
			private string default_skin_player = splited_line.Get(1);
			private string sets_numbers_player = splited_line.Get(2);
			private string points_spawn_player = splited_line.Get(3);
			private array<vector> temp_players_spawnpoints = new array<vector>; temp_players_spawnpoints.Clear();
			array<int> temp_players_sets = new array<int>; temp_players_sets.Clear();
			array<string> temp_players_spawnpoints_string = new array<string>; temp_players_spawnpoints_string.Clear();
			array<string> temp_players_sets_string = new array<string>; temp_players_sets_string.Clear();
			if (points_spawn_player.Contains(block_split_multi_parameters_config))
			{								
				points_spawn_player.Split( block_split_multi_parameters_config, temp_players_spawnpoints_string );								
			} else
			{
				temp_players_spawnpoints_string.Insert(points_spawn_player);
			}
			foreach (string line_spawnpoint : temp_players_spawnpoints_string)
			{
				if (line_spawnpoint == disable_read_parameter)
				{
					temp_players_spawnpoints.Insert(Set_Read_coords_disable());
				} else
				{
					temp_players_spawnpoints.Insert(line_spawnpoint.ToVector());
				}
			}
			if (sets_numbers_player.Contains(block_split_multi_parameters_config))
			{								
				sets_numbers_player.Split( block_split_multi_parameters_config, temp_players_sets_string );								
			} else
			{
				temp_players_sets_string.Insert(sets_numbers_player);
			}
			foreach (string line_set : temp_players_sets_string)
			{
				if (line_set == disable_read_parameter)
				{
					temp_players_sets.Insert(0);
				} else
				{
					temp_players_sets.Insert(line_set.ToInt());
				}
			}
			if (default_skin_player == disable_read_parameter)
			{
				default_skin_player = "0";
			}
			players_spawnpoints.Insert(UID_player,temp_players_spawnpoints);
			players_sets.Insert(UID_player,temp_players_sets);
			players_skins.Insert(UID_player,default_skin_player);
		}
	}
	
	vector Load_And_Check_Spawnpoints(PlayerIdentity identity, vector pos_default)
	{
		private string name_block_work = "[Load_And_Check_Spawnpoints] ";
		private vector retun_pos = pos_default;		
		if(identity)
		{
			private string Name_P = identity.GetName();
			private string UID_P = identity.GetPlainId();
			private string Game_UID_P = identity.GetId();
			private string Game_ID_P = identity.GetPlayerId().ToString();
			if (enabled_loading_custom_spawnpoints)
			{
				if ((players_spawnpoints.Count() > 0) && (enabled_loaded_successfull_config))
				{				
					if (players_spawnpoints.Contains(UID_P))
					{
						private array<vector> readed_lines_config = new array<vector>;
						readed_lines_config = players_spawnpoints.Get(UID_P);
						private vector retun_pos_check = readed_lines_config.GetRandomElement();
						if (!Check_coords_disable(retun_pos_check))
						{
                            retun_pos = retun_pos_check;						
                        } 						
					} 
				} 
			} 
		}		
		return retun_pos;
	}
	string Load_And_Check_SpawnSkin_Player(PlayerIdentity identity, string characterName)
	{
		private string name_block_work = "[SelectStartSkinPlayer] ";
		private string retun_skin = characterName;
		if(identity)
		{
			private string Name_P = identity.GetName();
			private string UID_P = identity.GetPlainId();
			private string Game_UID_P = identity.GetId();
			private string Game_ID_P = identity.GetPlayerId().ToString();
			if (enabled_loading_custom_skins)
			{
				if (players_skins.Count() > 0)
				{				
					if (players_skins.Contains(UID_P))
					{
						private string readed_skin = players_skins.Get(UID_P);
						if (readed_skin != "0")
						{
                            retun_skin = readed_skin;
						} 
					}
				}
			}
		}
		return retun_skin;
	}
	void Load_And_Check_StartLoadout(PlayerBase player)
	{
		private string name_block_work = "[SelectStartSetPlayer] ";
		private int use_set_id = -1;
		private PlayerIdentity identity = player.GetIdentity();
		if(identity)
		{
			private string Name_P = identity.GetName();
			private string UID_P = identity.GetPlainId();
			private string Game_UID_P = identity.GetId();
			private string Game_ID_P = identity.GetPlayerId().ToString();
			if (enabled_loading_custom_sets)
			{
				if ((players_sets.Count() > 0) && (enabled_loaded_successfull_config))
				{				
					if (players_sets.Contains(UID_P))
					{
						private array<int> readed_lines_config = new array<int>; readed_lines_config.Clear();
						readed_lines_config = players_sets.Get(UID_P);
						private int chek_use_set_id = readed_lines_config.GetRandomElement();
						if (chek_use_set_id != 0)
						{
							use_set_id = chek_use_set_id;
						} 
					}
				}
			}
			
			StartSetsPlayers.StartSetsPlayer(player, use_set_id);
		}		
	}
	
	private array<EntityAI> GetItemListinobject(PlayerBase player,EntityAI item, bool check_player)
	{
		array<EntityAI> EntityAI_list_items = new array<EntityAI>; EntityAI_list_items.Clear();
		int	attIdx = 0; int attCount = 0; EntityAI		item_in_object;
		if ((player) && (check_player))
		{
			attCount	=	player.GetInventory().AttachmentCount(); 
			for (attIdx	=	0; attIdx < attCount; attIdx++)
			{
				item_in_object	=	player.GetInventory().GetAttachmentFromIndex(attIdx);
				if ( item_in_object.IsItemBase() )
				{
					EntityAI_list_items.Insert(item_in_object);
				}
			}
		}
		if ((item) && !check_player)
		{
			attCount	=	item.GetInventory().AttachmentCount();
			for (attIdx	=	0; attIdx < attCount; attIdx++)
			{
				item_in_object	=	item.GetInventory().GetAttachmentFromIndex(attIdx);
				if ( item_in_object.IsItemBase() )
				{
					EntityAI_list_items.Insert(item_in_object);
				}
			}
		}
		return EntityAI_list_items;		
	}
};

