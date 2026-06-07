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

TASK 109 — Implementation: safe reward delivery core + подключение к финальному Reward

Статус:
Реализационная задача с жёсткими рамками.

Код менять можно только в рамках безопасной выдачи финальной награды.
Offer / giveItems и Completion rewards в этой задаче не подключать к новой safe-логике.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

По предыдущим задачам подтверждено:

1. TASK 106 добавил фундамент progress:
   - currentAttemptId;
   - lastFinalizedAttemptId;
   - pendingRewards[];
   - deliveredRewards[].

2. Текущая выдача предметов работает так:
   - сначала попытка положить предмет в инвентарь;
   - если не влезает, fallback создаёт предмет на земле.

3. Runtime-проверка пользователя подтвердила:
   при полном инвентаре предметы действительно падают на землю.

4. Но текущая выдача всё ещё небезопасна:
   - результат CreateObjectEx не проверяется;
   - нет guaranteed delivery;
   - нет довыдачи остатка;
   - нет защиты от partial-выдачи;
   - success выдачи не влияет на completed;
   - cooldown стартует после completed, даже если награда могла фактически не создаться.

5. Нужно начать безопасную систему с самого важного места:
   финальный Reward, потому что он закрывает квест и запускает repeat/cooldown через lastCompletedTime.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 109
--------------------------------------------------------------------------------

Сделать safe reward delivery core и подключить его только к финальному Reward.

Нужно реализовать серверную safe-функцию выдачи reward batch, которая:

1. Работает по ключу:
   questId + attemptId + rewardId.

2. Создаёт или использует pendingReward.

3. Проверяет deliveredReward перед выдачей:
   если reward уже delivered для текущего attemptId, повторно не выдавать.

4. Если pendingReward уже partial или failed:
   довыдавать только остаток.

5. Для каждого предмета:
   - пробовать создать в инвентаре;
   - если не получилось, пробовать создать на земле;
   - проверять результат создания;
   - увеличивать given только если объект реально создан.

6. Обновлять pendingRewards:
   - pending;
   - partial;
   - failed;
   - delivered.

7. При полной успешной выдаче:
   - добавить запись в deliveredRewards;
   - пометить pendingReward как delivered;
   - разрешить MarkQuestAsCompleted.

8. При partial/failed:
   - НЕ делать MarkQuestAsCompleted;
   - НЕ запускать cooldown;
   - оставить квест в состоянии reward_pending;
   - сохранить progress;
   - дать возможность повторить выдачу позже.

9. Не выдавать лишнего при повторном нажатии.

--------------------------------------------------------------------------------
ВАЖНО ПРО attemptId
--------------------------------------------------------------------------------

Для safe reward delivery нужен attemptId.

В рамках TASK 109 разрешено минимально подключить attemptId к жизненному циклу квеста:

1. При AcceptQuest:
   - если currentAttemptId <= 0, установить currentAttemptId = lastFinalizedAttemptId + 1;
   - если квест был completed и повторно берётся после cooldown, создать новый attemptId = lastFinalizedAttemptId + 1;
   - не очищать deliveredRewards;
   - не очищать pendingRewards без отдельной причины.

2. При успешном финальном Reward:
   - если награда полностью delivered;
   - перед или во время завершения квеста обновить lastFinalizedAttemptId = currentAttemptId.

3. Старые deliveredRewards с другим attemptId не должны блокировать выдачу в новой попытке.

Главное правило:
deliveredReward считается уже выданным только если совпали rewardId и attemptId текущей попытки.

--------------------------------------------------------------------------------
ЧТО НЕ НУЖНО ДЕЛАТЬ В TASK 109
--------------------------------------------------------------------------------

Не подключать safe delivery к:

- Offer / giveItems;
- Completion rewards.

Не менять:

- dev-квесты в JSON_Quvest;
- layout;
- UI;
- диалоги;
- objectives;
- rewards по смыслу;
- server profile вручную;
- player progress вручную.

