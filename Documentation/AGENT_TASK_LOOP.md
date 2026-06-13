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
- В `Documentation/SplitDoc/START.md` добавлено правило выборочного чтения `SplitDoc`.
- Подробные Git/REVIEW правила вынесены из `START.md` в `Documentation/SplitDoc/GIT_REVIEW_RULES.md`.
- В `START.md` оставлена короткая ссылка на `GIT_REVIEW_RULES.md` и базовое напоминание, что Git контролирует пользователь.
- В матрицу чтения `Documentation/AGENT_TASK_LOOP.md` добавлена строка для Git / REVIEW.
- В `Documentation/SplitDoc/TASK_HISTORY.md` добавлена короткая запись по `TASK 106`.
- Текущий `БЛОК 1` очищен по правилу из `START.md`.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation`

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\START.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

CREATED FILES:
- `P:\Silver_77_Quests\Documentation\SplitDoc\GIT_REVIEW_RULES.md`

DIFF:
- `START.md` стал короче в части Git/REVIEW: вместо подробных правил оставлен раздел `Git / REVIEW` со ссылкой на `GIT_REVIEW_RULES.md`.
- В `START.md` добавлено правило, что ChatGPT выбирает минимальный набор тематических `SplitDoc`, а агент не читает все `SplitDoc` подряд.
- `GIT_REVIEW_RULES.md` содержит подробные правила commit/push/reset/clean/checkout, GitHub-проверки, `СТАТУС КОММИТА`, commit message и статуса чтения GitHub-репозитория.
- В `AGENT_TASK_LOOP.md` матрица чтения дополнена строкой `Git / REVIEW / commit / push check / история Git`.
- История пополнена записью о `TASK 106`.
- `БЛОК 1` заменён на стандартную заглушку.

CHECKS:
- `START.md` стал короче в части Git/REVIEW правил.
- `GIT_REVIEW_RULES.md` содержит подробные Git/REVIEW правила.
- `START.md` содержит короткую ссылку на `GIT_REVIEW_RULES.md`.
- Правило “ChatGPT является маршрутизатором чтения правил” добавлено.
- Правило “агент не читает все SplitDoc подряд” добавлено.
- `AGENT_TASK_LOOP.md` содержит строку матрицы для Git / REVIEW.
- `БЛОК 1` очищен корректно.
- Документация сохранена в UTF-8 без BOM.
- Моды обновлять не нужно.

PROBLEMS:
- Нет.

CONCLUSION:
- TASK 106 выполнен как документационная правка: выборочное чтение закреплено, Git/REVIEW правила вынесены в отдельный `SplitDoc`, код/JSON/layout/server profile/runtime-данные не менялись.

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
