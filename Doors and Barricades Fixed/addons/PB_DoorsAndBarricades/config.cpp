class CfgPatches
{
	class PB_DoorsAndBarricades
	{
		units[]=
		{
			"PB_PlankDoor_Kit",
			"PB_WoodDoor_Kit",
			"PB_MetalDoor_Kit",
			"PB_PlankBarricade_Kit",
			"PB_PlankDoorBarricade_Kit",
			"PB_Well_Kit",
			"PB_PlankWindow_Kit",
			"PB_WoodGate_Kit",
			"PB_WoodWall_Kit",
			"PB_PlankDoor",
			"PB_WoodDoor",
			"PB_MetalDoor",
			"PB_PlankBarricade",
			"PB_MetalBarricade",
			"PB_BrickBarricade",
			"PB_PlankDoorBarricade",
			"PB_MetalDoorBarricade",
			"PB_BrickDoorBarricade",
			"PB_Well",
			"PB_PlankWindow",
			"PB_MetalWindow",
			"PB_WoodGate",
			"PB_MetalGate",
			"PB_WoodWall",
			"PB_MetalWall"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Gear_Camping",
			"DZ_Scripts",
			"DZ_Gear_Tools",
			"DZ_Gear_Containers",
			"DZ_Weapons_Melee",
			"DZ_Gear_Medical",
			"DZ_Gear_Consumables",
			"CodeLock",
			"GDZ_Codelock"
		};
	};
};
class CfgMods
{
	class PB_DoorsAndBarricades
	{
		type="mod";
		author="spahgettah";
		dir="PB_DoorsAndBarricades";
		name="PB_DoorsAndBarricades";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PB_DoorsAndBarricades/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PB_DoorsAndBarricades/Scripts/Common",
					"PB_DoorsAndBarricades/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PB_DoorsAndBarricades/Scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Container_Base;
	class Fence;
	class CombinationLock: Inventory_Base
	{
		/*
		inventorySlot[]+=
		{
			"Dragunov_1_18_FIX"
		};
		*/
	};
	class CodeLock: CombinationLock
	{
		inventorySlot[]+=
		{
			"Dragunov_1_18_FIX",
			"Att_CombinationLock"
		};
	};
	class PBDoorsBase: Fence
	{
		scope=0;
		displayName="#STR_PBDoor";
		descriptionShort="$STR_CfgVehicles_Fence1";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		handheld="false";
		lootCategory="Crafted";
		carveNavmesh=1;
		weight=10000;
		itemSize[]={2,3};
		hiddenSelections[]=
		{
			"door1",
			"door2",
			"door3",
			"door4"
		};
		physLayer="item_large";
		createProxyPhysicsOnInit="false";
		createdProxiesOnInit[]=
		{
			"Deployed"
		};
		rotationFlags=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
		attachments[]=
		{
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
			class Attachments
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Attachments";
				description="";
				attachmentSlots[]=
				{
					"Wall_Camonet",
					"Wall_Barbedwire_1",
					"Wall_Barbedwire_2"
				};
				icon="cat_bb_attachments";
				selection="wall";
			};
			class Material
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Materials";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks",
					"Material_MetalSheets",
					"Material_MetalWire"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class AnimRotate
			{
				source="user";
				component="Door1";
				animPeriod=1;
				initPhase=0;
				initOpened=0.5;
			};
			class Wall_Interact_Rotate: AnimRotate
			{
			};
			class Wall_Barbedwire_1_Mounted_Rotate: AnimRotate
			{
			};
			class Wall_Barbedwire_2_Mounted_Rotate: AnimRotate
			{
			};
			class Wall_Camonet_Rotate: AnimRotate
			{
			};
			class Wall_Gate_Rotate: AnimRotate
			{
			};
			class Wall_Base_Down_Rotate: AnimRotate
			{
			};
			class Wall_Base_Up_Rotate: AnimRotate
			{
			};
			class Wall_Wood_Down_Rotate: AnimRotate
			{
			};
			class Wall_Wood_Up_Rotate: AnimRotate
			{
			};
			class Wall_Metal_Down_Rotate: AnimRotate
			{
			};
			class Wall_Metal_Up_Rotate: AnimRotate
			{
			};
			class Material_WoodenLogs: AnimSourceHidden
			{
			};
			class Material_MetalWire: AnimSourceHidden
			{
			};
			class Wall_Barbedwire_1: AnimSourceHidden
			{
			};
			class Wall_Barbedwire_1_Mounted: AnimSourceHidden
			{
			};
			class Wall_Barbedwire_2: AnimSourceHidden
			{
			};
			class Wall_Barbedwire_2_Mounted: AnimSourceHidden
			{
			};
			class Wall_Camonet: AnimSourceHidden
			{
			};
			class Deployed: AnimSourceHidden
			{
			};
			class Base: AnimSourceHidden
			{
			};
			class Wall_Platform: AnimSourceHidden
			{
			};
			class Wall_Platform_Stairs_Left: AnimSourceHidden
			{
			};
			class Wall_Platform_Stairs_Right: AnimSourceHidden
			{
			};
			class Wall_Gate: AnimSourceHidden
			{
			};
			class Wall_Base_Down: AnimSourceHidden
			{
			};
			class Wall_Base_Up: AnimSourceHidden
			{
			};
			class Wall_Metal_Down: AnimSourceHidden
			{
			};
			class Wall_Metal_Up: AnimSourceHidden
			{
			};
			class Wall_Wood_Down: AnimSourceHidden
			{
			};
			class Wall_Wood_Up: AnimSourceHidden
			{
			};
			class Windown2: AnimRotate
			{
			};
			class Fechadura: AnimRotate
			{
			};
		};
		class Construction
		{
			class wall
			{
				class wall_gate
				{
					name="$STR_CfgVehicles_Construction_Part_Gate";
					is_gate=1;
					id=5;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=1;
					dismantle_action_type=1;
					material_type=5;
				};
			};
		};
		class PointLights
		{
			class PointLight
			{
				color[]={1,1,1,0.050000001};
				ambient[]={0.0099999998,0.0099999998,0.0099999998,0.0099999998};
				position="light";
				hitpoint="bulb";
				selection="bulb";
				size=0;
				radius=5;
				brightness=0.001;
				dayLight=1;
				heatHazeRadius=0;
				heatHazePower=0;
				fireEffect=0;
				fireEffectOctaves=0;
				fireEffectPersistence=0;
				fireEffectFract=0;
			};
		};
	};
	class PBDoorsKitBase: Container_Base
	{
		scope=0;
		carveNavmesh=1;
		itemSize[]={6,3};
		itemBehaviour=0;
		heavyItem=1;
		physLayer="item_large";
		simulation="inventoryItem";
		rotationFlags=2;
		isMeleeWeapon=0;
		hiddenSelectionsTextures[]={};
		hiddenSelectionsMaterials[]={};
		handheld="true";
		weight=3000;
		useEntityHierarchy="true";
		placement="ForceSlopeOnTerrain";
		absorbency=0;
		attachments[]=
		{
			"Material_Nails",
			"Material_WoodenPlanks",
			"Material_MetalSheets",
			"Material_WoodenLogs",
			"Material_MetalWire",
			"Slot_Brick"
		};
	};
	class PB_PlankDoor_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PlankDoorKit0";
		descriptionShort="#STR_PlankDoorKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W1_Nails",
			"Material_L2W2_WoodenPlanks",
			"Material_MetalWire"
		};
	};
	class PB_WoodDoor_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBWoodDoorKit0";
		descriptionShort="#STR_PBWoodDoorKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W2_Nails",
			"Material_L2W3_WoodenPlanks",
			"Material_MetalWire"
		};
	};
	class PB_MetalDoor_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBMetalDoorKit0";
		descriptionShort="#STR_PBMetalDoorKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W3_Nails",
			"Material_WoodenPlanks",
			"MetalSheets3",
			"Material_MetalWire"
		};
	};
	class PB_PlankBarricade_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBPlankBarricadeKit0";
		descriptionShort="#STR_PBPlankBarricadeKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W1_Nails",
			"Material_L2W2_WoodenPlanks"
		};
	};
	class PB_WoodWall_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBWoodWallKit0";
		descriptionShort="#STR_PBWoodWallKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W2_Nails",
			"Material_WoodenLogs"
		};
	};
	class PB_PlankDoorBarricade_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBPlankDoorBarricadeKit0";
		descriptionShort="#STR_PBPlankDoorBarricadeKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W1_Nails",
			"Material_L2W2_WoodenPlanks"
		};
	};
	class PB_Well_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBWellKit0";
		descriptionShort="Brick: 40 | Nails: 36 | WoodenLog: 2 | Planks: 20 | Items must be exact, otherwise you will lose them after placing.";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W2_Nails",
			"Material_WoodenPlanks",
			"Material_WoodenLogs",
			"Slot_Brick"
		};
	};
	class PB_WoodGate_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBWoodGateKit0";
		descriptionShort="#STR_PBWoodGateKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W2_Nails",
			"Material_WoodenLogs",
			"Material_MetalWire",
			"Material_WoodenPlanks"
		};
	};
	class PB_PlankWindow_Kit: PBDoorsKitBase
	{
		scope=2;
		displayName="#STR_PBPlankWindowKit0";
		descriptionShort="#STR_PBPlankWindowKit1";
		model="\DZ\gear\camping\wooden_case.p3d";
		attachments[]=
		{
			"Material_L2W1_Nails",
			"Material_L2W2_WoodenPlanks",
			"Material_MetalWire"
		};
	};
	class PB_PlankDoor: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBPlankDoor";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Wood\WoodPlankDoor.p3d";
		attachments[]=
		{
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
	class PB_WoodDoor: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBWoodDoor";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Wood\WoodDoor.p3d";
		attachments[]=
		{
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
	class PB_MetalDoor: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalDoor";
		descriptionShort="| T3 |";
		model="PB_DoorsAndBarricades\data\Metal\MetalDoor.p3d";
		attachments[]=
		{
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
	class PB_PlankBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBPlankBarricade";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Wood\WoodBarricade.p3d";
		attachments[]=
		{
			"MetalSheets1"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"MetalSheets1"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_MetalBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalBarricade";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Metal\MetalBarricade.p3d";
		attachments[]=
		{
			"Slot_Brick10"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"Slot_Brick10"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_BrickBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBBrickBarricade";
		descriptionShort="| T3 |";
		model="PB_DoorsAndBarricades\data\Brick\BrickBarricade.p3d";
		class GUIInventoryAttachmentsProps
		{
		};
	};
	class PB_WoodWall: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBWoodWall";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Wood\WoodWall.p3d";
		attachments[]=
		{
			"MetalSheets4"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"MetalSheets4"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_MetalWall: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalWall";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Metal\MetalWall.p3d";
		attachments[]=
		{
			"Slot_Brick"
		};
		class GUIInventoryAttachmentsProps
		{
		};
	};
	class PB_PlankDoorBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBPlankDoorBarricade";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Wood\WoodDoorBarricade.p3d";
		attachments[]=
		{
			"MetalSheets2"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"MetalSheets2"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_MetalDoorBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalDoorBarricade";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Metal\MetalDoorBarricade.p3d";
		attachments[]=
		{
			"Slot_Brick18"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"Slot_Brick18"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_BrickDoorBarricade: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBBrickDoorBarricade";
		descriptionShort="| T3 |";
		model="PB_DoorsAndBarricades\data\Brick\BrickDoorBarricade.p3d";
		attachments[]=
		{
			"Stones"
		};
		class GUIInventoryAttachmentsProps
		{
		};
	};
	class PB_Well: Inventory_Base
	{
		scope=2;
		displayName="#STR_PBWell0";
		descriptionShort="#STR_PBWell1";
		model="PB_DoorsAndBarricades\data\Well\Well.p3d";
		weight=10000;
		itemSize[]={10,10};
		itemBehaviour=0;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
	class PB_PlankWindow: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBPlankWindow";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Windows\PlankWindow.p3d";
		attachments[]=
		{
			"MetalSheets1",
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"MetalSheets1"
				};
				icon="cat_bb_material";
				selection="wall";
			};
			class Lock
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
	class PB_MetalWindow: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalWindow";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Windows\MetalWindow.p3d";
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
	class PB_WoodGate: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBWoodGate";
		descriptionShort="| T1 |";
		model="PB_DoorsAndBarricades\data\Gates\WoodGate.p3d";
		attachments[]=
		{
			"Dragunov_1_18_FIX",
			"MetalSheets4"
		};
		class GUIInventoryAttachmentsProps
		{
			class Lock
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
			class Base
			{
				name="#STR_PBUpgrade";
				description="";
				attachmentSlots[]=
				{
					"MetalSheets4"
				};
				icon="cat_bb_material";
				selection="wall";
			};
		};
	};
	class PB_MetalGate: PBDoorsBase
	{
		scope=2;
		displayName="#STR_PBMetalGate";
		descriptionShort="| T2 |";
		model="PB_DoorsAndBarricades\data\Gates\MetalGate.p3d";
		attachments[]=
		{
			"Dragunov_1_18_FIX"
		};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Dragunov_1_18_FIX"
				};
				icon="cat_bb_base";
				selection="wall";
			};
		};
	};
};
class CfgSlots
{
	class Slot_MetalSheets1
	{
		name="MetalSheets1";
		displayName="#STR_PBSmallMetalPlate0";
		ghostIcon="set:dayz_inventory image:plates";
		stackMax=4;
	};
	class Slot_MetalSheets2
	{
		name="MetalSheets2";
		displayName="#STR_PBSmallMetalPlate0";
		ghostIcon="set:dayz_inventory image:plates";
		stackMax=8;
	};
	class Slot_MetalSheets3
	{
		name="MetalSheets3";
		displayName="#STR_PBSmallMetalPlate0";
		ghostIcon="set:dayz_inventory image:plates";
		stackMax=16;
	};
	class Slot_MetalSheets4
	{
		name="MetalSheets4";
		displayName="#STR_PBSmallMetalPlate0";
		ghostIcon="set:dayz_inventory image:plates";
		stackMax=24;
	};
	class Slot_Material_L2W2_WoodenPlanks
	{
		stackMax=10;
	};
	class Slot_Material_L2W3_WoodenPlanks
	{
		stackMax=18;
	};
	class Slot_Material_L2W1_Nails
	{
		stackMax=20;
	};
	class Slot_Material_L2W2_Nails
	{
		stackMax=36;
	};
	class Slot_Material_L2W3_Nails
	{
		stackMax=75;
	};
	class Slot_Dragunov_1_18_FIX
	{
		name="Dragunov_1_18_FIX";
		displayName="#STR_SlotDragunov";
		ghostIcon="set:dayz_inventory image:combolock";
	};
};
