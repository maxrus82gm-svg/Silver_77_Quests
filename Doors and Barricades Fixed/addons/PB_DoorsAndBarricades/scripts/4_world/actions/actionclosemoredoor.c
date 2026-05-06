class ActionClosePBDoors: ActionInteractBase
{
	void ActionClosePBDoors()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override string GetText()
	{
		return "#close";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			PBDoorsBase fence = PBDoorsBase.Cast( targetObject );
			
			if ( fence && fence.CanCloseFence() )
			{
				return true;
			}
		}
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		PBDoorsBase fence = PBDoorsBase.Cast( action_data.m_Target.GetObject() );
		fence.CloseFence();
	}
}