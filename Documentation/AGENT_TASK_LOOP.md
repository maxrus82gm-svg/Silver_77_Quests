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

TASK 097 — Анализ: зафиксировать рабочую базу QuestMenu и найти безопасный способ clipping для ScrollWidget + MultilineTextWidget

Статус:
Аналитическая задача. Ничего не менять.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После нескольких UI-экспериментов QuestMenu был откатан к рабочему состоянию.

Текущий рабочий commit/base:

5ccb7c4d24de2ac0691136e64ad29e2789708874

Это состояние нужно считать текущей рабочей базой для QuestMenu.

В этом состоянии:

1. QuestMenu визуально снова целый.
2. DescriptionPanel находится на правильном месте.
3. DialogPanel находится на правильном месте.
4. RoutePanel находится на правильном месте.
5. QuestListbox находится на правильном месте.
6. Кнопки находятся на правильном месте.
7. Description/Dialog текст виден.
8. Native wrap работает.
9. Scrollbar работает.
10. QuestListbox и TriggerRouteListbox работают как раньше.

Текущий стек Description/Dialog:

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

Проблема осталась одна:

При прокрутке текст Description/Dialog прокручивается за границы своего окна и визуально выходит поверх соседних областей.

То есть сейчас НЕ нужно менять расположение панелей.
Нужно понять, как ограничить видимость scroll-content границами DescriptionScroll/DialogScroll.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 097
--------------------------------------------------------------------------------

Провести аналитику и найти безопасный способ исправить clipping для текущей рабочей схемы:

ScrollWidgetClass
  MultilineTextWidgetClass

Нужно:

1. Зафиксировать текущее состояние `5ccb7c4d24de2ac0691136e64ad29e2789708874` как рабочую базу.
2. Не менять текущий layout и код.
3. Понять, почему `clipchildren 1` на ScrollWidget не ограничивает видимость дочернего MultilineTextWidget.
4. Найти DayZ-compatible варианты clipping/viewport для ScrollWidget.
5. Проверить vanilla/reference на диске D.
6. Проверить, есть ли layout-свойства, которые реально включают clipping для ScrollWidget.
7. Проверить, влияет ли `style blank`, `clipchildren`, `ignorepointer`, `vexactsize`, `size to text v`, parent Panel, child size.
8. Дать рекомендацию следующей практической задачи.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Это аналитическая задача.

Агент НЕ должен менять:

- QuestMenu.layout
- QuestUI.c
- DayZ_layout viewer
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
- перепаковывать PBO
- запускать Addon Builder
- исправлять “заодно”
- снова переходить на RichTextWidget
- снова переходить на GridSpacer/RichText stack
- переходить на MultilineEditBoxWidget без отдельной будущей задачи
- менять размеры панелей
- менять расположение окон

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

9. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
10. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
11. P:\Silver_77_Quests\DayZ_layout\
12. P:\Silver_77_Quests\@Trader\

Обязательно проверить vanilla/reference на диске D, если доступны:

13. D:\Dayz\gui\layouts\
14. D:\Dayz\scripts\5_mission\gui\
15. D:\Dayz\scripts\
16. D:\Dayz\gui\layouts\new_ui\
17. D:\Dayz\gui\layouts\new_ui\mods_menu\
18. D:\Dayz\gui\layouts\new_ui\options\
19. D:\Dayz\gui\layouts\script_console\

--------------------------------------------------------------------------------
ЧТО ИСКАТЬ
--------------------------------------------------------------------------------

Искать по словам:

- ScrollWidgetClass
- MultilineTextWidgetClass
- clipchildren
- clipping
- Scrollbar V
- style blank
- style Empty
- size to text v
- vexactsize
- hexactsize
- ignorepointer
- SetClipChildren
- SetFlags
- WidgetFlags
- GetVScrollPos
- VScrollToPos
- VScrollToPos01
- VScrollStep
- SetContentOffset
- GetContentHeight
- SetSize
- Update
- Show
- PanelWidgetClass
- FrameWidgetClass
- WrapSpacerWidgetClass
- GridSpacerWidgetClass
- MultilineEditBoxWidgetClass
- RichTextWidgetClass

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ВЫЯСНИТЬ
--------------------------------------------------------------------------------

1. Зафиксировать рабочую базу

В отчёте явно написать:

- текущий base commit: 5ccb7c4d24de2ac0691136e64ad29e2789708874;
- текущая структура Description/Dialog;
- текущие размеры панелей и scroll областей;
- что сейчас работает;
- что сейчас не работает.

2. Проверить текущий layout

Проверить:

- правильна ли вложенность скобок;
- DescriptionPanel/DescriptionScroll/DescriptionText находятся в правильных parent blocks;
- DialogPanel/DialogScroll/DialogText находятся в правильных parent blocks;
- RoutePanel не затронут;
- QuestListbox не затронут;
- кнопки не затронуты;
- нет ли layout-свойства, которое отключает clipping;
- не мешает ли `clipchildren 0` на самом DescriptionText/DialogText;
- не нужно ли добавить/вернуть `clipchildren 1` на DescriptionPanel/DialogPanel;
- не нужно ли менять style у ScrollWidget;
- не нужно ли использовать FrameWidget вместо PanelWidget для clipping.

3. Проверить vanilla/reference

Найти реальные примеры, где:

- ScrollWidget ограничивает видимость дочернего контента;
- child widget больше scroll viewport;
- text не выходит за пределы viewport;
- используется MultilineTextWidget внутри ScrollWidget;
- используется другой container внутри ScrollWidget;
- используется другой widget class для long text.

Для каждого примера указать:

- файл;
- widget stack;
- какие свойства clipping/size/style используются;
- есть ли script-code для обновления scroll/content;
- можно ли применить это к QuestMenu без большого redesign.

4. Проверить варианты исправления

Сравнить варианты:

Вариант A:
Оставить ScrollWidget -> MultilineTextWidget и изменить только clip/style/parent flags.

Вариант B:
Оставить MultilineTextWidget, но добавить правильный vanilla container, если найден, без изменения общей геометрии.

Вариант C:
Использовать маску/Frame/Panel clipping вокруг ScrollWidget, если DayZ так делает.

Вариант D:
Вернуться к TextListboxWidget только как fallback, если clipping для MultilineTextWidget невозможно.

Вариант E:
Отдельной будущей задачей перейти на MultilineEditBoxWidget по подтверждённому vanilla pattern.

Для каждого варианта указать:

- плюсы;
- минусы;
- риск разъезда layout;
- риск compile error;
- риск runtime error;
- что менять;
- насколько это безопасно для текущей рабочей базы.

5. Отдельно проверить DayZ_layout viewer

Проверить:

- показывает ли viewer текущую рабочую базу корректно;
- не вводит ли viewer в заблуждение по clipping;
- нужно ли фиксировать viewer под текущую рабочую базу;
- нужно ли в будущем viewer научить показывать viewport/mask отдельно.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если агент вернул аналитический отчёт, где есть:

1. Подтверждение рабочей базы commit `5ccb7c4d24de2ac0691136e64ad29e2789708874`.
2. Описание текущей структуры и размеров.
3. Подтверждение, что файлы не менялись.
4. Анализ причины выхода текста за границы.
5. Проверка vanilla/reference на диске D.
6. Сравнение минимум 4 вариантов исправления.
7. Рекомендация следующего безопасного практического TASK.
8. Отдельный вывод по DayZ_layout viewer.

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

CURRENT LAYOUT ANALYSIS:
- вложенность;
- clipchildren;
- style;
- parent/child relationships;
- возможная причина выхода текста.

VANILLA / REFERENCE FINDINGS:
- найденные примеры;
- widget stack;
- clipping/size/style pattern;
- применимость к QuestMenu.

OPTIONS:
- вариант A;
- вариант B;
- вариант C;
- вариант D;
- вариант E;
- плюсы/минусы/риски.

DAYZ_LAYOUT VIEWER CHECK:
- что viewer показывает правильно;
- что viewer не умеет;
- нужно ли обновлять viewer.

RECOMMENDATION:
- какой самый безопасный следующий TASK;
- какие файлы менять в следующей практической задаче;
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

TASK 096 REVIEW

Статус:
Принято условно.

Что принято:
- Старый direct ScrollWidget -> MultilineTextWidget для Description/Dialog убран.
- Description/Dialog переведены на:
  - ScrollWidget
  - GridSpacerWidgetClass
  - RichTextWidgetClass
- Фиксированные 394x600 / 394x300 больше не используются как основная модель.
- Добавлен runtime sync высоты через RichTextWidget.GetContentHeight().
- Scroll API не вызывается на RichTextWidget.
- Reset scroll остался только на ScrollWidget.
- QuestListbox не изменён.
- TriggerRouteListbox не изменён.
- JSON не менялся.
- Server не менялся.
- QuestJournal не менялся.
- @Trader не трогался.
- DayZ_layout viewer обновлён под RichTextWidgetClass, по нашему правилу.

Что обязательно проверить в игре:
1. Компилируется ли `RichTextWidget` в Mission module.
2. Компилируется ли `RichTextWidget.GetContentHeight()`.
3. Компилируется ли `contentWidget.SetSize(contentWidth, richContentHeight)`.
4. Открывается ли QuestMenu без runtime error.
5. Видны ли Description и Dialog.
6. Есть ли native wrap.
7. Не разъезжается ли геометрия панелей.
8. Не вылезает ли текст за пределы scroll-областей.
9. Работает ли scroll wheel.
10. Работает ли drag scrollbar.
11. Не прыгает ли scroll наверх при чтении.
12. Кнопки и RoutePanel работают.
13. Кириллица отображается нормально.

Риски:
- `RichTextWidget` может оказаться чувствительным к plain text с `\n`. Если переносы строк не будут отображаться как ожидается, нужно будет смотреть, требует ли RichTextWidget markup/форматирование.
- `GetContentHeight()` может вернуть корректную высоту только после `Update()`, что агент учёл, но runtime всё равно надо проверить.
- `GridSpacerWidgetClass` с `Rows 1` и `"Size To Content V" 1` может вести себя не так, как ожидаем, если RichTextWidget сам не отдаст высоту.
- Scroll reset всё ещё вызывается при каждом `SetDescriptionText/SetDialogText`; если UpdateQuestDetails вызывается часто, scroll может прыгать наверх. Это уже отдельный маленький polish, если проявится.

Вывод:
TASK 096 выполнен правильно по направлению. Это уже не очередной костыль вокруг MultilineTextWidget, а нормальный тест нового vanilla-style стека. Теперь нужно собрать клиент и проверить runtime.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
