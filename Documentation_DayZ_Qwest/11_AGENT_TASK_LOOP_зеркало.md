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

TASK 120 — Реализовать локализованные игровые названия предметов и вывод награды в QuestJournal

## Цель

Сделать так, чтобы в квестовом UI и журнале квестов игрок видел нормальные игровые названия предметов, а не технические `className`.

Также добавить отображение награды в `QuestJournal`.

Сейчас в UI могут выводиться технические имена вроде:

```text
PotatoSeed
BandageDressing
GP5GasMask
```

Нужно, чтобы игрок видел локализованные игровые названия из DayZ:

```text
Картофель
Бинт
Противогаз
```

JSON-контракт при этом не менять. В JSON должны оставаться технические `className`, потому что они используются runtime-логикой для проверки objectives и выдачи rewards.

## Читать обязательно

Перед работой прочитать:

- `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md`
    
- `Documentation_DayZ_Qwest/12_START_зеркало.md`
    
- `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_UI_RULES.md`
    
- `Documentation_DayZ_Qwest/20_SplitDoc/QUEST_JSON_CONTRACT.md`
    
- `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`
    
- `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md`
    

Дополнительно читать только если выяснится, что награды или нужные данные не приходят на клиент:

- `Documentation_DayZ_Qwest/20_SplitDoc/DAYZ_RPC_SYNC_RULES.md`
    

## Контекст после TASK 117

В TASK 117 уже выяснено:

1. Сейчас `QuestUI.c` и `QuestJournalUI.c` формируют текст целей и наград через прямой вывод `obj.className` / `reward.className`.
    
2. Правильная цепочка получения имени:
    

```text
className
-> игровой config DayZ
-> displayName
-> $STR_...
-> локализация игры
-> нормальное имя игрока
```

3. Основной источник — игровые конфиги DayZ:
    

```text
CfgVehicles
CfgWeapons
CfgMagazines
CfgAmmo
```

4. Ручной словарь не должен быть основным решением. Его можно использовать только как fallback для кастомных предметов, если у них нет нормального `displayName`.
    
5. По картофелю уже подтверждено:
    

```text
Potato     -> Очищенный картофель
PotatoSeed -> Картофель
```

Значит если в игре предмет отображается как `КАРТОФЕЛЬ`, это соответствует `PotatoSeed`, а не `Potato`.

## Что нужно сделать

### 1. Найти текущие места вывода className

Найти и проверить текущие места формирования текста:

- `Silver_77_Quests_Client/scripts/5_Mission/QuestUI.c`
    
- `Silver_77_Quests_Client/scripts/5_Mission/QuestJournalUI.c`
    

Особенно проверить места, где сейчас выводятся:

```text
obj.className
reward.className
```

Нужно заменить только отображение для игрока. Логику objectives/rewards не менять.

### 2. Сделать helper получения игрового имени предмета

Создать клиентский helper, например:

```c
string GetQuestItemDisplayName(string className)
```

Название функции можно выбрать по стилю текущего кода, но смысл должен быть именно такой:

```text
className -> локализованное игровое имя предмета
```

Helper должен:

1. Получать `className`.
    
2. Искать `displayName` в конфигах DayZ в порядке:
    

```text
CfgVehicles
CfgWeapons
CfgMagazines
CfgAmmo
```

3. Если `displayName` найден и это обычный текст — вернуть его.
    
4. Если `displayName` имеет вид `$STR_...` — локализовать его через DayZ API.
    
5. Проверить вариант, рекомендованный в TASK 117:
    

```text
FormatRawConfigStringKeys
Widget.TranslateString
```

6. Если локализация не сработала — вернуть безопасный fallback.
    
7. Fallback не должен ломать UI.
    

Пример fallback-логики:

```text
если displayName найден, но не локализован -> показать displayName
если displayName не найден -> показать className
```

Ручной словарь как основное решение не делать.

### 3. Применить helper в QuestUI

В `QuestUI.c` заменить отображение технических имён предметов в целях и наградах.

Было по смыслу:

```text
Принести: GP5GasMask x6
Награда: BandageDressing x3
```

