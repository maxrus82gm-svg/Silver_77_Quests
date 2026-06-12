# AGENT TASK LOOP

Это рабочий shell-документ текущего агентского цикла.

Он нужен для того, чтобы хранить:

- текущую задачу в `БЛОКЕ 1`;
- последний `AGENT REPORT` в `БЛОКЕ 2`;
- последний `REVIEW / STATUS` в `БЛОКЕ 3`;
- короткую карту чтения `SplitDoc`.

Постоянные правила вынесены в:

- `Documentation/SplitDoc/START.md`
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
- `Documentation/SplitDoc/START.md`

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

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

## TASK 132 — Перевести UI npcEquipment на построчную систему

### Цель

Уплотнить интерфейс `NPC комплект / npcEquipment` в редакторе квестов.

Секции/блоки должны сохраниться:

* `Одежда / clothing`
* `Контейнеры / containers`
* `Items внутри контейнера`
* `Предмет в руках / hands`
* `Предметы за спиной / backItems`
* `Оружие / weapons`
* `Навесы / attachments`
* `Magazine / магазин`
* `Ammo / патроны`

Но содержимое внутри этих блоков должно отображаться строками.

Главный принцип:

Один item = одна строка.

### Читать

* `Documentation/AGENT_TASK_LOOP.md`
* `Documentation/SplitDoc/START.md`
* `Documentation/SplitDoc/QUEST_EDITOR_RULES.md`
* `Documentation/SplitDoc/QUEST_JSON_CONTRACT.md`
* `JSON_Quvest/app.js`
* `JSON_Quvest/styles.css`

Общие ограничения, правила Git, жёсткие рамки, очистка `БЛОКА 1`, статус чтения репозитория и правила SplitDoc действуют по `START.md`.

### Важно по модам

Моды обновлять не нужно.

Меняется только локальный редактор `JSON_Quvest`.

### Сделать

Переделать отображение `npcEquipment` так, чтобы элементы не растягивались в большие микроблоки.

Нужно сделать табличный/построчный вид:

* сверху один раз идут названия колонок;
* ниже каждая строка описывает один item;
* внутри строки поля идут горизонтально;
* повторяющиеся подписи у каждого item убрать;
* длинные пояснения под каждым input убрать;
* оставить только короткие заголовки секций.

### Для обычных item-строк

Для `Silver77_NpcItem` использовать строку с колонками:

* `Class`
* `Slot`
* `Qty`
* `SetQty`
* `ItemQty`
* actions

Это применить к:

* `clothing`
* `containers[].items`
* `hands`
* `backItems`
* `weapons[].attachments`
* `weapons[].ammo`

Пока не добавлять качество одежды.

Пока не менять структуру `Silver77_NpcItem`.

### Для clothing

Одежду тоже пока показывать строками.

На этом этапе НЕ добавлять поле качества.

Quantity пока можно оставить как есть, но UI должен быть компактным. Отдельная задача позже решит, нужно ли скрывать quantity для одежды и добавлять dropdown качества.

### Для containers

Секция `containers` должна остаться отдельным блоком.

Каждый container сделать компактно:

* строка контейнера:

  * `Class`
  * `Slot`
  * actions

Под ним вложенная таблица `Items внутри контейнера`, где каждый item — одна строка.

### Для hands

`hands` оставить как отдельный блок, но не как большая карточка.

Сделать одну строку:

* `Class`
* `Slot`
* `Qty`
* `SetQty`
* `ItemQty`

### Для backItems

Каждый backItem — одна строка.

### Для weapons

Секция `weapons` должна остаться отдельным блоком.

Каждое оружие сделать компактным блоком-строкой:

* `Class`
* `Target`
* `Magazine`
* `AmmoCount`
* actions

Внутри оружия:

* `attachments` — таблица строк;
* `ammo` — таблица строк;
* magazine желательно показывать прямо в строке оружия, а не отдельной большой карточкой.

### UX

* Раскрывающиеся секции можно оставить.
* ClassName-поля должны сохранить datalist/autocomplete из `item-class-reference`.
* Кнопки добавления/удаления должны остаться.
* Таблицы должны не ломаться при длинных className.
* Если места мало, разрешён горизонтальный scroll внутри таблицы.
* Визуально сделать аккуратно, технически понятно, без лишней воды.

### Не менять

Не менять JSON-структуру.

Не добавлять новые поля.

Не добавлять качество одежды в этой задаче.

Не менять названия существующих полей:

* `npcLoadoutPreset`
* `npcEquipment.clothing`
* `npcEquipment.containers`
* `npcEquipment.containers[].items`
* `npcEquipment.hands`
* `npcEquipment.backItems`
* `npcEquipment.weapons`
* `npcEquipment.weapons[].attachments`
* `npcEquipment.weapons[].magazine`
* `npcEquipment.weapons[].magazine.ammoCount`
* `npcEquipment.weapons[].ammo`

Не ломать legacy-поля:

* `npcLoadout`
* `npcHandsItem`
* `npcBackItems`

### Разрешено менять

* `JSON_Quvest/app.js`
* `JSON_Quvest/styles.css`
* `Documentation/AGENT_TASK_LOOP.md` только для очистки `БЛОКА 1` после успешного отчёта

### Нежелательно, но можно только если без этого нельзя

* `JSON_Quvest/index.html`

### Запрещено

* Не менять `QuestData.c`.
* Не менять `QuestServerManager.c`.
* Не менять основной JSON без необходимости.
* Не менять server profile.
* Не менять PBO / packed output.
* Не делать build.
* Не менять клиентский мод.
* Не менять серверный мод.
* Не добавлять поле качества одежды.
* Git-операции не делать.

### Проверить

* `node --check JSON_Quvest/app.js`
* `JSON_Quvest/Silver_77_Quests.json` успешно парсится.
* Редактор открывается.
* Блок `NPC комплект / npcEquipment` отображается компактнее.
* Каждый item занимает одну строку.
* Clothing отображается строками.
* Container отображается компактно, а его items идут строками.
* Hands отображается одной строкой.
* Weapon отображается компактно, magazine находится в строке оружия или рядом максимально компактно.
* Attachments и ammo отображаются строками.
* Сохранение JSON не ломает структуру `npcEquipment`.
* Legacy-поля остались на месте.
* В отчёте явно написать: “моды обновлять не нужно, менялся только JSON_Quvest”.

### Отчёт

AGENT REPORT

DONE:

* что сделано

CHANGED FILES:

* файлы

UI:

* что изменилось визуально

ROW_LAYOUT:

* какие секции стали построчными

CHECKS:

* что проверено

MOD UPDATE:

* нужны ли обновления модов

NOT DONE:

* что специально не делалось

PROBLEMS:

* проблемы/риски

CONCLUSION:

* готово ли к ручному тесту


================================================================================
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- `Documentation/SplitDoc/AGENT_RULES.md` физически заменён на `Documentation/SplitDoc/START.md`.
- Все старые правила из `AGENT_RULES.md` сохранены в `START.md`.
- В `START.md` добавлен раздел `## Правила Git / репозитория`.
- В Git-правило добавлен рабочий путь проекта: `P:\Silver_77_Quests`.
- Обновлены актуальные ссылки обязательного чтения на `START.md`.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\START.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

DIFF:
- `AGENT_RULES.md` переименован/заменён на `START.md`.
- В `AGENT_TASK_LOOP.md` постоянная карта правил и обязательное чтение указывают на `START.md`.
- В `START.md` добавлены правила проверки GitHub после отчёта Codex и обязательный формат начала REVIEW.
- В `Documentation/SplitDoc/TASK_HISTORY.md` добавлена запись по `TASK 103`.

GIT RULES CHECK:
- `START.md` содержит формат `СТАТУС КОММИТА`.
- Правило сохраняет commit message как есть, включая `+`, `01`, `02`.
- Правило требует честно писать, если message не подтверждён.
- Правило запрещает ассистенту утверждать, что он видит локальные изменения.
- Зафиксировано, что Git контролирует пользователь.

ENCODING CHECK:
- Документные файлы сохранены как UTF-8 без BOM.
- Кириллица в изменённых документах читается корректно.

PROBLEMS:
- Нет.

CONCLUSION:
- TASK 103 выполнен как документационная правка: стартовый документ правил теперь `START.md`, Git/review-правило добавлено, код/JSON/layout/server/PBO не менялись.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# 🟨 БЛОК 3 — REVIEW / STATUS
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

TASK 101 REVIEW

Статус:
Принято после runtime-проверки пользователя.

Commit baseline:
`2fc3212f8a381efd7d33bdd60ff2491f5b5e3800`

Что изменено:
- QuestPanelBackdrop: 1040x620 -> 1040x710
- QuestPanel: 980x560 -> 980x650

Description:
- DescriptionPanel: 430x204 -> 430x272
- DescriptionScroll: 414x156 -> 414x224
- DescriptionText: 394x600 -> 406x224
- Scrollbar V: 1 -> 0

Dialog:
- DialogPanel: y 286 -> 358
- DialogPanel: 430x122 -> 430x164
- DialogScroll: 414x74 -> 414x116
- DialogText: 394x300 -> 406x116
- Scrollbar V: 1 -> 0

Buttons:
- AcceptButton y: 420 -> 540
- CompleteButton y: 420 -> 540
- CloseButton y: 490 -> 594

Подтверждено в runtime:
- текущий вариант визуально нормальный и пока принимается как рабочий baseline;
- `Description/Dialog` увеличены по вертикали;
- `Scrollbar V` отключён;
- используется простой стек `ScrollWidget -> MultilineTextWidget`;
- `QuestUI.c` не менялся;
- `wrapper/mask/SizeToChild/RichText/EditBox` не используются.

Known limitation:
- очень длинный текст может обрезаться, потому что scroll/clipping тема пока отложена.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
