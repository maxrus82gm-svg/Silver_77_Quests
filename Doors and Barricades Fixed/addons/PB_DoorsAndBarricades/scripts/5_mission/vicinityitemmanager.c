modded class VicinityItemManager
{
    override bool IsObstructed (Object filtered_object)
    {
        if ( filtered_object.CanUseConstruction() )
        {
            PBDoorsBase moredoor = PBDoorsBase.Cast( filtered_object );
            if ( moredoor )
            {
                return false;
            }
        }
        
        return super.IsObstructed (filtered_object);
    }
};