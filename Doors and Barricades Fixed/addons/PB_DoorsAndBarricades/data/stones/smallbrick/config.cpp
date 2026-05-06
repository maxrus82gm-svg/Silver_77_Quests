class CfgPatches
{
	class PB_Brick
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class PB_Brick: Inventory_Base
	{
		displayName="#STR_PBBrick";
		descriptionShort="";
		scope=2;
		visibilityModifier=0.60000002;
		model="PB_DoorsAndBarricades\data\Stones\SmallBrick\textures\brick.p3d";
		weight=500;
		itemSize[]={3,3};
		canBeSplit=1;
		varQuantityDestroyOnMin=1;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=32;
		varStackMax=4;
		absorbency=0.2;
		physLayer="item_large";
		rotationFlags=16;
		inventorySlot[]=
		{
			"Slot_Brick",
			"Slot_Brick18",
			"Slot_Brick10"
		};
	};
};
class CfgSlots
{
	class Slot_Brick
	{
		name="Slot_Brick";
		displayName="#STR_PBBrick";
		ghostIcon="set:dayz_inventory image:stones";
		stackMax=40;
	};
	class Slot_Brick18
	{
		name="Slot_Brick18";
		displayName="#STR_PBBrick";
		ghostIcon="set:dayz_inventory image:stones";
		stackMax=18;
	};
	class Slot_Brick10
	{
		name="Slot_Brick10";
		displayName="#STR_PBBrick";
		ghostIcon="set:dayz_inventory image:stones";
		stackMax=10;
	};
};
