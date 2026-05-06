class PBDoorsKitBase extends ItemBase
{
	ref protected EffectSound 						m_DeployLoopSound;
	Object											moreDoorKit;
	//protected bool m_hasThings;

	string slotNails = "Material_Nails";
	string slotPlanks = "Material_WoodenPlanks";
	string slotMetal = "Material_MetalSheets";
	string slotLogs = "Material_WoodenLogs";
	string slotWire = "Material_MetalWire";
	string slotStone = "Slot_Brick";

	string slotPlanks10 = "Material_L2W2_WoodenPlanks";
	string slotPlanks18 = "Material_L2W3_WoodenPlanks";

	string slotNails20 = "Material_L2W1_Nails";
	string slotNails36 = "Material_L2W2_Nails";
	string slotNails75 = "Material_L2W3_Nails";

	string slotMetal16 = "MetalSheets3";

	int slot_id;
	int slot_id2;
	int slot_id3;
	int slot_id4;

	ItemBase slotCast;
	ItemBase slotCast2;
	ItemBase slotCast3;
	ItemBase slotCast4;

	int plankCost;
	int nailCost;
	int metalCost;
	int logCost;
	int wireCost;
	int stoneCost;

	int j_Count;
	int j_Count2;
	int j_Count3;
	int j_Count4;

	void PBDoorsKitBase()
	{
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
	}

	bool hasTheGoodStuff()
	{
		return false;
	}

	string j_Door()
	{
		return "SodaCan_Spite";
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( IsDeploySound() )
		{
			PlayDeploySound();
		}

		if ( CanPlayDeployLoopSound() )
		{
			PlayDeployLoopSound();
		}

		if ( m_DeployLoopSound && !CanPlayDeployLoopSound() )
		{
			StopDeployLoopSound();
		}
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		if ( IsEmpty() )
		{
			return true;
		}
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanReleaseCargo(EntityAI attachment)
	{
		return true;
	}

	void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(10);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
		GetInventory().CreateAttachment( "MetalWire" );
		ItemBase j_goods3 = ItemBase.Cast(GetInventory().CreateAttachment("Stone" ));
		j_goods3.SetQuantity(10);
	}

	//================================================================
	// ADVANCED PLACEMENT
	//================================================================

	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( GetGame().IsServer() && hasTheGoodStuff() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );

			moreDoorKit = GetGame().CreateObject( this.j_Door(), position, false );
			moreDoorKit.SetPosition( position );
			moreDoorKit.SetOrientation( orientation );

			this.Delete();
		}

		SetIsDeploySound( true );
		SetLifetime(1209600);
	}

	override bool IsDeployable()
	{
		return true;
	}

	override string GetDeploySoundset()
	{
		return "putDown_FenceKit_SoundSet";
	}

	override string GetLoopDeploySoundset()
	{
		return "BarbedWire_Deploy_loop_SoundSet";
	}

	void PlayDeployLoopSound()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			m_DeployLoopSound = SEffectManager.PlaySound( GetLoopDeploySoundset(), GetPosition() );
		}
	}

	void StopDeployLoopSound()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			m_DeployLoopSound.SoundStop();
			delete m_DeployLoopSound;
		}
	}

	override bool IsContainer()
	{
		return true;
	}

	// override bool IsHeavyBehaviour()
	// {
	// 	return true;
	// }

	// override bool IsTwoHandedBehaviour()
	// {
	// 	return true;
	// }

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
};

//KIT DOOR PLANK - T1
class PB_PlankDoor_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 10;
		nailCost = 20;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails20);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks10);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_PlankDoor";
		}
		return "PB_PlankDoor_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(5);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
		GetInventory().CreateAttachment( "MetalWire" );
	}
};

//KIT DOOR WOOD - T2
class PB_WoodDoor_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 18;
		nailCost = 36;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails36);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks18);
		slot_id3 = InventorySlots.GetSlotIdFromString(slotWire);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL && slotCast3 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_WoodDoor";
		}
		return "PB_WoodDoor_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(9);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(16);
		GetInventory().CreateAttachment( "MetalWire" );
	}
};

//KIT DOOR METAL - T3
class PB_MetalDoor_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 20;
		nailCost = 75;
		metalCost = 16;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails75);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks);
		slot_id3 = InventorySlots.GetSlotIdFromString(slotWire);
		slot_id4 = InventorySlots.GetSlotIdFromString(slotMetal16);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		slotCast4 = ItemBase.Cast( GetInventory().FindAttachment(slot_id4) );

		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();
		j_Count4 = slotCast4.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL && slotCast3 != NULL && slotCast4 != NULL)
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost && j_Count4 >=metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_MetalDoor";
		}
		return "PB_MetalDoor_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(10);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(45);
		ItemBase j_goods3 = ItemBase.Cast(GetInventory().CreateAttachment("PB_SmallMetalPlate" ));
		j_goods3.SetQuantity(6);
		GetInventory().CreateAttachment( "MetalWire" );
	}
};

