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

TASK 099 — Hotfix: откатить ContentParent SizeToChild и вернуть рабочий baseline QuestMenu

Статус:
Срочный UI hotfix после runtime-проверки TASK 098.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

Текущий рабочий baseline QuestMenu:

5ccb7c4d24de2ac0691136e64ad29e2789708874

В этом baseline:

1. QuestMenu визуально целый.
2. Окна находятся на правильных местах.
3. Description/Dialog текст виден.
4. Native wrap работает.
5. Scrollbar работает.
6. QuestListbox / RoutePanel / кнопки находятся на местах.
7. Главная оставшаяся проблема baseline: текст при прокрутке выходит за границы scroll-окна.

После TASK 098 был сделан тест:

ScrollWidget
  FrameWidgetClass ContentParent
    MultilineTextWidget

Runtime-проверка показала, что этот тест НЕ подходит:

- геометрия снова разъехалась;
- Dialog уехал вниз и оказался рядом с кнопками;
- кнопки начали визуально пересекаться с Dialog;
- Description/Dialog больше не ведут себя как стабильные области;
- scrollbar визуально остаётся в одной области, а content ощущается как будто живёт в другом coordinate space.

Вывод:

`ContentParent + SizeToChild` в текущем QuestMenu ломает компоновку и должен быть откатан.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 099
--------------------------------------------------------------------------------

Вернуть QuestMenu к рабочему baseline-состоянию:

ScrollWidgetClass
  MultilineTextWidgetClass

То есть откатить изменения TASK 098:

- убрать `DescriptionContent`;
- убрать `DialogContent`;
- убрать `scriptclass "SizeToChild"`;
- вернуть `DescriptionText` прямым ребёнком `DescriptionScroll`;
- вернуть `DialogText` прямым ребёнком `DialogScroll`;
- не делать новых экспериментов;
- не чинить clipping в этой задаче.

Главная цель:

- восстановить рабочую геометрию QuestMenu;
- вернуть состояние, где всё на местах;
- оставить известную проблему clipping для отдельной будущей аналитики/задачи.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Агент обязан:

- сделать только откат TASK 098;
- не искать новое решение в этой задаче;
- не делать новый wrapper;
- не менять архитектуру;
- не менять размеры основных панелей;
- не менять позиции основных панелей;
- не менять QuestListbox;
- не менять TriggerRouteListbox;
- не менять кнопки;
- не трогать JSON;
- не трогать server;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать git commit / push / reset / clean;
- не запускать Addon Builder;
- не перепаковывать PBO.

Запрещено:

- оставлять `DescriptionContent`;
- оставлять `DialogContent`;
- оставлять `scriptclass "SizeToChild"` в QuestMenu.layout;
- переходить на RichTextWidget;
- переходить на MultilineEditBoxWidget;
- переходить на HtmlWidget;
- возвращать TextListboxWidget для Description/Dialog;
- делать новые clipping-эксперименты;
- менять layout кроме отката TASK 098.

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

Прочитать текущие файлы:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c
9. P:\Silver_77_Quests\DayZ_layout\dayz_layout_viewer.html

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. P:\Silver_77_Quests\DayZ_layout\dayz_layout_viewer.html

QuestUI.c менять нельзя, если там нет следов TASK 098.

Если QuestUI.c уже не менялся в TASK 098 и по-прежнему использует:

- MultilineTextWidget;
- SetText;
- Update;
- ResetScrollWidgetToTop на ScrollWidget;

то QuestUI.c оставить без изменений.

--------------------------------------------------------------------------------
БАЗОВАЯ ГЕОМЕТРИЯ, КОТОРУЮ НЕЛЬЗЯ ЛОМАТЬ
--------------------------------------------------------------------------------

Не менять эти позиции и размеры:

DescriptionPanel:
- position 300 74
- size 430 204

DescriptionScroll:
- position 8 34
- size 414 156