Должно стать:

```text
Принести: Противогаз x6
Награда: Бинт x3
```

Важно: менять только текст для игрока. Проверка квеста, сдача предметов и выдача наград должны продолжать использовать `className`.

### 4. Применить helper в QuestJournalUI

В `QuestJournalUI.c` заменить отображение technical `className` в целях и наградах.

В журнале должны отображаться нормальные игровые имена.

### 5. Добавить блок награды в QuestJournal

В `QuestJournalUI.c` добавить отображение награды в правой панели журнала квеста.

Примерный формат:

```text
Цели:
- Принести: Картофель x6
  Сдано: 1 / 6

Награда:
- Бинт x3
- Противогаз x1
```

Если награды нет или данные о награде не пришли на клиент, UI не должен ломаться.

В таком случае:

- либо не показывать блок `Награда:`;
    
- либо показать аккуратный fallback, если это уже принято в текущем UI.
    

Но если выяснится, что данные о rewards вообще не передаются на клиент, не делать самовольную переработку sync. Нужно остановиться, описать проблему в `PROBLEMS` и предложить отдельную задачу на sync.

### 6. Убрать техническую фразу из журнала, если она есть

Если в `QuestJournal` сейчас выводится фраза вида:

```text
Сдать квест можно у подходящего trigger / NPC по логике квеста.
```

Заменить её на нормальный player-facing текст или убрать, если она не нужна.

Пример нормального варианта:

```text
Сдать квест можно у подходящего персонажа.
```

Не использовать слова `trigger`, `className`, `runtime`, `objective`, `reward` в тексте для игрока.

## Разрешённые файлы

Разрешено менять только:

```text
Silver_77_Quests_Client/scripts/5_Mission/QuestUI.c
Silver_77_Quests_Client/scripts/5_Mission/QuestJournalUI.c
Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md
Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md
Documentation_DayZ_Qwest/05_Очередь_задач.md
```

Если для реализации helper окажется, что в проекте уже есть подходящий общий UI/helper файл, можно предложить использовать его, но не менять дополнительные файлы без явной необходимости. Если нужен дополнительный файл — сначала указать это в `PROBLEMS` / `QUESTIONS`, а не создавать самовольно.

## Запрещено

Не менять:

```text
JSON-квесты
QUEST_JSON_CONTRACT.md
.layout
server profile
PBO/build output
игровые данные
редактор JSON_Quvest
```

Не менять JSON-контракт.

Не переименовывать `className`.

Не заменять `PotatoSeed` на `Potato`.

Не делать ручной словарь основным решением.

Не делать глобальную архитектурную переработку UI.

Не чинить соседние проблемы “заодно”.

Не делать без прямой команды пользователя:

```text
git commit
git push
git pull
git merge
git rebase
git reset
git clean
git checkout
git switch
создание веток
изменение remote
изменение Git-конфига
```

## Проверки

После реализации проверить:

1. Проект компилируется.
    
2. `QuestUI.c` не показывает технические `className` в целях.
    
3. `QuestUI.c` не показывает технические `className` в наградах.
    
4. `QuestJournalUI.c` не показывает технические `className` в целях.
    
5. `QuestJournalUI.c` показывает блок `Награда:`, если награда доступна клиенту.
    
6. `PotatoSeed` отображается как `Картофель`.
    
7. `BandageDressing` отображается как `Бинт`.
    
8. `GP5GasMask` отображается как `Противогаз`.
    
9. Если встречается неизвестный className, UI не ломается.
    
10. Русский текст не повреждён.
    
11. UTF-8 / BOM / кириллица проверены по `ENCODING_RULES.md`.
    
12. JSON, layout, server profile и PBO/build output не менялись.
    

## В отчёте обязательно указать

```text
AGENT REPORT

DONE:
-

CHANGED FILES:
-

IMPLEMENTATION:
- где создан helper;
- как ищется displayName;
- как обрабатывается $STR_...;
- какой fallback используется;

UI RESULT:
- как теперь отображаются цели;
- как теперь отображаются награды;
- что изменилось в QuestJournal;

CHECKS:
-

ENCODING CHECK:
-

GIT:
-

PROBLEMS:
-

CONCLUSION:
-
```

