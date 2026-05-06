class CfgPatches
{
	class PB_SmallMetalPlate
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
	class PB_SmallMetalPlate: Inventory_Base
	{
		scope=2;
		displayName="#STR_PBSmallMetalPlate0";
		descriptionShort="#STR_PBSmallMetalPlate1";
		model="PB_DoorsAndBarricades\data\Metal\SmallMetalPlate\SmallPlate.p3d";
		itemSize[]={5,5};
		inventorySlot[]=
		{
			"MetalSheets1",
			"MetalSheets2",
			"MetalSheets3",
			"MetalSheets4"
		};
		rotationFlags=0;
		weight=1200;
		itemBehaviour=0;
		canBeSplit=1;
		varQuantityInit=4;
		varQuantityMin=0;
		varQuantityMax=40;
		varQuantityDestroyOnMin=1;
		varStackMax=8;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
				};
			};
		};
	};
};