Не делать полноценную UX-систему сообщений игроку.
Можно добавить server Print/debug log, если нужно для диагностики.

Не переносить окончательно cooldown/repeat на отдельную финализацию attempt, кроме минимального условия:
если final reward не delivered, MarkQuestAsCompleted не вызывается и cooldown не стартует.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Разрешено менять:

1. Silver_77_Quests_Server/scripts/4_World/QuestServerManager.c

Разрешено менять только при реальной необходимости компиляции:

2. Silver_77_Quests_Client/scripts/3_Game/PlayerQuestData.c

Запрещено менять:

- JSON_Quvest/
- DayZ_layout/
- Documentation/
- Documentation/AGENT_TASK_LOOP.md
- Documentation/SplitDoc/TASK_HISTORY.md
- Silver_77_Quests_Client/scripts/4_World/QuestClientManager.c
- Silver_77_Quests_Server/scripts/4_World/QuestServerRPC.c
- любые layout-файлы
- server profile
- player progress
- PBO

Запрещено делать:

- git commit
- git push
- git reset
- git clean
- git checkout
- запуск Addon Builder
- перепаковку PBO

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

Обязательно проверить код:

1. Silver_77_Quests_Client/scripts/3_Game/PlayerQuestData.c
2. Silver_77_Quests_Server/scripts/4_World/QuestServerManager.c

Можно читать для понимания, но не менять:

1. JSON_Quvest/Silver_77_Quests.json
2. Silver_77_Quests_Client/scripts/3_Game/QuestData.c
3. Silver_77_Quests_Client/scripts/4_World/QuestClientManager.c
4. Silver_77_Quests_Server/scripts/4_World/QuestServerRPC.c

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Найти текущую выдачу финального Reward.

Проверить места:

- FinalizeQuestReward;
- CompleteQuest;
- ветка rewardTriggerIds;
- ветка финального reward без отдельного Reward trigger, если она есть;
- MarkQuestAsCompleted;
- SpawnQuestItem.

2. Добавить safe-функцию создания предмета.

Смысл функции:

- получить player и Silver77_QuestItem;
- попытаться создать предмет в инвентаре, если spawnOnGround не требует землю сразу;
- если инвентарь не принял, создать предмет на земле;
- вернуть успех только если реально получен объект EntityAI;
- не считать успешной выдачу, если CreateInInventory и CreateObjectEx вернули null.

Важно:
не менять смысл quantity/spawnOnGround относительно текущей SpawnQuestItem.
Если текущий SpawnQuestItem содержит особую обработку quantity, здоровья, stack или другого поля, safe-логика должна повторять это поведение максимально близко.

3. Добавить safe-функцию выдачи reward batch.

Примерный смысл:

- TryDeliverRewardBatchSafe(player, quest, progress, triggerId, rewardItems, stage/actionType)

Функция должна:

- получить currentAttemptId;
- построить стабильный rewardId;
- проверить deliveredRewards по rewardId + attemptId;
- если already delivered, вернуть success/no-op;
- найти или создать pendingReward;
- выдать только остаток need - given;
- обновить given только после реального создания предмета;
- если всё выдано, status = delivered;
- если часть выдана, status = partial;
- если ничего не выдано, status = failed или pending с lastError;
- записать lastError при ошибке;
- обновить updatedAt.

4. Подключить safe delivery только к финальному Reward.

Подключить в местах, где rewardPhase == true или где финальная награда закрывает квест.

Правило:

- если safe delivery вернула success:
  - RecordStageVisit reward;
  - MarkQuestAsCompleted;
  - SavePlayerData;
  - g_ServerQuestDataRevision++.

- если safe delivery вернула partial/failed:
  - НЕ вызывать MarkQuestAsCompleted;
  - progress.status = reward_pending;
  - SavePlayerData;
  - g_ServerQuestDataRevision++;
  - вернуть true или false в зависимости от текущего стиля кода, но квест не должен стать completed.

Важно:
Completion rewards с actionType completion пока оставить на старой выдаче.

