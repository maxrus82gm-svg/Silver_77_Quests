class PBDoorsBase extends Fence
{
	bool HasGate()
	{
		return true;
	}

	override bool HasHinges()
	{
		return true;
	}

	bool IsMoreDoor()
	{
		return true;
	}
	//Teste de Tier de ferramentas de raid
	bool IsTier1()
	{
		return false;
	}
	
	bool IsTier2()
	{
		return false;
	}

	bool IsTier3()
	{
		return false;
	}
	///////
	override void AfterStoreLoad()
	{
		SetGateState( true );
		SetBaseState( true );
	}

	string j_moreDoorKit()
	{
		return "MoreDoorSmallKit";
	}

	bool AllowDismantle()
	{
		return true;
	}

	bool AllowDestruction()
	{
		return true;
	}

	bool IsMetalDoor()
	{
		return false;
	}

	override bool CanCloseFence()
	{
		if ( IsOpened() )
		{
			return true;
		}

		return false;
	}

    override void OpenFence()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{
			SetAnimationPhase( "Windown2", 					1 );
			SetAnimationPhase( "Fechadura", 					1 );
			SetAnimationPhase( "Wall_Gate_Rotate", 					1 );
			SetOpenedState( true );

			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );

			//synchronize
			SynchronizeBaseState();
		}

		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateOpenStart();
		}
	}

	override void CloseFence()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{
			SetAnimationPhase( "Windown2", 					0 );
			SetAnimationPhase( "Fechadura", 					0 );
			SetAnimationPhase( "Wall_Gate_Rotate", 					0 );
			//
			
			//

			SetOpenedState( false );

			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );

			//synchronize
			SynchronizeBaseState();
		}

		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateCloseStart();
		}
	};

	override void SetPartsAfterStoreLoad()
	{
		//update server data
		SetPartsFromSyncData();

		//set base state
		SetBaseState( true ) ;

		//synchronize after load
		SynchronizeBaseState();
	}

	override bool NameOverride(out string output)
    {
        return false;
    }

    override bool CanDisplayAttachmentSlot( string slot_name )
	{
		/*
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;
		
		if ( !GateAttachmentConditions(InventorySlots.GetSlotIdFromString(slot_name)) )
			return false;
		*/
		return true;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return true;
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		/*
		if ( !super.CanReceiveAttachment(attachment, slotId) )
			return false;
		*/
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( player )
			{
				ConstructionActionData construction_action_data = player.GetConstructionActionData();
				
				//reset action initiator
				construction_action_data.SetActionInitiator( NULL );				
			}
		}
		
		if ( attachment.IsInherited( ATTACHMENT_COMBINATION_LOCK ) )
		{
			return ( HasFullyConstructedGate() && !IsOpened() );
		}
		/*
		if ( !GateAttachmentConditions(slotId) )
			return false;
		*/
		return true;
	}
	
	bool GateAttachmentConditions(int slotId)
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionDialCombinationLockOnPBDoors);
		AddAction(ActionNextCombinationLockDialOnPBDoors);
		AddAction(ActionOpenPBDoors);
		AddAction(ActionClosePBDoors);
		RemoveAction(ActionFoldBaseBuildingObject);
		RemoveAction(ActionOpenFence);
		RemoveAction(ActionCloseFence);
	}

};

//PLANK DOOR - T1
class PB_PlankDoor extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankDoor_Kit";
	}

	override bool IsTier1()
	{
		return true;
	}
};

//WOOD DOOR - T2
class PB_WoodDoor extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodDoor_Kit";
	}

	override bool IsTier2()
	{
		return true;
	}
};

//METAL DOOR - T3
class PB_MetalDoor extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_MetalDoor_Kit";
	}

	override bool IsMetalDoor()
	{
		return true;
	}

	override bool IsTier3()
	{
		return true;
	}
};

//PLANK BARRICADE - T1
class PB_PlankBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankBarricade_Kit";
	}

	override void SetActions()
	{

	}

	override bool IsTier1()
	{
		return true;
	}

	bool hasTheGoodStuff()
	{
		int metalCost = 4;

		int slot_id = InventorySlots.GetSlotIdFromString("MetalSheets1");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override void UpdateAttachmentPhysics( string slot_name, bool is_locked )
	{

	}

	override void UpdateAttachmentVisuals( string slot_name, bool is_locked )
	{

	}



};

//METAL BARRICADE - T2
class PB_MetalBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankBarricade_Kit";
	}

	override bool IsTier2()
	{
		return true;
	}

	override void EEItemDetached ( EntityAI item, string slot_name )
    {

    }

	override void SetActions()
	{

	}

	bool hasTheGoodStuff()
	{
		int stoneCost = 10;

		int slot_id = InventorySlots.GetSlotIdFromString("Slot_Brick10");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= stoneCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}
};

