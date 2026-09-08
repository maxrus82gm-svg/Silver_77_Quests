# Структура проекта Silver_77_Zones

Рабочий source root:

`P:\Silver_77_Quests\Silver_77_Zones`

Текущая структура исходников:

```text
Silver_77_Zones
├── Silver_77_Zones_Client
└── Silver_77_Zones_Server
```

Документация проекта:

`P:\Silver_77_Quests\Documentation_DayZ_Qwest`

Reference старых модов:

`P:\Silver_77_Quests\Source_Modes\zones`

## Разделение ответственности

### Server

- каноническая логика зон;
- gameplay;
- Hazard / Exposure / Protection;
- активация и синхронизация.

### Client

- User Lighting;
- Eye Accommodation;
- PPE;
- локальные lights;
- audio / ambient;
- прочие визуальные эффекты.

На первом этапе допускается отдельно разрабатывать и проверять клиентский lighting prototype. Это не меняет будущий принцип: gameplay-логика полноценной `Silver_77_Zones` должна оставаться максимально server-side.

Папки Client / Server являются текущей рабочей структурой исходников. Финальная структура PBO и `@Mod` будет определена позже, при создании первого рабочего prototype.
