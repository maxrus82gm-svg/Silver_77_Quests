# Silver_77_Medicine — структура проекта

Это документационная и планируемая структура. Source folders этим этапом не создаются.

## Предполагаемая схема

```text
Silver_77_Medicine
├── Silver_77_Medicine_Client
└── Silver_77_Medicine_Server
```

### Server

- disease engine / state;
- transmission;
- stages;
- medicine / treatment;
- persistence;
- integration hooks.

### Client

- symptom presentation;
- PPE;
- local sound;
- camera / visual effects.

Фактические PBO и source paths будут утверждаться только при первой реализации.

## Связи систем

```text
Silver_77_Zones → exposure / trigger → Medicine
Creature hit → Medicine
Food / Water → Medicine
Quest / Event → Medicine
```

Medicine не владеет координатами зон. Zones не владеет внутренней логикой болезни.
