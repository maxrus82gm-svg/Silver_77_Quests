modded class ItemBase
{
	CodeLock GetCodeLock()
	{
		ItemBase A;
		ItemBase B;
		A = FindAttachmentBySlotName("Att_CombinationLock");
		B = FindAttachmentBySlotName("Dragunov_1_18_FIX");
        if(A || B)
		{
			if(A)
			{
				return CodeLock.Cast(FindAttachmentBySlotName("Att_CombinationLock"));
			}
			else if(B)
			{
				return CodeLock.Cast(FindAttachmentBySlotName("Dragunov_1_18_FIX"));
			}
		} 
		return null;
	}
}