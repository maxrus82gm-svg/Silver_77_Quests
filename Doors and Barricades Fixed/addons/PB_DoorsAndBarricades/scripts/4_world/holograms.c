modded class Hologram
{	
	override string ProjectionBasedOnParent()
	{
		PBDoorsKitBase item_in_hands = PBDoorsKitBase.Cast( m_Player.GetHumanInventory().GetEntityInHands() );

		if ( item_in_hands )
		{
			return item_in_hands.j_Door();
		}
		
		return super.ProjectionBasedOnParent();
	}

	EntityAI PlaceEntity( EntityAI entity_for_placing )
	{
		if ( entity_for_placing.IsInherited( PBDoorsKitBase ))
		{
			return entity_for_placing;
		}
		
		return super.PlaceEntity(entity_for_placing);
	}		

	override void EvaluateCollision(ItemBase action_item = null)
	{	
		ItemBase item_in_hands = m_Parent;

		if ( item_in_hands.IsInherited(PBDoorsKitBase))
		{
			SetIsColliding(false);
			return;
		}
			
		super.EvaluateCollision();
	}
}