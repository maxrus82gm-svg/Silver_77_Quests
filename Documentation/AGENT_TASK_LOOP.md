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

TASK 092 — Hotfix: вернуть отображение Description/Dialog после ScrollWidget + MultilineTextWidget

Статус:
Срочный UI hotfix после runtime-проверки TASK 091.

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

- делать только hotfix по пропавшему тексту Description/Dialog;
- не чинить “заодно” соседние UI-проблемы;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не трогать server;
- не трогать Quest Editor;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать git commit / push / reset / clean;
- если ScrollWidget + MultilineTextWidget не удаётся быстро починить — описать проблему в отчёте, а не уходить в новую архитектуру.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 091 QuestMenu компилируется и открывается, но в игре пропал текст:

- DescriptionPanel показывает заголовок “Описание квеста”, но сам текст описания не виден.
- DialogPanel показывает заголовок “Диалог NPC”, но сам диалог не виден.
- QuestListbox отображается.
- RoutePanel отображается.
- Кнопки отображаются.

Значит проблема, скорее всего, не в данных квеста, а в отображении DescriptionText/DialogText после перехода на:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

В текущем layout у вложенных MultilineTextWidget стоит:

size 414 1
"size to text v" 1

Вероятная причина:
DayZ runtime не растягивает MultilineTextWidget внутри ScrollWidget по высоте через "size to text v" так, как ожидалось. Из-за этого текст может иметь высоту 1 или клипаться внутри ScrollWidget.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 092
--------------------------------------------------------------------------------

Вернуть видимый текст Description/Dialog в QuestMenu.

Нужно:

1. Сохранить тестовую архитектуру ScrollWidget + MultilineTextWidget, если это возможно.
2. Исправить отображение DescriptionText.
3. Исправить отображение DialogText.
4. Не возвращаться к TextListboxWidget без отдельного разрешения.
5. Не переходить на RichTextWidget или MultilineEditBoxWidget без отдельной задачи.
6. Не менять quest logic.
7. Не трогать JSON/server/QuestJournal/@Trader.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Сначала проверить гипотезу с высотой.

Скорее всего, проблема в этом:

MultilineTextWidgetClass DescriptionText {
  size 414 1
  "size to text v" 1
}

MultilineTextWidgetClass DialogText {
  size 414 1
  "size to text v" 1
}

Нужно временно дать внутреннему MultilineTextWidget нормальную высоту, чтобы ScrollWidget получил видимый content.

Предпочтительный минимальный fix:

DescriptionScroll:
- оставить размер контейнера примерно 414 x 156;
- DescriptionText сделать уже на ширину scrollbar-зоны;
- DescriptionText дать фиксированную большую высоту, например:
  - size 394 600

DialogScroll:
- оставить размер контейнера примерно 414 x 74;
- DialogText сделать уже на ширину scrollbar-зоны;
- DialogText дать фиксированную большую высоту, например:
  - size 394 300

Почему ширина 394:
- чтобы текст не залезал под вертикальный scrollbar;
- если scrollbar занимает около 16–20 px, нужно оставить запас.

Если после такого текст появится:
- значит проблема была именно в auto-height/size-to-text внутри ScrollWidget;
- дальше отдельной задачей можно будет красиво подобрать высоту/поведение.

Если текст не появится:
- не уходить в новую архитектуру;
- описать проблему в PROBLEMS;
- предложить отдельную задачу на RichTextWidget или возврат к TextListboxWidget.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это только hotfix отображения Description/Dialog.

Агент должен:

1. Проверить текущие блоки DescriptionScroll/DescriptionText.
2. Проверить текущие блоки DialogScroll/DialogText.
3. Исправить размеры вложенных MultilineTextWidget так, чтобы текст стал видимым.
4. Сохранить ScrollWidget + MultilineTextWidget.
5. Проверить QuestUI.c на очевидные проблемы с SetText.
6. Вернуть отчёт в чат.

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
- переходить на RichTextWidget;
- переходить на MultilineEditBoxWidget;
- возвращать TextListboxWidget без отдельной команды;
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

QuestUI.c менять только если там есть явная причина пропажи текста:
- неправильный cast;
- неправильное имя widget;
- SetText не вызывается;
- текст очищается;
- reset scroll вызывает проблему.

DayZ_layout viewer в этой задаче не менять, если он не нужен для hotfix.

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

1. Проверить QuestMenu.layout

Проверить блоки:

