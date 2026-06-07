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

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ЗАДАЧИ
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# TASK 106 — ДВА БЛОКА ДЛЯ АГЕНТА
================================================================================

ВНИМАНИЕ ДЛЯ ПОЛЬЗОВАТЕЛЯ:

1. Сначала вставь раздел "ПОЛНЫЙ БЛОК 1" в:
   Documentation/AGENT_TASK_LOOP.md

2. Потом отправь раздел "МАЛЫЙ БЛОК В ЧАТ АГЕНТУ" агенту в чат.

================================================================================
# ПОЛНЫЙ БЛОК 1 ДЛЯ Documentation/AGENT_TASK_LOOP.md
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ЗАДАЧИ
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

TASK 106 — Implementation Foundation: добавить в player progress основу для attemptId, pendingRewards и deliveredRewards

Статус:
Реализационная задача с жёсткими рамками.

Цель задачи — заложить безопасный фундамент для новой системы выдачи квестовых предметов:
- история попыток квеста;
- история обещанных/ожидающих наград;
- история успешно выданных наград;
- защита будущей логики от дублей и потерь.

Важно:
В этой задаче НЕ нужно полностью переписывать выдачу Offer / Completion / Reward.
В этой задаче нужно добавить структуры данных и безопасное сохранение/загрузку progress.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

По результатам TASK 104 и TASK 105 выяснено:

1. Сейчас выдача предметов NPC идёт через SpawnQuestItem.
2. Предмет сначала пробуется в инвентарь, потом fallback на землю.
3. Успешность финального создания предмета не проверяется как часть логики квеста.
4. Квест может стать active/completed даже если предмет фактически не был выдан.
5. Сейчас progress хранится по questId.
6. Нет отдельной сущности попытки повторяемого квеста.
7. Нет pendingRewards.
8. Нет deliveredRewards.
9. Cooldown повторяемого квеста может стартовать раньше, чем награда фактически гарантированно доставлена.
10. Для безопасной новой системы нужна модель:

questId = тип квеста
attemptId = конкретное прохождение / повтор квеста
rewardId = конкретная выдача внутри attemptId

Главное правило будущей системы:

Старые deliveredRewards относятся только к своему attemptId.
Они не должны засчитываться как выданная награда для нового повтора квеста.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 106
--------------------------------------------------------------------------------

Добавить в player progress фундаментальные структуры:

1. currentAttemptId
2. lastFinalizedAttemptId
3. pendingRewards[]
4. deliveredRewards[]

Также добавить минимальные helper-функции, которые позволят будущим задачам:

- создавать rewardId;
- проверять, есть ли pending reward;
- проверять, выдана ли награда для текущего attemptId;
- добавлять pending reward;
- помечать pending reward как delivered;
- не путать старые deliveredRewards с новой попыткой повторяемого квеста.

В этой задаче НЕ нужно:
- подключать новую систему к реальной выдаче Offer;
- подключать новую систему к Completion;
- подключать новую систему к Reward;
- менять SpawnQuestItem;
- менять MarkQuestAsCompleted;
- менять cooldown;
- менять dev-квесты в JSON_Quvest, если это не требуется для компиляции.

--------------------------------------------------------------------------------
ВАЖНОЕ ПРАВИЛО ПО JSON_Quvest
--------------------------------------------------------------------------------

`P:\Silver_77_Quests\JSON_Quvest` — рабочий dev-источник квестов.

JSON_Quvest можно менять в будущих задачах, если новая архитектура этого потребует.

Но для TASK 106 ожидается, что JSON менять не нужно, потому что:
- attemptId;
- pendingRewards;
- deliveredRewards;
- given;
- status;
- lastError

являются runtime/progress-данными конкретного игрока, а не описанием самого квеста.

Если агент считает, что без изменения JSON_Quvest задачу невозможно выполнить, он НЕ должен молча менять JSON.
Он должен остановиться в рамках этой части и написать это в PROBLEMS / QUESTIONS.

Общее правило на будущее:

Если JSON_Quvest всё же меняется в отдельной будущей задаче, нельзя менять:
- визуальную структуру dev-блоков;
- закономерность Offer -> Completion -> Reward;
- смысл квестов;
- NPC / trigger-роли;
- objectives;
- rewards по смыслу;
- диалоги;
- читаемую структуру, в которой вручную составляются квесты.

Разрешены только технические изменения "под капотом", если они нужны новой системе.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Разрешено менять только файлы, которые реально нужны для модели player progress и её сохранения/загрузки.

Разрешено менять:

1. Silver_77_Quests_Client/scripts/3_Game/PlayerQuestData.c
2. Silver_77_Quests_Server/scripts/4_World/QuestServerManager.c

Дополнительно можно менять только если без этого не компилируется или progress не синхронизируется:

3. Silver_77_Quests_Client/scripts/3_Game/QuestData.c
4. Silver_77_Quests_Client/scripts/4_World/QuestClientManager.c
5. Silver_77_Quests_Server/scripts/4_World/QuestServerRPC.c

