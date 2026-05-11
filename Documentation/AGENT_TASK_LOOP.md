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

Текущий runtime-подтверждённый рабочий baseline QuestMenu:

178d980b8a1cc2897df69b29052c6fde65e01d78

В этом baseline:

1. QuestMenu визуально целый.
2. DescriptionPanel находится на правильном месте.
3. DialogPanel находится на правильном месте.
4. RoutePanel находится на правильном месте.
5. QuestListbox находится на правильном месте.
6. Кнопки находятся на правильном месте.
7. Description/Dialog текст виден.
8. Native wrap работает.
9. Scrollbar работает.
10. Меню играбельно.

Оставшаяся проблема:

При прокрутке `DescriptionText` / `DialogText` текст может выходить за границы scroll-окна.

После аналитики решено НЕ продолжать борьбу со scroll clipping сейчас.

Новое направление:

- отказаться от прокрутки в Description/Dialog;
- увеличить текстовые окна по вертикали примерно на треть;
- сделать компоновку аккуратной и пропорциональной;
- сохранить центральную основную область QuestMenu;
- добиться, чтобы обычный квестовый текст помещался без scroll.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 101
--------------------------------------------------------------------------------

Пропорционально увеличить вертикальное пространство для Description/Dialog и отключить scrollbars.

Главная идея:

Было:
- небольшие Description/Dialog окна;
- ScrollWidget со scrollbar;
- текст прокручивается, но overflow работает плохо.

Должно стать:
- Description/Dialog окна выше примерно на 30–35%;
- текстовые области выше;
- scrollbar отключён;
- игрок в обычном случае читает текст без прокрутки;
- QuestMenu остаётся аккуратным и центрированным.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Агент обязан:

- делать только UI layout adjustment;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не менять quest logic;
- не менять Offer / Completion / Reward;
- не трогать server;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать git commit / push / reset / clean;
- не запускать Addon Builder;
- не перепаковывать PBO;
- не делать новых scroll-clipping экспериментов;
- не добавлять wrapper;
- не добавлять mask;
- не использовать SizeToChild;
- не использовать RichTextWidget;
- не использовать MultilineEditBoxWidget;
- не использовать HtmlWidget;
- не возвращать TextListboxWidget для Description/Dialog.

Запрещено:

- добавлять `DescriptionContent`;
- добавлять `DialogContent`;
- добавлять `scriptclass "SizeToChild"`;
- менять `DescriptionText` / `DialogText` на другой widget class;
- менять QuestUI.c без крайней необходимости;
- менять JSON;
- менять server;
- менять QuestJournal;
- менять @Trader;
- менять DayZ_layout viewer больше минимально необходимого;
- делать большой redesign QuestMenu.

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

Разрешено менять:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

Разрешено менять только если реально нужно из-за отключения scroll:

2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Разрешено менять только минимально для sanity-check под новый layout:

3. P:\Silver_77_Quests\DayZ_layout\dayz_layout_viewer.html

Если QuestUI.c можно не трогать — не трогать.

Если viewer и так показывает структуру достаточно — не трогать.

--------------------------------------------------------------------------------
ТЕКУЩИЙ BASELINE, ОТ КОТОРОГО ОТТАЛКИВАЕМСЯ
--------------------------------------------------------------------------------

Текущие важные размеры baseline:

DescriptionPanel:
- position 300 74
- size 430 204

DescriptionScroll:
- position 8 34
- size 414 156
- "Scrollbar V" 1

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
- "Scrollbar V" 1

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

1. Пропорционально увеличить DescriptionPanel по вертикали

Текущий baseline:

DescriptionPanel:
- position 300 74
- size 430 204

Нужно увеличить высоту примерно на треть.

Ориентир:

DescriptionPanel:
- position 300 74
- size 430 примерно 270

Важно:
- X position не менять без необходимости.
- Width не менять без необходимости.
- Верх DescriptionPanel желательно оставить на месте.
- Увеличение должно идти вниз.
- Компоновка должна остаться аккуратной и центрированной.

2. Увеличить DescriptionScroll / видимую текстовую область

Текущий baseline:

DescriptionScroll:
- position 8 34
- size 414 156

Нужно увеличить высоту пропорционально панели.

Ориентир:

DescriptionScroll:
- position 8 34
- size 414 примерно 220

