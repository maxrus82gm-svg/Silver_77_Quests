# Освещение зоны Silver_77_Zone

## 1. Назначение документа

Этот документ фиксирует техническую основу локального освещения для будущей системы `Silver_77_Zone`.

Он является **обязательным reference перед реализацией Visual / Lighting части Silver_77_Zone**.

Родительский документ:

[[19_Silver_77_Zone|Silver_77_Zone]]

Связь в Obsidian:

```text
19_Silver_77_Zone
        │
        └── Освещение зоны
```

Этот документ не описывает Hazard, Protection, Exposure, Quest или Migration целиком. Здесь рассматривается только локальная визуальная среда зоны: темнота, освещение, Eye Accommodation, источники света, PPE и связанные эффекты.

---

# 2. Статусы технических выводов

Используются три статуса.

### CONFIRMED

Механизм непосредственно существует в актуальных vanilla scripts DayZ и подтверждён исходным кодом.

### NEEDS RUNTIME TEST

Механизм существует или выглядит технически применимым, но конкретное поведение для открытой outdoor-зоны `Silver_77_Zone` необходимо проверить непосредственно в DayZ.

### DESIGN TARGET

Целевое поведение нашей системы. Это архитектурное решение Silver_77_Zone, а не гарантия vanilla engine.

---

# 3. Актуальная база исследования

Исследование проводилось по официальному репозиторию:

`BohemiaInteractive/DayZ-Script-Diff`

Проверенный build:

```text
DayZ 1.29.163709
Scripts Rev. 125372
commit:
86974a0f5bd16b1ee3e334ad828133c93dca80a1
date:
2026-08-12
```

Основные vanilla reference:

```text
scripts/3_game/global/world.c
scripts/4_world/classes/undergroundhandlerclient.c
scripts/4_world/entities/scriptedlightbase.c
scripts/3_game/ppemanager/requesters/
```

Ключевой vanilla lighting profile:

```text
dz\data\lighting\lighting_underground.txt
```

Таким образом исследование относится к актуальной ветке DayZ 1.29, а не к старым форумным примерам или старым модам.

---

# 4. Главная техническая находка

## CONFIRMED

В `World` существует native API управления lighting configuration:

```c
LoadNewLightingCfg(string path)
LoadUserLightingCfg(string path, string name)
SetUserLightingLerp(float val)
```

Также существуют:

```c
GetEyeAccom()
SetEyeAccom(float eyeAccom)
```

Ключевой для `Silver_77_Zone` механизм:

```c
LoadUserLightingCfg(...)
+
SetUserLightingLerp(...)
```

Это позволяет загрузить отдельный **USER LIGHTING PROFILE** и смешивать его с обычным освещением мира.

Это принципиально отличается от простого затемнения изображения через PPE или Eye Accommodation.

---

# 5. Vanilla Underground — основной reference

## CONFIRMED

Vanilla DayZ использует User Lighting для underground areas.

`UndergroundHandlerClient` содержит:

```c
const string UNDERGROUND_LIGHTING =
    "dz\\data\\lighting\\lighting_underground.txt";
```

и загружает профиль:

```c
g_Game.GetWorld().LoadUserLightingCfg(
    UNDERGROUND_LIGHTING,
    "Underground"
);
```

После этого lighting profile применяется через:

```c
g_Game.GetWorld().SetUserLightingLerp(m_LightingLerp);
```

При выходе значение возвращается к обычному освещению:

```c
SetUserLightingLerp(0);
```

Следовательно vanilla underground не просто накладывает чёрный фильтр на камеру.

Он использует отдельный lighting profile, который смешивается с текущим освещением мира.

---

# 6. Почему старый NightZone недостаточен

Старый `NightZone` полезен как reference, но его подход не должен становиться основой новой системы.

Он в основном использует изменение Eye Accommodation.

Практический результат:

```text
обычный день
↓
изображение становится темнее
```

Это похоже на затемнение картинки, а не на полноценную локальную ночь.

Из-за этого:

* фонари ведут себя неправильно;
* костры и другие источники света не получают полноценного ночного поведения;
* освещение окружения остаётся дневным по своей природе.

Vanilla underground использует более полноценную комбинацию:

```text
User Lighting Config
+
Lighting Lerp
+
Eye Accommodation
+
NVG handling
+
night-light handling
+
ambient handling
```

Именно этот путь является главным reference для `Silver_77_Zone`.

---

# 7. Плавное смешивание освещения

## CONFIRMED

`UndergroundHandlerClient` использует:

```c
LIGHT_BLEND_SPEED_IN
LIGHT_BLEND_SPEED_OUT
```

а также `AnimationTimer` и easing.

Lighting profile не обязан включаться мгновенно.

`m_LightingLerp` может плавно двигаться между:

```text
0.0 = обычное освещение мира
1.0 = полностью User Lighting
```

Для `Silver_77_Zone` целевая схема может выглядеть так:

```text
OUTSIDE
0.0

WARNING / OUTER
0.1
0.2

TRANSITION
0.4
0.6
0.8

INNER DARK ZONE
1.0
```

## DESIGN TARGET

Переход должен зависеть от положения игрока внутри внешнего радиуса зоны, а не происходить одной резкой командой.

Цель:

```text
обычный день
↓
постепенное изменение атмосферы
↓
заметное потемнение
↓
полная тёмная зона
```

Без резкого:

```text
DAY → NIGHT
```

---

# 8. Предпочтительная структура Dark Zone

## DESIGN TARGET

```text
OUTSIDE
│
│ normal lighting
│ LightingLerp = 0
▼

WARNING RADIUS
│
├─ начинает расти LightingLerp
├─ появляется слабая атмосфера
├─ может начинаться Audio fade-in
└─ Hazard ещё может отсутствовать
│
▼

TRANSITION
│
├─ User Lighting примерно 20–80%
├─ Eye Accommodation плавно меняется
├─ включается нужное поведение night lights
├─ появляется лёгкий PPE / haze
└─ атмосфера усиливается
│
▼

INNER DARK ZONE
│
├─ User Lighting около 100%
├─ Eye Accommodation настроен под тёмную среду
├─ фонари и локальные lights должны работать
├─ PPE используется только как дополнительный слой
├─ Audio активен
└─ Hazard может воздействовать независимо от Visual radius
```

Visual radius может быть больше Hazard radius.

Например:

```text
Visual radius  = 150 m
Audio radius   = 130 m
Hazard radius  = 100 m
```

Игрок сначала замечает изменение мира, затем атмосферу/звук и только потом входит в реальную опасность.

---

# 9. Ночные источники света

## CONFIRMED

Vanilla underground имеет отдельный механизм для источников света, которые обычно работают только ночью.

В `UndergroundHandlerClient` существует логика вида:

```c
EnableLights(bool enable)
```

Она проходит по:

```c
ScriptedLightBase.m_NightTimeOnlyLights
```

и вызывает:

```c
SetVisibleDuringDaylight(enable);
```

Кроме того, `ScriptedLightBase` при инициализации учитывает нахождение локального игрока underground.

Сам vanilla `ScriptedLightBase` прямо указывает:

```text
Lights work only on client side!
```

Это крайне важно для `Silver_77_Zone`.

Vanilla уже решает близкую задачу:

```text
для мира сейчас день
+
локальный игрок находится в тёмном пространстве
+
ночные источники света должны работать
```

---

# 10. Фонарик внутри локальной зоны

## CONFIRMED

Механизм разрешения night-time lights в дневное время существует.

## NEEDS RUNTIME TEST

Необходимо отдельно проверить качество результата для outdoor-зоны.

Главная рабочая гипотеза:

```text
Player A
находится снаружи
→ продолжает видеть обычный день

Player B
входит в Silver_77_Zone
→ получает локальный dark lighting profile
→ night-time lights становятся доступными
→ его фонарь визуально работает как источник света в темноте
```

Серверное время при этом не меняется.

Другому игроку не должна навязываться глобальная ночь.

Это один из главных критериев будущего prototype.

---

# 11. Важный риск NightTimeOnlyLights

## NEEDS RUNTIME TEST

Vanilla `EnableLights()` работает со списком:

```c
ScriptedLightBase.m_NightTimeOnlyLights
```

То есть необходимо выяснить, какие именно lights на конкретном клиенте попадают под переключение.

Нужно проверить, не возникает ли побочный эффект:

```text
игрок находится внутри Silver_77_Zone
↓
разрешаются night-time lights
↓
одновременно становятся видимыми дневные/внешние источники,
которые находятся далеко за пределами зоны
```

Если такой эффект существует, для `Silver_77_Zone` потребуется более точное управление lights.

Нельзя автоматически копировать vanilla underground `EnableLights()` без runtime-проверки открытого пространства.

---

# 12. Какие источники света обязательно тестировать

Первый runtime prototype должен проверить минимум:

```text
ручной фонарь
налобный фонарь
оружейный фонарь
костёр
факел
химсвет
локальные scripted lights
ПНВ / NVG
```

Для каждого проверить:

* включается ли он днём внутри Dark Zone;
* действительно ли освещает поверхность;
* правильны ли дальность и яркость;
* появляются ли нормальные тени;
* что видит другой игрок;
* что происходит на границе зоны;
* что происходит сразу после выхода.

Дополнительно помнить, что vanilla предупреждает:

```text
Lights are very performance heavy.
Especially if they cast shadows.
```

Поэтому искусственно создавать большое количество новых динамических lights без необходимости нельзя.

---

# 13. Eye Accommodation

## CONFIRMED

DayZ имеет:

```c
GetEyeAccom()
SetEyeAccom(...)
```

Vanilla underground управляет Eye Accommodation отдельно от LightingLerp.

Следовательно:

```text
Lighting
≠
Eye Accommodation
```

Это два разных слоя.

Предпочтительное разделение:

```text
User Lighting
→ физическая база локальной темноты

Eye Accommodation
→ способность глаза игрока адаптироваться к темноте
```

Eye Accommodation не должен использоваться как единственный способ создания Dark Zone.

Он является дополнением к lighting profile.

---

# 14. PPE

## CONFIRMED

В актуальном vanilla существуют PPE requesters, в том числе:

```text
PPERequester_ContaminatedAreaTint
PPERequester_SpookyAreaTint
PPERequester_PainBlur
PPERUndergroundAcco
```

Также доступны эффекты, связанные с:

```text
GaussFilter
RadialBlur
FilmGrain
Chromatic Aberration
Glow
Colorization
```

Например `SpookyAreaTint` уже является vanilla reference для плавного атмосферного эффекта и использует цветовую обработку и film grain.

---

# 15. Правильная роль PPE

## DESIGN TARGET

PPE не должен создавать основную ночь.

Правильное разделение:

```text
User Lighting
→ настоящая база темноты

Eye Accommodation
→ адаптация зрения

PPE
→ художественная атмосфера и симптомы
```

PPE можно использовать для:

* лёгкой десатурации;
* изменения оттенка;
* слабой виньетки;
* film grain;
* haze;
* токсического blur;
* красноты;
* приступов помутнения;
* симптомов радиации/токсина.

Не использовать сильный blur как способ скрыть тот факт, что окружающий мир на самом деле остаётся дневным.

---

# 16. Солнце

Здесь необходимо разделять две разные задачи.

```text
1. Ослабить/убрать прямое солнечное освещение поверхности.

2. Скрыть сам визуальный диск солнца на небе.
```

## CONFIRMED

User Lighting profile может существенно менять освещение сцены.

## NEEDS RUNTIME TEST

Пока не доказано, что custom User Lighting profile может полностью убрать визуальный диск солнца из открытого дневного неба.

Возможны три результата.

### Вариант A

Lighting config самостоятельно решает:

```text
surface lighting
+
sky
+
sun
```

Это лучший вариант.

### Вариант B

Поверхность становится практически ночной, но солнечный диск остаётся видимым.

Тогда можно добавить:

```text
лёгкий haze
+
локальный PPE
```

чтобы скрыть визуальный конфликт.

### Вариант C

Полноценная буквальная ночь на открытом пространстве визуально не получается.

Тогда Dark Zone оформляется как:

```text
аномальная тёмная мгла
/
заражённая атмосферная зона
```

а не как буквальная смена времени суток.

До исследования `lighting_underground.txt` и runtime prototype нельзя обещать полное исчезновение солнца.

---

# 17. Туман и погода

## DESIGN TARGET

Не использовать глобальную серверную Weather/Fog систему как основной способ создания локальной зоны одного игрока.

Причина:

```text
Weather
→ мировая / серверная система

Silver_77_Zone Visual
→ должна быть локальной для конкретного клиента
```

Если одному игроку нужна видимость 10–15 метров, это не должно автоматически менять погоду для всех игроков сервера.

Для локальной мглы исследовать комбинацию:

```text
User Lighting
+
PPE
+
haze
+
локальные particles/effects
```

Целевая дальность видимости должна быть параметром конкретного Visual profile, а не обязательным свойством каждой зоны.

---

# 18. SERVER / CLIENT разделение

## DESIGN TARGET

Максимально возможная gameplay-логика `Silver_77_Zone` остаётся SERVER-side.

SERVER отвечает за:

```text
Zone Geometry
активацию зоны
Hazard
Exposure
Damage
Protection
Filters
Persistence
Quest linkage
Migration linkage
```

CLIENT отвечает только за то, что необходимо локально отображать игроку:

```text
User Lighting
LightingLerp
Eye Accommodation
night-light visibility
PPE
haze
particles
music
ambient
```

Изменение локального освещения не должно изменять:

```text
server world time
глобальную погоду
освещение всех остальных игроков
```

---

# 19. Предполагаемый Visual pipeline

## DESIGN TARGET

```text
SERVER
Zone активна
│
├─ Geometry / Conditions
├─ Hazard
├─ Exposure
└─ сообщает клиенту Visual state
        │
        ▼
CLIENT
Silver_77_Zone Visual Controller
        │
        ├─ LoadUserLightingCfg(...)
        ├─ SetUserLightingLerp(...)
        ├─ Eye Accommodation
        ├─ Night Lights handling
        ├─ PPE
        ├─ Haze / particles
        └─ Ambient / Audio
```

Не нужно каждую frame пересылать с сервера визуальные параметры.

Клиент должен иметь локальное состояние зоны и самостоятельно выполнять плавную интерполяцию после получения необходимых данных об активной зоне.

---

# 20. Будущий lighting profile

Предполагаемое рабочее имя:

```text
S77_Zone_Darkness
```

или:

```text
S77_Zone_Dark
```

Финальное имя и путь определяются во время реализации.

Концептуально:

```c
LoadUserLightingCfg(
    "<path_to_s77_lighting_profile>",
    "S77_Zone_Darkness"
);
```

Далее:

```c
SetUserLightingLerp(value);
```

где:

```text
0.0 = обычный мир
1.0 = полный профиль зоны
```

Не использовать `LoadNewLightingCfg()` как основной механизм локальной зоны без отдельного исследования.

Главный кандидат:

```text
LoadUserLightingCfg()
+
SetUserLightingLerp()
```

---

# 21. Следующий ключевой объект исследования

Перед написанием Lighting-кода необходимо получить актуальный файл:

```text
dz\data\lighting\lighting_underground.txt
```

из DayZ 1.29 / актуальных DayZ data или DayZ Tools.

Это следующий главный технический шаг.

Нужно выяснить, какие параметры реально содержит lighting profile.

В первую очередь исследовать:

```text
directional sun
sun intensity
ambient lighting
sky lighting
ground lighting
reflections
environment intensity
night lighting
sky parameters
возможные параметры sun disk
```

Нужно понять:

1. Какие параметры изменяет vanilla Underground.
2. Какие параметры отвечают за прямой солнечный свет.
3. Какие параметры отвечают за ambient.
4. Можно ли менять внешний вид неба.
5. Можно ли скрывать/ослаблять солнечный диск.
6. Какие параметры необходимы для нормальной работы фонарей.
7. Можно ли создать собственный outdoor profile без визуальных артефактов.

Только после этого создавать полноценный:

```text
S77_Zone_Darkness lighting profile
```

---

# 22. Первый runtime prototype

Первый prototype должен быть максимально маленьким.

Не подключать сразу:

```text
Quest
Migration
Exposure
Protection
Web UI
Persistence
```

Prototype:

```text
одна цилиндрическая зона
+
локальный User Lighting
+
LightingLerp
+
Eye Accommodation
+
night-light handling
+
минимальный PPE
```

Геометрия:

```text
CYLINDER
X/Z radius
при необходимости minY / maxY
```

---

# 23. Сценарии тестирования prototype

Проверить:

```text
солнечный день
облачный день
сумерки
обычную ночь
вход в зону
медленный вход
быстрый вход
выход
повторный вход
нахождение на границе
```

Источники света:

```text
ручной фонарь
налобник
оружейный фонарь
костёр
факел
химсвет
scripted lights
NVG
```

Multiplayer:

```text
Player A — снаружи зоны
Player B — внутри зоны
```

Также проверить:

* что видит Player A на Player B;
* что видит Player B на Player A;
* не меняется ли глобальный день;
* не включаются ли лишние distant night lights;
* нет ли резкого скачка картинки на границе;
* корректно ли восстанавливается обычное освещение после выхода.

---

# 24. Главный acceptance test

Основной критерий успешности технологии:

```text
Серверное время — день.

Player A стоит снаружи Silver_77_Zone.
Для него остаётся обычный солнечный день.

Player B входит в Silver_77_Zone.

Для Player B:
освещение плавно темнеет;
окружение становится близким к настоящей ночной/аномальной тёмной среде;
обычный день не выглядит просто картинкой с чёрным фильтром;
фонарик реально освещает пространство;
night lights работают;
Eye Accommodation ведёт себя естественно;
PPE используется только как дополнительная атмосфера.

Player A при этом продолжает видеть обычный день.
```

