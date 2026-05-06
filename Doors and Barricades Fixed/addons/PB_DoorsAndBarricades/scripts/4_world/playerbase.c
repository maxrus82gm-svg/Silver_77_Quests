modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
    {
        super.SetActions(InputActionMap);

		AddAction(ActionDestroyT1, InputActionMap);
		AddAction(ActionDestroyT2, InputActionMap);
        AddAction(ActionDestroyT3, InputActionMap);
	}
};

modded class ModItemRegisterCallbacks
{
    override void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
        super.RegisterHeavy( pType, pBehavior );     
        pType.AddItemInHandsProfileIK("PBDoorsKitBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");
        pType.AddItemInHandsProfileIK("PB_Stone", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
        pType.AddItemInHandsProfileIK("PB_SmallMetalPlate", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/metal_sheet.anm");
    };
    override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
        super.RegisterTwoHanded(pType, pBehavior);
		pType.AddItemInHandsProfileIK("PB_Brick", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/stone.anm");
	}
};  