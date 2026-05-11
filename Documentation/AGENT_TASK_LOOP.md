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

TASK 095 — Анализ: найти правильный DayZ pattern для ScrollWidget + wrapped text после разъезда QuestMenu

Статус:
Аналитическая задача после runtime-проверки TASK 094.

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
   - лучше подходит для Description/Dialog как для обычного текста.

3. ScrollWidget + MultilineTextWidget:
   - в целом запускается;
   - текст появляется;
   - native wrap работает;
   - scrollbar выглядит как родной DayZ.

Но после попыток довести схему до нормального clipping появились проблемы:

- Description/Dialog визуально разъезжаются;
- текст может выходить за рамки своих областей;
- элементы меню могут накладываться друг на друга;
- DayZ_layout viewer показывает, что геометрия ScrollWidget/MultilineTextWidget стала нестабильной;
- фиксированная большая высота внутренних текстовых widgets, например 394x600 и 394x300, может влиять на итоговую компоновку;
- content-wrapper внутри ScrollWidget тоже не дал стабильного результата.

Нужно остановить правки вслепую и провести повторную аналитику с учётом уже полученного опыта.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 095
--------------------------------------------------------------------------------

Найти правильный DayZ-compatible pattern для длинного wrapped text со scroll внутри QuestMenu.

Нужно выяснить:

1. Почему текущий ScrollWidget + MultilineTextWidget разъезжает layout.
2. Как в vanilla DayZ сделаны scrollable text blocks.
3. Как правильно задавать размеры content внутри ScrollWidget.
4. Нужно ли использовать:
   - MultilineTextWidget;
   - RichTextWidget;
   - MultilineEditBoxWidget;
   - HtmlWidget;
   - WrapSpacerWidget;
   - GridSpacerWidget;
   - PanelWidget wrapper.
5. Какой вариант лучше применить именно для QuestMenu Description/Dialog.
6. Что надо менять в следующей практической задаче.
7. Нужно ли перед этим обновлять DayZ_layout viewer.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Это аналитическая задача.

Агент НЕ должен менять:

- QuestMenu.layout;
- QuestUI.c;
- DayZ_layout viewer;
- JSON;
- server;
- QuestJournal;
- @Trader;
- Documentation;
- любые .c;
- любые .layout;
- любые PBO.

Агент должен только читать, анализировать и вернуть отчёт в чат.

Запрещено:

- делать git commit;
- делать git push;
- делать git reset;
- делать git clean;
- перепаковывать PBO;
- запускать Addon Builder;
- исправлять “заодно”.

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

Прочитать текущие файлы QuestMenu:

7. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
8. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Можно читать, но не менять:

9. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
10. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
11. P:\Silver_77_Quests\DayZ_layout\
12. P:\Silver_77_Quests\@Trader\

Также обязательно посмотреть распакованные vanilla DayZ reference на диске D, если доступны:

13. D:\Dayz\gui\layouts\
14. D:\Dayz\scripts\5_mission\gui\
15. D:\Dayz\scripts\
16. D:\Dayz\gui\layouts\script_console\
17. D:\Dayz\gui\layouts\new_ui\
18. D:\Dayz\gui\layouts\new_ui\mods_menu\
19. D:\Dayz\gui\layouts\new_ui\options\
20. D:\Dayz\gui\layouts\day_z_book.layout
21. D:\Dayz\scripts\5_mission\gui\bookmenu.c

--------------------------------------------------------------------------------
ЧТО ИСКАТЬ
--------------------------------------------------------------------------------

Искать по словам:

- ScrollWidgetClass
- MultilineTextWidgetClass
- RichTextWidgetClass
- MultilineEditBoxWidgetClass
- HtmlWidget
- WrapSpacerWidgetClass
- GridSpacerWidgetClass
- PanelWidgetClass
- ContentScroll
- Content
- Description
- Details
- Info
- Notes
- Book
- Credits
- ModsMenu
- UniversalInfoDialog
- script_console_universal_info_dialog
- Scrollbar V
- clipchildren
- size to text v
- wrap
- condense whitespace
- SetText
- SetContentOffset
- GetContentHeight
- SetLineBreakingOverride
- VScrollToPos
- VScrollToPos01
- VScrollStep
- GetVScrollPos
- OnMouseWheel

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ВЫЯСНИТЬ
--------------------------------------------------------------------------------

1. Текущая проблема QuestMenu

Проанализировать текущий QuestMenu.layout и QuestUI.c.

Ответить:

- какая сейчас структура Description/Dialog;
- какие размеры у ScrollWidget;
- какие размеры у MultilineTextWidget;
- почему большая высота child widget может разносить layout;
- почему content-wrapper мог сделать хуже;
- где именно viewer показывает разъезд;
- есть ли признаки неправильных скобок/вложенности в layout;
- есть ли риск, что элемент оказался не в том parent block.

2. Правильный vanilla pattern

Найти в vanilla DayZ или reference примеры:

- scrollable long text;
- wrapped long text;
- text inside ScrollWidget;
- scrollable dialog/info panels;
- book/credits/info dialog/mod details screens.

Для каждого найденного примера указать:

- файл layout;
- файл script, если есть;
- widget stack;
- как задаётся scroll;
- как задаётся content size;
- есть ли wrapper;
- используется ли MultilineTextWidget / RichTextWidget / EditBox / HtmlWidget;
- как решён clipping.

3. Сравнить варианты для QuestMenu

Сравнить варианты:

Вариант A:
ScrollWidget + MultilineTextWidget напрямую.

