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

TASK 093 — QuestMenu: ограничить ScrollWidget text внутри панелей и уменьшить шаг прокрутки

Статус:
UI polish / hotfix после runtime-проверки TASK 092.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Перед выполнением задачи агент обязан прочитать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
6. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Агент обязан соблюдать принцип жёстких рамок:

- делать только настройку ScrollWidget + MultilineTextWidget для Description/Dialog;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не трогать server;
- не трогать Quest Editor;
- не трогать QuestJournal;
- не трогать @Trader;
- не возвращать TextListboxWidget;
- не переходить на RichTextWidget;
- не переходить на MultilineEditBoxWidget;
- не делать большой редизайн QuestMenu;
- не делать git commit / push / reset / clean.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 092 текст Description/Dialog снова появился.

Это подтвердило, что связка:

ScrollWidgetClass
  MultilineTextWidgetClass

в целом работает.

Положительный результат:
- native wrap работает;
- текст появился;
- scroll выглядит как родной DayZ scroll.

Но runtime-проверка показала новые проблемы:

1. Текст выходит за рамки своих областей.
   При прокрутке Description/Dialog текст визуально уходит поверх других частей меню:
   - выше своей панели;
   - поверх заголовков;
   - поверх кнопок;
   - иногда выглядит так, будто текст не клипается внутри окна ScrollWidget.

2. Шаг прокрутки слишком большой.
   Scroll двигает текст слишком резко / дискретно.
   Нужно сделать прокрутку чувствительнее, чтобы текст двигался меньшими шагами.

Текущий layout после TASK 092 примерно такой:

DescriptionScroll size 414 156
  DescriptionText size 394 600

DialogScroll size 414 74
  DialogText size 394 300

Вероятная причина проблемы:
- внутренний MultilineTextWidget получил большую высоту, но ScrollWidget/родительский контейнер не клипает его визуально;
- возможно, clipchildren нужно включить/настроить на правильных родителях;
- возможно, нужен дополнительный внутренний content-контейнер внутри ScrollWidget;
- возможно, требуется другое свойство ScrollWidget / child widget для корректного clipping.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 093
--------------------------------------------------------------------------------

Сохранить рабочую архитектуру ScrollWidget + MultilineTextWidget, но довести её до нормального отображения:

1. DescriptionText должен быть виден только внутри области DescriptionScroll.
2. DialogText должен быть виден только внутри области DialogScroll.
3. Текст не должен выходить поверх заголовков, кнопок, RoutePanel или других элементов.
4. Native wrap должен сохраниться.
5. ScrollWidget должен сохраниться.
6. Шаг прокрутки должен стать меньше / чувствительнее, если это возможно безопасно.
7. QuestListbox и TriggerRouteListbox не трогать.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Сначала нужно решить clipping.

Предпочтительный порядок проверки:

1. Проверить clipchildren у:
   - DescriptionPanel
   - DescriptionScroll
   - DescriptionText
   - DialogPanel
   - DialogScroll
   - DialogText

2. Убедиться, что ScrollWidget реально является clipping container.

3. Если одного clipchildren недостаточно, попробовать паттерн:

DescriptionPanel
  ScrollWidgetClass DescriptionScroll
    PanelWidgetClass DescriptionContent
      MultilineTextWidgetClass DescriptionText

DialogPanel
  ScrollWidgetClass DialogScroll
    PanelWidgetClass DialogContent
      MultilineTextWidgetClass DialogText

Где:
- ScrollWidget имеет видимую область и scrollbar;
- Content/Panel имеет большую высоту;
- MultilineTextWidget находится внутри Content;
- clipping должен происходить на уровне ScrollWidget.

4. Если DayZ ScrollWidget требует content-wrapper типа:
   - WrapSpacerWidgetClass
   - GridSpacerWidgetClass
   - PanelWidgetClass

то использовать минимальный подход, который ближе к vanilla/reference.

