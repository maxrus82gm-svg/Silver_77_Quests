# Silver_77_Quests_Server

Private server-side part of Silver 77 Quests.

Do not publish this mod in Steam Workshop. Keep it only on the server and run it with:

```bat
-serverMod=@Silver_77_Quests_Server
```

The public client mod must also be loaded:

```bat
-mod=@Silver_77_Quests_Client
```

This server mod contains the authoritative quest manager, RPC request handling, JSON loading, rewards, item removal, cooldowns, and player progress saving.
