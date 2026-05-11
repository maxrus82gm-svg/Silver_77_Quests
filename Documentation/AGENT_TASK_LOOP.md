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

TASK 100 — Анализ: почему ScrollWidget прокручивает MultilineTextWidget за границы окна в QuestMenu

Статус:
Глубокая аналитическая задача для другого агента. Ничего не менять.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

В QuestMenu есть рабочий, играбельный baseline.

Текущий runtime-подтверждённый рабочий baseline:

178d980b8a1cc2897df69b29052c6fde65e01d78

Это состояние считается текущим безопасным состоянием QuestMenu.

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
10. QuestListbox и TriggerRouteListbox работают.
11. Меню играбельно.

Текущий рабочий стек Description/Dialog:

Description:
ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Dialog:
ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Текущие важные размеры:

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

Оставшаяся проблема:

При прокрутке текст Description/Dialog может выходить за границы своего scroll-окна и визуально накладываться на другие области.

Важно:
это не ломает всю геометрию baseline, но является неправильным поведением clipping/viewport.

--------------------------------------------------------------------------------
ИСТОРИЯ ЭКСПЕРИМЕНТОВ
--------------------------------------------------------------------------------

Нужно учитывать уже проведённые попытки.

1. TextListboxWidget

Ранее Description/Dialog были переведены на TextListboxWidget.

Результат:
- scroll работал хорошо;
- clipping был лучше;
- но абзацный текст выглядел плохо;
- пришлось делать ручной перенос строк;
- maxCharsPerLine / weighted-wrap были неудобны и неадаптивны;
- это не финальное решение для нормального текста.

Вывод:
TextListboxWidget подходит для списков, но плохо подходит для Description/Dialog как для обычного текста.

2. ScrollWidget + MultilineTextWidget с "size to text v" 1

Была попытка:

ScrollWidgetClass
  MultilineTextWidgetClass

с маленьким размером текста и `"size to text v" 1`.

Результат:
- QuestMenu открывался;
- но текст Description/Dialog был не виден или почти не виден.

Предположительная причина:
MultilineTextWidget внутри ScrollWidget не получил корректную высоту контента или был обрезан до высоты 1.

3. ScrollWidget + MultilineTextWidget с фиксированной высотой

Был сделан hotfix:

DescriptionText size 394 600
DialogText size 394 300
"size to text v" 0

Результат:
- текст появился;
- native wrap заработал;
- scrollbar заработал;
- layout стал играбельным;
- но текст при прокрутке выходит за границы scroll-окна.

Это текущий рабочий baseline.

4. Wrapper через PanelWidget / Content wrapper

Была попытка добавить промежуточный wrapper:

ScrollWidget
  PanelWidget Content
    MultilineTextWidget

Результат:
- геометрия QuestMenu разъехалась;
- элементы начали визуально накладываться;
- layout стал хуже.

Вывод:
этот wrapper-подход не подходит в текущем виде.

5. RichTextWidget stack

Была попытка:

ScrollWidget
  GridSpacerWidgetClass / Size-To-Content container
    RichTextWidgetClass

Результат:
- подход выглядел логичным по аналитике;
- viewer был обновлён под RichTextWidget;
- но направление не стало текущим рабочим baseline;
- было решено откатиться к рабочему MultilineTextWidget baseline.

Вывод:
RichTextWidget пока не считается проверенным стабильным решением для QuestMenu.

6. ContentParent + SizeToChild

Была попытка:

ScrollWidget
  FrameWidgetClass ContentParent
    MultilineTextWidget

с `scriptclass "SizeToChild"`.

Результат runtime:
- QuestMenu снова разъехался;
- Dialog уехал вниз;
- кнопки начали пересекаться с Dialog;
- scrollbars визуально остались в своих областях, но content ощущался как будто живёт в другом coordinate space;
- пользователь отметил, что “скролл работает слева, хотя само окно по центру снизу”, то есть scrollbar и content ощущаются несогласованными.

Вывод:
ContentParent + SizeToChild в текущем QuestMenu ломает coordinate space / layout и не подходит.

7. Откат TASK 099

После отката ContentParent + SizeToChild QuestMenu вернулся в нормальный играбельный вид.

Runtime подтверждён:
- всё вернулось в нормальный вид;
- меню играбельно;
- clipping остаётся нерешённым.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 100
--------------------------------------------------------------------------------

Провести глубокую аналитику:

Почему в текущем рабочем QuestMenu `ScrollWidget` не ограничивает видимость `MultilineTextWidget` внутри своих границ?

Нужно понять:

1. Почему `clipchildren 1` на `ScrollWidget` не даёт ожидаемый clipping.
2. Является ли это особенностью `MultilineTextWidget`.
3. Является ли это особенностью `ScrollWidget`.
4. Является ли это следствием фиксированной высоты `MultilineTextWidget` 600 / 300.
5. Является ли это следствием `style blank`.
6. Является ли это следствием `vexactsize 1`.
7. Нужно ли включать clipping не через layout, а через script / widget flags.
8. Есть ли в DayZ/Enfusion API методы или flags для runtime clipping.
9. Можно ли решить проблему без изменения текущей рабочей геометрии.
10. Какие варианты безопасны для будущей практической задачи.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Это аналитическая задача.

Агент НЕ должен менять:

- QuestMenu.layout
- QuestUI.c
- dayz_layout_viewer.html
- JSON
- server
- QuestJournal
- @Trader
- Documentation
- любые .c
- любые .layout
- любые .json
- любые .html
- любые PBO

Агент должен только читать, анализировать и вернуть отчёт в чат.

Запрещено:

- делать git commit
- делать git push
- делать git reset
- делать git clean
- делать git checkout
- перепаковывать PBO
- запускать Addon Builder
- исправлять “заодно”
- менять layout
- менять script
- менять viewer
- менять JSON
- менять server
- предлагать сразу практический фикс без анализа рисков
- снова предлагать wrapper как первый вариант без объяснения, почему прежние wrapper-подходы ломались

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ
--------------------------------------------------------------------------------

Обязательно прочитать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
6. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Прочитать текущую рабочую базу:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Можно читать, но не менять:

9. P:\Silver_77_Quests\DayZ_layout\dayz_layout_viewer.html
10. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
11. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
12. P:\Silver_77_Quests\@Trader\

Обязательно проверить vanilla/reference на диске D, если доступны:

13. D:\Dayz\scripts\1_Core\proto\enwidgets.c
14. D:\Dayz\scripts\3_game\gui\
15. D:\Dayz\scripts\5_mission\gui\
16. D:\Dayz\gui\layouts\
17. D:\Dayz\gui\layouts\new_ui\
18. D:\Dayz\gui\layouts\script_console\
19. D:\Dayz\gui\layouts\inventory_new\

Особенно искать в:

- enwidgets.c
- sizetochild.c
- scriptconsoleuniversalinfodialog.c
- bookmenu.c
- leftarea.c
- options/menu scripts
- inventory/new UI scripts
- любые scripts/layouts, где ScrollWidget реально клипит контент

--------------------------------------------------------------------------------
ЧТО ИСКАТЬ
--------------------------------------------------------------------------------

Искать по словам:

- ScrollWidget
- ScrollWidgetClass
- MultilineTextWidget
- MultilineTextWidgetClass
- clipchildren
- ClipChildren
- clipping
- viewport
- WidgetFlags
- SetFlags
- ClearFlags
- GetFlags
- SetClip
- SetClipChildren
- SetSort
- Update
- SetSize
- GetSize
- GetScreenSize
- GetScreenPos
- VScrollToPos
- VScrollToPos01
- VScrollStep
- GetVScrollPos
- GetVScrollPos01
- GetContentHeight
- SetContentOffset
- GetContentOffset
- style blank
- style None
- style Empty
- vexactsize
- hexactsize
- "size to text v"
- "Scrollbar V"
- ignorepointer
- FrameWidgetClass
- PanelWidgetClass
- MultilineEditBoxWidgetClass
- RichTextWidgetClass
- HtmlWidget
- SizeToChild
- ScrollbarWidget
- WidgetFlags.CLIPCHILDREN
- WidgetFlags

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ВЫЯСНИТЬ
--------------------------------------------------------------------------------

1. Текущий baseline

В отчёте явно подтвердить:

- текущий runtime-подтверждённый baseline: 178d980b8a1cc2897df69b29052c6fde65e01d78;
- рабочая структура Description/Dialog;
- текущие размеры;
- что сейчас работает;
- что сейчас не работает.

2. Почему wrapper-подходи ломались

Проанализировать, почему:

- PanelWidget wrapper из раннего теста разъезжал layout;
- FrameWidget ContentParent + SizeToChild разъезжал layout;
- GridSpacer/RichText не стал текущим baseline;
- почему insertion wrapper может менять coordinate space / content bounds / scroll origin.

Нужно объяснить это максимально технически, но с пометкой, где вывод подтверждён reference, а где это гипотеза.

3. Почему ScrollWidget не клипит MultilineTextWidget

Проверить:

- что реально означает `clipchildren 1` в .layout;
- работает ли `clipchildren` для render clipping или только для hit-test/children bounds;
- есть ли отличие clipping для TextWidget / MultilineTextWidget / RichTextWidget / EditBox;
- может ли MultilineTextWidget рендериться вне parent viewport;
- влияет ли `ignorepointer`;
- влияет ли `style blank`;
- влияет ли `style None`;
- влияет ли `vexactsize 1`;
- влияет ли `"size to text v" 0`;
- влияет ли большая высота child widget;
- влияет ли то, что child прямой потомок ScrollWidget;
- должен ли ScrollWidget иметь специальный content widget;
- должен ли ScrollWidget работать только с определёнными child classes.

