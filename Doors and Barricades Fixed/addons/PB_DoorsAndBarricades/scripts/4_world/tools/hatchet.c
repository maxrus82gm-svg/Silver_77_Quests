modded class Hatchet extends ToolBase
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDismantlePBDoors);
	}
};

modded class Crowbar
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDismantlePBDoors);
	}
};

modded class Hammer
{
	override void SetActions()
	{
		super.SetActions();
		//BARRICADA DE JANELAS
		AddAction(ActionUpgradeBarricadePB);
		AddAction(ActionUpgradeBarricadePBT3);
		//BARRICADA DE PORTA
		AddAction(ActionUpgradeDoorBarricade);
		AddAction(ActionUpgradeDoorBarricadeT3);
		//JANELAS
		AddAction(ActionUpgradeWindow);
		//PAREDES
		AddAction(ActionUpgradeWall);
		//AddAction(ActionUpgradeWallT3);
		//GATES
		AddAction(ActionUpgradeGate);
	}
};

modded class Shovel
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDismantleWell);
	}
};
