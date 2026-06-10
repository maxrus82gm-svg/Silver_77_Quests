modded class Fireplace
{
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		bool result = super.CanReceiveAttachment(attachment, slotId);
		Silver77_LogReceive("Fireplace", attachment, slotId, result);
		return result;
	}

	override bool CanLoadAttachment(EntityAI attachment)
	{
		bool result = super.CanLoadAttachment(attachment);
		Silver77_LogLoad("Fireplace", attachment, result);
		return result;
	}

	void Silver77_LogReceive(string ownerType, EntityAI attachment, int slotId, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanReceiveAttachment attachment=" + attachmentType + " slotId=" + slotId + " slot=" + InventorySlots.GetSlotName(slotId) + " result=" + result);
	}

	void Silver77_LogLoad(string ownerType, EntityAI attachment, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanLoadAttachment attachment=" + attachmentType + " result=" + result);
	}
}

modded class FireplaceIndoor
{
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		bool result = super.CanReceiveAttachment(attachment, slotId);
		Silver77_LogReceive("FireplaceIndoor", attachment, slotId, result);
		return result;
	}

	override bool CanLoadAttachment(EntityAI attachment)
	{
		bool result = super.CanLoadAttachment(attachment);
		Silver77_LogLoad("FireplaceIndoor", attachment, result);
		return result;
	}

	void Silver77_LogReceive(string ownerType, EntityAI attachment, int slotId, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanReceiveAttachment attachment=" + attachmentType + " slotId=" + slotId + " slot=" + InventorySlots.GetSlotName(slotId) + " result=" + result);
	}

	void Silver77_LogLoad(string ownerType, EntityAI attachment, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanLoadAttachment attachment=" + attachmentType + " result=" + result);
	}
}

modded class BarrelHoles_ColorBase
{
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		bool result = super.CanReceiveAttachment(attachment, slotId);
		Silver77_LogReceive("BarrelHoles_ColorBase", attachment, slotId, result);
		return result;
	}

	override bool CanLoadAttachment(EntityAI attachment)
	{
		bool result = super.CanLoadAttachment(attachment);
		Silver77_LogLoad("BarrelHoles_ColorBase", attachment, result);
		return result;
	}

	void Silver77_LogReceive(string ownerType, EntityAI attachment, int slotId, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanReceiveAttachment attachment=" + attachmentType + " slotId=" + slotId + " slot=" + InventorySlots.GetSlotName(slotId) + " result=" + result);
	}

	void Silver77_LogLoad(string ownerType, EntityAI attachment, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanLoadAttachment attachment=" + attachmentType + " result=" + result);
	}
}

modded class PortableGasStove
{
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		bool result = super.CanReceiveAttachment(attachment, slotId);
		Silver77_LogReceive("PortableGasStove", attachment, slotId, result);
		return result;
	}

	void Silver77_LogReceive(string ownerType, EntityAI attachment, int slotId, bool result)
	{
		string attachmentType = "<null>";
		if (attachment)
			attachmentType = attachment.GetType();

		Print("[SilverCookingFix][DIAG] " + ownerType + " CanReceiveAttachment attachment=" + attachmentType + " slotId=" + slotId + " slot=" + InventorySlots.GetSlotName(slotId) + " result=" + result);
	}
}

modded class Pot
{
	override bool CanPutAsAttachment(EntityAI parent)
	{
		bool result = super.CanPutAsAttachment(parent);

		string parentType = "<null>";
		if (parent)
			parentType = parent.GetType();

		Print("[SilverCookingFix][DIAG] Pot CanPutAsAttachment parent=" + parentType + " result=" + result);
		return result;
	}
}
