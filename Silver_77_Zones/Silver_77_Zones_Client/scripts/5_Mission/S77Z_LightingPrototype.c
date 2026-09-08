enum S77ZLightingPrototypeState
{
    S77Z_LIGHTING_UNINITIALIZED,
    S77Z_LIGHTING_OUTSIDE,
    S77Z_LIGHTING_TRANSITION,
    S77Z_LIGHTING_INNER
}

modded class MissionGameplay
{
    protected const string S77Z_LIGHTING_PROFILE = "dz\\data\\lighting\\lighting_underground.txt";
    protected const float S77Z_ZONE_FORWARD_DISTANCE = 25.0;
    protected const float S77Z_ZONE_OUTER_RADIUS = 20.0;
    protected const float S77Z_ZONE_INNER_RADIUS = 8.0;

    protected bool m_S77ZLightingInitialized;
    protected vector m_S77ZStartPosition;
    protected vector m_S77ZStartDirection;
    protected vector m_S77ZTestZoneCenter;
    protected S77ZLightingPrototypeState m_S77ZLightingState = S77Z_LIGHTING_UNINITIALIZED;

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (!GetGame() || !GetGame().IsClient())
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        if (!m_S77ZLightingInitialized)
        {
            InitializeS77ZLightingPrototype(player);
        }

        if (!m_S77ZLightingInitialized)
            return;

        UpdateS77ZLightingPrototype(player);
    }

    override void OnMissionFinish()
    {
        ResetS77ZLightingPrototype();
        super.OnMissionFinish();
    }

    protected void InitializeS77ZLightingPrototype(PlayerBase player)
    {
        World world = GetGame().GetWorld();
        if (!world)
            return;

        m_S77ZStartPosition = player.GetPosition();
        m_S77ZStartDirection = player.GetDirection();
        m_S77ZStartDirection[1] = 0.0;

        float directionX = m_S77ZStartDirection[0];
        float directionZ = m_S77ZStartDirection[2];
        float directionLengthSquared = directionX * directionX;
        directionLengthSquared = directionLengthSquared + directionZ * directionZ;
        float directionLength = Math.Sqrt(directionLengthSquared);

        if (directionLength > 0.001)
        {
            m_S77ZStartDirection[0] = directionX / directionLength;
            m_S77ZStartDirection[2] = directionZ / directionLength;
        }
        else
        {
            m_S77ZStartDirection = "0 0 1";
        }

        vector centerOffset = m_S77ZStartDirection * S77Z_ZONE_FORWARD_DISTANCE;
        m_S77ZTestZoneCenter = m_S77ZStartPosition + centerOffset;

        world.LoadUserLightingCfg(S77Z_LIGHTING_PROFILE, "S77Z_LightingPrototype");
        world.SetUserLightingLerp(0.0);

        m_S77ZLightingInitialized = true;
        Print("[S77Z] prototype initialized");

        string centerLine = "[S77Z] test zone center=";
        centerLine = centerLine + m_S77ZTestZoneCenter.ToString();
        Print(centerLine);
    }

    protected void UpdateS77ZLightingPrototype(PlayerBase player)
    {
        World world = GetGame().GetWorld();
        if (!world)
            return;

        vector position = player.GetPosition();
        float deltaX = position[0] - m_S77ZTestZoneCenter[0];
        float deltaZ = position[2] - m_S77ZTestZoneCenter[2];
        float distanceSquared = deltaX * deltaX;
        distanceSquared = distanceSquared + deltaZ * deltaZ;
        float distanceXZ = Math.Sqrt(distanceSquared);

        float lightingLerp;
        S77ZLightingPrototypeState nextState;

        if (distanceXZ >= S77Z_ZONE_OUTER_RADIUS)
        {
            lightingLerp = 0.0;
            nextState = S77Z_LIGHTING_OUTSIDE;
        }
        else if (distanceXZ <= S77Z_ZONE_INNER_RADIUS)
        {
            lightingLerp = 1.0;
            nextState = S77Z_LIGHTING_INNER;
        }
        else
        {
            float transitionWidth = S77Z_ZONE_OUTER_RADIUS - S77Z_ZONE_INNER_RADIUS;
            float transitionDistance = S77Z_ZONE_OUTER_RADIUS - distanceXZ;
            lightingLerp = transitionDistance / transitionWidth;
            nextState = S77Z_LIGHTING_TRANSITION;
        }

        world.SetUserLightingLerp(lightingLerp);
        LogS77ZLightingState(nextState);
    }

    protected void LogS77ZLightingState(S77ZLightingPrototypeState nextState)
    {
        if (nextState == m_S77ZLightingState)
            return;

        m_S77ZLightingState = nextState;

        if (nextState == S77Z_LIGHTING_OUTSIDE)
        {
            Print("[S77Z] state=OUTSIDE");
        }
        else if (nextState == S77Z_LIGHTING_TRANSITION)
        {
            Print("[S77Z] state=TRANSITION");
        }
        else if (nextState == S77Z_LIGHTING_INNER)
        {
            Print("[S77Z] state=INNER");
        }
    }

    protected void ResetS77ZLightingPrototype()
    {
        if (GetGame())
        {
            World world = GetGame().GetWorld();
            if (world)
            {
                world.SetUserLightingLerp(0.0);
            }
        }

        m_S77ZLightingInitialized = false;
        m_S77ZLightingState = S77Z_LIGHTING_UNINITIALIZED;
        Print("[S77Z] reset");
    }
}
