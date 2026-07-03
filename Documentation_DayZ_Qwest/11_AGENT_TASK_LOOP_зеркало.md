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
- `Encoding / кириллица / восстановление русского текста / кракозябры` -> `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
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
- Добавлен helper `Silver77_GetQuestItemDisplayName` для получения player-facing имени предмета по `className`.
- `QuestUI.c` переведён на локализованные имена предметов в целях и наградах.
- `QuestJournalUI.c` переведён на локализованные имена предметов в целях и наградах.
- В `QuestJournal` добавлен вывод блока `Награда:` из `quest.rewards` или финального reward action.
- Из player-facing текста журнала убрана фраза `trigger / NPC`; в `QuestUI` видимое `NPC` заменено на `персонаж`.
- В `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` добавлена запись `TASK 120`.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена выполненная `TASK 120`.
- Активный `БЛОК 1` очищен после выполнения задачи.

CHANGED FILES:
- `P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c`
- `P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`

IMPLEMENTATION:
- Helper создан в `QuestUI.c` как глобальные функции `Silver77_GetQuestItemConfigDisplayName` и `Silver77_GetQuestItemDisplayName`, чтобы его мог использовать и `QuestJournalUI.c`.
- `displayName` ищется в порядке `CfgVehicles`, `CfgWeapons`, `CfgMagazines`, `CfgAmmo`.
- Для строк из config используется `ConfigGetTextRaw`, затем `$STR_...` переводится в script-friendly формат через `GetGame().FormatRawConfigStringKeys`.
- Локализация выполняется через `Widget.TranslateString`.
- Fallback: если `displayName` найден, но не локализован, показывается `displayName`; если `displayName` не найден, показывается исходный `className`.
- Ручной словарь не добавлялся; `PotatoSeed` не заменялся на `Potato`.

UI RESULT:
- Цели теперь формируются как `Принести: <игровое имя> xN`, а не через прямой вывод `obj.className`.
- Награды теперь формируются как `<игровое имя> xN`, а не через прямой вывод `reward.className`.
- `QuestJournal` показывает блок `Награда:`, если награда есть в `quest.rewards` или в финальном `triggerActions[].rewards`.
- Если награды в клиентских данных нет, блок `Награда:` не показывается и UI не ломается.
- Техническая фраза `trigger / NPC` заменена на нормальный текст про подходящего персонажа.

CHECKS:
- Сигнатуры `ConfigGetTextRaw`, `FormatRawConfigStringKeys` и `Widget.TranslateString` сверены по `P:\scripts`.
- Проверен поиск прямого player-facing вывода `obj.className` / `reward.className`; оставшиеся совпадения относятся к логике, а не к отображаемым строкам.
- Проверено, что `QuestJournal` может брать награду из `triggerActions[].rewards` через существующий `QuestClientManager.GetQuestActionRewards`.
- Скобки в изменённых `.c` файлах сбалансированы простой статической проверкой.
- Полноценная компиляция DayZ не запускалась: build/PBO запрещены текущей задачей, а DayZ Workbench/Diag CLI в окружении не найден.
- JSON, JSON-контракт, layout, server profile, редактор `JSON_Quvest` и PBO/build output не менялись.
- Активный `БЛОК 1` очищен.

ENCODING CHECK:
- UTF-8: изменённые `.c` и Markdown-файлы читаются как UTF-8.
- BOM: не обнаружен.
- Кириллица: отображается нормально.
- Повторяющиеся знаки вопроса и символы замены: не обнаружены.
- Кириллица не восстанавливалась, повреждений не обнаружено.

SYNC:
- Отдельная переработка sync не потребовалась.
- Награды доступны в клиентской config-модели через `quest.rewards` и `triggerActions[].rewards`; для журнала использован существующий `QuestClientManager.GetQuestActionRewards`.

GIT:
- Git-команды не выполнялись.
- Commit не создавался агентом.
- Push не выполнялся агентом.
- Git-история не изменялась агентом.

PROBLEMS:
- Полноценная компиляция проекта не выполнялась из-за запрета build/PBO в активной задаче и отсутствия найденного DayZ Workbench/Diag CLI в окружении.
- Фактические строки `PotatoSeed -> Картофель`, `BandageDressing -> Бинт`, `GP5GasMask -> Противогаз` нужно подтвердить в игре после обновления клиентской части мода; код использует именно DayZ config/localization цепочку, а не ручной словарь.

CONCLUSION:
- TASK 120 выполнен. Изменены клиентские скрипты. Для проверки в игре потребуется обновить/пересобрать клиентскую часть мода по обычному процессу проекта.
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
