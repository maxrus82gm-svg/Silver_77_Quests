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

TASK 086 — Изучить scroll в @Trader и заменить fallback-пагинацию QuestMenu на настоящий DayZ-compatible scroll

Статус:
Новая активная UI-задача после ручной проверки в игре.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Перед выполнением задачи агент обязан прочитать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Агент обязан соблюдать принцип жёстких рамок:

- делать только то, что прямо указано в этом БЛОКЕ 1;
- не чинить “заодно” соседние проблемы;
- не расширять задачу самостоятельно;
- менять только явно разрешённые файлы;
- читать @Trader только как технический референс;
- не менять @Trader;
- не копировать чужой мод целиком;
- не трогать JSON, server, editor, RPC/sync и quest logic;
- не возвращать неподдерживаемый MultilineTextWidget API:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible;
- если точный способ Trader-scroll не подходит для Description/Dialog, сначала описать проблему в PROBLEMS, а не ломать QuestMenu.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 085 QuestMenu получил fallback/page approach для DescriptionText и DialogText.

Ручная проверка показала:

1. Page/fallback через кнопки < / > — не подходит как финальное решение.
   Пользователь явно указал:
   “Это решение как сейчас не годится. Нужен именно scroll.”

2. В качестве рабочего примера есть распакованный мод Trader:

P:\Silver_77_Quests\@Trader

На скриншоте Trader UI видно, что там есть настоящий scroll:
- список предметов прокручивается;
- есть вертикальный красный scrollbar;
- UI выглядит как нормальный DayZ-compatible интерфейс со scroll;
- фон Trader UI лучше перекрывает игровой мир.

Нужно изучить, как scroll реализован в @Trader:
- через какие layout widget classes;
- через какие script classes;
- какие методы используются;
- как связаны list/text widget и scrollbar;
- можно ли этот паттерн безопасно перенести в QuestMenu.

Актуальный hash / commit:

85e241d0a93d9325964be346438fe0801730d2cb

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 086
--------------------------------------------------------------------------------

Заменить текущую fallback-пагинацию QuestMenu на настоящий DayZ-compatible scroll, основанный на проверенном паттерне из @Trader.

Главная цель:

1. DescriptionPanel должен иметь настоящий scroll.
2. DialogPanel должен иметь настоящий scroll.
3. Кнопки < / > и page-индикаторы из TASK 085 должны быть удалены, если scroll успешно реализован.
4. Нельзя использовать неподдерживаемые методы MultilineTextWidget:
   - VScrollToPos01
   - VScrollStep
   - IsScrollbarVisible
5. Не сломать Offer / Completion / Reward.
6. Не менять JSON-контракт.
7. Не трогать серверную логику.
8. Не трогать QuestJournal в этой задаче.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:

Сначала изучить @Trader, а не угадывать.

Нужно найти конкретно:
- layout-файл Trader menu;
- widget, который даёт scroll;
- script-класс Trader menu;
- как script наполняет список/текст;
- как работает scrollbar;
- какие методы DayZ API используются;
- какие из них безопасны и компилируются.

Моё предпочтение:

Если Trader использует TextListboxWidget с нативной прокруткой:
- рассмотреть вариант заменить длинные Description/Dialog text areas на scrollable list/text containers;
- например, разбивать текст на строки и добавлять строки в TextListboxWidget;
- тогда DayZ сам даст нативный scrollbar, как в Trader.

Если Trader использует ScrollWidget / ScrollPanel:
- перенести этот layout/script pattern минимально;
- не переписывать весь QuestMenu.

Если Trader scroll подходит только для списков, но не для MultilineTextWidget:
- не пытаться снова скроллить MultilineTextWidget unsupported API;
- лучше сделать scrollable TextListbox-based display для Description/Dialog.

Текущее page/fallback-решение считать временным и заменить, если найден рабочий scroll.

Важно:
- не копировать лишнюю торговую логику;
- не переносить RPC Trader;
- не переносить safezone;
- не переносить currency/item logic;
- взять только UI-scroll pattern.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это UI-задача только для QuestMenu scroll.

Агент должен:

