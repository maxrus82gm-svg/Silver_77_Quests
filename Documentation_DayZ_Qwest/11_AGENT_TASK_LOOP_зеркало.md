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
- Создан `Documentation_DayZ_Qwest/20_SplitDoc/IMAGE_GENERATION_PRESET.md` с постоянным пресетом генерации сюжетных картинок по ЛОРу.
- В `Documentation_DayZ_Qwest/12_START_зеркало.md` добавлен раздел `1.3` с обязательным чтением `IMAGE_GENERATION_PRESET.md` перед подготовкой сюжетных изображений.
- В `.gitignore` добавлено точечное правило для `Documentation_DayZ_Qwest/.obsidian/workspace.json`.
- Выполнен `git rm --cached -- Documentation_DayZ_Qwest/.obsidian/workspace.json`; файл оставлен на диске и снят с Git-отслеживания.
- В `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` добавлена запись `TASK 115`.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена выполненная `TASK 115`.
- Активный `БЛОК 1` очищен после выполнения задачи.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`
- Правила кодировки: `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`

CHANGED FILES:
- `P:\Silver_77_Quests\.gitignore`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\IMAGE_GENERATION_PRESET.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_HISTORY.md`

DIFF:
- Добавлен новый постоянный `SplitDoc` для единого визуального пресета сюжетных картинок.
- В `START` закреплено обязательное чтение пресета перед генерацией иллюстраций по ЛОРу.
- `workspace.json` Obsidian исключён из Git адресно, без изменения содержимого файла.
- История задач и очередь дополнены статусом `TASK 115`.
- Активный `БЛОК 1` возвращён в состояние ожидания новой задачи.

CHECKS:
- `Documentation_DayZ_Qwest/20_SplitDoc/IMAGE_GENERATION_PRESET.md` создан и читается как UTF-8.
- `Documentation_DayZ_Qwest/12_START_зеркало.md` содержит ссылку на `IMAGE_GENERATION_PRESET.md`.
- `.gitignore` содержит правило для `Documentation_DayZ_Qwest/.obsidian/workspace.json`.
- `workspace.json` больше не отслеживается Git и не удалён с диска.
- `TASK_HISTORY.md` содержит `TASK 115`.
- `05_Очередь_задач.md` содержит `TASK 115` со статусом `Готово`.
- Активный `БЛОК 1` очищен.
- Кириллица в изменённых документах читается корректно.
- Моды, JSON квестов, редактор, server profile и PBO/build output не менялись.

ENCODING CHECK:
- В изменённых Markdown-файлах использована кириллица.
- Кириллица отображается нормально при чтении как UTF-8.
- Новые и обновлённые документы сохранены без повреждения текста.
- Риск кодировки после правки не обнаружен.

PROBLEMS:
- Проблем при создании пресета, обновлении правил и снятии `workspace.json` с отслеживания Git не обнаружено.

CONCLUSION:
- TASK 115 выполнен: пресет сюжетных картинок зафиксирован, `START` обновлён, `workspace.json` исключён из Git без удаления с диска, моды обновлять не нужно.
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