DescriptionText:
- position 0 0
- size 394 600
- wrap 1
- "size to text v" 0

DialogPanel:
- position 300 286
- size 430 122

DialogScroll:
- position 8 34
- size 414 74

DialogText:
- position 0 0
- size 394 300
- wrap 1
- "size to text v" 0

RoutePanel:
- position 740 74
- size 210 334

QuestListbox:
- position 30 82
- size 250 450

AcceptButton:
- position 300 420
- size 210 48

CompleteButton:
- position 530 420
- size 210 48

CloseButton:
- position 300 490
- size 650 48

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Откатить Description stack

Сейчас после TASK 098 может быть:

ScrollWidgetClass DescriptionScroll
  FrameWidgetClass DescriptionContent
    MultilineTextWidgetClass DescriptionText

Нужно вернуть:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Удалить:

- FrameWidgetClass DescriptionContent
- scriptclass "SizeToChild"
- m_ChildName "DescriptionText"
- m_ResizeHorizontal
- m_ResizeVertical
- offsets SizeToChild

`DescriptionText` должен снова лежать напрямую внутри `DescriptionScroll`.

2. Откатить Dialog stack

Сейчас после TASK 098 может быть:

ScrollWidgetClass DialogScroll
  FrameWidgetClass DialogContent
    MultilineTextWidgetClass DialogText

Нужно вернуть:

ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Удалить:

- FrameWidgetClass DialogContent
- scriptclass "SizeToChild"
- m_ChildName "DialogText"
- m_ResizeHorizontal
- m_ResizeVertical
- offsets SizeToChild

`DialogText` должен снова лежать напрямую внутри `DialogScroll`.

3. Сохранить MultilineTextWidget baseline

DescriptionText должен остаться:

- MultilineTextWidgetClass
- size 394 600
- wrap 1
- "size to text v" 0
- font "gui/fonts/metron16"
- текущий цвет
- text halign left
- text valign top

DialogText должен остаться:

- MultilineTextWidgetClass
- size 394 300
- wrap 1
- "size to text v" 0
- font "gui/fonts/metron16"
- текущий цвет
- text halign left
- text valign top

4. QuestUI.c

Не менять, если там нет следов TASK 098.

Проверить, что там по-прежнему:

- private MultilineTextWidget m_QuestDescription;
- private MultilineTextWidget m_DialogText;
- MultilineTextWidget.Cast(...);
- SetDescriptionText(...) делает SetText + Update + ResetScrollWidgetToTop(m_DescriptionScroll);
- SetDialogText(...) делает SetText + Update + ResetScrollWidgetToTop(m_DialogScroll);
- scroll API не вызывается на MultilineTextWidget.

5. DayZ_layout viewer

Viewer после TASK 098 мог быть настроен на ожидание:

FrameWidgetClass -> MultilineTextWidgetClass

Нужно минимально вернуть viewer sanity-check к baseline:

Description:
DescriptionScroll -> DescriptionText

Dialog:
DialogScroll -> DialogText

Viewer должен проверять:

- DescriptionScroll существует;
- DialogScroll существует;
- DescriptionText существует и является MultilineTextWidgetClass;
- DialogText существует и является MultilineTextWidgetClass.

Не нужно делать большой redesign viewer.
Не нужно пытаться симулировать runtime clipping.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. DescriptionContent удалён.
3. DialogContent удалён.
4. scriptclass "SizeToChild" удалён из QuestMenu.layout.
5. DescriptionText снова прямой ребёнок DescriptionScroll.
6. DialogText снова прямой ребёнок DialogScroll.
7. DescriptionText остался MultilineTextWidgetClass.
8. DialogText остался MultilineTextWidgetClass.
9. DescriptionPanel position/size не изменились.
10. DialogPanel position/size не изменились.
11. RoutePanel position/size не изменились.
12. QuestListbox position/size не изменились.
13. TriggerRouteListbox position/size не изменились.
14. AcceptButton/CompleteButton/CloseButton position/size не изменились.
15. RichTextWidget не используется.
16. MultilineEditBoxWidget не используется.
17. HtmlWidget не используется.
18. TextListboxWidget для Description/Dialog не возвращён.
19. QuestUI.c не менялся или остался совместимым.
20. JSON не менялся.
21. Server не менялся.
22. QuestJournal не менялся.
23. @Trader не менялся.
24. DayZ_layout viewer проверен и минимально приведён к baseline sanity.
25. Кириллица не повреждена.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout и возможно viewer html.

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