Вариант B:
ScrollWidget + Panel/Spacer wrapper + MultilineTextWidget.

Вариант C:
ScrollWidget + RichTextWidget.

Вариант D:
ScrollWidget + MultilineEditBoxWidget в read-only/disabled режиме.

Вариант E:
HtmlWidget / book-like pattern.

Для каждого варианта указать:

- плюсы;
- минусы;
- риск разъезда layout;
- риск compile error;
- риск runtime error;
- качество native wrap;
- качество scroll;
- насколько сложно поддержать в DayZ_layout viewer;
- подходит ли для QuestMenu Description/Dialog.

4. Проверить DayZ_layout viewer

Проверить:

- корректно ли viewer отображает текущий QuestMenu.layout;
- не вводит ли viewer в заблуждение по позициям;
- понимает ли он ScrollWidgetClass;
- понимает ли он MultilineTextWidgetClass;
- корректно ли отображает вложенность ScrollWidget -> child;
- нужно ли отдельное обновление viewer перед следующими UI-правками.

Важно:
по нашему правилу, если меняем QuestMenu/layout/widget stack, совместимость с DayZ_layout viewer должна учитываться.

5. Дать рекомендацию

В конце отчёта дать чёткий вывод:

- какой pattern лучше выбрать дальше;
- нужно ли откатиться к TextListboxWidget временно;
- или стоит продолжать ScrollWidget + MultilineTextWidget;
- или перейти на RichTextWidget;
- какие конкретно файлы менять в следующей практической задаче;
- нужен ли отдельный viewer update;
- какой самый безопасный следующий TASK.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если агент вернул аналитический отчёт, где есть:

1. Анализ текущего разъезда QuestMenu.
2. Проверка вложенности layout.
3. Проверка vanilla/reference examples на диске D.
4. Сравнение минимум 3 вариантов text + scroll.
5. Рекомендация лучшего варианта.
6. Отдельный вывод по DayZ_layout viewer.
7. Подтверждение, что файлы не менялись.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что было изучено;
- какие файлы/папки просмотрены;
- какие reference examples найдены.

CURRENT QUESTMENU ANALYSIS:
- текущая структура Description/Dialog;
- что может вызывать разъезд;
- есть ли подозрения на неправильную вложенность/скобки/layout size.

VANILLA / REFERENCE FINDINGS:
- найденные примеры;
- widget stack каждого примера;
- как там устроены scroll + text + clipping.

OPTIONS:
- вариант A;
- вариант B;
- вариант C;
- вариант D;
- плюсы/минусы/риски.

DAYZ_LAYOUT VIEWER CHECK:
- корректно ли viewer показывает текущую структуру;
- что viewer не умеет;
- нужно ли обновлять viewer.

RECOMMENDATION:
- какой вариант выбрать дальше;
- какие файлы менять в следующей задаче;
- чего не делать.

CHANGED FILES:
- должно быть: none.

PROBLEMS:
- что не удалось подтвердить.

CONCLUSION:
- краткий вывод: куда двигаться дальше.

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

TASK 093 REVIEW

Статус:
Принято условно.

Что принято:
- Архитектура ScrollWidget + MultilineTextWidget сохранена.
- TextListboxWidget для Description/Dialog не возвращался.
- RichTextWidget не использовался.
- MultilineEditBoxWidget не использовался.
- Добавлен content-wrapper:
  - DescriptionContent
  - DialogContent
- Clipping включён на нескольких уровнях:
  - DescriptionPanel
  - DescriptionScroll
  - DescriptionContent
  - DescriptionText
  - DialogPanel
  - DialogScroll
  - DialogContent
  - DialogText
- QuestListbox не изменён.
- TriggerRouteListbox не изменён.
- JSON не менялся.
- Server не менялся.
- QuestJournal не менялся.
- DayZ_layout viewer не менялся, и это нормально: новый wrapper — обычный PanelWidgetClass, viewer уже должен его показывать.
- Scroll sensitivity уменьшена через QuestUI.c, а не через костыль в layout.
- Scroll API применяется только к ScrollWidget.

Что нужно проверить в игре:
1. Компилируется ли `current = current.GetParent();` в `IsWidgetInside(...)`.
2. Компилируется ли `scrollWidget.GetVScrollPos()`.
3. Компилируется ли `scrollWidget.VScrollToPos(nextPos)`.
4. Текст Description больше не выходит выше своей панели.
5. Текст Dialog больше не выходит выше/ниже своей панели.
6. Текст не рисуется поверх кнопок.
7. Текст не рисуется поверх RoutePanel.
8. Колёсико мыши действительно стало мягче.
9. Drag scrollbar всё ещё работает.
10. Не конфликтует ли кастомный `OnMouseWheel` с родным ScrollWidget scroll.
11. Scroll не сбрасывается наверх во время чтения.
12. Кнопки и выбор квеста работают.

Возможный риск:
`OnMouseWheel` теперь вручную перехватывает scroll. Если DayZ всё равно параллельно обрабатывает родной scroll, может быть двойной эффект. Но так как метод возвращает `true`, по идее событие должно считаться обработанным.

Ещё один риск:
Если `GetVScrollPos()` возвращает значение не в пикселях, а в другой шкале, шаг `18.0` может быть слишком маленьким или странным. Это проверяется только runtime.

Вывод:
TASK 093 сделан правильно по направлению. Это аккуратная доработка текущей рабочей схемы, без отката и без смены widget-стека. Теперь надо проверить в игре именно clipping и ощущение scroll wheel.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
