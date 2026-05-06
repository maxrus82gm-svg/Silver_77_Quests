modded class CarDoor
{
	bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Door_1_1
{
	override bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Door_1_2
{
	override bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Door_2_1
{
	override bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Door_2_2
{
	override bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Hood
{
	override bool IsCarDestroy()
	{
		return true;
	};
};
modded class Hatchback_02_Trunk
{
	override bool IsCarDestroy()
	{
		return true;
	};
};

////// Para adicionar ação a um item especifico
/*class ActionDestroyPartsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(8);
	}
}
class ActionDestroyParts: ActionContinuousBase
{
	void ActionDestroyParts()
	{
		m_CallbackClass = ActionDestroyPartsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MINEROCK;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	}

    override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override string GetText()
	{
		return "Destroy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (target)
		{
			if (player.GetItemInHands().GetType() == "SledgeHammer")
			{
				CarDoor m_DParts = CarDoor.Cast(target.GetObject());
				if (m_DParts)
				{
					if (m_DParts.IsCarDestroy())
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		if (action_data.m_Target)
		{
			CarDoor m_DParts = CarDoor.Cast(action_data.m_Target.GetObject());
			if (m_DParts)
			{
				ItemBase MetalDrop = action_data.m_Target.GetObject().GetGame().CreateObjectEx("MetalPlate", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE);
				MetalDrop.SetQuantity(1);
				m_DParts.Delete();
				action_data.m_MainItem.AddHealth("", "Health", -20);
			};
		};
	}

} */
//Registra a Ação
modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDestroyParts);
	}
}

//Registra a Ação no Players sem precisar registrar em um item
modded class PlayerBase extends ManBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(ActionDestroyParts, InputActionMap);
	}
}

//AÇÃO DE DESTRUIR PARA FERRAMENTAS VARIADAS NO ARQUIVO .JSON
class ActionDestroyPartsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(15);
	}
}
class ActionDestroyParts: ActionContinuousBase
{
	void ActionDestroyParts()
	{
		m_CallbackClass = ActionDestroyPartsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MINEROCK;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	}

    override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override string GetText()
	{
		return "Destroy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (target)
		{
			if (HasDestroyCarPartsTools(item))
			{
				CarDoor m_DParts = CarDoor.Cast(target.GetObject());
				if (m_DParts)
				{
					if (m_DParts.IsCarDestroy())
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool HasDestroyCarPartsTools(ItemBase item_in_hands)
	{
		ref TStringArray j_DestroyCarParts = g_Game.GetPBConfig().Get_DestroyCarPartsTools();

		foreach (string tool : j_DestroyCarParts) 
		{
			if ( item_in_hands && item_in_hands.IsKindOf(tool) )
			{
				return true;
			}
		}
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		if (action_data.m_Target)
		{
			CarDoor m_DParts = CarDoor.Cast(action_data.m_Target.GetObject());
			if (m_DParts)
			{
				ItemBase MetalDrop = action_data.m_Target.GetObject().GetGame().CreateObjectEx("MetalPlate", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE);
				MetalDrop.SetQuantity(1);
				m_DParts.Delete();
				action_data.m_MainItem.AddHealth("", "Health", -20);
			};
		};
	}

}
