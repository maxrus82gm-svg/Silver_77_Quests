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

TASK 096 — QuestMenu: тестово заменить Description/Dialog на ScrollWidget + Size-To-Content container + RichTextWidget

Статус:
Тестовая UI-задача после аналитики TASK 095.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После серии задач по QuestMenu выяснено:

1. TextListboxWidget:
   - даёт рабочий scroll;
   - но плохо подходит для абзацного текста;
   - требует ручного maxCharsPerLine / weighted-wrap;
   - не является финальным решением для Description/Dialog.

2. MultilineTextWidget:
   - даёт native word wrap;
   - но в связке ScrollWidget + MultilineTextWidget напрямую оказался нестабилен для нашего QuestMenu:
     - текст появлялся;
     - native wrap работал;
     - но clipping и геометрия в runtime были нестабильны;
     - фиксированные размеры 394x600 / 394x300 оказались не нормальной моделью content-height.

3. TASK 095 показал, что более правильное направление:
   - ScrollWidget;
   - внутри size-to-content container;
   - внутри RichTextWidgetClass как read-only wrapped text.

Текущий прямой стек:

ScrollWidgetClass
  MultilineTextWidgetClass

больше не развивать как основное направление.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 096
--------------------------------------------------------------------------------

Тестово заменить Description/Dialog в QuestMenu на новый DayZ-compatible content stack:

ScrollWidgetClass
  WrapSpacerWidgetClass или GridSpacerWidgetClass
    RichTextWidgetClass

Цель:

1. Сохранить native scroll через ScrollWidget.
2. Получить wrapped text через RichTextWidget.
3. Уйти от фиксированной высоты 394x600 / 394x300 как основной модели.
4. Использовать Size-To-Content container для расчёта высоты контента.
5. Сохранить Description/Dialog как read-only текстовые области.
6. Не трогать QuestListbox и TriggerRouteListbox.
7. Проверить и при необходимости минимально обновить DayZ_layout viewer под RichTextWidgetClass.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Агент обязан:

- делать только то, что прямо указано в этой задаче;
- не чинить “заодно” соседние проблемы;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не трогать server;
- не трогать Quest Editor;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать большой редизайн QuestMenu;
- не делать git commit / push / reset / clean;
- если RichTextWidget не компилируется или не работает — остановиться и описать проблему в PROBLEMS.

Запрещено:

- возвращать TextListboxWidget для Description/Dialog;
- переходить на MultilineEditBoxWidget в этой задаче;
- переходить на HtmlWidget в этой задаче;
- менять JSON;
- менять server;
- менять QuestJournal;
- менять quest logic;
- менять Offer / Completion / Reward;
- перепаковывать PBO;
- запускать Addon Builder.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Обязательно прочитать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
6. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Прочитать текущий UI:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Можно читать для reference, но не менять:

9. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
10. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
11. P:\Silver_77_Quests\DayZ_layout\
12. P:\Silver_77_Quests\@Trader\

Можно читать vanilla/reference на диске D, если доступны:

13. D:\Dayz\gui\layouts\
14. D:\Dayz\scripts\5_mission\gui\
15. D:\Dayz\scripts\
16. D:\Dayz\gui\layouts\new_ui\
17. D:\Dayz\gui\layouts\new_ui\mods_menu\
18. D:\Dayz\gui\layouts\new_ui\options\
19. D:\Dayz\gui\layouts\script_console\

Особенно полезно свериться с примерами, найденными в TASK 095:

- options_menu.layout
- character_creation.layout
- mods_menu_detailed_entry.layout
- mods_menu_detailed.layout
- credits_menu.layout
- server_browser_tab.layout
- scrolltest.layout

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Дополнительно разрешено менять только если DayZ_layout viewer не показывает актуальный QuestMenu.layout корректно после перехода на RichTextWidgetClass:

3. P:\Silver_77_Quests\DayZ_layout\*

DayZ_layout менять только минимально:

- добавить поддержку RichTextWidgetClass как текстового widget;
- показать его текст/рамку/размеры в preview;
- показать структуру ScrollWidget -> Size-To-Content container -> RichTextWidget;
- не переписывать viewer полностью;
- не делать большой viewer redesign.

Если viewer уже показывает новую структуру достаточно корректно:
- DayZ_layout не менять;
- указать это в отчёте.

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- P:\Silver_77_Quests\@Trader\
- P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
- P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
- P:\Silver_77_Quests\Silver_77_Quests_Server\
- P:\Silver_77_Quests\JSON_Quvest\
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
- переход на MultilineEditBoxWidget;
- переход на HtmlWidget;
- возврат к TextListboxWidget для Description/Dialog.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Изменить Description stack в QuestMenu.layout

