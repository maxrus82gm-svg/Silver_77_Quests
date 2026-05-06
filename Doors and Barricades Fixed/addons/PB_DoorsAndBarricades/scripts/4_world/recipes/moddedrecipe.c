modded class PluginRecipesManagerBase
{
	override void RegisterRecipies()
	{
		super.RegisterRecipies();
		RegisterRecipe(new CraftPlankDoorT1_PB);	
		RegisterRecipe(new CraftWoodDoorT2_PB);
		RegisterRecipe(new CraftMetalDoorT3_PB);
		RegisterRecipe(new CraftDoorBarricadeT1_PB);
		RegisterRecipe(new CraftPlankWindowBarricadeT1_PB);
		RegisterRecipe(new CraftWallWoodKitT1_PB);
		RegisterRecipe(new CraftWellKit_PB);
		RegisterRecipe(new CraftWindowPlankT1_PB);
		RegisterRecipe(new CraftWoodGateT1_PB);
		RegisterRecipe(new CraftSmallMetal_PB);
		RegisterRecipe(new SplitStones_PB);
		RegisterRecipe(new SplitStoneWithStone_PB);
	}
};