4. Runtime/script clipping

Проверить API и vanilla code:

- есть ли у Widget методы для flags;
- есть ли WidgetFlags для clipping;
- есть ли runtime-способ включить clipping;
- есть ли примеры `SetFlags(...)` или похожих вызовов;
- можно ли вызвать clipping flags на:
  - DescriptionScroll;
  - DialogScroll;
  - DescriptionPanel;
  - DialogPanel;
  - DescriptionText;
  - DialogText;
- насколько это безопасно;
- может ли это решить проблему без изменения layout-вложенности.

5. Vanilla/reference patterns

Найти и сравнить реальные DayZ patterns:

A. ScrollWidget с простым content child.
B. ScrollWidget с FrameWidget/Panel content parent.
C. ScrollWidget с MultilineEditBoxWidget.
D. ScrollWidget с HtmlWidget.
E. ScrollWidget с listbox.
F. MultilineTextWidget без scroll, но с clipping.

Для каждого найденного примера указать:

- файл layout;
- файл script, если есть;
- widget stack;
- clipping/size/style flags;
- используется ли script для обновления size/content;
- применимость к QuestMenu;
- риск для рабочей геометрии.

6. DayZ_layout viewer

Проверить отдельно:

- viewer сейчас согласован с baseline или нет;
- viewer не моделирует runtime clipping;
- нужно ли в будущем улучшать viewer, чтобы он показывал scroll viewport/mask;
- нужно ли сейчас что-то менять в viewer — ответ должен быть “нет”, так как задача аналитическая.

7. Варианты будущего решения

Сравнить варианты:

Option A:
Оставить текущий layout, но включить clipping через runtime flags/script.

Option B:
Оставить текущий layout, но изменить только style/clipchildren/vexactsize свойства без wrapper.

Option C:
Сделать минимальный invisible mask/clip parent, но не менять scroll-content coordinate space.

Option D:
Вернуться к TextListboxWidget как fallback.

Option E:
Перейти на MultilineEditBoxWidget по vanilla pattern.

Option F:
Использовать HtmlWidget/book-like pattern.

Для каждого варианта указать:

- что менять;
- плюсы;
- минусы;
- риск сломать рабочую геометрию;
- риск compile error;
- риск runtime error;
- насколько решение похоже на vanilla;
- что нужно проверить в игре.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если агент вернул аналитический отчёт, где есть:

1. Подтверждение baseline `178d980b8a1cc2897df69b29052c6fde65e01d78`.
2. Подробное объяснение текущей проблемы.
3. Анализ, почему wrapper-подходи ломали layout.
4. Проверка `enwidgets.c` / WidgetFlags / clipping API.
5. Проверка vanilla/reference.
6. Сравнение минимум 5 будущих вариантов.
7. Чёткая рекомендация самого безопасного следующего TASK.
8. Подтверждение, что файлы не менялись.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что было прочитано;
- какие reference paths проверены;
- какие vanilla examples найдены.

WORKING BASELINE:
- base commit;
- текущая структура Description/Dialog;
- текущие размеры;
- что работает;
- что не работает.

WHY WRAPPERS BROKE LAYOUT:
- анализ PanelWidget wrapper;
- анализ ContentParent + SizeToChild;
- анализ coordinate space / scroll origin гипотезы.

CURRENT CLIPPING ANALYSIS:
- что делает clipchildren;
- почему ScrollWidget может не клипить MultilineTextWidget;
- роль style / vexactsize / child size / size-to-text;
- подтверждённые факты и гипотезы.

SCRIPT / WIDGET FLAGS FINDINGS:
- найденные методы/flags;
- можно ли включить clipping в runtime;
- где это можно применить;
- риски.

VANILLA / REFERENCE FINDINGS:
- найденные примеры;
- widget stack;
- clipping/size/style pattern;
- применимость к QuestMenu.

DAYZ_LAYOUT VIEWER CHECK:
- что viewer показывает правильно;
- что viewer не умеет;
- нужно ли обновлять viewer в будущем.

OPTIONS:
- Option A;
- Option B;
- Option C;
- Option D;
- Option E;
- Option F;
- плюсы/минусы/риски.

RECOMMENDATION:
- какой самый безопасный следующий TASK;
- какие файлы менять в будущей практической задаче;
- чего не делать.

CHANGED FILES:
- должно быть: none.

PROBLEMS:
- что не удалось подтвердить.

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