Текущий проблемный стек:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Нужно тестово заменить на:

ScrollWidgetClass DescriptionScroll
  WrapSpacerWidgetClass или GridSpacerWidgetClass DescriptionContent
    RichTextWidgetClass DescriptionText

Требования:

- DescriptionScroll остаётся ScrollWidgetClass.
- DescriptionContent должен быть size-to-content по вертикали.
- DescriptionText должен быть RichTextWidgetClass.
- DescriptionText должен иметь wrap 1, если это свойство поддерживается.
- DescriptionText должен быть read-only display text.
- DescriptionText должен занимать ширину с запасом под scrollbar.
- Не использовать фиксированную высоту 600 как основной способ content-height.
- Не возвращать TextListboxWidget.

2. Изменить Dialog stack в QuestMenu.layout

Текущий проблемный стек:

ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Нужно тестово заменить на:

ScrollWidgetClass DialogScroll
  WrapSpacerWidgetClass или GridSpacerWidgetClass DialogContent
    RichTextWidgetClass DialogText

Требования:

- DialogScroll остаётся ScrollWidgetClass.
- DialogContent должен быть size-to-content по вертикали.
- DialogText должен быть RichTextWidgetClass.
- DialogText должен иметь wrap 1, если это свойство поддерживается.
- DialogText должен быть read-only display text.
- DialogText должен занимать ширину с запасом под scrollbar.
- Не использовать фиксированную высоту 300 как основной способ content-height.
- Не возвращать TextListboxWidget.

3. Изменить QuestUI.c под RichTextWidget

Заменить типы:

Было сейчас:
- private MultilineTextWidget m_QuestDescription;
- private MultilineTextWidget m_DialogText;

Должно стать:
- private RichTextWidget m_QuestDescription;
- private RichTextWidget m_DialogText;

В Init():

- найти DescriptionText;
- найти DialogText;
- кастовать их как RichTextWidget.

SetDescriptionText(text):

- должен делать SetText(text) на RichTextWidget;
- должен делать Update(), если это безопасно и компилируется;
- reset scroll делать только на DescriptionScroll.

SetDialogText(text):

- должен делать SetText(text) на RichTextWidget;
- должен делать Update(), если это безопасно и компилируется;
- reset scroll делать только на DialogScroll.

Важно:

- не вызывать scroll API на RichTextWidget;
- scroll API только на ScrollWidget;
- если RichTextWidget не поддерживает какой-то метод, не костылить — описать в PROBLEMS.

4. Убрать MultilineText-specific code

Удалить или отключить для Description/Dialog:

- MultilineTextWidget casts;
- SetLineBreakingOverride(LinebreakOverrideMode.LINEBREAK_WESTERN) для Description/Dialog.

Если RichTextWidget имеет свой аналог line breaking/wrap и он подтверждён reference — можно использовать.
Если нет — оставить только layout wrap / native rich text behavior.

5. Проверить scroll reset

Сохранить reset scroll только через ScrollWidget:

- m_DescriptionScroll.VScrollToPos01(0)
- m_DialogScroll.VScrollToPos01(0)

Если `VScrollToPos01(0)` на ScrollWidget компилируется — оставить.
Если появится compile error — убрать reset scroll и указать в PROBLEMS.

6. Проверить QuestListbox / RoutePanel

Не менять:

- QuestListbox
- TriggerRouteListbox
- RoutePanel
- кнопки
- Offer / Completion / Reward logic

7. Проверить DayZ_layout viewer

По правилу SplitDoc нужно проверить viewer.

Если после RichTextWidget viewer не показывает текстовые области нормально:

- минимально обновить dayz_layout_viewer.html;
- добавить RichTextWidgetClass в text-node detection;
- добавить preview RichTextWidgetClass как текстового блока;
- не переписывать viewer целиком.

Если viewer показывает структуру достаточно:
- не менять viewer;
- написать в DAYZ_LAYOUT CHECK.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. QuestUI.c синтаксически целый.
3. DescriptionText теперь RichTextWidgetClass.
4. DialogText теперь RichTextWidgetClass.
5. DescriptionScroll остался ScrollWidgetClass.
6. DialogScroll остался ScrollWidgetClass.
7. DescriptionContent существует как size-to-content container.
8. DialogContent существует как size-to-content container.
9. m_QuestDescription кастуется как RichTextWidget.
10. m_DialogText кастуется как RichTextWidget.
11. SetDescriptionText использует SetText.
12. SetDialogText использует SetText.
13. Scroll API не вызывается на RichTextWidget.
14. Reset scroll, если есть, вызывается только на ScrollWidget.
15. MultilineTextWidget больше не используется для Description/Dialog.
16. TextListboxWidget не возвращён для Description/Dialog.
17. QuestListbox не изменён.
18. TriggerRouteListbox не изменён.
19. JSON не менялся.
20. Server не менялся.
21. QuestJournal не менялся.
22. @Trader не менялся.
23. DayZ_layout viewer проверен.
24. Кириллица не повреждена.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout, .c и возможно viewer html.

