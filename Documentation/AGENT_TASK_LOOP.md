# AGENT TASK LOOP

Это рабочий shell-документ текущего агентского цикла.

Он нужен для того, чтобы хранить:

- текущую задачу в `БЛОКЕ 1`;
- последний `AGENT REPORT` в `БЛОКЕ 2`;
- последний `REVIEW / STATUS` в `БЛОКЕ 3`;
- короткую карту чтения `SplitDoc`.

Постоянные правила вынесены в:

- `Documentation/SplitDoc/AGENT_RULES.md`
- `Documentation/SplitDoc/ENCODING_RULES.md`
- `Documentation/SplitDoc/DAYZ_RPC_SYNC_RULES.md`
- `Documentation/SplitDoc/QUEST_EDITOR_RULES.md`
- `Documentation/SplitDoc/QUEST_JSON_CONTRACT.md`
- `Documentation/SplitDoc/QUEST_UI_RULES.md`
- `Documentation/SplitDoc/DAYZ_LAYOUT_VIEWER_RULES.md`
- `Documentation/SplitDoc/SERVER_PROFILE_RULES.md`
- `Documentation/SplitDoc/TASK_HISTORY.md`

## Что читать всегда

Агент всегда читает:

- `Documentation/AGENT_TASK_LOOP.md`
- `Documentation/SplitDoc/AGENT_RULES.md`

## Матрица чтения SplitDoc

- `UI / QuestMenu / QuestJournal` -> `QUEST_UI_RULES.md` + `QUEST_JSON_CONTRACT.md` + `ENCODING_RULES.md`
- `Quest Editor / JSON_Quvest` -> `QUEST_EDITOR_RULES.md` + `QUEST_JSON_CONTRACT.md` + `SERVER_PROFILE_RULES.md`
- `JSON contract / quest logic` -> `QUEST_JSON_CONTRACT.md` + `Documentation/QUEST_LOGIC_SPEC.md`
- `DayZ layout viewer` -> `DAYZ_LAYOUT_VIEWER_RULES.md` + `ENCODING_RULES.md`
- `RPC / client-server sync` -> `DAYZ_RPC_SYNC_RULES.md`
- `Encoding / кириллица` -> `ENCODING_RULES.md`
- `Server profile / dev JSON / перенос на сервер` -> `SERVER_PROFILE_RULES.md`
- `History / status / прошлые решения` -> `TASK_HISTORY.md`

Если задача явно разрешает правку документации, агент может сам обновлять:

- `БЛОК 2 — AGENT REPORT`
- `БЛОК 3 — REVIEW / STATUS`
- `Documentation/SplitDoc/TASK_HISTORY.md`
- новые подтверждённые правила в нужном `SplitDoc`

================================================================================
# 🟦 БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА ДЛЯ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ЗАДАЧИ
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

TASK 083 — Аналитика мода PB_DoorsAndBarricades: понять устройство и причину пропавшей коллизии

Статус:
Новая активная аналитическая задача для агента.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Перед выполнением задачи агент обязан прочитать:

1. D:\GitHub\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
4. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Агент обязан соблюдать принцип жёстких рамок:

- это аналитическая задача;
- ничего не менять;
- не чинить “заодно”;
- не перепаковывать PBO;
- не удалять и не переносить файлы;
- если найдена проблема вне scope — записать в PROBLEMS / QUESTIONS / RECOMMENDED NEXT TASK, но не исправлять сам.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

Пользователь распаковал PBO мода дверей:

D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\addons\PB_DoorsAndBarricades.pbo

Распакованное содержимое находится в папке:

D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\

Также пользователь указал commit:

d814d0c2d6a0a688e8b23c781534d7229c9929c9

Мод относится к DayZ Doors / Barricades / Walls / Gates / Windows / Kits.

Главная проблема:
у объектов мода пропала или не работает коллизия.

Нужно понять:
- как мод устроен;
- какие классы и модели используются;
- где задаётся физика / collision / construction;
- почему collision может отсутствовать;
- что проверять дальше;
- какую следующую безопасную задачу поставить для фикса.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 083
--------------------------------------------------------------------------------

