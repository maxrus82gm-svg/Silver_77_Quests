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

TASK 108 — Analysis / Runtime Check: проверить почему завершённый квест снова доступен и зафиксировать поведение выдачи при полном инвентаре

Статус:
Аналитико-диагностическая задача. Код, JSON, layout, server profile и player progress не менять.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 106 был сделан билд клиента и сервера.

Пользователь проверил квест в игре:

1. Квестовое меню открывается.
2. Квесты отображаются.
3. Диалог NPC отображается.
4. Один из квестов после завершения снова стал доступен.
5. У пользователя был полный инвентарь.
6. Предметы, которые не влезли в инвентарь, сбросились на землю.

Наблюдение по выдаче предметов:
fallback на землю при полном инвентаре фактически работает.

Но нужно понять:
почему квест снова стал доступен.

Возможные причины:

- квест repeatable;
- cooldown уже прошёл;
- player progress был очищен или создан заново;
- использован другой SteamID / другой profile;
- lastCompletedTime старый или некорректный;
- статус квеста был сброшен;
- логика repeat/cooldown работает как задумано;
- или есть баг, из-за которого одноразовый квест снова доступен.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 108
--------------------------------------------------------------------------------

Провести диагностику без правок.

Нужно ответить:

1. Почему конкретный квест снова стал доступен после завершения.
2. Является ли это ожидаемым поведением по JSON и текущей логике.
3. Включён ли у квеста repeatable.
4. Какой у квеста cooldownSeconds.
5. Какой статус квеста записан в player progress.
6. Какой lastCompletedTime записан в player progress.
7. Создались ли новые поля из TASK 106:
   - currentAttemptId;
   - lastFinalizedAttemptId;
   - pendingRewards;
   - deliveredRewards.
8. Не был ли player progress создан заново.
9. Не используется ли другой SteamID / другой profile.
10. Подтвердить по коду и/или логам, что fallback выдачи предметов на землю при полном инвентаре соответствует текущей реализации.
11. Сформулировать, нужно ли менять repeat/cooldown-логику в следующих задачах.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Это только диагностика.

Запрещено менять любые файлы.

Запрещено:

- менять .c файлы;
- менять .json файлы;
- менять .layout файлы;
- менять документацию;
- менять server profile;
- менять player progress;
- чистить player progress;
- удалять progress;
- запускать reset progress;
- делать git commit;
- делать git push;
- делать git reset;
- делать git clean;
- делать git checkout;
- запускать Addon Builder;
- перепаковывать PBO.

Git контролирует пользователь.

Если найдена проблема, её нужно только описать в отчёте и предложить следующую задачу.
Ничего не исправлять.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Обязательно прочитать:

1. Documentation/AGENT_TASK_LOOP.md
2. Documentation/SplitDoc/START.md
3. Documentation/SplitDoc/QUEST_JSON_CONTRACT.md
4. Documentation/QUEST_LOGIC_SPEC.md
5. Documentation/SplitDoc/SERVER_PROFILE_RULES.md
6. Documentation/SplitDoc/TASK_HISTORY.md

Обязательно проверить код:

1. Silver_77_Quests_Client/scripts/3_Game/PlayerQuestData.c
2. Silver_77_Quests_Client/scripts/3_Game/QuestData.c
3. Silver_77_Quests_Server/scripts/4_World/QuestServerManager.c
4. Silver_77_Quests_Server/scripts/4_World/QuestServerRPC.c
5. Silver_77_Quests_Client/scripts/4_World/QuestClientManager.c

Разрешено читать:

1. JSON_Quvest/Silver_77_Quests.json
2. server profile quest JSON, если он доступен локально
3. player progress JSON, если он доступен локально
4. server RPT / script logs, если они доступны локально

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Найти квест, который снова стал доступен.

По скриншоту это квест с названием:

Картошка с маслицем

Нужно найти его в JSON_Quvest/Silver_77_Quests.json и определить его questId.

2. Проверить JSON-настройки этого квеста.

Нужно выписать:

- questId;
- title/name;
- repeatable;
- cooldownSeconds;
- offerTriggerIds;
- completionTriggerIds;
- rewardTriggerIds;
- rewards;
- giveItems;
- triggerActions, связанные с Offer / Completion / Reward.

3. Проверить текущую repeat/cooldown-логику в коде.

Нужно найти и описать:

- где проверяется repeatable;
- где проверяется cooldownSeconds;
- где используется lastCompletedTime;
- где квест снова становится доступным;
- при каких условиях completed-квест можно взять повторно.

4. Проверить player progress.

Если player progress доступен, найти запись по questId и выписать:

- questId;
- status;
- lastCompletedTime;
- currentAttemptId;
- lastFinalizedAttemptId;
- pendingRewards;
- deliveredRewards;
- objectiveProgress;
- completedCompletionTriggerIds;
- stageVisits.

Важно:
ничего в progress не менять.

5. Проверить, был ли progress создан заново.

Признаки:

- отсутствует старая запись по questId;
- lastCompletedTime = 0;
- status = available, хотя квест проходился;
- другой steamId;
- другой путь profile;
- новый файл player progress.

6. Проверить fallback на землю.

Пользователь сообщил runtime-наблюдение:
при полном инвентаре предметы сбросились на землю.

Нужно сверить это с кодом:

- где вызывается CreateInInventory;
- где fallback на CreateObjectEx;
- проверяется ли результат CreateObjectEx;
- есть ли сообщение игроку;
- влияет ли успех выдачи на status квеста.

7. Сделать вывод:

- квест снова доступен потому что repeatable/cooldown;
- или потому что progress сброшен/другой;
- или потому что есть баг;
- или без player progress/RPT точно подтвердить нельзя.

8. Предложить следующую задачу.

Если поведение нормальное:
предложить TASK 109 на safe-функцию выдачи предметов.

Если поведение ошибочное:
предложить отдельную TASK на исправление repeat/cooldown или progress-state.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Моё мнение:

Сейчас не нужно сразу исправлять код.

Нужно сначала понять, почему квест снова доступен:

1. Если квест repeatable и cooldown прошёл — это нормальное старое поведение.
2. Если progress был пересоздан — это не баг квеста, а эффект нового/чистого progress.
3. Если квест не repeatable, но снова доступен при сохранённом progress — это баг.
4. Если cooldown стартует сразу после completed, это уже известный риск, который нужно будет исправлять после внедрения safe reward delivery.

Факт, что предметы падают на землю при полном инвентаре, полезен:
текущий fallback работает, но он всё ещё не гарантирует безопасную выдачу, потому что результат CreateObjectEx не проверяется и нет pendingRewards-довыдачи.

--------------------------------------------------------------------------------
ФОРМАТ ОТЧЁТА
--------------------------------------------------------------------------------

Вернуть отчёт в чат:

AGENT REPORT

DONE:
- что было проверено

READ FILES:
- какие файлы прочитаны

QUEST CHECK:
- questId
- название квеста
- repeatable
- cooldownSeconds
- Offer / Completion / Reward triggers
- rewards / giveItems

PLAYER PROGRESS CHECK:
- путь progress-файла, если проверялся
- steamId, если найден
- status
- lastCompletedTime
- currentAttemptId
- lastFinalizedAttemptId
- pendingRewards
- deliveredRewards
- признаки старого или нового progress

REPEAT / COOLDOWN CHECK:
- где в коде проверяется repeatable
- где проверяется cooldown
- почему квест снова доступен
- это норма или риск

FULL INVENTORY / GROUND FALLBACK CHECK:
- где код пытается положить предмет в инвентарь
- где fallback создаёт предмет на земле
- проверяется ли результат
- соответствует ли это runtime-наблюдению пользователя

PROBLEMS:
- реальные проблемы и риски

QUESTIONS:
- только реальные вопросы

RECOMMENDED NEXT TASK:
- предложить следующий безопасный шаг

CONCLUSION:
- короткий итог: почему квест снова доступен и что делать дальше


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