Нужно соблюдать ENCODING_RULES.md:

- не делать массовую перекодировку;
- не ломать кириллицу;
- не менять русские строки без необходимости;
- не сохранять файл в неправильной кодировке;
- если файл был UTF-8 без BOM — сохранить UTF-8 без BOM;
- в отчёте указать ENCODING CHECK.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. Description/Dialog тестово переведены на ScrollWidget + Size-To-Content container + RichTextWidget.
2. TextListboxWidget для Description/Dialog не возвращён.
3. MultilineTextWidget для Description/Dialog больше не используется.
4. ScrollWidget сохранён.
5. QuestListbox и TriggerRouteListbox не изменены.
6. Quest logic не изменена.
7. JSON не изменён.
8. Server не изменён.
9. QuestJournal не изменён.
10. DayZ_layout viewer проверен и при необходимости минимально обновлён.
11. Агент вернул отчёт в чат.

Если RichTextWidget не компилируется или не отображается:
- задача всё равно может считаться выполненной как тест, если агент не ушёл в новую архитектуру, а честно описал проблему в PROBLEMS и предложил следующий безопасный вариант.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что изменено в QuestMenu.layout;
- что изменено в QuestUI.c;
- какой container выбран: WrapSpacerWidgetClass или GridSpacerWidgetClass;
- используется ли RichTextWidgetClass;
- менялся ли DayZ_layout viewer.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c
- DayZ_layout\..., если менялся

DIFF:
- кратко описать изменения layout;
- кратко описать изменения script;
- кратко описать изменения viewer, если были.

COMPILE-SAFETY CHECK:
- нет cast mismatch;
- нет scroll API на RichTextWidget;
- reset scroll только на ScrollWidget;
- QuestListbox/TriggerRouteListbox сохранены.

DAYZ_LAYOUT CHECK:
- показывает ли viewer ScrollWidget + content container + RichTextWidget;
- обновлялся ли viewer;
- какие ограничения viewer остаются.

RUNTIME EXPECTATION:
- что должно измениться в игре;
- что нужно проверить руками.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- если RichTextWidget не подходит — почему.

RECOMMENDED NEXT TASK:
- если RichTextWidget работает — runtime polish;
- если не работает — отдельная задача на MultilineEditBoxWidget по vanilla pattern.

CONCLUSION:
- краткий вывод: тестовый переход на RichTextWidget выполнен или почему остановлен.

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

TASK 095 REVIEW

Статус:
Принято.

Что принято:
- Агент правильно остановил ветку ScrollWidget + MultilineTextWidget напрямую.
- Правильно определил, что проблема не только в размерах панелей, а в неверной модели scroll-content.
- Правильно заметил, что фиксированные 394x600 / 394x300 — это не нормальная модель content-height.
- Правильно отделил:
  - native wrap;
  - scroll container;
  - content sizing;
  - clipping.
- Правильно проверил влияние DayZ_layout viewer:
  - viewer помогает видеть структуру;
  - но не моделирует реальный clipping;
  - поэтому viewer не может быть единственным арбитром scroll-поведения.
- Правильно нашёл, что самый подтверждённый vanilla-pattern для длинного scrollable text — это MultilineEditBoxWidget в scroll-контейнере.
- Но также логично предложил сначала попробовать более чистый read-only вариант:
  - ScrollWidget
  - Size-To-Content container
  - RichTextWidget

Главный вывод:
Текущий прямой вариант:

ScrollWidget
  MultilineTextWidget

лучше больше не развивать.

Следующий разумный практический шаг:
TASK 096 — тестово перейти на vanilla-style content stack.

Предпочтительный путь:
ScrollWidget
  WrapSpacerWidgetClass или GridSpacerWidgetClass
    RichTextWidgetClass

Сразу с разрешением минимально обновить DayZ_layout viewer, потому что viewer сейчас не считает RichTextWidgetClass текстовым виджетом.

Резервный путь:
Если RichTextWidget не заведётся — отдельной задачей пробовать MultilineEditBoxWidget по vanilla-паттерну script_console_universal_info_dialog.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