//BRICK BARRICADE - T3
class PB_BrickBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankBarricade_Kit";
	}

	override bool IsTier3()
	{
		return true;
	}

	override void EEItemDetached ( EntityAI item, string slot_name )
    {

    }

	override void SetActions()
	{

	}
};

//WOOD WALL - T1
class PB_WoodWall extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodWall_Kit";
	}

	override bool IsTier1()
	{
		return true;
	}

	override void EEItemDetached ( EntityAI item, string slot_name )
    {

    }

	override void SetActions()
	{

	}

	bool hasTheGoodStuff()
	{
		int metalCost = 24;

		int slot_id = InventorySlots.GetSlotIdFromString("MetalSheets4");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}
};

//METAL WALL - T2
class PB_MetalWall extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodWall_Kit";
	}

	override bool IsTier2()
	{
		return true;
	}

	override void EEItemDetached ( EntityAI item, string slot_name )
    {

    }

	override void SetActions()
	{

	}

	bool hasTheGoodStuff()
	{
		int stoneCost = 40;

		int slot_id = InventorySlots.GetSlotIdFromString("Slot_Brick");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= stoneCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}
};

//BRICK WALL - T3
/*class PB_BrickWall extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodWall_Kit";
	}

	override bool IsTier3()
	{
		return true;
	}

	override void EEItemDetached ( EntityAI item, string slot_name )
    {

    }

	override void SetActions()
	{

	}
};*/

//PLANK DOOR BARRICADE - T1
class PB_PlankDoorBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankDoorBarricade_Kit";
	}

	override void SetActions()
	{

	}

	override bool IsTier1()
	{
		return true;
	}

	bool hasTheGoodStuff()
	{
		int metalCost = 8;

		int slot_id = InventorySlots.GetSlotIdFromString("MetalSheets2");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}

	override void UpdateAttachmentPhysics( string slot_name, bool is_locked )
	{

	}

	override void UpdateAttachmentVisuals( string slot_name, bool is_locked )
	{

	}



};

//METAL DOOR BARRICADE - T2
class PB_MetalDoorBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankDoorBarricade_Kit";
	}

	override void SetActions()
	{

	}

	override bool IsTier2()
	{
		return true;
	}

	bool hasTheGoodStuff()
	{
		int stoneCost = 18;

		int slot_id = InventorySlots.GetSlotIdFromString("Slot_Brick18");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= stoneCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}


	override void UpdateAttachmentPhysics( string slot_name, bool is_locked )
	{

	}

	override void UpdateAttachmentVisuals( string slot_name, bool is_locked )
	{

	}
};

//Brick DOOR BARRICADE - T3
class PB_BrickDoorBarricade extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankDoorBarricade_Kit";
	}

	override void SetActions()
	{

	}

	override bool IsTier3()
	{
		return true;
	}

	override void UpdateAttachmentPhysics( string slot_name, bool is_locked )
	{

	}

	override void UpdateAttachmentVisuals( string slot_name, bool is_locked )
	{

	}

};

//Well - Poço de agua
class PB_Well extends ItemBase
{
	override bool IsWell()
	{
		return true;
	}
	
	override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }
	
	override bool CanPutInCargo( EntityAI parent )
	{
        return false;
    }
	
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
        return false;
    }
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionWashHandsWellOne);
		AddAction(ActionDrinkWellContinuous);
	}
};

//JANELA PLANK - T1
class PB_PlankWindow extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankWindow_Kit";
	}

	override bool IsTier1()
	{
		return true;
	}

	bool hasTheGoodStuff()
	{
		int metalCost = 4;

		int slot_id = InventorySlots.GetSlotIdFromString("MetalSheets1");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}
};

//JANELA METAL - T2
class PB_MetalWindow extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_PlankWindow_Kit";
	}

	override bool IsTier2()
	{
		return true;
	}

};

//GATE WOOD - T1
class PB_WoodGate extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodGate_Kit";
	}

	override bool IsTier1()
	{
		return true;
	}

	bool hasTheGoodStuff()
	{
		int metalCost = 24;

		int slot_id = InventorySlots.GetSlotIdFromString("MetalSheets4");
		ItemBase slotCast = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );

		if( slotCast != NULL )
		{
			int j_Count = slotCast.GetQuantity();
			if( j_Count >= metalCost )
			{
				return true;
			}
			return false;
		}
		return false;
	}
};

//GATE METAL - T2
class PB_MetalGate extends PBDoorsBase
{
	override string j_moreDoorKit()
	{
		return "PB_WoodGate_Kit";
	}

	override bool IsTier2()
	{
		return true;
	}
};

//METAL PEQUENO - SMALL METAL PLATE
class PB_SmallMetalPlate extends Inventory_Base
{
	override bool CanPutInCargo( EntityAI parent )
    {
        return true;
    }

	override bool CanPutIntoHands(EntityAI parent)
    {
        return true;
    }
};
 