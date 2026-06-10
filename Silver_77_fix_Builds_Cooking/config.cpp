#define _ARMA_

class CfgPatches
{
	class Silver_77_fix_Builds_Cooking
	{
		units[] = {"Silver77_FixedPot","Silver77_FixedFryingPan"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Cooking","dz_aod_assets_part1","KKCR_Liquids"};
	};
};

class CfgMods
{
	class Silver_77_fix_Builds_Cooking
	{
		dir = "Silver_77_fix_Builds_Cooking";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "Silver_77_fix_Builds_Cooking";
		credits = "Silver77";
		author = "Silver77";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"World","Mission"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"Silver_77_fix_Builds_Cooking/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"Silver_77_fix_Builds_Cooking/scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class Inventory_Base;
	class Bottle_Base;
	class FryingPan;
	class FireplaceBase;

	class Fireplace: FireplaceBase
	{
		attachments[] = {"Firewood","WoodenStick","CookingEquipment","Rags","MedicalBandage","Paper","Bark","CookingTripod","Stones","DirectCookingA","DirectCookingB","DirectCookingC","SmokingA","SmokingB","SmokingC","SmokingD"};
		class GUIInventoryAttachmentsProps
		{
			class CookingEquipment
			{
				name = "$STR_attachment_CookingEquipment0";
				description = "";
				attachmentSlots[] = {"CookingTripod","CookingEquipment"};
				icon = "set:dayz_inventory image:cat_fp_cooking";
				view_index = 1;
			};
			class Smoking
			{
				name = "$STR_attachment_Smoking";
				description = "";
				attachmentSlots[] = {"SmokingA","SmokingB","SmokingC","SmokingD"};
				icon = "set:dayz_inventory image:cookingequipment";
				view_index = 2;
			};
			class DirectCooking
			{
				name = "$STR_attachment_CookingEquipment0";
				description = "";
				attachmentSlots[] = {"DirectCookingA","DirectCookingB","DirectCookingC"};
				icon = "set:dayz_inventory image:cookingequipment";
				view_index = 3;
			};
			class Upgrade
			{
				name = "$STR_attachment_Upgrade0";
				description = "";
				attachmentSlots[] = {"Stones"};
				icon = "set:dayz_inventory image:cat_fp_upgrade";
				view_index = 4;
			};
			class Fuel
			{
				name = "$STR_attachment_Fuel0";
				description = "";
				attachmentSlots[] = {"Firewood","WoodenStick"};
				icon = "set:dayz_inventory image:cat_fp_fuel";
				view_index = 5;
			};
			class Kindling
			{
				name = "$STR_attachment_Kindling0";
				description = "";
				attachmentSlots[] = {"Rags","MedicalBandage","Paper","Bark"};
				icon = "set:dayz_inventory image:cat_fp_kindling";
				view_index = 6;
			};
		};
	};

	class FireplaceIndoor: FireplaceBase
	{
		attachments[] = {"Firewood","WoodenStick","Rags","MedicalBandage","Paper","Bark","DirectCookingA","DirectCookingB","SmokingA","SmokingB","SmokingC","SmokingD"};
		class GUIInventoryAttachmentsProps
		{
			class Smoking
			{
				name = "$STR_attachment_Smoking";
				description = "";
				attachmentSlots[] = {"SmokingA","SmokingB","SmokingC","SmokingD"};
				icon = "set:dayz_inventory image:cookingequipment";
			};
			class CookingEquipment
			{
				name = "$STR_attachment_CookingEquipment0";
				description = "";
				attachmentSlots[] = {"DirectCookingA","DirectCookingB"};
				icon = "set:dayz_inventory image:cookingequipment";
			};
			class Fuel
			{
				name = "$STR_attachment_Fuel0";
				description = "";
				attachmentSlots[] = {"Firewood","WoodenStick"};
				icon = "set:dayz_inventory image:cat_fp_fuel";
			};
			class Kindling
			{
				name = "$STR_attachment_Kindling0";
				description = "";
				attachmentSlots[] = {"Rags","MedicalBandage","Paper","Bark"};
				icon = "set:dayz_inventory image:cat_fp_kindling";
			};
		};
	};