1. Изучить @Trader как reference.
2. Найти, как в Trader реализован scroll.
3. Описать найденный scroll pattern в отчёте.
4. Перенести подход на QuestMenu.
5. Заменить fallback/page approach для DescriptionText и DialogText на настоящий scroll.
6. Удалить или отключить page-кнопки и page-индикаторы из QuestMenu, если scroll успешно реализован.
7. Сохранить текущий mapping:
   - DescriptionPanel = quest.description / статус / цели / прогресс / награды;
   - DialogPanel = triggerActions[].dialogText / текущий NPC dialog.
8. Сохранить текущие основные кнопки:
   - ВЗЯТЬ КВЕСТ
   - СДАТЬ КВЕСТ
   - Закрыть рот и уйти
9. Вернуть отчёт в чат.

Агент не должен:

- менять @Trader;
- менять QuestJournal;
- менять JSON;
- менять server;
- менять RPC/sync;
- менять Quest Editor;
- менять quest logic;
- менять Offer / Completion / Reward архитектуру;
- трогать Doors and Barricades Fixed;
- делать commit/push/reset/clean;
- перепаковывать PBO;
- запускать Addon Builder.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ И ПАПКИ ДЛЯ ЧТЕНИЯ
--------------------------------------------------------------------------------

Можно читать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Можно читать @Trader:

6. P:\Silver_77_Quests\@Trader\

Особенно искать и читать внутри @Trader:

- *.layout
- *.c
- *.cpp
- *.h
- config.cpp
- scripts\
- gui\
- layouts\
- menu-related файлы
- Trader menu UI files
- файлы, где встречаются:
  - Scroll
  - Scrollbar
  - Listbox
  - TextListbox
  - OnMouseWheel
  - Slider
  - VScroll
  - Update
  - AddItem
  - ClearItems
  - SelectRow

Можно читать текущий QuestMenu:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Можно читать, но не менять:

9. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
10. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- P:\Silver_77_Quests\@Trader\
- P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
- P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
- P:\Silver_77_Quests\Silver_77_Quests_Server\
- P:\Silver_77_Quests\JSON_Quvest\
- P:\Silver_77_Quests\DayZ_layout\
- P:\Silver_77_Quests\Documentation\
- P:\Silver_77_Quests\Documentation\SplitDoc\
- P:\Silver_77_Quests\Support\
- P:\Silver_77_Quests\Doors and Barricades Fixed\
- любые quest JSON;
- любые server profile файлы;
- любые PBO.

Запрещено делать:

- git commit;
- git push;
- git reset;
- git clean;
- git checkout;
- перенос файлов;
- удаление файлов;
- переименование файлов;
- перепаковку PBO;
- запуск Addon Builder;
- изменение JSON-контракта;
- изменение RPC/sync;
- изменение логики Offer / Completion / Reward;
- перенос Trader gameplay/server logic;
- перенос Trader RPC;
- перенос Trader currency/trade logic.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Найти Trader UI files

В @Trader найти файлы, отвечающие за торговое меню.

Нужно определить:

- layout-файл меню Trader;
- script-файл меню Trader;
- где создаётся меню;
- где находятся виджеты списка;
- где находится scrollbar;
- какой widget class используется для scroll;
- как script наполняет список;
- как обрабатывается OnMouseWheel;
- используется ли TextListboxWidget;
- используется ли ScrollWidget / ScrollPanel;
- используется ли SliderWidget;
- используется ли стандартный scrollbar layout.

2. Описать scroll pattern Trader

В отчёте кратко описать:

- какие widget classes дают scroll;
- какие имена виджетов используются;
- какие script methods используются;
- есть ли ручная обработка колеса мыши;
- есть ли встроенный scrollbar;
- можно ли применить этот подход к QuestMenu.

3. Выбрать безопасный способ переноса

Вариант A — если Trader использует TextListboxWidget scroll:

- сделать Description как scrollable list/text block на базе TextListboxWidget или аналогичного виджета;
- разбивать full description text на строки;
- добавлять строки в список;
- использовать нативный scrollbar списка;
- аналогично для Dialog.

Вариант B — если Trader использует ScrollWidget / ScrollPanel:

- применить такой же layout pattern к DescriptionPanel и DialogPanel;
- сохранить текст внутри scrollable container;
- не использовать unsupported MultilineTextWidget API.

Вариант C — если Trader использует другой безопасный widget:

- перенести только UI-scroll pattern;
- объяснить в отчёте, почему выбран именно он.