5. Не возвращать TextListboxWidget.
6. Не переходить на RichTextWidget/EditBox в этой задаче.

По шагу прокрутки:

Нужно найти безопасный способ уменьшить scroll step.

Проверить:
- есть ли layout-свойство у ScrollWidget для step/speed;
- есть ли script API для более мелкого scroll;
- можно ли перехватить OnMouseWheel для DescriptionScroll/DialogScroll и делать меньший шаг через ScrollWidget;
- не будет ли это конфликтовать с native scroll.

Если безопасного способа быстро нет:
- не ломать scroll;
- описать в PROBLEMS;
- предложить отдельную задачу на scroll sensitivity.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это только UI polish текущей ScrollWidget + MultilineTextWidget схемы.

Агент должен:

1. Исправить clipping DescriptionText внутри DescriptionScroll.
2. Исправить clipping DialogText внутри DialogScroll.
3. Сохранить native wrap.
4. Сохранить ScrollWidget.
5. Проверить возможность уменьшить шаг прокрутки.
6. Если возможно безопасно — уменьшить шаг прокрутки.
7. Проверить DayZ_layout viewer compatibility.
8. Вернуть отчёт в чат.

Агент не должен:

- менять QuestJournal;
- менять JSON;
- менять server;
- менять RPC/sync;
- менять Quest Editor;
- менять quest logic;
- менять Offer / Completion / Reward архитектуру;
- менять @Trader;
- менять QuestListbox;
- менять TriggerRouteListbox;
- возвращать TextListboxWidget для Description/Dialog;
- переходить на RichTextWidget;
- переходить на MultilineEditBoxWidget;
- делать большой редизайн QuestMenu;
- делать commit/push/reset/clean;
- перепаковывать PBO;
- запускать Addon Builder.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

QuestUI.c менять только если нужно:
- настроить scroll reset;
- уменьшить шаг прокрутки;
- обработать OnMouseWheel для DescriptionScroll/DialogScroll;
- исправить явную проблему с SetText/Update/reset.

Дополнительно разрешено менять только если DayZ_layout viewer перестал корректно показывать актуальный QuestMenu.layout:

3. P:\Silver_77_Quests\DayZ_layout\*

DayZ_layout менять только минимально:
- поддержка content-wrapper внутри ScrollWidget;
- preview clipping/scroll-зоны;
- отображение ScrollWidget + MultilineTextWidget.

Если viewer уже показывает достаточно корректно:
- DayZ_layout не менять;
- указать это в отчёте.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ И ПАПКИ ДЛЯ ЧТЕНИЯ
--------------------------------------------------------------------------------

Можно читать:

1. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
5. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
6. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Можно читать текущий UI:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Можно читать для reference, но не менять:

9. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
10. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
11. P:\Silver_77_Quests\@Trader\
12. P:\Silver_77_Quests\DayZ_layout\

Если есть доступ к vanilla/extracted DayZ layouts/scripts — можно читать как reference, но не менять.

Искать по словам:

- ScrollWidgetClass
- clipchildren
- WrapSpacerWidgetClass
- GridSpacerWidgetClass
- PanelWidgetClass
- Scrollbar V
- VScrollStep
- VScrollToPos01
- GetVScrollPos01
- OnMouseWheel
- ContentScroll
- MultilineTextWidgetClass
- size to text v
- wrap

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
- переход на RichTextWidget;
- переход на MultilineEditBoxWidget;
- возврат к TextListboxWidget без отдельной команды пользователя.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Проверить clipping в QuestMenu.layout

Проверить:

- DescriptionPanel
- DescriptionScroll
- DescriptionText
- DialogPanel
- DialogScroll
- DialogText

Нужно добиться:
- текст DescriptionText не рисуется за пределами DescriptionScroll;
- текст DialogText не рисуется за пределами DialogScroll.

2. Исправить структуру внутри ScrollWidget, если нужно

Если текущая структура:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

не клипает child-текст, попробовать добавить content wrapper:

ScrollWidgetClass DescriptionScroll
  PanelWidgetClass DescriptionContent
    MultilineTextWidgetClass DescriptionText

ScrollWidgetClass DialogScroll
  PanelWidgetClass DialogContent
    MultilineTextWidgetClass DialogText

Или использовать другой vanilla-compatible content wrapper, если reference показывает лучший вариант.

Требования:
- ScrollWidget остаётся видимой областью;
- content-wrapper имеет большую высоту;
- MultilineTextWidget находится внутри wrapper;
- wrapper/text не должны рисоваться за пределами scroll viewport.

3. Настроить размеры

Учитывать scrollbar:

- DescriptionScroll width 414;
- внутренний текст может быть около 394 width;
- DescriptionContent height должен быть достаточным для scroll;
- DialogContent height должен быть достаточным для scroll.

Не делать высоту слишком огромной без необходимости.
Если фиксированная высота остаётся единственным рабочим вариантом:
- оставить разумные значения;
- указать риск в PROBLEMS.

4. Уменьшить шаг прокрутки, если возможно

Проверить безопасные варианты:

A. Layout-свойство ScrollWidget для шага прокрутки, если существует.

B. OnMouseWheel в QuestUI.c:
- если wheel пришёл на DescriptionScroll или DescriptionText — прокручивать DescriptionScroll меньшим шагом;
- если wheel пришёл на DialogScroll или DialogText — прокручивать DialogScroll меньшим шагом;
- не вызывать scroll API на MultilineTextWidget;
- не ломать обычный scroll.

C. Использовать ScrollWidget API:
- GetVScrollPos01
- VScrollToPos01
- VScrollStep

только если это безопасно и компилируется.

Важно:
- не возвращать старые ошибки, где scroll API вызывался на MultilineTextWidget;
- если нет уверенности, лучше не менять scroll sensitivity в этой задаче, а описать next task.

5. Проверить QuestUI.c

Если менялся QuestUI.c, проверить:

- нет scroll API на MultilineTextWidget;
- нет ClearItems/AddItem/SetItemColor для Description/Dialog;
- QuestListbox/TriggerRouteListbox не затронуты;
- SetDescriptionText/SetDialogText всё ещё используют SetText.

6. Проверить DayZ_layout viewer

Если layout структура изменилась:
- проверить, показывает ли viewer ScrollWidget + content wrapper + MultilineTextWidget;
- если viewer совсем не показывает новую структуру — минимально обновить viewer;
- не делать большой viewer redesign.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. QuestUI.c синтаксически целый, если менялся.
3. DescriptionText виден.
4. DialogText виден.
5. DescriptionText не выходит за пределы DescriptionScroll.
6. DialogText не выходит за пределы DialogScroll.
7. Текст не рисуется поверх заголовков.
8. Текст не рисуется поверх кнопок.
9. Текст не рисуется поверх RoutePanel.
10. Native wrap сохранён.
11. ScrollWidget сохранён.
12. Scroll крутится.
13. Если менялся шаг scroll — он стал меньше.
14. Scroll API не вызывается на MultilineTextWidget.
15. QuestListbox не изменён.
16. TriggerRouteListbox не изменён.
17. JSON не менялся.
18. Server не менялся.
19. QuestJournal не менялся.
20. @Trader не менялся.
21. Кириллица не повреждена.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout, возможно .c и возможно viewer html.

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

1. Description/Dialog остаются ScrollWidget + MultilineTextWidget.
2. DescriptionText виден только внутри своей области.
3. DialogText виден только внутри своей области.
4. Текст не выходит поверх других частей меню.
5. Native wrap сохранён.
6. Scroll работает.
7. Если удалось безопасно — шаг прокрутки уменьшен.
8. TextListboxWidget для Description/Dialog не возвращён.
9. RichTextWidget не используется.
10. MultilineEditBoxWidget не используется.
11. QuestListbox и TriggerRouteListbox не изменены.
12. Quest logic не изменена.
13. JSON не изменён.
14. Server не изменён.
15. QuestJournal не изменён.
16. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что исправлено по clipping;
- осталась ли структура ScrollWidget + MultilineTextWidget;
- менялась ли структура content внутри ScrollWidget;
- удалось ли уменьшить scroll step.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c, если менялся
- DayZ_layout\..., если менялся