1. QuestMenu возвращён к рабочей baseline-структуре.
2. Description/Dialog снова имеют прямой стек:
   - ScrollWidget -> MultilineTextWidget
3. ContentParent / SizeToChild удалены.
4. Геометрия основных панелей не менялась.
5. QuestUI.c не сломан.
6. Viewer sanity-check больше не ждёт TASK 098 stack.
7. Агент вернул отчёт в чат.
8. В PROBLEMS зафиксировано, что clipping остаётся отдельной нерешённой проблемой.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что откатили;
- удалён ли DescriptionContent;
- удалён ли DialogContent;
- удалён ли SizeToChild;
- вернулся ли direct stack ScrollWidget -> MultilineTextWidget;
- менялся ли viewer;
- трогался ли QuestUI.c.

CHANGED FILES:
- QuestMenu.layout
- dayz_layout_viewer.html, если менялся
- QuestUI.c, только если реально менялся

DIFF:
- кратко описать изменения layout;
- кратко описать изменения viewer, если были.

BASELINE GEOMETRY CHECK:
- подтвердить, что позиции и размеры DescriptionPanel/DialogPanel/RoutePanel/QuestListbox/buttons не изменились.

COMPILE-SAFETY CHECK:
- DescriptionText/DialogText остались MultilineTextWidget;
- QuestUI.c cast mismatch отсутствует;
- scroll API только на ScrollWidget.

DAYZ_LAYOUT CHECK:
- viewer больше не ожидает RichText stack;
- viewer больше не ожидает TASK 098 ContentParent stack;
- viewer проверяет baseline stack.

RUNTIME EXPECTATION:
- QuestMenu должен вернуться к рабочей геометрии baseline;
- Description/Dialog должны быть видимы;
- wrap и scrollbar должны работать;
- clipping-проблема может остаться.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- clipping остаётся нерешённым;
- ContentParent + SizeToChild признан неудачным тестом для текущего QuestMenu.

RECOMMENDED NEXT TASK:
- аналитика без правок: почему ScrollWidget viewport не клипит MultilineTextWidget и есть ли runtime/widget flags для clipping;
- либо отдельный тест другого widget-типа только после разрешения.

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

TASK 098 REVIEW

Статус:
Принято условно.

Что принято:
- Description/Dialog остались на MultilineTextWidget.
- RichTextWidget не используется.
- MultilineEditBoxWidget не используется.
- TextListboxWidget для Description/Dialog не возвращён.
- QuestUI.c не трогался.
- Scroll API остаётся только на ScrollWidget.
- Базовые панели и кнопки по отчёту не сдвигались.
- Внутри DescriptionScroll добавлен DescriptionContent.
- Внутри DialogScroll добавлен DialogContent.
- Viewer обновлён под текущий тестовый stack.

Что теперь надо проверить в игре:
1. Компилируется ли layout с `scriptclass "SizeToChild"`.
2. QuestMenu открывается без runtime error.
3. Окна остались на местах.
4. Description текст виден.
5. Dialog текст виден.
6. Scrollbar работает.
7. Wrap сохранился.
8. Текст больше не выходит за границы DescriptionScroll.
9. Текст больше не выходит за границы DialogScroll.
10. Кнопки работают.
11. RoutePanel работает.
12. QuestListbox работает.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