4. Убрать fallback-пагинацию TASK 085

Если настоящий scroll реализован:

Удалить или отключить из QuestMenu.layout:

- DescriptionPageInfo
- DescriptionPrevButton
- DescriptionNextButton
- DialogPageInfo
- DialogPrevButton
- DialogNextButton

Удалить или отключить из QuestUI.c:

- m_DescriptionPageInfo
- m_DialogPageInfo
- m_DescriptionPrevButton
- m_DescriptionNextButton
- m_DialogPrevButton
- m_DialogNextButton
- m_DescriptionPages
- m_DialogPages
- m_DescriptionPageIndex
- m_DialogPageIndex
- BuildPagedText
- UpdatePagedTextControls
- ApplyDescriptionPage
- ApplyDialogPage
- SetDescriptionPagedText
- SetDialogPagedText
- ChangeDescriptionPage
- ChangeDialogPage

И заменить это на scroll-compatible implementation.

Если часть этих функций больше не нужна — удалить.
Если безопаснее временно оставить неиспользуемыми — не оставлять мёртвый код без причины. Лучше убрать, если компиляция позволяет.

5. Реализовать scroll для DescriptionPanel

DescriptionPanel должен показывать:

- quest.description;
- status;
- requirements;
- objectives;
- deposited progress;
- rewards.

Текст должен быть читаем через настоящий scroll.

Нельзя возвращать старый режим, где NPC dialogText подмешивался в DescriptionPanel.

6. Реализовать scroll для DialogPanel

DialogPanel должен показывать:

- triggerActions[].dialogText;
- current NPC dialog;
- selected route dialog.

Текст должен быть читаем через настоящий scroll.

7. Сохранить фон / readability

Фон из TASK 085 можно оставить или минимально поправить, если это связано с layout-перестройкой под scroll.

Не делать отдельный большой редизайн.

Требование:
- фон должен оставаться лучше, чем до TASK 085;
- не возвращать прозрачный QuestMenu;
- action prompt должен меньше мешать чтению.

8. Проверить отсутствие unsupported API

После правок обязательно проверить, что в QuestUI.c нет executable-вызовов:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

Если эти слова встречаются только в комментарии — допустимо, но лучше убрать комментарии с этими именами, чтобы не путать будущий поиск.

9. Проверить compile-safety

Проверить:

- QuestUI.c синтаксически целый;
- QuestMenu.layout синтаксически целый;
- нет ссылок на удалённые widget names;
- нет ссылок на удалённые variables;
- OnClick не ссылается на удалённые page-buttons;
- OnMouseWheel не ссылается на удалённые page-state variables;
- UpdateQuestDetails пишет текст в новую scroll-систему.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout и .c файлы с кириллицей.

Нужно соблюдать ENCODING_RULES.md:

- не делать массовую перекодировку;
- не ломать кириллицу;
- не менять русские строки без необходимости;
- не сохранять файл в неправильной кодировке;
- если файл был UTF-8 без BOM — сохранить UTF-8 без BOM;
- в отчёте указать ENCODING CHECK.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ ДЛЯ ЭТОЙ ЗАДАЧИ
--------------------------------------------------------------------------------

Это только QuestMenu scroll task на основе @Trader reference.

Разрешено исправить только:

- заменить fallback-пагинацию на настоящий scroll;
- сохранить/минимально поправить фон QuestMenu, если это связано с layout-scroll;
- изучить @Trader как reference.

Нельзя:

- менять QuestJournal;
- менять JSON;
- менять server;
- менять Quest Editor;
- менять RPC/sync;
- менять progress logic;
- менять Offer / Completion / Reward;
- менять структуру квестового контракта;
- переносить Trader gameplay logic;
- переносить Trader RPC;
- переносить Trader economy/currency logic;
- менять @Trader;
- возвращать unsupported scroll API;
- чинить другие ошибки, если они не связаны напрямую с QuestMenu scroll.

Если Trader pattern не удаётся безопасно перенести:

- не ломать QuestMenu;
- оставить текущий рабочий fallback только если иначе будет хуже;
- подробно описать, почему scroll не перенесён;
- предложить следующую точечную задачу.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. @Trader изучен как reference.
2. В отчёте указан конкретный Trader scroll pattern.
3. QuestMenu.layout синтаксически целый.
4. QuestUI.c синтаксически целый.
5. DescriptionPanel имеет настоящий scroll.
6. DialogPanel имеет настоящий scroll.
7. Page/fallback кнопки удалены или больше не используются.
8. В QuestUI.c нет executable-вызовов:
   - VScrollToPos01
   - VScrollStep
   - IsScrollbarVisible
9. Кнопки не сломаны:
   - ВЗЯТЬ КВЕСТ
   - СДАТЬ КВЕСТ
   - Закрыть рот и уйти
10. Mapping Description/Dialog не изменён.
11. Кириллица не повреждена.
12. CHANGED FILES содержит только:
   - QuestMenu.layout
   - QuestUI.c

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. Найден и описан scroll pattern из @Trader.
2. QuestMenu использует настоящий DayZ-compatible scroll.
3. DescriptionText / DescriptionPanel читается через scroll.
4. DialogText / DialogPanel читается через scroll.
5. Fallback page approach из TASK 085 удалён или отключён.
6. Неподдерживаемый MultilineTextWidget API не используется.
7. Изменены только разрешённые файлы.
8. @Trader не менялся.
9. JSON не менялся.
10. Server не менялся.
11. Quest Editor не менялся.
12. Documentation не менялась.
13. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- какие Trader files были изучены;
- какой scroll pattern найден;
- что изменено в QuestMenu.layout;
- что изменено в QuestUI.c;
- что сделано с fallback/page approach;
- подтверждение, что unsupported API не возвращался.

TRADER SCROLL PATTERN:
- layout file;
- script file;
- widget classes;
- scrollbar/listbox/scroll implementation;
- какие элементы перенесены как подход;
- какие элементы Trader не переносились.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

DIFF:
- кратко описать изменения layout;
- кратко описать изменения QuestUI.c;
- указать, чем заменена page-пагинация.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- если Trader pattern частично несовместим — почему;
- какие ограничения остались.

QUESTIONS:
- только реальные вопросы, если есть.

RECOMMENDED NEXT TASK:
- если нужно, предложить отдельную задачу на QuestJournal;
- если нужно, предложить отдельную задачу на улучшение внешнего вида scroll.

