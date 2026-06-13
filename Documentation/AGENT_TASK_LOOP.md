# AGENT TASK LOOP

Это рабочий shell-документ текущего агентского цикла.

Он нужен для того, чтобы хранить:

- текущую задачу в `БЛОКЕ 1`;
- последний `AGENT REPORT` в `БЛОКЕ 2`;
- последний `REVIEW / STATUS` в `БЛОКЕ 3`;
- короткую карту чтения `SplitDoc`.

Правила:

- Постоянные правила: `Documentation/SplitDoc/START.md`
- Тематические правила: по матрице `SplitDoc` ниже

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
- `Git / REVIEW / commit / push check / история Git` -> `GIT_REVIEW_RULES.md`
- `Encoding / кириллица` -> `ENCODING_RULES.md`
- `Server profile / dev JSON / перенос на сервер` -> `SERVER_PROFILE_RULES.md`
- `History / status / прошлые решения` -> `TASK_HISTORY.md`

================================================================================
# 🟦 БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА ДЛЯ АГЕНТА
================================================================================

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

Активной задачи нет.
Предыдущая задача выполнена.
Ожидается новая задача от пользователя.





================================================================================
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- В Obsidian-базе `Documentation_DayZ_Qwest` исправлены старые упоминания `DayZ_Qwest` и старые пути на актуальное имя/путь.
- В `Documentation_DayZ_Qwest/02_Решения.md` исправлена сломанная Markdown-строка пути.
- В заметках `02`-`06` добавлены нормальные первые заголовки.
- В `Documentation_DayZ_Qwest/00_Главная.md` добавлен раздел `Агентская система`.
- Созданы компактные Obsidian-зеркала агентских правил.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена запись о подготовке Obsidian-перехода.
- В `Documentation/SplitDoc/TASK_HISTORY.md` добавлена короткая запись по `TASK 107`.
- Текущий `БЛОК 1` очищен по правилу из `START.md`.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation`

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\00_Главная.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\01_Текущее_состояние.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\02_Решения.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\03_Открытые_вопросы.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\04_Архитектура.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\06_Идеи.md`

CREATED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`

DIFF:
- Старые упоминания Obsidian-базы `DayZ_Qwest` заменены на `Documentation_DayZ_Qwest`.
- Актуальный путь базы указан как `P:\Silver_77_Quests\Documentation_DayZ_Qwest`.
- `02_Решения.md` получил корректный путь ``Documentation_DayZ_Qwest``.
- В `00_Главная.md` добавлены ссылки `[[10_Правила_агента]]`, `[[11_AGENT_TASK_LOOP_зеркало]]`, `[[12_START_зеркало]]`.
- Новые зеркала описывают текущие рабочие оригиналы, но не заменяют их.
- История пополнена записью о `TASK 107`.
- `БЛОК 1` заменён на стандартную заглушку.

CHECKS:
- Старые упоминания `DayZ_Qwest` в Obsidian-базе заменены на `Documentation_DayZ_Qwest`.
- Путь указан точно: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`.
- `02_Решения.md` больше не содержит сломанную Markdown-кавычку.
- Новые Obsidian-файлы созданы и связаны из `00_Главная.md`.
- Старые рабочие документы остались на своих местах.
- `БЛОК 1` очищен корректно.
- Документация сохранена в UTF-8 без BOM.
- Моды обновлять не нужно.

PROBLEMS:
- Нет.

CONCLUSION:
- TASK 107 выполнен как подготовительный Obsidian-переход: база `Documentation_DayZ_Qwest` нормализована, зеркала агентских документов созданы, старые рабочие документы не удалялись и физически не переносились.

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
