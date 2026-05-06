class CfgPatches
{
	class PB_Stone
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
	class PB_Stone: Inventory_Base
	{
		displayName="#STR_PBStone";
		descriptionShort="";
		scope=2;
		visibilityModifier=0.60000002;
		model="PB_DoorsAndBarricades\data\Stones\Stone\textures\stone.p3d";
		weight=50000;
		itemSize[]={5,5};
		itemBehaviour=0;
		canBeSplit=0;
		varQuantityDestroyOnMin=1;
		varQuantityInit=1;
		varQuantityMin=1;
		varQuantityMax=1;
		varStackMax=1;
		absorbency=0.1;
		physLayer="item_large";
		rotationFlags=12;
	};
};