Сделать аналитический разбор мода PB_DoorsAndBarricades и дать техническое заключение:

1. Какие основные классы есть в config.cpp.
2. От чего они наследуются.
3. Какие классы являются kit-объектами.
4. Какие классы являются установленными buildable-объектами.
5. Какие модели .p3d используются.
6. Какие настройки могут влиять на collision.
7. Есть ли в config.cpp подозрительные поля:
   - physLayer
   - simulation
   - createProxyPhysicsOnInit
   - createdProxiesOnInit
   - collision_data
   - carveNavmesh
   - placement
   - bounding
   - class Construction
8. Есть ли в скриптах логика, которая может скрывать / создавать / заменять объект после установки.
9. Есть ли признаки, что проблема может быть не в config.cpp, а в .p3d Geometry LOD.
10. Дать список вероятных причин исчезновения коллизии по степени вероятности.
11. Предложить следующую безопасную TASK 084 для точечной проверки или фикса.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:
сначала не чинить, а понять устройство мода.

По DayZ buildable-объектам проблема коллизии часто бывает не только в config.cpp.

Самые вероятные зоны риска:

1. В модели .p3d нет корректного Geometry LOD / Fire Geometry / View Geometry.
2. Geometry LOD есть, но не соответствует named selections или components.
3. createProxyPhysicsOnInit / createdProxiesOnInit не создают нужную deployed-физику.
4. collision_data[] пустой или не соответствует construction part.
5. Объект наследуется от Fence, но кастомная модель не повторяет нужную структуру vanilla Fence.
6. Скрипт после установки создаёт объект без нужного deployed state / animation source.
7. Объект визуально появляется, но физический proxy / geometry не активируется.
8. Model path / selection / memory point / proxy не совпадает с тем, что ожидает config и scripts.

На этой задаче нельзя сразу править.

Нужно составить карту мода, найти наиболее вероятный источник проблемы и предложить следующий точечный шаг.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это только аналитика.

Агент должен:
- читать файлы;
- анализировать config / scripts / models;
- составить карту классов;
- найти вероятные причины проблемы collision;
- предложить следующую задачу.

Агент не должен:
- править файлы;
- перепаковывать PBO;
- запускать фиксы;
- менять config.cpp;
- менять .p3d;
- менять scripts;
- менять JSON/XML;
- менять основной мод Silver_77_Quests;
- делать commit.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ И ПАПКИ ДЛЯ ЧТЕНИЯ
--------------------------------------------------------------------------------

Можно читать:

D:\GitHub\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Можно читать распакованный мод:

D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\addons\
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\PB_DoorsAndBarricades\
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\config.cpp
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.c
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.p3d
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.rvmat
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.cpp
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.json
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\*.xml
D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\**\$PBOPREFIX$

Если фактический путь отличается:
- найти папку распакованного PB_DoorsAndBarricades внутри репозитория;
- указать реальный путь в отчёте.

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено:
- менять любые файлы;
- перепаковывать PBO;
- запускать Addon Builder;
- менять config.cpp;
- менять .p3d;
- менять scripts .c;
- менять JSON;
- менять XML;
- менять types.xml;
- менять основной Silver_77_Quests_Client;
- менять основной Silver_77_Quests_Server;
- менять Quest Editor;
- менять Documentation / SplitDoc;
- удалять файлы;
- переносить файлы;
- переименовывать файлы;
- делать git commit;
- делать git push;
- делать git reset;
- делать git clean;
- делать git checkout для отката.

Это только аналитика.

Если агент нашёл очевидное решение:
не исправлять, а описать в CONCLUSION и предложить TASK 084.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ПРОВЕРИТЬ
--------------------------------------------------------------------------------

1. Структура распакованного PBO

Нужно вывести дерево верхнего уровня:

- есть ли config.cpp;
- есть ли Scripts;
- есть ли Data / models / .p3d;
- есть ли textures / rvmat;
- есть ли modded scripts;
- есть ли settings/config JSON;
- есть ли types.xml;
- есть ли $PBOPREFIX$.

