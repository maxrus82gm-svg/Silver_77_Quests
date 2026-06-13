# AGENT TASK LOOP

Это рабочий shell-документ текущего агентского цикла.

Он нужен для того, чтобы хранить:

- текущую задачу в `БЛОКЕ 1`;
- последний `AGENT REPORT` в `БЛОКЕ 2`;
- последний `REVIEW / STATUS` в `БЛОКЕ 3`;
- короткую карту чтения `SplitDoc`.

Правила:

- Постоянные правила: `Documentation_DayZ_Qwest/12_START_зеркало.md`
- Тематические правила: `Documentation_DayZ_Qwest/20_SplitDoc` по матрице ниже

## Что читать всегда

Агент всегда читает:

- `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md`
- `Documentation_DayZ_Qwest/12_START_зеркало.md`

## Матрица чтения SplitDoc

- `UI / QuestMenu / QuestJournal` -> `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_UI_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_JSON_CONTRACT.md` + `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
- `Quest Editor / JSON_Quvest` -> `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_EDITOR_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_JSON_CONTRACT.md` + `Documentation_DayZ_Qwest/20_SplitDoc/SERVER_PROFILE_RULES.md`
- `JSON contract / quest logic` -> `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_JSON_CONTRACT.md` + `Documentation_DayZ_Qwest/21_Project_Docs/QUEST_LOGIC_SPEC.md`
- `DayZ layout viewer` -> `Documentation_DayZ_Qwest/20_SplitDoc/DAYZ_LAYOUT_VIEWER_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
- `RPC / client-server sync` -> `Documentation_DayZ_Qwest/20_SplitDoc/DAYZ_RPC_SYNC_RULES.md`
- `Git / REVIEW / commit / push check / история Git` -> `Documentation_DayZ_Qwest/20_SplitDoc/GIT_REVIEW_RULES.md`
- `Encoding / кириллица` -> `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
- `Server profile / dev JSON / перенос на сервер` -> `Documentation_DayZ_Qwest/20_SplitDoc/SERVER_PROFILE_RULES.md`
- `History / status / прошлые решения` -> `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md`
## Канонические пути

Тематические правила читать из:

- `Documentation_DayZ_Qwest/20_SplitDoc`

Проектные документы читать из:

- `Documentation_DayZ_Qwest/21_Project_Docs`

## Очистка выполненной задачи

После успешного выполнения задачи агент обязан:

- обновить `БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА`;
- при необходимости обновить `БЛОК 3 — REVIEW / STATUS`;
- добавить запись в `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md`;
- очистить активный `БЛОК 1` в `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md`.

После очистки `БЛОК 1` должен выглядеть так:

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

Активной задачи нет.  
Предыдущая задача выполнена.  
Ожидается новая задача от пользователя.

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
- Выполнена диагностика старых путей `Documentation` / `Documentation/...` по Markdown-документам проекта.
- Активные хвосты внутри `Documentation_DayZ_Qwest` заменены на Obsidian-пути или явно классифицированы.
- Старые bridge-файлы проверены и оставлены без удаления.
- Создан отчёт-инвентаризация хвостов старой `Documentation`.
- В `10_Правила_агента.md` добавлено правило не использовать `Documentation/...` как активный источник правды.
- В `TASK_HISTORY.md` добавлена запись `TASK 111`.
- В `05_Очередь_задач.md` добавлена выполненная TASK 111, старую будущую задачу про карту путей перенумеровал в TASK 112, чтобы не было конфликта номеров.
- Активный `БЛОК 1` в Obsidian-loop очищен.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\00_Главная.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\01_Текущее_состояние.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\04_Архитектура.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\00_INDEX.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\ENCODING_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\QUEST_EDITOR_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\QUEST_JSON_CONTRACT.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\QUEST_UI_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\SERVER_PROFILE_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\21_Project_Docs\00_INDEX.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\21_Project_Docs\BUILD.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\21_Project_Docs\README.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\21_Project_Docs\RUSSIAN_ENCODING.md`

CREATED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\22_Legacy_Check\00_Проверка_хвостов_Documentation.md`

DIFF:
- Активные индексы `20_SplitDoc` и `21_Project_Docs` больше не называют старую `Documentation` источником правды.
- `START`, текущее состояние, архитектура и внутренние ссылки `SplitDoc` переведены на `Documentation_DayZ_Qwest`.
- В проектных документах Obsidian обновлены активные входные ссылки агента и ссылка на split-checklist.
- Добавлена навигационная ссылка на legacy-check в `00_Главная.md`.

CHECKS:
- Старый `Documentation/AGENT_TASK_LOOP.md` указывает на `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md`.
- Старый `Documentation/SplitDoc/START.md` указывает на Obsidian START, Obsidian loop, `20_SplitDoc` и `21_Project_Docs`.
- Обязательное чтение `Documentation_DayZ_Qwest/12_START_зеркало.md` сохранено.
- В активном `БЛОКЕ 1` нет TASK 111 после очистки.
- Отчёт `22_Legacy_Check/00_Проверка_хвостов_Documentation.md` создан.
- `00_Главная.md` содержит ссылку на отчёт.
- `TASK_HISTORY.md` содержит запись TASK 111.
- Документация сохранена в UTF-8 без BOM.
- Моды обновлять не нужно.

FOUND LEGACY REFERENCES:
- Старые bridge-файлы: `Documentation/AGENT_TASK_LOOP.md`, `Documentation/SplitDoc/START.md`.
- Legacy-описание в `10_Правила_агента.md` оставлено намеренно.
- `Documentation/` в деревьях сборки `BUILD.md` / `SPLIT_CLIENT_SERVER.md` оставлено как описание структуры старого/публикуемого мода, не как активный источник правил.

LEFT AS HISTORY:
- Старые пути в `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` оставлены как история TASK 104-110.
- Старые отчёты, CODEX-документы, worklog/changelog и архивные заметки в `Documentation` не переписывались.
- Ссылки на старые CODEX-документы в `21_Project_Docs/RUSSIAN_ENCODING.md` оставлены как legacy-источник для восстановления текста.

PROBLEMS:
- Вне разрешённого scope остались старые ссылки в корневом `README.md`, `JSON_Quvest/README.md`, `Support/.../README.md` и старых CODEX-документах. Они записаны в отчёт-инвентаризацию как хвосты для отдельной задачи, если понадобится полная чистка.

CONCLUSION:
- TASK 111 выполнен: активные Obsidian-документы больше не используют старую `Documentation` как источник правды, bridge и история сохранены, текущий `БЛОК 1` очищен.
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

- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md`
