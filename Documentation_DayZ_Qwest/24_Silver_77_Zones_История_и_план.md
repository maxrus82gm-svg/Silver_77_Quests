# Silver_77_Zones — история и план

## Текущее состояние

- концепция `Silver_77_Zone` и отдельное исследование Lighting описаны;
- рабочие Client / Server source-папки разделены;
- TASK 164 создала первый Client Lighting prototype;
- source-код prototype существует, но build и runtime ещё не подтверждены;
- для `Silver_77_Zones` принят отдельный собственный profile JSON;
- отдельного `Silver_77_Zones_WEB` не будет;
- Zones станет самостоятельной второй вкладкой общего Workshop;
- Quest и Zones сохраняют независимые JSON, пути, backup и state;
- интерфейс Zones не является копией quest-форм;
- общими остаются оболочка и визуальный стиль.

Единая оболочка уменьшает дублирование, а независимые модули снижают риск поломки существующего Quest Editor.

## Краткая история

- принято решение написать собственную систему зон;
- NightZone / SoundZone / MedicalAttention оставлены как reference;
- найден vanilla User Lighting / Underground reference;
- создана Client / Server структура;
- начата реализация Client Lighting prototype;
- выбран общий Workshop с независимыми Quest и Zones модулями.

## Продолжение

- [[25_Silver_77_Zones_Roadmap|Короткий roadmap]]
- [[26_Silver_77_Zones_Приоритетная_задача|Текущая приоритетная задача]]