2. config.cpp

Нужно найти и описать:

- CfgPatches;
- CfgMods;
- CfgVehicles;
- базовые классы;
- kit-классы;
- deployed/buildable-классы;
- door/barricade/wall/gate/window классы;
- model paths;
- physLayer;
- simulation;
- bounding;
- createProxyPhysicsOnInit;
- createdProxiesOnInit;
- collision_data;
- class Construction;
- AnimationSources;
- attachments;
- hiddenSelections;
- rotation / animation sources;
- inheritance chain.

Особенно проверить классы, похожие на:

- PBDoorsBase
- PBDoorsKitBase
- PB_PlankDoor
- PB_WoodDoor
- PB_MetalDoor
- PB_PlankBarricade
- PB_MetalBarricade
- PB_BrickBarricade
- PB_PlankDoorBarricade
- PB_MetalDoorBarricade
- PB_BrickDoorBarricade
- PB_WoodWall
- PB_MetalWall
- PB_WoodGate
- PB_MetalGate
- PB_PlankWindow
- PB_MetalWindow
- все *_Kit классы

3. Модели .p3d

Нужно перечислить .p3d модели, которые используются для дверей / баррикад / стен / ворот / окон.

Если возможно без изменения файлов:
- проверить имена моделей;
- проверить, существуют ли файлы по путям из config.cpp;
- указать, какие модели нужно открыть в Object Builder для проверки Geometry LOD.

Важно:
агент может не уметь надёжно читать внутренние LOD .p3d.
Если не может проверить LOD напрямую, он должен честно сказать:

"Нужно открыть модель в Object Builder и проверить Geometry / Fire Geometry / View Geometry LOD".

4. Скрипты

Нужно проверить Scripts:

- есть ли классы установки kit;
- есть ли placement logic;
- есть ли OnPlacementComplete;
- есть ли create object / replace object logic;
- есть ли SetAnimationPhase;
- есть ли HideSelection / ShowSelection;
- есть ли CreateDynamicPhysics / SetDynamicPhysicsLifeTime / dBody / collision-related calls;
- есть ли Init / EEInit / OnStoreLoad / AfterStoreLoad logic;
- есть ли логика, которая переводит объект в Deployed state;
- есть ли logic для gates/doors open/close.

5. Construction / collision

Нужно отдельно проверить:

- class Construction;
- collision_data[];
- required_parts[];
- conflicted_parts[];
- is_gate;
- id;
- material_type;
- build_action_type;
- dismantle_action_type;
- proxy physics;
- deployed selections.

6. Вероятные причины пропавшей коллизии

Составить список вероятных причин по приоритету:

Например:
A. Нет Geometry LOD в .p3d.
B. Geometry LOD есть, но не настроены components / mass / named selections.
C. Неправильная настройка createProxyPhysicsOnInit / createdProxiesOnInit.
D. Пустой collision_data[] для Construction.
E. Неверный inheritance / simulation / physLayer.
F. Скрипт ставит не тот объект или не переводит его в Deployed state.
G. Модель путь неверный или визуальный объект есть, но physical geometry не активируется.
H. Коллизия есть только у части/selection, которая скрыта animation source.
I. Объект наследуется от Fence, но кастомная модель не соответствует vanilla fence expectations.

7. Что проверить вручную в Object Builder

Дать пользователю список:

- какие .p3d открыть;
- какие LOD проверить;
- какие named selections проверить;
- какие components должны быть в Geometry LOD;
- какие свойства geometry важны;
- что сравнить с vanilla Fence / Gate / Watchtower;
- где посмотреть Fire Geometry / View Geometry;
- что проверить по mass / convexity / components.

8. Следующая задача

Предложить TASK 084:

Варианты:
- TASK 084A — точечная проверка config.cpp collision/proxy settings;
- TASK 084B — проверка/исправление .p3d Geometry LOD;
- TASK 084C — тестовый фикс createProxyPhysicsOnInit / createdProxiesOnInit / collision_data;
- TASK 084D — проверка скрипта установки kit/deployed object.