- DescriptionPanel
- DescriptionScroll
- DescriptionText
- DialogPanel
- DialogScroll
- DialogText

Особое внимание:

- size у DescriptionText;
- size у DialogText;
- clipchildren;
- "size to text v";
- wrap 1;
- положение внутри ScrollWidget;
- ширина с учётом scrollbar.

2. Исправить высоту DescriptionText

Если сейчас:

DescriptionText size 414 1

заменить на фиксированную content-height высоту, например:

DescriptionText size 394 600

Требования:

- сохранить wrap 1;
- сохранить text halign left;
- сохранить text valign top;
- оставить внутри DescriptionScroll;
- не делать TextListboxWidget;
- не делать RichTextWidget.

3. Исправить высоту DialogText

Если сейчас:

DialogText size 414 1

заменить на фиксированную content-height высоту, например:

DialogText size 394 300

Требования:

- сохранить wrap 1;
- сохранить text halign left;
- сохранить text valign top;
- оставить внутри DialogScroll;
- не делать TextListboxWidget;
- не делать RichTextWidget.

4. Проверить ширину

Учитывать scrollbar.

Если ScrollWidget имеет width 414, то внутренний text лучше сделать не 414, а примерно 394, чтобы текст не залезал под scrollbar.

5. Проверить QuestUI.c

Убедиться:

- m_QuestDescription кастуется как MultilineTextWidget;
- m_DialogText кастуется как MultilineTextWidget;
- m_DescriptionScroll кастуется как ScrollWidget;
- m_DialogScroll кастуется как ScrollWidget;
- SetDescriptionText вызывает m_QuestDescription.SetText(text);
- SetDialogText вызывает m_DialogText.SetText(text);
- нет ClearItems/AddItem/SetItemColor для Description/Dialog;
- Scroll API не вызывается на MultilineTextWidget.

6. Не делать новую архитектуру

Если фикс высоты кажется недостаточным:
- не переходить на RichTextWidget;
- не переходить на MultilineEditBoxWidget;
- не возвращать TextListboxWidget;
- написать в PROBLEMS, что подход требует отдельной задачи.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. QuestUI.c синтаксически целый, если менялся.
3. DescriptionText остался MultilineTextWidgetClass.
4. DialogText остался MultilineTextWidgetClass.
5. DescriptionScroll остался ScrollWidgetClass.
6. DialogScroll остался ScrollWidgetClass.
7. DescriptionText больше не имеет высоту 1.
8. DialogText больше не имеет высоту 1.
9. DescriptionText имеет ширину с запасом под scrollbar.
10. DialogText имеет ширину с запасом под scrollbar.
11. SetDescriptionText использует SetText.
12. SetDialogText использует SetText.
13. QuestListbox не изменён.
14. TriggerRouteListbox не изменён.
15. JSON не менялся.
16. Server не менялся.
17. QuestJournal не менялся.
18. @Trader не менялся.
19. Кириллица не повреждена.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout и, возможно, .c файлы.

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

1. DescriptionText больше не имеет высоту 1.
2. DialogText больше не имеет высоту 1.
3. Description/Dialog остаются ScrollWidget + MultilineTextWidget.
4. Ручной maxCharsPerLine не возвращён.
5. TextListboxWidget для Description/Dialog не возвращён.
6. RichTextWidget не используется.
7. MultilineEditBoxWidget не используется.
8. QuestListbox и TriggerRouteListbox не изменены.
9. Quest logic не изменена.
10. JSON не изменён.
11. Server не изменён.
12. QuestJournal не изменён.
13. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что исправлено;
- какая была вероятная причина пропажи текста;
- какие размеры поставлены для DescriptionText/DialogText;
- остался ли ScrollWidget + MultilineTextWidget.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c, если менялся

DIFF:
- кратко описать изменения layout;
- кратко описать изменения script, если были.

RUNTIME EXPECTATION:
- что должно измениться в игре после hotfix.

COMPILE-SAFETY CHECK:
- нет cast mismatch;
- нет listbox methods на MultilineTextWidget;
- нет scroll API на MultilineTextWidget.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- какие риски остаются.

RECOMMENDED NEXT TASK:
- если hotfix сработает — runtime polish ScrollWidget + MultilineTextWidget;
- если hotfix не сработает — отдельная задача на RichTextWidget или возврат к TextListboxWidget.

CONCLUSION:
- краткий вывод: текст должен снова появиться или почему подход требует отдельного решения.

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