Scrollbar отключить:

- заменить `"Scrollbar V" 1` на `"Scrollbar V" 0`, если это корректный layout-синтаксис;
- либо удалить/отключить scrollbar тем способом, который уже используется в DayZ layout;
- в отчёте указать точный способ отключения.

3. Настроить DescriptionText под режим без scroll

DescriptionText должен остаться:

- MultilineTextWidgetClass
- имя DescriptionText
- wrap 1
- font "gui/fonts/metron16"
- текущий цвет
- text halign left
- text valign top

Высоту DescriptionText сделать примерно равной видимой области DescriptionScroll, а не 600.

Ориентир:

DescriptionText:
- position 0 0
- size 394 примерно 220

Рассмотреть безопасное изменение:

- `"size to text v" 0` оставить, если это стабильнее;
- либо `"size to text v" 1` только если это не ломает видимость текста и подтверждено reference.

Предпочтение:
- для первого теста оставить `"size to text v" 0`;
- сделать фиксированную высоту текста равной новой видимой области.

4. Пропорционально увеличить DialogPanel по вертикали

Текущий baseline:

DialogPanel:
- position 300 286
- size 430 122

Нужно увеличить высоту примерно на треть или больше, насколько позволяет нижняя зона.

Ориентир:

DialogPanel:
- position ниже DescriptionPanel с аккуратным отступом;
- size 430 примерно 160–180

Важно:
- DialogPanel должна быть ниже DescriptionPanel;
- не должна пересекаться с DescriptionPanel;
- не должна пересекаться с кнопками;
- общая центральная колонка должна выглядеть пропорционально.

5. Увеличить DialogScroll / видимую текстовую область

Текущий baseline:

DialogScroll:
- position 8 34
- size 414 74

Нужно увеличить высоту.

Ориентир:

DialogScroll:
- position 8 34
- size 414 примерно 115–135

Scrollbar отключить:

- заменить `"Scrollbar V" 1` на `"Scrollbar V" 0`, если это корректно;
- либо удалить/отключить scrollbar безопасным способом;
- в отчёте указать точный способ.

6. Настроить DialogText под режим без scroll

DialogText должен остаться:

- MultilineTextWidgetClass
- имя DialogText
- wrap 1
- font "gui/fonts/metron16"
- текущий цвет
- text halign left
- text valign top

Высоту DialogText сделать примерно равной видимой области DialogScroll, а не 300.

Ориентир:

DialogText:
- position 0 0
- size 394 примерно 115–135

Предпочтение:
- оставить `"size to text v" 0`;
- использовать фиксированную высоту текста равную новой видимой области.

7. Сдвинуть нижние кнопки вниз пропорционально

Так как Description/Dialog станут выше, кнопки нужно аккуратно сдвинуть ниже.

Текущий baseline:

AcceptButton:
- position 300 420
- size 210 48

CompleteButton:
- position 530 420
- size 210 48

CloseButton:
- position 300 490
- size 650 48

Нужно:
- сохранить горизонтальное расположение;
- сохранить размеры кнопок;
- сдвинуть кнопки вниз так, чтобы они не пересекались с DialogPanel;
- сохранить визуальные отступы.

Ориентир:
- AcceptButton / CompleteButton можно поставить примерно после DialogPanel + 12–18 px.
- CloseButton можно поставить ниже Accept/Complete примерно на 65–70 px.

Если нижняя граница QuestMenu не позволяет:
- допускается немного увеличить высоту root/container меню;
- но делать это аккуратно и описать в отчёте.

8. RoutePanel

RoutePanel сейчас:

- position 740 74
- size 210 334

Нужно проверить, не нарушилась ли визуальная высота относительно новой центральной колонки.

Разрешено:
- не менять RoutePanel, если он визуально остаётся нормальным;
- слегка увеличить RoutePanel по высоте вниз, если это улучшает пропорцию и не ломает layout.

Запрещено:
- менять RoutePanel по X;
- переносить RoutePanel в другое место;
- менять TriggerRouteListbox без необходимости.

9. QuestListbox

QuestListbox сейчас:

- position 30 82
- size 250 450

Не менять, если не требуется.

QuestListbox / список квестов должен остаться стабильным.

10. QuestUI.c

Так как scroll отключается, проверить:

- не вызывает ли ResetScrollWidgetToTop(...) ошибок при отключённом scrollbar;
- если ScrollWidget остаётся как container, QuestUI.c можно не менять;
- если ScrollWidget убирается, QuestUI.c пришлось бы менять, но в этой задаче НЕ нужно убирать ScrollWidget.

Предпочтение:

- оставить ScrollWidgetClass как контейнер;
- просто отключить scrollbar;
- QuestUI.c не трогать.

11. DayZ_layout viewer

Проверить viewer по правилу SplitDoc.

Если sanity-check завязан на старые размеры и ругается:
- минимально обновить expected sizes / checks;
- не переписывать viewer;
- не моделировать clipping;
- viewer должен отражать новую структуру без scrollbars.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. DescriptionPanel увеличен по высоте примерно на треть.
3. DescriptionScroll увеличен по высоте.
4. DescriptionText уменьшен с 600 до высоты новой видимой области.
5. `"Scrollbar V"` для DescriptionScroll отключён.
6. DialogPanel увеличен по высоте примерно на треть или больше.
7. DialogScroll увеличен по высоте.
8. DialogText уменьшен с 300 до высоты новой видимой области.
9. `"Scrollbar V"` для DialogScroll отключён.
10. AcceptButton / CompleteButton не пересекаются с DialogPanel.
11. CloseButton не пересекается с AcceptButton / CompleteButton.
12. QuestListbox не сломан.
13. RoutePanel не сломан.
14. DescriptionText/DialogText остались MultilineTextWidgetClass.
15. RichTextWidget не используется.
16. MultilineEditBoxWidget не используется.
17. HtmlWidget не используется.
18. TextListboxWidget для Description/Dialog не возвращён.
19. ContentParent / SizeToChild не добавлены.
20. QuestUI.c не менялся или остался совместимым.
21. JSON не менялся.
22. Server не менялся.
23. QuestJournal не менялся.
24. @Trader не менялся.
25. DayZ_layout viewer проверен.
26. Кириллица не повреждена.

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

1. Description/Dialog визуально увеличены по вертикали.
2. Scrollbar для Description/Dialog отключён.
3. DescriptionText/DialogText остались MultilineTextWidgetClass.
4. Рабочая центральная компоновка сохранена.
5. Кнопки не пересекаются с DialogPanel.
6. QuestListbox и RoutePanel остались рабочими.
7. Новые wrapper/mask/SizeToChild не добавлены.
8. QuestUI.c не сломан.
9. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что увеличено;
- какие scrollbars отключены;
- менялся ли QuestUI.c;
- менялся ли viewer.

CHANGED FILES:
- QuestMenu.layout
- QuestUI.c, если менялся
- dayz_layout_viewer.html, если менялся

DIFF:
- старые и новые размеры DescriptionPanel/DescriptionScroll/DescriptionText;
- старые и новые размеры DialogPanel/DialogScroll/DialogText;
- старые и новые позиции кнопок;
- изменения viewer, если были.

LAYOUT BALANCE CHECK:
- не пересекаются ли DescriptionPanel и DialogPanel;
- не пересекаются ли DialogPanel и кнопки;
- не пересекается ли CloseButton;
- QuestListbox на месте;
- RoutePanel на месте;
- центральная колонка выглядит пропорционально.

COMPILE-SAFETY CHECK:
- DescriptionText/DialogText остались MultilineTextWidget;
- ScrollWidget остался container;
- QuestUI.c совместим;
- новых widget class нет.

DAYZ_LAYOUT CHECK:
- viewer проверен;
- нужно ли было обновлять expected sizes;
- ограничения viewer.

RUNTIME EXPECTATION:
- Description/Dialog должны вместить больше текста без прокрутки;
- scrollbars не должны отображаться;
- clipping-проблема должна стать неактуальной для обычных текстов;
- очень длинный текст всё ещё может не поместиться, это отдельное ограничение.

ENCODING CHECK:
- BOM не появился;
- кириллица не повреждена.

PROBLEMS:
- что не удалось проверить без игры;
- есть ли риск, что очень длинные тексты не поместятся.

RECOMMENDED NEXT TASK:
- если runtime хороший — принять layout baseline;
- если текста всё ещё мало — точечно увеличить окна ещё немного;
- если исчезновение scrollbars вызовет проблемы — вернуть baseline.

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
