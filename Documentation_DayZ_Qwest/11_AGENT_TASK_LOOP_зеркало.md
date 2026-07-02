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
- `Documentation / Markdown / создание документов / обновление правил` -> `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md` + `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
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
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- Обновлён `Documentation_DayZ_Qwest/20_SplitDoc/GIT_REVIEW_RULES.md` под проект `Silver_77_Quests`.
- Создан `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md` для правил создания и обновления Markdown-документов.
- В `Documentation_DayZ_Qwest/12_START_зеркало.md` добавлена краткая маршрутизация для Git / REVIEW и Documentation / Markdown задач.
- В матрицу чтения `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md` добавлена строка для документационных задач.
- В `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` добавлена запись `TASK 118`.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена выполненная `TASK 118`.
- Активный `БЛОК 1` очищен после выполнения задачи.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\GIT_REVIEW_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\DOCUMENTATION_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`

CREATED FILES:
- `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md`

UPDATED RULES:
- Git контролирует пользователь; агент не выполняет изменяющие Git-команды без прямой команды пользователя.
- Read-only Git-команды выполняются только если текущая задача прямо разрешает Git-проверку или сама связана с Git / review.
- После `AGENT REPORT` ChatGPT выполняет review через репозиторий или доступные файлы, если это возможно.
- Итоговый `СТАТУС КОММИТА` пишет ChatGPT, а не агент.
- Документы создаются и обновляются только по задаче пользователя или активной задаче в `БЛОКЕ 1`.
- Для Markdown-документов закреплены UTF-8 без BOM, настоящие переносы строк, запрет дублей и запрет случайного переноса чужих путей.

CHECKS:
- Аналог `DOCUMENTATION_RULES.md` в `Documentation_DayZ_Qwest/20_SplitDoc` не найден, поэтому создан новый документ.
- `GIT_REVIEW_RULES.md` обновлён, а не продублирован.
- `START` содержит только краткую маршрутизацию, подробности оставлены в SplitDoc.
- `AGENT_TASK_LOOP` содержит новую строку матрицы для документационных задач.
- `TASK_HISTORY.md` содержит `TASK 118`.
- `05_Очередь_задач.md` содержит `TASK 118` со статусом `Готово`.
- Активный `БЛОК 1` очищен.
- JSON, UI, layout, моды, server profile и PBO/build output не менялись.

ENCODING CHECK:
- Изменённые Markdown-файлы читаются как UTF-8.
- BOM не обнаружен.
- Кириллица не повреждена.
- Markdown не записан одной строкой.
- Текстовая последовательность `\n` вместо переносов в новых правилах не используется.

GIT:
- Git-команды не выполнялись.
- Commit не создавался агентом.
- Push не выполнялся агентом.
- Git-история не изменялась агентом.

PROBLEMS:
- Прямых файлов с исходными именами пользовательских документов в рабочем дереве не найдено; использованы тексты из attachments и конкретные требования активного `БЛОКА 1`.

CONCLUSION:
- TASK 118 выполнен. Правила Git-review и создания документов адаптированы под `Silver_77_Quests`, активный `БЛОК 1` очищен. Моды обновлять не нужно.
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


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
