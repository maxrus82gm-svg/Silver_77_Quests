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

TASK 101 — QuestMenu: пропорционально увеличить Description/Dialog по вертикали и отключить scroll

Статус:
Практическая UI-задача. Без новых экспериментов со ScrollWidget clipping.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

TASK 102 — Documentation: зафиксировать runtime-accepted QuestMenu layout после TASK 101

Статус:
Документационная задача. UI-код не менять.

Цель:
Зафиксировать, что commit `2fc3212f8a381efd7d33bdd60ff2491f5b5e3800` принят после runtime-проверки как текущий рабочий вариант QuestMenu.

Контекст:
После проблем со ScrollWidget clipping было принято компромиссное решение:
- не чинить scroll clipping сейчас;
- увеличить Description/Dialog по вертикали;
- отключить scrollbar;
- оставить простой стек `ScrollWidget -> MultilineTextWidget`;
- не использовать wrapper/mask/SizeToChild/RichText/EditBox.

Runtime-проверка пользователем:
- всё нормально;
- визуально принимаем;
- фиксируем так пока что.

Разрешено менять только:
- P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
- P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Запрещено менять:
- QuestMenu.layout
- QuestUI.c
- dayz_layout_viewer.html
- JSON
- server
- QuestJournal
- @Trader
- любые UI/layout/script файлы

Что записать:
1. TASK 101 принят runtime.
2. Commit: `2fc3212f8a381efd7d33bdd60ff2491f5b5e3800`.
3. Текущий QuestMenu baseline:
   - Description/Dialog увеличены по вертикали.
   - Scrollbar V отключён.
   - DescriptionText/DialogText остались MultilineTextWidgetClass.
   - Стек остался:
     - DescriptionScroll -> DescriptionText
     - DialogScroll -> DialogText
   - QuestUI.c не менялся.
4. Known limitation:
   - очень длинные тексты могут обрезаться;
   - scroll/clipping остаётся отложенной отдельной темой.
5. Decision:
   - текущий вариант принят как рабочий baseline;
   - новые scroll/clipping эксперименты не делать без отдельной задачи.

Ожидаемый отчёт:
AGENT REPORT

DONE:
- что записано в документацию.

CHANGED FILES:
- список файлов.

DIFF:
- кратко описать добавленные записи.

PROBLEMS:
- если не удалось обновить какой-то doc.

CONCLUSION:
- краткий вывод.
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
- В `AGENT_TASK_LOOP.md` зафиксировано, что commit `2fc3212f8a381efd7d33bdd60ff2491f5b5e3800` принят после runtime-проверки как текущий рабочий baseline `QuestMenu`.
- В `БЛОКЕ 3` обновлён `TASK 101 REVIEW`: статус переведён из условного в runtime-accepted.
- В `TASK_HISTORY.md` добавлены записи по `TASK 101` и `TASK 102`.
- В документации зафиксированы текущие свойства baseline:
  - `Description/Dialog` увеличены по вертикали;
  - `Scrollbar V` отключён;
  - стек остаётся `ScrollWidget -> MultilineTextWidget`;
  - `QuestUI.c` не менялся;
  - `wrapper/mask/SizeToChild/RichText/EditBox` не используются;
  - очень длинный текст всё ещё может обрезаться;
  - тема `scroll/clipping` отложена.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

DIFF:
- В `AGENT_TASK_LOOP.md` обновлён последний `AGENT REPORT` под `TASK 102`.
- В `AGENT_TASK_LOOP.md` обновлён `TASK 101 REVIEW`: runtime-проверка пользователя подтверждает, что текущий вариант пока принимается как рабочий baseline.
- В `TASK_HISTORY.md` добавлена история принятия `TASK 101` и документирующего `TASK 102`.
- Зафиксирован commit baseline: `2fc3212f8a381efd7d33bdd60ff2491f5b5e3800`.

PROBLEMS:
- `P:` в этой сессии не смонтирован, поэтому документация обновлена в рабочей копии `D:\Dayz\Silver_77_Quests`, которая используется как локальное зеркало проекта.

QUESTIONS:
- Нет.

CONCLUSION:
- Runtime-проверка пользователя приняла `TASK 101` как текущий рабочий baseline `QuestMenu`, и это состояние теперь явно зафиксировано в `AGENT_TASK_LOOP.md` и `TASK_HISTORY.md` без правок UI-кода.

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
