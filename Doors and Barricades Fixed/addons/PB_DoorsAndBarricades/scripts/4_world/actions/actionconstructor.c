
modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert( ActionNextCombinationLockDialOnPBDoors);
        actions.Insert( ActionClosePBDoors);
        actions.Insert( ActionOpenPBDoors);
        actions.Insert( ActionDialCombinationLockOnPBDoors);
        actions.Insert( ActionDismantlePBDoors);
        actions.Insert( ActionDestroyCombinationLockPBDoors);
        //
        actions.Insert( ActionUpgradeBarricadePB);
        actions.Insert( ActionUpgradeBarricadePBT3 );
        //
        actions.Insert( ActionUpgradeDoorBarricade );
        actions.Insert( ActionUpgradeDoorBarricadeT3 );
        //
        actions.Insert( ActionUpgradeWindow );
        //
        actions.Insert( ActionDestroyT1);
        actions.Insert( ActionDestroyT2);
        actions.Insert( ActionDestroyT3 );
        //
        actions.Insert( ActionDismantleWell );
        //
        actions.Insert( ActionUpgradeWall );
        //
        actions.Insert( ActionUpgradeGate );
        //actions.Insert( ActionUpgradeWallT3 );
        //actions.Insert( ActionDisableMoreDoorDismantle);
    }
};
