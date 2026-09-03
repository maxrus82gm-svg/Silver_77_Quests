class S77MigrateAPI
{
    static bool Activate(string activationId)
    {
        return S77MigrateManager.ActivateById(activationId);
    }

    static bool StartGroup(string groupId)
    {
        return S77MigrateManager.StartGroupById(groupId);
    }

    static bool StartEvent(string eventId)
    {
        return S77MigrateManager.StartEventById(eventId);
    }
}
