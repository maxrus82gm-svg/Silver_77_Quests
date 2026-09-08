# Silver_77_Zones — история и план

## Текущее состояние

- концепция `Silver_77_Zone` описана;
- отдельное исследование Lighting создано;
- рабочая папка `Silver_77_Zones` создана;
- Client и Server разделены;
- код мода ещё не начат;
- следующий технический этап — первый lighting prototype.

## Краткая история

- принято решение написать собственную систему зон;
- старые NightZone / SoundZone / MedicalAttention оставлены как reference;
- найден vanilla User Lighting / Underground reference;
- создана структура `Silver_77_Zones_Client` / `Silver_77_Zones_Server`;
- начинается реализация.

## Ближайший план

1. Получить и исследовать `lighting_underground.txt`.
2. Создать минимальный Client skeleton.
3. Проверить локальный User Lighting / LightingLerp.
4. Проверить вход / выход и источники света.
5. После доказательства технологии подключать server-side Zone logic.