DIFF:
- кратко описать изменения layout;
- кратко описать изменения script, если были;
- кратко описать изменения viewer, если были.

RUNTIME EXPECTATION:
- что должно измениться в игре.

SCROLL SENSITIVITY:
- удалось ли уменьшить шаг;
- каким способом;
- если не удалось — почему.

COMPILE-SAFETY CHECK:
- нет cast mismatch;
- нет listbox methods на MultilineTextWidget;
- нет scroll API на MultilineTextWidget.

DAYZ_LAYOUT CHECK:
- проверялся ли viewer;
- нужно ли было его менять.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- какие риски остаются.

RECOMMENDED NEXT TASK:
- если clipping исправлен — polish scroll sensitivity / размеры;
- если clipping не исправлен — отдельная задача на RichTextWidget или возврат к TextListboxWidget.

CONCLUSION:
- краткий вывод: текст теперь должен оставаться внутри своих областей, scroll сохранён.

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

TASK 091 REVIEW

Статус:
Принято условно.

Что принято:
- Description/Dialog переведены с TextListboxWidget на ScrollWidget + MultilineTextWidget.
- QuestListbox не тронут.
- TriggerRouteListbox не тронут.
- Ручной maxCharsPerLine / weighted-wrap удалён для Description/Dialog.
- SetDescriptionText / SetDialogText теперь работают через SetText.
- Scroll reset делается через ScrollWidget, а не через MultilineTextWidget.
- Scroll API не вызывается на MultilineTextWidget.
- DayZ_layout viewer минимально обновлён под ScrollWidgetClass.
- JSON не менялся.
- Server не менялся.
- QuestJournal не менялся.
- @Trader не трогался.

Важные моменты, которые нужно проверить в игре:
1. Компилируется ли `SetLineBreakingOverride(LinebreakOverrideMode.LINEBREAK_WESTERN)`.
2. Компилируется ли `scrollWidget.VScrollToPos01(0)` именно на ScrollWidget.
3. Открывается ли QuestMenu без runtime error.
4. DescriptionText реально переносится по ширине контейнера.
5. DialogText реально переносится по ширине контейнера.
6. Scroll появляется, когда текста больше высоты панели.
7. Scroll крутится колёсиком мыши.
8. Scroll не сбрасывается слишком часто при Update.
9. Текст не обрезается по высоте из-за `size 414 1`.
10. `size to text v 1` реально увеличивает внутренний MultilineTextWidget.
11. Кнопки ВЗЯТЬ / СДАТЬ / Закрыть работают.
12. RoutePanel и список маршрутов работают.
13. HUD/prompt не вернулся поверх меню.
14. Кириллица отображается нормально.

Главный риск:
В layout у вложенных MultilineTextWidget стоит `size 414 1` и `"size to text v" 1`. Идея правильная, но именно это нужно проверить в runtime: DayZ должен реально растянуть внутренний текст по высоте контента. Если не растянет — scroll-контейнеру может быть нечего скроллить.

Второй риск:
`SetDescriptionText` / `SetDialogText` вызывают `ResetScrollWidgetToTop(...)`. Если эти методы вызываются часто через `UpdateQuestDetails`, scroll может постоянно прыгать наверх. Если при ручной прокрутке текст сбрасывается наверх — это будет отдельная маленькая задача: сбрасывать scroll только при смене текста/квеста/маршрута, а не при каждом обновлении.

Вывод:
TASK 091 выполнен в правильном направлении. Теперь нужно обязательно проверить в игре. Это как раз тот момент, где теория должна подтвердиться runtime-тестом DayZ.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
