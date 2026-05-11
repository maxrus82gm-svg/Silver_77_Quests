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

TASK 094 — Hotfix: вернуть стабильную компоновку QuestMenu после TASK 093

Статус:
Срочный UI hotfix после runtime-проверки TASK 093.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 092 текст Description/Dialog появился, но выходил за границы scroll-областей.

После TASK 093 был добавлен content-wrapper внутри ScrollWidget:

DescriptionScroll
  DescriptionContent
    DescriptionText

DialogScroll
  DialogContent
    DialogText

Также был добавлен кастомный wheel-scroll step.

Runtime-проверка показала, что после TASK 093 QuestMenu визуально разъехался:

- элементы меню сместились/налезли друг на друга;
- Description/Dialog/Route области отображаются некорректно;
- часть текста оказывается не там, где должна быть;
- общий layout стал хуже, чем после TASK 092.

Значит content-wrapper / clipping-правка в текущем виде не подходит.

--------------------------------------------------------------------------------
ЦЕЛЬ
--------------------------------------------------------------------------------

Вернуть стабильную компоновку QuestMenu.

Нужно:

1. Убрать изменения TASK 093, которые привели к разъезду layout.
2. Сохранить рабочую схему из TASK 092:
   - ScrollWidget + MultilineTextWidget;
   - фиксированная высота внутреннего текста;
   - native wrap.
3. Не возвращать TextListboxWidget.
4. Не переходить на RichTextWidget/EditBox.
5. Не трогать JSON/server/QuestJournal/@Trader.

--------------------------------------------------------------------------------
РАЗРЕШЕНО МЕНЯТЬ
--------------------------------------------------------------------------------

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

QuestUI.c менять только если нужно убрать кастомный OnMouseWheel из TASK 093.

--------------------------------------------------------------------------------
ЧТО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. В QuestMenu.layout убрать content-wrapper

Вернуть структуру:

DescriptionScroll
  MultilineTextWidgetClass DescriptionText

DialogScroll
  MultilineTextWidgetClass DialogText

Убрать:

- PanelWidgetClass DescriptionContent
- PanelWidgetClass DialogContent

2. Сохранить фикс TASK 092

DescriptionText должен остаться примерно:

- position 0 0
- size 394 600
- wrap 1
- "size to text v" 0
- "text halign" left
- "text valign" top

DialogText должен остаться примерно:

- position 0 0
- size 394 300
- wrap 1
- "size to text v" 0
- "text halign" left
- "text valign" top

3. Проверить clipchildren

Оставить минимально безопасно:

- DescriptionScroll clipchildren 1
- DialogScroll clipchildren 1

Не включать лишний clipping на новых wrapper-ах, потому что wrapper-ы убрать.

4. В QuestUI.c проверить TASK 093 wheel-scroll

Если кастомный OnMouseWheel / HandleScrollWidgetWheel мог повлиять на поведение:
- временно убрать кастомный wheel-scroll;
- вернуть стандартный `return super.OnMouseWheel(w, x, y, wheel);`

Важно:
сейчас главная цель — вернуть стабильную геометрию, а не тюнить scroll sensitivity.

5. Не делать новую архитектуру

Запрещено:
- не возвращать TextListboxWidget;
- не переходить на RichTextWidget;
- не переходить на MultilineEditBoxWidget;
- не трогать QuestListbox;
- не трогать TriggerRouteListbox;
- не менять JSON/server/QuestJournal.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что откатили из TASK 093;
- какая структура Description/Dialog теперь осталась;
- убран ли content-wrapper;
- оставлен ли ScrollWidget + MultilineTextWidget;
- трогался ли custom OnMouseWheel.

CHANGED FILES:
- QuestMenu.layout
- QuestUI.c, если менялся

DIFF:
- кратко описать изменения.

RUNTIME EXPECTATION:
- QuestMenu должен вернуться к стабильной компоновке TASK 092;
- текст должен быть видимым;
- layout не должен разъезжаться;
- clipping может всё ещё быть неидеальным, но это уже отдельная задача.

PROBLEMS:
- что остаётся нерешённым.

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
