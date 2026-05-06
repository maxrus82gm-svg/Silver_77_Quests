class ActionDestroyPBDoorCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		Object targetObject = m_ActionData.m_Target.GetObject();
		int raidTime = g_Game.GetPBConfig().Get_MoreDoorDestroyTime(targetObject.GetType());
		m_ActionData.m_ActionComponent = new CAContinuousTime( raidTime );
	}
};

//Destroy T1

class ActionDestroyT1: ActionContinuousBase
{
	void ActionDestroyT1()
	{
		m_CallbackClass = ActionDestroyPBDoorCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;		
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}
		
	override string GetText()
	{
		return "#STR_PBDestroy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PBDoorsBase base_building = PBDoorsBase.Cast( target_object );
		if ( base_building )
		{
			if (HasFancyRaidTools(item))
			{
				if( base_building.AllowDestruction() && CfgAllowDestruct() && base_building.IsTier1())
				{
					//return true;
					return DestroyCondition( player, target, item, true );
				}
			}
		}
        
		return false;
	}

	bool HasFancyRaidTools(ItemBase item_in_hands)
	{
		ref TStringArray j_raidingTools = g_Game.GetPBConfig().Get_RaidToolsT1();

		foreach (string tool : j_raidingTools) 
		{
			if ( item_in_hands && item_in_hands.IsKindOf(tool) )
			{
				return true;
			}
		}
		return false;
	}

	bool CfgAllowDestruct()
	{
		return g_Game.GetPBConfig().Get_CanDestroyMoreDoor();
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return DestroyCondition( action_data.m_Player, action_data.m_Target, action_data.m_MainItem , false );
	}	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		int damageToTool = g_Game.GetPBConfig().Get_MoreDoorToolDamage(target_object.GetType());

		target_object.Delete();

		action_data.m_MainItem.DecreaseHealth( damageToTool, false );
	}
	
	protected bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			PBDoorsBase base_building = PBDoorsBase.Cast( target_object );

			// if( base_building && base_building.DismantleDisabled())
			// {
			// 	return true;
			// }

			if ( base_building )
			{
				string part_name = "door1";

				//camera and position checks
				if ( base_building.IsFacingPlayer( player, part_name ) && !player.GetInputController().CameraIsFreeLook() )
				{
					//Camera check (client-only)
					if ( camera_check )
					{
						if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
						{
							if ( !base_building.IsFacingCamera( part_name ) )
							{
								return false;
							}
						}
					}
					return true;				
				}
			}
		}
		return false;
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName();
	}
}

//Destroy T2

class ActionDestroyT2: ActionContinuousBase
{
	void ActionDestroyT2()
	{
		m_CallbackClass = ActionDestroyPBDoorCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;		
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}
		
	override string GetText()
	{
		return "#STR_PBDestroy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PBDoorsBase base_building = PBDoorsBase.Cast( target_object );
		if ( base_building )
		{
			if (HasFancyRaidTools(item))
			{
				if( base_building.AllowDestruction() && CfgAllowDestruct() && base_building.IsTier2())
				{
					//return true;
					return DestroyCondition( player, target, item, true );
				}
			}
		}
        
		return false;
	}

	bool HasFancyRaidTools(ItemBase item_in_hands)
	{
		ref TStringArray j_raidingTools = g_Game.GetPBConfig().Get_RaidToolsT2();

		foreach (string tool : j_raidingTools) 
		{
			if ( item_in_hands && item_in_hands.IsKindOf(tool) )
			{
				return true;
			}
		}
		return false;
	}

	bool CfgAllowDestruct()
	{
		return g_Game.GetPBConfig().Get_CanDestroyMoreDoor();
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return DestroyCondition( action_data.m_Player, action_data.m_Target, action_data.m_MainItem , false );
	}	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		int damageToTool = g_Game.GetPBConfig().Get_MoreDoorToolDamage(target_object.GetType());

		target_object.Delete();

		action_data.m_MainItem.DecreaseHealth( damageToTool, false );
	}
	
	protected bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			PBDoorsBase base_building = PBDoorsBase.Cast( target_object );

			// if( base_building && base_building.DismantleDisabled())
			// {
			// 	return true;
			// }

			if ( base_building )
			{
				string part_name = "door1";

				//camera and position checks
				if ( base_building.IsFacingPlayer( player, part_name ) && !player.GetInputController().CameraIsFreeLook() )
				{
					//Camera check (client-only)
					if ( camera_check )
					{
						if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
						{
							if ( !base_building.IsFacingCamera( part_name ) )
							{
								return false;
							}
						}
					}
					return true;				
				}
			}
		}
		return false;
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName();
	}
}

//Destroy T3

class ActionDestroyT3: ActionContinuousBase
{
	void ActionDestroyT3()
	{
		m_CallbackClass = ActionDestroyPBDoorCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;		
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}
		
	override string GetText()
	{
		return "#STR_PBDestroy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PBDoorsBase base_building = PBDoorsBase.Cast( target_object );
		if ( base_building )
		{
			if (HasFancyRaidTools(item))
			{
				if( base_building.AllowDestruction() && CfgAllowDestruct() && base_building.IsTier3() )
				{
					//return true;
					return DestroyCondition( player, target, item, true );
				}
			}
		}
        
		return false;
	}

	bool HasFancyRaidTools(ItemBase item_in_hands)
	{
		ref TStringArray j_raidingTools = g_Game.GetPBConfig().Get_RaidToolsT3();

		foreach (string tool : j_raidingTools) 
		{
			if ( item_in_hands && item_in_hands.IsKindOf(tool) )
			{
				return true;
			}
		}
		return false;
	}

	bool CfgAllowDestruct()
	{
		return g_Game.GetPBConfig().Get_CanDestroyMoreDoor();
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return DestroyCondition( action_data.m_Player, action_data.m_Target, action_data.m_MainItem , false );
	}	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		int damageToTool = g_Game.GetPBConfig().Get_MoreDoorToolDamage(target_object.GetType());

		target_object.Delete();

		action_data.m_MainItem.DecreaseHealth( damageToTool, false );
	}
	
	protected bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			PBDoorsBase base_building = PBDoorsBase.Cast( target_object );

			// if( base_building && base_building.DismantleDisabled())
			// {
			// 	return true;
			// }

			if ( base_building )
			{
				string part_name = "door1";

				//camera and position checks
				if ( base_building.IsFacingPlayer( player, part_name ) && !player.GetInputController().CameraIsFreeLook() )
				{
					//Camera check (client-only)
					if ( camera_check )
					{
						if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
						{
							if ( !base_building.IsFacingCamera( part_name ) )
							{
								return false;
							}
						}
					}
					return true;				
				}
			}
		}
		return false;
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName();
	}
}