5. Минимально подключить currentAttemptId.

В AcceptQuest:

- при первом взятии квеста установить currentAttemptId = lastFinalizedAttemptId + 1, если currentAttemptId <= 0;
- при повторном взятии completed repeatable-квеста после cooldown установить новый currentAttemptId = lastFinalizedAttemptId + 1;
- не использовать старые deliveredRewards для новой попытки.

6. Обновить lastFinalizedAttemptId при успешном completed.

Когда финальная награда полностью delivered и квест переводится в completed:

- lastFinalizedAttemptId должен стать currentAttemptId, если currentAttemptId больше 0.

7. Не менять JSON_Quvest.

В отчёте явно указать:

- JSON_Quvest не менялся;
- dev-квесты не менялись;
- смысл rewards/objectives/dialogs не менялся.

8. Не менять UI.

Сообщения игроку можно оставить на следующую задачу.

9. Проверить риски компиляции.

Если DayZ tools недоступны, не запускать сборку.
Но нужно сделать самопроверку кода:
- нет ли очевидных ошибок сигнатур;
- нет ли обращения к null-массивам;
- нет ли смены поведения Offer/Completion;
- нет ли случайного изменения JSON/UI.

--------------------------------------------------------------------------------
ОЖИДАЕМОЕ ПОВЕДЕНИЕ ПОСЛЕ TASK 109
--------------------------------------------------------------------------------

1. Финальный Reward при полном инвентаре:
   - пробует инвентарь;
   - если не получилось, создаёт на земле;
   - если объект создан, считается delivered.

2. Если предмет не создался нигде:
   - квест не становится completed;
   - cooldown не стартует;
   - pendingReward остаётся в progress;
   - следующая попытка выдачи должна довыдать остаток.

3. Повторное нажатие на Reward:
   - не выдаёт уже delivered награду повторно;
   - если была partial, довыдаёт только остаток.

4. Старый deliveredReward другого attemptId:
   - не блокирует новую попытку repeatable-квеста.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Лучшее решение для TASK 109:

1. Не ломать старую выдачу целиком.
2. Добавить safe delivery рядом с текущей логикой.
3. Подключить только финальный Reward.
4. Не трогать Offer и Completion.
5. Не трогать JSON.
6. Не трогать UI.
7. Если safe reward failed/partial, переводить квест в reward_pending, а не completed.

Главное:
финальный Reward не должен запускать completed/cooldown, пока награда не подтверждена как delivered.

--------------------------------------------------------------------------------
ФОРМАТ ОТЧЁТА
--------------------------------------------------------------------------------

Вернуть отчёт в чат:

AGENT REPORT

DONE:
- что сделано

READ FILES:
- какие файлы прочитаны

CHANGED FILES:
- какие файлы изменены

SAFE DELIVERY CORE:
- какие функции добавлены
- как проверяется CreateInInventory
- как проверяется CreateObjectEx
- как считается need/given/failed
- как обновляются pendingRewards/deliveredRewards

REWARD CONNECTION:
- где подключена safe delivery к финальному Reward
- какие старые места остались без изменений
- что происходит при success
- что происходит при partial/failed

ATTEMPT_ID CHECK:
- где создаётся currentAttemptId
- где обновляется lastFinalizedAttemptId
- как исключена блокировка новой попытки старым deliveredReward

JSON_QUEST_CHECK:
- менялся ли JSON_Quvest
- подтверждение, что dev-квесты не затронуты

BEHAVIOR CHECK:
- Offer/giveItems не подключались к safe delivery
- Completion rewards не подключались к safe delivery
- cooldown напрямую не переписывался
- MarkQuestAsCompleted не вызывается при failed/partial final reward

PROBLEMS:
- реальные проблемы и риски

QUESTIONS:
- только реальные вопросы

RECOMMENDED NEXT TASK:
- следующий безопасный шаг

CONCLUSION:
- короткий итог, готова ли safe delivery для финального Reward


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