Запрещено менять без отдельной задачи:

- JSON_Quvest/
- DayZ_layout/
- Documentation/
- Documentation/AGENT_TASK_LOOP.md
- Documentation/SplitDoc/TASK_HISTORY.md
- layout-файлы
- quest logic по смыслу
- rewards по смыслу
- objectives по смыслу
- диалоги
- server profile вручную
- player progress вручную
- PBO

Запрещено делать:

- git commit
- git push
- git reset
- git clean
- git checkout
- запускать Addon Builder
- перепаковывать PBO

Git контролирует пользователь.

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

Обязательно перечитать код:

1. Silver_77_Quests_Client/scripts/3_Game/PlayerQuestData.c
2. Silver_77_Quests_Client/scripts/3_Game/QuestData.c
3. Silver_77_Quests_Server/scripts/4_World/QuestServerManager.c
4. Silver_77_Quests_Server/scripts/4_World/QuestServerRPC.c
5. Silver_77_Quests_Client/scripts/4_World/QuestClientManager.c

Разрешено читать:

1. JSON_Quvest/Silver_77_Quests.json

Только для проверки, что новые runtime-поля не должны добавляться в dev-квесты.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Найти текущие классы/структуры player progress.

Особенно проверить:

- PlayerQuestData
- PlayerQuestProgress
- objectiveProgress
- completedCompletionTriggerIds
- stageVisits
- status
- lastCompletedTime
- сохранение progress в $profile
- загрузку progress из $profile
- синхронизацию progress клиенту, если она есть

2. Добавить модель попытки квеста.

В PlayerQuestProgress добавить поля:

- int currentAttemptId
- int lastFinalizedAttemptId

Правила значений по умолчанию:

- currentAttemptId = 0, если попытка ещё не создавалась;
- lastFinalizedAttemptId = 0, если ни одна попытка ещё не финализирована.

Если старый progress-файл не содержит этих полей, код должен безопасно работать с дефолтными значениями.

3. Добавить модель pending reward item.

Создать класс/структуру, например:

QuestRewardItemProgress

Поля:

- string className
- int need
- int given
- bool spawnOnGround
- string lastError

Смысл:

- className — какой предмет надо выдать;
- need — сколько нужно выдать;
- given — сколько уже реально выдано;
- spawnOnGround — техническая политика/признак, если уже используется в наградах;
- lastError — последняя ошибка выдачи, если будет нужна будущей safe-логике.

4. Добавить модель pending reward.

Создать класс/структуру, например:

QuestPendingRewardProgress

Поля:

- string questId
- int attemptId
- string rewardId
- string stage
- string triggerId
- string actionType
- string status
- ref array<ref QuestRewardItemProgress> items
- int createdAt
- int updatedAt
- string lastError

Допустимые значения status на будущее:

- pending
- partial
- delivered
- failed

В TASK 106 можно только заложить эти значения как строки/комментарии/константы, если в проекте уже используется такой стиль.

5. Добавить модель delivered reward.

Создать класс/структуру, например:

QuestDeliveredRewardProgress

Поля:

- string questId
- int attemptId
- string rewardId
- string stage
- string triggerId
- string actionType
- int deliveredAt

Смысл:

deliveredRewards — это история успешно закрытых выдач.
Она нужна для защиты от повторной выдачи.

Важно:

delivered reward с attemptId = 1 не должен считаться выданной наградой для attemptId = 2.

6. Добавить массивы в PlayerQuestProgress.

Добавить:

- ref array<ref QuestPendingRewardProgress> pendingRewards
- ref array<ref QuestDeliveredRewardProgress> deliveredRewards

Обеспечить инициализацию массивов.

Если в проекте есть constructor / Init / Ensure методы, использовать стиль проекта.

Если таких методов нет, добавить минимальную безопасную инициализацию там, где progress создаётся или загружается.

7. Добавить helper-функции.

Нужно добавить минимальные функции, не подключая их пока к реальной выдаче.

Примерный набор:

- string BuildRewardId(string questId, int attemptId, string stage, string triggerId, string actionType, int index)
- QuestPendingRewardProgress FindPendingReward(string rewardId, int attemptId)
- bool HasDeliveredReward(string rewardId, int attemptId)
- void AddPendingReward(QuestPendingRewardProgress reward)
- void MarkPendingRewardDelivered(string rewardId, int attemptId)
- bool HasOpenPendingRewardsForAttempt(int attemptId)
- bool IsAttemptFullyFinalized(int attemptId)

Если сигнатуры нужно адаптировать под стиль текущего кода — адаптировать.

Главное:
функции должны проверять attemptId и не работать только по questId.

8. Не менять текущую игровую логику выдачи.

В TASK 106 нельзя менять фактическое поведение:

- AcceptQuest
- Completion
- FinalizeQuestReward
- SpawnQuestItem
- MarkQuestAsCompleted
- cooldown logic

Исключение:
можно добавить безопасную инициализацию новых progress-полей, если без этого save/load ломается.