Выбрать один наиболее вероятный вариант по результатам анализа.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Если агент читает .cpp / .c / .xml / .json с кириллицей:

- не менять кодировку;
- не сохранять файлы;
- если видит кракозябры, указать в отчёте;
- это аналитика, поэтому ничего не перекодировать.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ ДЛЯ ЭТОЙ ЗАДАЧИ
--------------------------------------------------------------------------------

Это аналитическая задача.

Агент не имеет права менять файлы.

Если найдено очевидное решение:
- не исправлять;
- записать в CONCLUSION;
- предложить TASK 084.

Если найдено несколько проблем:
- ранжировать их;
- не чинить ни одну.

Если для анализа нужен файл вне разрешённой зоны:
- не читать/не менять без необходимости;
- указать в PROBLEMS, какой файл нужен и почему.

Если проблема найдена вне задачи:
- писать в PROBLEMS / QUESTIONS / RECOMMENDED NEXT TASK;
- не исправлять сам.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. Дано дерево верхнего уровня распакованного мода.
2. Описаны основные классы config.cpp.
3. Разделены kit-классы и deployed/buildable-классы.
4. Описаны model paths и .p3d, которые нужно проверить.
5. Найдены collision-related настройки config.cpp.
6. Проверены scripts на placement/deployed/collision логику.
7. Составлен список вероятных причин пропавшей коллизии.
8. Дана ручная checklist-проверка для Object Builder.
9. Предложена следующая TASK 084.
10. Ничего не изменялось.
11. CHANGED FILES содержит “ничего не изменялось”.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

ANALYSIS:
- кратко как устроен мод;
- что найдено по config.cpp;
- что найдено по scripts;
- что найдено по models;
- где вероятнее всего зона проблемы collision.

STRUCTURE:
- дерево верхнего уровня распакованного PBO;
- основные папки и файлы.

CLASSES:
- базовые классы;
- kit-классы;
- deployed/buildable-классы;
- какие классы наследуются от чего.

MODELS:
- список .p3d;
- какие классы используют какие модели;
- какие модели нужно проверить в Object Builder.

COLLISION FINDINGS:
- physLayer;
- simulation;
- createProxyPhysicsOnInit;
- createdProxiesOnInit;
- collision_data;
- Construction;
- Geometry LOD risk;
- скриптовые риски.

LIKELY CAUSES:
- список причин по вероятности.

MANUAL CHECKLIST:
- что пользователю проверить руками в Object Builder / игре.

CHANGED FILES:
- ничего не изменялось.

PROBLEMS:
- что не удалось проверить;
- какие файлы / LOD требуют ручной проверки;
- какие проблемы найдены вне scope.

RECOMMENDED NEXT TASK:
- предложить TASK 084 с чёткой областью и разрешёнными файлами.

CONCLUSION:
- краткий вывод: где вероятнее всего причина пропавшей коллизии.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ЗАДАЧИ
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- Создана папка `Documentation/SplitDoc`.
- Созданы и заполнены `AGENT_RULES.md`, `ENCODING_RULES.md`, `DAYZ_RPC_SYNC_RULES.md`, `QUEST_EDITOR_RULES.md`, `QUEST_JSON_CONTRACT.md`, `QUEST_UI_RULES.md`, `DAYZ_LAYOUT_VIEWER_RULES.md`, `SERVER_PROFILE_RULES.md`, `TASK_HISTORY.md`.
- В `AGENT_RULES.md` закреплены новые правила: ChatGPT всегда даёт полный `БЛОК 1`, краткую задачу агенту, своё мнение / предпочтительное решение, полный `REVIEW`, а новые правила поручает внести в нужный `SplitDoc`.
- В `AGENT_RULES.md` закреплено новое правило: если задача разрешает, агент сам обновляет `AGENT REPORT`, `REVIEW / STATUS`, `TASK_HISTORY` и новые правила в нужных `SplitDoc`.
- `AGENT_TASK_LOOP.md` сокращён до рабочего shell-документа с матрицей чтения, `БЛОКОМ 1`, `БЛОКОМ 2`, `БЛОКОМ 3` и ссылкой на `TASK_HISTORY.md`.
- В `БЛОК 3` внесён `TASK 079 REVIEW`.
- В `TASK_HISTORY.md` добавлены записи по `TASK 079` и `TASK 080`.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_RPC_SYNC_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_EDITOR_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_JSON_CONTRACT.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\SERVER_PROFILE_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