Если этот сценарий работает, базовая технология Visual / Lighting для `Silver_77_Zone` считается доказанной.

---

# 25. Что уже можно считать доказанным

## CONFIRMED

На актуальных vanilla scripts DayZ 1.29 подтверждено:

```text
✔ существует LoadUserLightingCfg(...)

✔ существует SetUserLightingLerp(...)

✔ User Lighting может плавно смешиваться с обычным lighting

✔ vanilla Underground использует отдельный
  lighting_underground.txt

✔ Underground использует LightingLerp

✔ Underground отдельно управляет Eye Accommodation

✔ переход выполняется плавно через interpolation/easing

✔ vanilla имеет специальную обработку
  night-time-only lights

✔ SetVisibleDuringDaylight(...) используется
  для работы таких lights в underground

✔ ScriptedLightBase является client-side

✔ vanilla имеет отдельную обработку NVG underground

✔ существует большой набор PPE requesters
  и post-process эффектов
```

Это означает, что концепция локальной тёмной зоны имеет реальную engine-базу и не строится только на старом NightZone.

---

# 26. Что пока не доказано

## NEEDS RUNTIME TEST

```text
? Полностью ли custom lighting profile способен
  скрыть визуальный диск солнца.

? Насколько убедительно underground profile
  будет выглядеть не под землёй, а на открытой местности.

? Будет ли ручной фонарь внутри outdoor Dark Zone
  визуально идентичен фонарю настоящей ночью.

? Как поведут себя костры, факелы,
  оружейные фонари и другие разновидности lights.

? Какие distant NightTimeOnlyLights станут видимыми
  после SetVisibleDuringDaylight(true).

? Как будет выглядеть граница зоны
  при открытом горизонте и дальних объектах.

? Как lighting profile взаимодействует
  с солнцем, облаками, дождём и разным временем суток.

? Как будет выглядеть NVG.

? Понадобится ли дополнительный PPE/haze,
  чтобы скрыть дневное небо или солнечный диск.
```

До prototype эти пункты нельзя считать решёнными.

---

# 27. Базовый принцип Silver_77_Zone Lighting

Финальная рабочая гипотеза:

```text
НЕ:

Eye Accommodation
=
Night Zone


А:

User Lighting Profile
+
Lighting Lerp
+
Eye Accommodation
+
Night Lights handling
+
минимальный PPE
=
Silver_77_Zone Dark Visual
```

`NightZone` остаётся старым reference.

`Vanilla Underground` становится главным техническим reference.

---

# 28. Порядок дальнейшей работы

Правильная последовательность:

```text
1. Получить lighting_underground.txt.

2. Разобрать реальные параметры lighting profile.

3. Определить, что происходит с:
   sun
   ambient
   sky
   reflections
   light sources.

4. Создать минимальный
   S77_Zone_Darkness profile.

5. Сделать маленький runtime prototype.

6. Проверить day / night / lights / NVG / multiplayer.

7. Только после подтверждения технологии
   проектировать полноценный Visual Controller
   Silver_77_Zone.

8. После этого связывать Visual с:
   Hazard
   Audio
   Quest
   Migration
   Web UI.
```

Не писать полноценную систему зон до подтверждения основной lighting-технологии.

---

# 29. Итог

На DayZ 1.29 подтверждено, что движок имеет значительно более подходящий механизм для локальной темноты, чем старое простое изменение Eye Accommodation.

Главная техническая основа:

```text
LoadUserLightingCfg(...)
+
SetUserLightingLerp(...)
```

Vanilla Underground доказывает существование отдельного локального lighting profile, плавного перехода, отдельной Eye Accommodation и специальной обработки night-time lights.

Поэтому наиболее перспективная архитектура `Silver_77_Zone`:

```text
локальный User Lighting
+
плавный LightingLerp
+
Eye Accommodation
+
правильная работа источников света
+
минимальный художественный PPE
```

Следующий обязательный шаг исследования:

```text
dz\data\lighting\lighting_underground.txt
```

После его изучения — минимальный runtime prototype.

До runtime-проверки отдельно не считать доказанными:

```text
полное исчезновение солнечного диска;
идеальную работу всех фонарей на открытой местности;
отсутствие побочных эффектов NightTimeOnlyLights;
качество outdoor darkness.
```

Этот документ должен читаться перед любой будущей задачей, которая реализует или существенно меняет:

```text
Silver_77_Zone
→ Visual
→ Lighting
→ Dark Zone
→ Eye Accommodation
→ локальные night lights
```