9. Проверить сохранение и загрузку.

Нужно убедиться:

- новые поля сериализуются в player progress;
- новые поля загружаются обратно;
- отсутствие новых полей в старом progress не приводит к ошибке;
- массивы pendingRewards/deliveredRewards не остаются null после загрузки.

10. Проверить, что JSON_Quvest не нужен для TASK 106.

Нужно явно написать в отчёте:

- менялся ли JSON_Quvest;
- если не менялся — почему;
- подтверждение, что текущие dev-квесты по смыслу не затронуты.

11. Ничего не менять в документации.

Документацию в TASK 106 не менять.
Отчёт вернуть только в чат.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Моё мнение:

TASK 106 должен быть фундаментом, а не полной переделкой выдачи.

Лучший порядок:

1. Сначала добавить структуры данных в progress.
2. Убедиться, что они сохраняются и загружаются.
3. Убедиться, что старые progress-файлы без новых полей не ломают загрузку.
4. Не менять выдачу предметов.
5. Не менять cooldown.
6. Не менять JSON_Quvest.
7. После этого отдельной TASK 107 сделать safe-функцию выдачи, которая уже будет использовать pendingRewards/given/delivered.

Главное архитектурное правило:

Нельзя хранить историю выдачи только по questId.
Все проверки выдачи должны быть готовы работать по:

questId + attemptId + rewardId

Иначе повторяемые квесты сломаются.

--------------------------------------------------------------------------------
ФОРМАТ ОТЧЁТА
--------------------------------------------------------------------------------

Вернуть отчёт в чат в таком формате:

AGENT REPORT

DONE:
- что сделано

READ FILES:
- какие файлы прочитаны

CHANGED FILES:
- какие файлы изменены

ADDED DATA MODEL:
- какие классы/структуры добавлены
- какие поля добавлены в PlayerQuestProgress
- какие helper-функции добавлены

SAVE / LOAD CHECK:
- как новые поля сохраняются
- как новые поля загружаются
- что происходит со старым progress без этих полей
- где выполняется инициализация массивов

JSON_QUEST_CHECK:
- менялся ли JSON_Quvest
- если не менялся, почему
- подтверждение, что dev-квесты не затронуты

BEHAVIOR CHECK:
- подтвердить, что фактическая выдача Offer / Completion / Reward не менялась
- подтвердить, что cooldown не менялся
- подтвердить, что SpawnQuestItem не менялся, если он не менялся

PROBLEMS:
- реальные проблемы и риски

QUESTIONS:
- только реальные вопросы

RECOMMENDED NEXT TASK:
- предложить следующую задачу

CONCLUSION:
- короткий итог, готов ли фундамент для safe reward delivery


================================================================================
# МАЛЫЙ БЛОК В ЧАТ АГЕНТУ
================================================================================

TASK 106 — Implementation Foundation: добавить в player progress основу для attemptId, pendingRewards и deliveredRewards

Прочитай текущий БЛОК 1 в Documentation/AGENT_TASK_LOOP.md и выполни только его.

Задача реализационная, но с жёсткими рамками.

Главная цель:
добавить в player progress фундамент для новой безопасной выдачи наград:

- currentAttemptId;
- lastFinalizedAttemptId;
- pendingRewards[];
- deliveredRewards[];
- helper-функции для rewardId, pending/delivered и проверки attemptId.

Важно:
не переписывай выдачу Offer / Completion / Reward в этой задаче.
Не меняй SpawnQuestItem.
Не меняй cooldown.
Не меняй JSON_Quvest, если это не требуется для компиляции.

Главное архитектурное правило:
проверка выдачи должна быть готова работать по questId + attemptId + rewardId, чтобы старые delivered-награды не засчитывались для нового повтора квеста.

Верни отчёт в чат по формату из БЛОКА 1.

================================================================================
# КОНЕЦ TASK 106
================================================================================

================================================================================
# МАЛЫЙ БЛОК В ЧАТ АГЕНТУ
================================================================================

TASK 105 — Analysis / Design: спроектировать безопасную систему выдачи наград с attemptId, pendingRewards и deliveredRewards для повторяемых квестов

Прочитай текущий БЛОК 1 в Documentation/AGENT_TASK_LOOP.md и выполни только его.

Задача аналитико-проектная: ничего не менять.

Главная цель:
проверить текущий progress игрока, repeat/cooldown, выдачу rewards/giveItems и спроектировать безопасную модель:

- questId = тип квеста;
- attemptId = конкретная попытка / повтор квеста;
- rewardId = конкретная выдача внутри попытки;
- pendingRewards = что осталось довыдать;
- deliveredRewards = что уже успешно выдано.

Особенно важно:

Старые delivered-награды не должны засчитываться для нового повтора квеста.
Проверка выдачи всегда должна идти по текущему attemptId.
Новый attemptId и cooldown должны появляться только после полной финализации предыдущей попытки.

Верни отчёт в чат по формату из БЛОКА 1.

================================================================================
# КОНЕЦ TASK 105
================================================================================

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