В `PROBLEMS` отдельно указать, если:

- rewards не приходят на клиент;
    
- DayZ API не локализует `$STR_...` так, как ожидалось;
    
- для helper нужен отдельный общий файл;
    
- какой-то className не найден в конфигах.
    

Если менялись клиентские `.c` файлы, в `CONCLUSION` не писать автоматически “моды обновлять не нужно”. Вместо этого указать:

```text
Изменены клиентские скрипты. Для проверки в игре потребуется обновить/пересобрать клиентскую часть мода по обычному процессу проекта.
```

================================================================================
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- В `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md` добавлен аварийный протокол при повреждении кириллицы.
- В `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md` добавлена ссылка на аварийный протокол и расширены проверки Markdown-документов.
- В `Documentation_DayZ_Qwest/12_START_зеркало.md` добавлена короткая маршрутизация для задач восстановления русского текста и проблем кодировки.
- В матрице чтения `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md` уточнена строка `Encoding / кириллица`.
- В `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` добавлена запись `TASK 119`.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена выполненная `TASK 119`.
- Активный `БЛОК 1` очищен после выполнения задачи.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\ENCODING_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\DOCUMENTATION_RULES.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`

UPDATED RULES:
- Если кириллица выглядит повреждённой, агент сначала определяет причину: реальная порча файла, неверное чтение, терминал, PowerShell или инструмент записи.
- Если кириллица восстанавливалась, в отчёте обязательны файл, участок, причина, способ восстановления, источник корректного текста и проверки после восстановления.
- Фраза вида "кириллицу восстановил" без подробностей запрещена.
- Если восстановление касалось `11_AGENT_TASK_LOOP_зеркало.md`, это указывается отдельно.
- Для Markdown-документов с русским текстом закреплена проверка повторяющихся знаков вопроса, символов замены, mojibake-фрагментов, UTF-8 и BOM.

CHECKS:
- `ENCODING_RULES.md` содержит отдельный аварийный протокол.
- `DOCUMENTATION_RULES.md` ссылается на аварийный протокол и требует отражать восстановление в `ENCODING CHECK` / `PROBLEMS`.
- `START` содержит только короткую маршрутизацию, полный текст оставлен в SplitDoc.
- `AGENT_TASK_LOOP` содержит уточнённую строку матрицы для encoding / кириллицы / восстановления русского текста.
- `TASK_HISTORY.md` содержит `TASK 119`.
- `05_Очередь_задач.md` содержит `TASK 119` со статусом `Готово`.
- Активный `БЛОК 1` очищен.
- JSON, UI, layout, моды, server profile и PBO/build output не менялись.

ENCODING CHECK:
- UTF-8: изменённые Markdown-файлы читаются как UTF-8.
- BOM: не обнаружен.
- Кириллица: отображается нормально.
- Повторяющиеся знаки вопроса: случайных повреждений не обнаружено; упоминания таких признаков остались только как описанные примеры в правилах кодировки.
- Символы замены: случайных повреждений не обнаружено; упоминания таких признаков остались только как описанные примеры в правилах кодировки.
- Mojibake-фрагменты: случайных повреждений не обнаружено; упоминания `Рџ`, `Рќ`, `Р’`, `СЃ`, `С‚` остались только как описанные примеры в правилах кодировки.
- Markdown одной строкой: не обнаружено.
- `\n` вместо переносов: не обнаружено.
- Чужие пути: не обнаружены.
- Кириллица не восстанавливалась, повреждений не обнаружено.

GIT:
- Git-команды не выполнялись.
- Commit не создавался агентом.
- Push не выполнялся агентом.
- Git-история не изменялась агентом.

PROBLEMS:
- Нет.

CONCLUSION:
- TASK 119 выполнен. Правила защиты кириллицы и отчётности при восстановлении текста усилены, активный `БЛОК 1` очищен. Моды обновлять не нужно.
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