//KIT PLANK BARRICADE - T1
class PB_PlankBarricade_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 10;
		nailCost = 20;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails20);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks10);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_PlankBarricade";
		}
		return "PB_PlankBarricade_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(5);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
	}
};

//KIT WOOD WALL - T1
class PB_WoodWall_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		logCost = 2;
		nailCost = 36;
		//stoneCost = 40;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails36);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotLogs);
		//slot_id3 = InventorySlots.GetSlotIdFromString(slotStone);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		//slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();
		//j_Count3 = slotCast3.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= logCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_WoodWall";
		}
		return "PB_WoodWall_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(16);
		//ItemBase j_goods3 = ItemBase.Cast(GetInventory().CreateAttachment("Stone" ));
		//j_goods3.SetQuantity(20);
	}
};

//KIT PLANK DOOR BARRICADE - T1
class PB_PlankDoorBarricade_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 10;
		nailCost = 20;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails20);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks10);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_PlankDoorBarricade";
		}
		return "PB_PlankDoorBarricade_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(5);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
	}
};

//KIT WELL
class PB_Well_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		logCost = 2;
		nailCost = 36;
		stoneCost = 40;
		plankCost = 20;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails36);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotLogs);
		slot_id3 = InventorySlots.GetSlotIdFromString(slotStone);
		slot_id4 = InventorySlots.GetSlotIdFromString(slotPlanks);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		slotCast4 = ItemBase.Cast( GetInventory().FindAttachment(slot_id4) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();
		j_Count3 = slotCast3.GetQuantity();
		j_Count4 = slotCast4.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL && slotCast3 != NULL && slotCast4 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= logCost && j_Count3 >= stoneCost && j_Count4 >= plankCost)
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_Well";
		}
		return "PB_Well_Kit";
	}

	/*override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(8);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(16);
		ItemBase j_goods3 = ItemBase.Cast(GetInventory().CreateAttachment("Stone" ));
		j_goods3.SetQuantity(18);
	}*/
};

//KIT JANELA PLANK - T1
class PB_PlankWindow_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 10;
		nailCost = 20;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails20);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks10);
		slot_id3 = InventorySlots.GetSlotIdFromString(slotWire);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL && slotCast3 != NULL )
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_PlankWindow";
		}
		return "PB_PlankWindow_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(5);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
		GetInventory().CreateAttachment( "MetalWire" );
	}
};

//KIT - WOOD GATE
class PB_WoodGate_Kit extends PBDoorsKitBase
{
	override bool hasTheGoodStuff()
	{
		plankCost = 20;
		nailCost = 36;
		logCost = 2;

		slot_id = InventorySlots.GetSlotIdFromString(slotNails36);
		slot_id2 = InventorySlots.GetSlotIdFromString(slotPlanks);
		slot_id3 = InventorySlots.GetSlotIdFromString(slotWire);
		slot_id4 = InventorySlots.GetSlotIdFromString(slotLogs);
		slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
		slotCast2 = ItemBase.Cast( GetInventory().FindAttachment(slot_id2) );
		slotCast3 = ItemBase.Cast( GetInventory().FindAttachment(slot_id3) );
		slotCast4 = ItemBase.Cast( GetInventory().FindAttachment(slot_id4) );
		j_Count = slotCast.GetQuantity();
		j_Count2 = slotCast2.GetQuantity();
		j_Count4 = slotCast4.GetQuantity();

		if( slotCast != NULL && slotCast2 != NULL && slotCast3 != NULL && slotCast4 != NULL)
		{
			if( j_Count >= nailCost && j_Count2 >= plankCost && j_Count4 >= logCost)
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override string j_Door()
	{
		if ( hasTheGoodStuff() )
		{
			return "PB_WoodGate";
		}
		return "PB_WoodGate_Kit";
	}

	override void ReturnGoodsFromDismantle()
	{
		ItemBase j_goods = ItemBase.Cast(GetInventory().CreateAttachment("WoodenPlank" ));
		j_goods.SetQuantity(10);
		ItemBase j_goods2 = ItemBase.Cast(GetInventory().CreateAttachment("Nail" ));
		j_goods2.SetQuantity(10);
		GetInventory().CreateAttachment( "MetalWire" );
	}
};