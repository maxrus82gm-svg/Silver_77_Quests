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
- `Trader / TraderObjects / TraderConfig` -> `Documentation_DayZ_Qwest/20_SplitDoc/TRADER_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/SERVER_PROFILE_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
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
- Выполнена финальная миграция хвостов старой `Documentation`.
- Активные README и пользовательские точки входа переведены на `Documentation_DayZ_Qwest`.
- Создан Obsidian-архив `Documentation_DayZ_Qwest/90_Legacy_Archive`.
- Старые Markdown-документы из `Documentation` и `Documentation/SplitDoc` скопированы в архив полным содержимым.
- Перед удалением старых `.md`-дублей выполнена SHA256-сверка архивных копий.
- Старые `.md`-дубли удалены из `Documentation` и `Documentation/SplitDoc`, кроме bridge-файлов.
- Старые bridge-файлы оставлены: `Documentation/AGENT_TASK_LOOP.md` и `Documentation/SplitDoc/START.md`.
- В `00_Главная.md` добавлена ссылка на `90_Legacy_Archive`.
- В `10_Правила_агента.md` закреплено, что старую `Documentation` нельзя использовать как рабочий канон.
- В `22_Legacy_Check/00_Проверка_хвостов_Documentation.md` добавлен раздел TASK 112.
- В `TASK_HISTORY.md` добавлена запись TASK 112.
- В `05_Очередь_задач.md` добавлена выполненная TASK 112, будущая задача про карту путей перенумерована в TASK 113.
- Активный `БЛОК 1` очищен.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`

CHANGED FILES:
- `P:\Silver_77_Quests\README.md`
- `P:\Silver_77_Quests\JSON_Quvest\README.md`
- `P:\Silver_77_Quests\Support\JSON_Quvest\README.md`
- `P:\Silver_77_Quests\Support\Arh_29042026\JSON_Quvest\README.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\00_Главная.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\22_Legacy_Check\00_Проверка_хвостов_Documentation.md`

CREATED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\90_Legacy_Archive\00_INDEX.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\90_Legacy_Archive\Documentation\*.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\90_Legacy_Archive\Documentation_SplitDoc\*.md`

DELETED FILES:
- `P:\Silver_77_Quests\Documentation\BUILD.md`
- `P:\Silver_77_Quests\Documentation\CHANGELOG.md`
- `P:\Silver_77_Quests\Documentation\CODEX_CONTEXT_2026-04-20.md`
- `P:\Silver_77_Quests\Documentation\CODEX_CONTROL_CONTEXT.md`
- `P:\Silver_77_Quests\Documentation\CODEX_EMERGENCY_CONTEXT.md`
- `P:\Silver_77_Quests\Documentation\CODEX_FAST_HANDOFF_2026-04-27.md`
- `P:\Silver_77_Quests\Documentation\CODEX_SESSION_HANDOFF_2026-04-25.md`
- `P:\Silver_77_Quests\Documentation\CODEX_SESSION_HANDOFF_2026-04-26.md`
- `P:\Silver_77_Quests\Documentation\CODEX_SESSION_HANDOFF_2026-04-29.md`
- `P:\Silver_77_Quests\Documentation\CODEX_START_HERE.md`
- `P:\Silver_77_Quests\Documentation\CODEX_WORKLOG.md`
- `P:\Silver_77_Quests\Documentation\QUEST_LOGIC_SPEC.md`
- `P:\Silver_77_Quests\Documentation\README.md`
- `P:\Silver_77_Quests\Documentation\README_INSTALLATION.md`
- `P:\Silver_77_Quests\Documentation\README_JSON_CONFIG.md`
- `P:\Silver_77_Quests\Documentation\REPORT_2026-04-18.md`
- `P:\Silver_77_Quests\Documentation\RUSSIAN_ENCODING.md`
- `P:\Silver_77_Quests\Documentation\SPLIT_CLIENT_SERVER.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_RPC_SYNC_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\GIT_REVIEW_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_EDITOR_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_JSON_CONTRACT.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\SERVER_PROFILE_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

MOVED TO OBSIDIAN ARCHIVE:
- `Documentation/*.md` -> `Documentation_DayZ_Qwest/90_Legacy_Archive/Documentation/*.md`
- `Documentation/SplitDoc/*.md` -> `Documentation_DayZ_Qwest/90_Legacy_Archive/Documentation_SplitDoc/*.md`
- Архивные копии проверены по SHA256 перед удалением старых дублей.

DIFF:
- README-точки входа теперь указывают на Obsidian-loop и Obsidian-START.
- В Obsidian добавлен архив старой `Documentation` с индексом.
- Старые `.md`-дубли удалены после подтверждённого переноса.
- Старые bridge-файлы оставлены, но не являются рабочим каноном.

CHECKS:
- `README.md` больше не ведёт на старую `Documentation` как рабочий источник.
- `JSON_Quvest/README.md` больше не ведёт на старую `Documentation` как рабочий источник.
- `Support/**/README.md` больше не ведут на старую `Documentation` как рабочий источник.
- В старой `Documentation` из Markdown остался только `AGENT_TASK_LOOP.md` bridge.
- В старой `Documentation/SplitDoc` из Markdown остался только `START.md` bridge.
- `90_Legacy_Archive/00_INDEX.md` создан.
- `00_Главная.md` содержит ссылку на `90_Legacy_Archive`.
- `10_Правила_агента.md` запрещает использовать старую `Documentation` как рабочий канон.
- `TASK_HISTORY.md` содержит TASK 112.
- Активный `БЛОК 1` очищен.
- Документация сохранена в UTF-8 без BOM.
- Моды обновлять не нужно.

REMAINING LEGACY REFERENCES:
- `Documentation/AGENT_TASK_LOOP.md` — оставленный bridge.
- `Documentation/SplitDoc/START.md` — оставленный bridge.
- Старые ссылки внутри `Documentation_DayZ_Qwest/90_Legacy_Archive` — историческое содержимое архивных копий.
- Старые ссылки в `TASK_HISTORY`, legacy-check и отчётах — история миграции.
- `Documentation_DayZ_Qwest/20_SplitDoc/00_INDEX.md` — legacy-описание старого источника.
- `Documentation_DayZ_Qwest/21_Project_Docs/BUILD.md` и `SPLIT_CLIENT_SERVER.md` — `Documentation/` как часть старой/публикуемой структуры мода.
- `Documentation_DayZ_Qwest/21_Project_Docs/RUSSIAN_ENCODING.md` — старые CODEX-ссылки как legacy-источник восстановления текста; архивные копии теперь лежат в `90_Legacy_Archive`.

PROBLEMS:
- `.obsidian/graph.json`, `.obsidian/workspace.json`, `Documentation_DayZ_Qwest/2026-06-13.md` и `Documentation_DayZ_Qwest/Без названия.canvas` уже отображаются в `git status` как внешние изменения/новые Obsidian-файлы; я их не редактировал и не удалял.

CONCLUSION:
- TASK 112 выполнен: рабочий канон перенесён в `Documentation_DayZ_Qwest`, активные README больше не ведут в старую `Documentation`, старые Markdown-документы перенесены в Obsidian-архив, а старые `.md`-дубли удалены после проверки копий.
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