CONCLUSION:
- краткий вывод: QuestMenu теперь должен использовать настоящий scroll по DayZ-compatible паттерну из Trader.

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
- Создана папка `Documentation/SplitDoc`.
- Созданы и заполнены `AGENT_RULES.md`, `ENCODING_RULES.md`, `DAYZ_RPC_SYNC_RULES.md`, `QUEST_EDITOR_RULES.md`, `QUEST_JSON_CONTRACT.md`, `QUEST_UI_RULES.md`, `DAYZ_LAYOUT_VIEWER_RULES.md`, `SERVER_PROFILE_RULES.md`, `TASK_HISTORY.md`.
- В `AGENT_RULES.md` закреплены новые правила: ChatGPT всегда даёт полный `БЛОК 1`, краткую задачу агенту, своё мнение / предпочтительное решение, полный `REVIEW`, а новые правила поручает внести в нужный `SplitDoc`.
- В `AGENT_RULES.md` закреплено новое правило: если задача разрешает, агент сам обновляет `AGENT REPORT`, `REVIEW / STATUS`, `TASK_HISTORY` и новые правила в нужных `SplitDoc`.
- `AGENT_TASK_LOOP.md` сокращён до рабочего shell-документа с матрицей чтения, `БЛОКОМ 1`, `БЛОКОМ 2`, `БЛОКОМ 3` и ссылкой на `TASK_HISTORY.md`.
- В `БЛОК 3` внесён `TASK 079 REVIEW`.
- В `TASK_HISTORY.md` добавлены записи по `TASK 079` и `TASK 080`.

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_RPC_SYNC_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_EDITOR_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_JSON_CONTRACT.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\SERVER_PROFILE_RULES.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`

DIFF:
- Огромные постоянные правила вынесены из `AGENT_TASK_LOOP.md` в тематические `SplitDoc`.
- История задач вынесена в отдельный `TASK_HISTORY.md`.
- Общее агентское поведение и процесс постановки задач теперь собраны в `AGENT_RULES.md`.
- Encoding, RPC/sync, editor, JSON contract, UI, layout viewer и server profile разделены по отдельным документам.
- Старый конфликт со ссылкой на "будущую TASK 079" внутри UI-заметок устранён за счёт вынесения backlog-правил из loop-файла.

PROBLEMS:
- В проекте уже существуют пересекающиеся документы: `Documentation/RUSSIAN_ENCODING.md`, `Documentation/QUEST_LOGIC_SPEC.md`, `Documentation/README_JSON_CONFIG.md`. Они не менялись, но теперь есть риск двойного канона, если `SplitDoc` и старые документы начнут расходиться.
- `TASK_HISTORY.md` перенесён как рабочая история из старого loop-файла. Если нужен полный аудит всех прошлых задач вне этого списка, это потребует отдельной задачи.

QUESTIONS:
- Нет.

CONCLUSION:
- `SplitDoc`-структура создана, постоянные правила разнесены по темам, а `AGENT_TASK_LOOP.md` переведён в рабочий shell-формат. Код, JSON, мод, layout и внешние канонические документы не менялись.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# 🟨 БЛОК 3 — REVIEW / STATUS
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

TASK 085 REVIEW

Статус:
Принято условно.

Что принято:
- Scope соблюдён.
- Изменены только разрешённые файлы:
  - Silver_77_Quests_Client\gui\QuestMenu.layout
  - Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c
- QuestJournal не трогался.
- JSON не трогался.
- Server не трогался.
- Quest Editor не трогался.
- Documentation / SplitDoc не трогались.
- Неподдерживаемый MultilineTextWidget scroll API не возвращался:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible
- Для DescriptionText реализован fallback/page approach.
- Для DialogText реализован fallback/page approach.
- Добавлены page-индикаторы и кнопки переключения страниц.
- OnMouseWheel теперь безопасно листает страницы Description/Dialog, а не вызывает unsupported scroll API.
- Mapping Description/Dialog сохранён:
  - DescriptionPanel = описание, статус, цели, прогресс, награды;
  - DialogPanel = NPC dialog / triggerActions[].dialogText.
- Фон усилен через QuestPanelBackdrop и непрозрачные panel backing.

Что требует ручной проверки в игре:
1. Компилируется ли Mission script module после изменений.
2. Не вызывает ли ButtonWidgetClass QuestPanelBackdrop побочных эффектов в input/click handling.
3. Действительно ли фон стал непрозрачным и перекрывает игровой мир/action prompt.
4. Не перекрывает ли QuestPanelBackdrop реальные кнопки.
5. Видны ли кнопки DescriptionPrevButton / DescriptionNextButton.
6. Видны ли кнопки DialogPrevButton / DialogNextButton.
7. Работают ли кнопки < / > для описания.
8. Работают ли кнопки < / > для диалога.
9. Листается ли Description/Dialog колесом мыши при наведении на соответствующую панель.
10. Не ломаются ли основные кнопки:
    - ВЗЯТЬ КВЕСТ
    - СДАТЬ КВЕСТ
    - Закрыть рот и уйти
11. Не сломались ли Offer / Completion / Reward.
12. Не повреждена ли кириллица.
13. Достаточно ли размер страниц: сейчас Description разбивается примерно на 46 символов x 9 строк, Dialog на 46 символов x 4 строки.

Риски:
- Это не настоящий scrollbar, а fallback-пагинация.
- Page-разбивка по символам может иногда резать текст неидеально, особенно при длинных className или длинных словах.
- ButtonWidgetClass как backing-слой может сработать лучше визуально, но это обязательно надо проверить в игре: он не должен перехватывать клики, потому что стоит ignorepointer 1.
- Если фон всё ещё будет прозрачным, значит проблема не только в alpha/color, и нужно будет искать DayZ-способ через ImageWidget/texture/background из vanilla UI.

Вывод:
TASK 085 выполнена корректно в рамках задачи. Принимается условно до ручной проверки в игре.

Следующий шаг:
Пересобрать клиентский PBO, запустить сервер/клиент и проверить QuestMenu на NPC с длинным текстом.

Если всё работает:
- TASK 085 можно закрывать.

Если фон всё ещё прозрачный:
- следующая задача должна быть только про реальный DayZ background/texture backing.

Если пагинация работает, но неудобна:
- отдельная задача на улучшение UX страниц или поиск подтверждённого ScrollWidget/ScrollPanel.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
