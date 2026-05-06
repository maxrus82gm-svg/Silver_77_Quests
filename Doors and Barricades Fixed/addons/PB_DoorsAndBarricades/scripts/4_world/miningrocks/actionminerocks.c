//AÇÃO PARA QUEBRAR PEDRAS = PB_STONE
class ActionBrokeStoneCB : ActionContinuousBaseCB
{
	PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT_CONSTRUCT );
	}
};

class ActionBrokeStone: ActionContinuousBase
{
	void ActionBrokeStone()
	{
		m_CallbackClass = ActionBrokeStoneCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MINEROCK;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//RETORNA A AÇÃO COMO FALSA SE O JOGADOR ESTIVER COM E PERNA QUEBRADA
		if (player.m_BrokenLegState == eBrokenLegs.BROKEN_LEGS)
			return false;

		Object target_object = target.GetObject();
		if ( target_object )
		{
			PB_Stone ntarget = PB_Stone.Cast( target_object );
			if( ntarget )
			{
				return true;
			}
		}
		return false;
	}

	override string GetText()
	{
		return "Broke";
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		PB_Stone ntarget = PB_Stone.Cast( target_object );

		target_object.GetGame().CreateObjectEx("PB_Brick", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE);
		target_object.GetGame().CreateObjectEx("PB_Brick", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE);

		ntarget.Delete();

		action_data.m_MainItem.AddHealth("", "Health", -20);
	}
};


//MODIFICAÇÃO DA CLASSE ORIGINAL PARA AUMENTAR O TEMPO DE MINERAÇÃO
modded class ActionMineRockCB : ActionContinuousBaseCB
{
	//EDITAR O VALOR ABAIXO
	private const float TIME_BETWEEN_MATERIAL_DROPS = 20;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousMineRock(TIME_BETWEEN_MATERIAL_DROPS);
	}
};