	class BarrelHoles_ColorBase: FireplaceBase
	{
		inventorySlot[] = {"Truck_01_Barrel1","Truck_01_Barrel2","Truck_01_Barrel3","Truck_01_Barrel4"};
		attachments[] = {"Firewood","WoodenStick","Rags","MedicalBandage","Paper","Bark","DirectCookingA","DirectCookingB","DirectCookingC","SmokingA","SmokingB","SmokingC","SmokingD"};
		class GUIInventoryAttachmentsProps
		{
			class Smoking
			{
				name = "$STR_attachment_Smoking";
				description = "";
				attachmentSlots[] = {"SmokingA","SmokingB","SmokingC","SmokingD"};
				icon = "set:dayz_inventory image:cookingequipment";
			};
			class CookingEquipment
			{
				name = "$STR_attachment_CookingEquipment0";
				description = "";
				attachmentSlots[] = {"DirectCookingA","DirectCookingB","DirectCookingC"};
				icon = "set:dayz_inventory image:cookingequipment";
			};
			class Fuel
			{
				name = "$STR_attachment_Fuel0";
				description = "";
				attachmentSlots[] = {"Firewood","WoodenStick"};
				icon = "set:dayz_inventory image:cat_fp_fuel";
			};
			class Kindling
			{
				name = "$STR_attachment_Kindling0";
				description = "";
				attachmentSlots[] = {"Rags","MedicalBandage","Paper","Bark"};
				icon = "set:dayz_inventory image:cat_fp_kindling";
			};
		};
	};

	class Silver77_FixedPot: Bottle_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Pot0";
		descriptionShort = "$STR_CfgVehicles_Pot1";
		model = "\dz\gear\cooking\CookingPot.p3d";
		weight = 1000;
		itemSize[] = {4,3};
		itemsCargoSize[] = {4,3};
		inventorySlot[] = {"CookingEquipment","DirectCookingA","DirectCookingB","DirectCookingC"};
		stackedRandom = 0;
		canBeDigged = 1;
		allowOwnedCargoManipulation = 1;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 2097152 + 4194304 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 2000;
		varTemperatureMax = 200;
		varTemperatureFreezePoint = -200;
		varTemperatureThawPoint = -200;
		varTemperatureFreezeTime = 3300;
		varTemperatureThawTime = 3300;
		varTemperatureOverheatTime = -1;
		temperaturePerQuantityWeight = 6;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\gear\cooking\data\cooking_pot.rvmat"}},{0.7,{"DZ\gear\cooking\data\cooking_pot.rvmat"}},{0.5,{"DZ\gear\cooking\data\cooking_pot_damage.rvmat"}},{0.3,{"DZ\gear\cooking\data\cooking_pot_damage.rvmat"}},{0.0,{"DZ\gear\cooking\data\cooking_pot_destruct.rvmat"}}};
				};
			};
		};
		repairableWithKits[] = {10};
		repairCosts[] = {25.0};
		class AnimationSources
		{
			class handleRotate
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		soundImpactType = "metal";
	};

	class Silver77_FixedFryingPan: FryingPan
	{
		scope = 2;
		inventorySlot[] = {"CookingEquipment","DirectCookingA","DirectCookingB","DirectCookingC"};
	};

	class Tripod: Inventory_Base
	{
		inventorySlot[] = {"CookingTripod","Backpack_1"};
	};

	class Tripod_Proxy_Test: Inventory_Base
	{
		inventorySlot[] = {"CookingTripod"};
	};
};

class CfgNonAIVehicles
{
	class ProxyAttachment;

	class ProxyCookingPot: ProxyAttachment
	{
		inventorySlot[] = {"CookingEquipment"};
	};

	class Proxydirect_cooking_a: ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"DirectCookingA"};
	};

	class Proxydirect_cooking_b: ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"DirectCookingB"};
	};

	class Proxydirect_cooking_c: ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"DirectCookingC"};
	};

	class Proxytripod_deployed: ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"CookingTripod"};
	};

	class ProxyBook_kniga: ProxyAttachment
	{
		inventorySlot[] = {"Book"};
	};
};