DIFF:
- Огромные постоянные правила вынесены из `AGENT_TASK_LOOP.md` в тематические `SplitDoc`.
- История задач вынесена в отдельный `TASK_HISTORY.md`.
- Общее агентское поведение и процесс постановки задач теперь собраны в `AGENT_RULES.md`.
- Encoding, RPC/sync, editor, JSON contract, UI, layout viewer и server profile разделены по отдельным документам.
- Старый конфликт со ссылкой на "будущую TASK 079" внутри UI-заметок устранён за счёт вынесения backlog-правил из loop-файла.

PROBLEMS:
- В проекте уже существуют пересекающиеся документы: `Documentation/RUSSIAN_ENCODING.md`, `Documentation/QUEST_LOGIC_SPEC.md`, `Documentation/README_JSON_CONFIG.md`. Они не менялись, но теперь есть риск двойного канона, если `SplitDoc` и старые документы начнут расходиться.
- `TASK_HISTORY.md` перенесён как рабочая история из старого loop-файла. Если нужен полный аудит всех прошлых задач вне этого списка, это потребует отдельной задачи.

QUESTIONS:
- Нет.

CONCLUSION:
- `SplitDoc`-структура создана, постоянные правила разнесены по темам, а `AGENT_TASK_LOOP.md` переведён в рабочий shell-формат. Код, JSON, мод, layout и внешние канонические документы не менялись.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# 🟨 БЛОК 3 — REVIEW / STATUS
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

TASK 082 REVIEW

Статус:
Принято условно.

Что принято:
- В QuestMenu.layout добавлен full-screen BackgroundOverlay.
- В QuestJournal.layout добавлен full-screen BackgroundOverlay.
- Усилен фон QuestPanel и QuestJournalPanel.
- В QuestMenu возвращён label “Список квестов” над QuestListbox.
- Для DescriptionPanel и DialogPanel добавлена wheel-прокрутка через widget scroll API.
- Для QuestJournal добавлена wheel-прокрутка DescriptionText.
- Action prompt не отключался и input/action-система не трогалась.
- JSON-контракт не менялся.
- Server profile не трогался.
- Mapping DescriptionPanel / DialogPanel не переписывался.
- Кириллица по отчёту не повреждена.
- Forbidden files не менялись.

Что требует ручной проверки в игре:
- Реально ли full-screen overlay затемняет мир и action prompt достаточно сильно.
- Работает ли VScrollStep / VScrollToPos01 на MultilineTextWidget в DayZ runtime.
- Работает ли wheel-scroll в QuestMenu для DescriptionText и DialogText.
- Работает ли wheel-scroll в QuestJournal.
- Видна ли подпись “Список квестов” и не перекрывает ли она QuestListbox.
- Не сломалось ли управление кнопками.
- Не сломались ли Offer / Completion / Reward.

Риски:
- Scroll реализован не через отдельный scroll-container, а через wheel-scroll на MultilineTextWidget. Это может работать, но требует проверки в runtime.
- Для QuestJournal агент сам отметил риск: курсор там по-прежнему не включается, поэтому wheel-scroll может потребовать отдельной проверки.
- Если VScrollStep окажется нестабильным или не сработает, следующая задача должна быть только про явный DayZ-compatible scroll container / scrollbar.

Вывод:
TASK 082 выполнен в рамках жёстких рамок. Следующий шаг — собрать клиентский PBO и проверить QuestMenu / QuestJournal в игре.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
