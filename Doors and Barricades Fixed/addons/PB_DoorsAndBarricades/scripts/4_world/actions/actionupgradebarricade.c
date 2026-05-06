class ActionUpgradeBarricadePBCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 5 );
	}
};
//UPGRADE PARA T2 BARRICADA DE JANELA
class ActionUpgradeBarricadePB: ActionContinuousBase
{
	void ActionUpgradeBarricadePB()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_PlankBarricade base_building = PB_PlankBarricade.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_MetalBarricade upgrade = PB_MetalBarricade.Cast( GetGame().CreateObject("PB_MetalBarricade", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};

//UPGRADE PARA T3 BARRICADA DE JANELA
class ActionUpgradeBarricadePBT3: ActionContinuousBase
{
	void ActionUpgradeBarricadePBT3()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_MetalBarricade base_building = PB_MetalBarricade.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_BrickBarricade upgrade = PB_BrickBarricade.Cast( GetGame().CreateObject("PB_BrickBarricade", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};

//UPGRADE PARA T2 DA BARRICADA DE PORTA
class ActionUpgradeDoorBarricade: ActionContinuousBase
{
	void ActionUpgradeDoorBarricade()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_PlankDoorBarricade base_building = PB_PlankDoorBarricade.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_MetalDoorBarricade upgrade = PB_MetalDoorBarricade.Cast( GetGame().CreateObject("PB_MetalDoorBarricade", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};
//UPGRADE PARA T3 DA BARRICADA DE PORTA
class ActionUpgradeDoorBarricadeT3: ActionContinuousBase
{
	void ActionUpgradeDoorBarricadeT3()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_MetalDoorBarricade base_building = PB_MetalDoorBarricade.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_BrickDoorBarricade upgrade = PB_BrickDoorBarricade.Cast( GetGame().CreateObject("PB_BrickDoorBarricade", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};

//UPGRADE PARA T2 JANELA DE PLANK
class ActionUpgradeWindow: ActionContinuousBase
{
	void ActionUpgradeWindow()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_PlankWindow base_building = PB_PlankWindow.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_MetalWindow upgrade = PB_MetalWindow.Cast( GetGame().CreateObject("PB_MetalWindow", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};

//UPGRADE PARA T2 DA PAREDE
class ActionUpgradeWall: ActionContinuousBase
{
	void ActionUpgradeWall()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_WoodWall base_building = PB_WoodWall.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_MetalWall upgrade = PB_MetalWall.Cast( GetGame().CreateObject("PB_MetalWall", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};
//UPGRADE PARA T3 DA PAREDE
/*class ActionUpgradeWallT3: ActionContinuousBase
{
	void ActionUpgradeWallT3()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_MetalWall base_building = PB_MetalWall.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_BrickWall upgrade = PB_BrickWall.Cast( GetGame().CreateObject("PB_BrickWall", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};*/

//Upgrade Gate T1 para T2
class ActionUpgradeGate: ActionContinuousBase
{
	void ActionUpgradeGate()
	{
		m_CallbackClass = ActionUpgradeBarricadePBCB;
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
		return "#STR_PBUpgrade";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		PB_WoodGate base_building = PB_WoodGate.Cast( target_object );
		if ( base_building )
		{
			if (base_building.hasTheGoodStuff())
			{
				return true;	
			}
		}
        
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PBDoorsBase base_building = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
        vector pos = base_building.GetPosition();
        vector ori = base_building.GetOrientation();

		base_building.Delete();

        PB_MetalGate upgrade = PB_MetalGate.Cast( GetGame().CreateObject("PB_MetalGate", pos, false ));
		upgrade.SetPosition( pos );
		upgrade.SetOrientation( ori );
	}
	
};
