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

## Web editor / Workshop

Текущий рабочий root:

`P:\Silver_77_Quests\JSON_Quvest`

`JSON_Quvest` пока не переименовывается и не переносится. Это существующий root Quest Workshop и будущая общая оболочка для независимых модулей:

```text
Workshop
├── Quest module
└── Zones module
```

Runtime/source структура `Silver_77_Zones` остаётся только Client + Server и не получает отдельную WEB-папку.

Каждый Workshop-модуль должен иметь собственные JSON, состояние, save path, backup path и profile export path. Общими остаются оболочка, навигация и визуальный стиль.

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
