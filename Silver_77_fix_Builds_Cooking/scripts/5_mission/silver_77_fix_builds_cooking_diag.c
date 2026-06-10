modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		Print("[SilverCookingFix][DIAG] ===== Cooking config runtime dump =====");

		Silver77_DumpTextArray("CfgVehicles Pot inventorySlot");
		Silver77_DumpTextArray("CfgVehicles Silver77_FixedPot inventorySlot");
		Silver77_DumpTextArray("CfgVehicles KKCR_Potty inventorySlot");
		Silver77_DumpTextArray("CfgVehicles KKCR_TeaPot inventorySlot");

		Silver77_DumpTextArray("CfgVehicles Fireplace attachments");
		Silver77_DumpTextArray("CfgVehicles Fireplace GUIInventoryAttachmentsProps CookingEquipment attachmentSlots");
		Silver77_DumpTextArray("CfgVehicles Fireplace GUIInventoryAttachmentsProps DirectCooking attachmentSlots");

		Silver77_DumpTextArray("CfgVehicles FireplaceIndoor attachments");
		Silver77_DumpTextArray("CfgVehicles FireplaceIndoor GUIInventoryAttachmentsProps CookingEquipment attachmentSlots");

		Silver77_DumpTextArray("CfgVehicles BarrelHoles_ColorBase attachments");
		Silver77_DumpTextArray("CfgVehicles BarrelHoles_ColorBase GUIInventoryAttachmentsProps CookingEquipment attachmentSlots");

		Silver77_DumpSlot("CookingEquipment");
		Silver77_DumpSlot("DirectCookingA");
		Silver77_DumpSlot("DirectCookingB");
		Silver77_DumpSlot("DirectCookingC");
	}

	void Silver77_DumpTextArray(string path)
	{
		TStringArray values = new TStringArray;
		GetGame().ConfigGetTextArray(path, values);

		string textValue;
		GetGame().ConfigGetText(path, textValue);

		Print("[SilverCookingFix][DIAG] " + path + " arrayCount=" + values.Count() + " text='" + textValue + "'");

		for (int i = 0; i < values.Count(); i++)
		{
			Print("[SilverCookingFix][DIAG]   [" + i + "] " + values[i]);
		}
	}

	void Silver77_DumpSlot(string slotName)
	{
		int slotId = InventorySlots.GetSlotIdFromString(slotName);
		Print("[SilverCookingFix][DIAG] Slot " + slotName + " id=" + slotId);
	}
}
