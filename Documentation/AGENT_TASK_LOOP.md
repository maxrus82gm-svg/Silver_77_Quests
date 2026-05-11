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

TASK 091 — QuestMenu: тестово заменить Description/Dialog на ScrollWidget + MultilineTextWidget

Статус:
Новая тестовая UI-задача после аналитики TASK 090.

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

- делать только то, что прямо указано в этом БЛОКЕ 1;
- не чинить “заодно” соседние проблемы;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не трогать server;
- не трогать Quest Editor;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать git commit / push / reset / clean;
- если подход не компилируется или не работает — не изобретать новый большой костыль, а описать проблему в отчёте.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 086 был внедрён рабочий scroll через TextListboxWidget.

После TASK 089 стало ясно, что ручной перенос через:

- QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE
- QUEST_UI_DIALOG_MAX_CHARS_PER_LINE
- BuildWrappedTextLines(...)
- weighted-wrap

не является правильным финальным решением.

Причина:

- перенос зависит от вручную подобранных чисел;
- на другом разрешении / масштабе UI / aspect ratio строки могут обрезаться или переноситься неправильно;
- TextListboxWidget хорош для списков, но не для абзацного текста;
- нужен DayZ-native word wrap по фактической ширине контейнера.

TASK 090 показал предпочтительное направление:

- ScrollWidgetClass отвечает за прокрутку;
- MultilineTextWidgetClass отвечает за нативный перенос текста;
- scroll API нельзя вызывать на MultilineTextWidget;
- если нужен reset scroll, его можно пробовать только на ScrollWidget.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 091
--------------------------------------------------------------------------------

Тестово заменить текстовые области QuestMenu:

- DescriptionText
- DialogText

с текущего TextListboxWidget-подхода на:

ScrollWidgetClass
  MultilineTextWidgetClass

Нужно добиться:

1. Нативного word wrap через MultilineTextWidget.
2. Прокрутки через ScrollWidget.
3. Ухода от ручного maxCharsPerLine для Description/Dialog.
4. Сохранения текущей логики Description/Dialog.
5. Сохранения QuestListbox и TriggerRouteListbox как TextListboxWidget.
6. Сохранения кнопок и маршрутов.
7. Проверки совместимости с DayZ_layout viewer.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительная структура layout:

DescriptionPanel
  ScrollWidgetClass DescriptionScroll
    MultilineTextWidgetClass DescriptionText

DialogPanel
  ScrollWidgetClass DialogScroll
    MultilineTextWidgetClass DialogText

Для MultilineTextWidgetClass использовать:

- wrap 1
- "condense whitespace" 1, если поддерживается в layout
- "size to text v" 1
- font "gui/fonts/metron16"
- text halign left
- text valign top

В QuestUI.c:

- m_QuestDescription должен стать MultilineTextWidget;
- m_DialogText должен стать MultilineTextWidget;
- добавить ссылки на ScrollWidget:
  - m_DescriptionScroll
  - m_DialogScroll
- SetDescriptionText должен делать SetText(text) на MultilineTextWidget;
- SetDialogText должен делать SetText(text) на MultilineTextWidget;
- если нужно сбросить scroll — сбрасывать только ScrollWidget, а не MultilineTextWidget;
- удалить/перестать использовать ручной wrap для Description/Dialog.

Важно:
не вызывать на MultilineTextWidget:
- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

Если эти методы используются, то только на ScrollWidget и только если компилируются.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это тестовая UI-задача только для QuestMenu Description/Dialog.

Агент должен:

1. Изменить QuestMenu.layout только в области DescriptionPanel и DialogPanel.
2. Заменить DescriptionText/DialogText с TextListboxWidgetClass на MultilineTextWidgetClass внутри ScrollWidgetClass.
3. Изменить QuestUI.c под новые widget-типы.
4. Убрать ручной wrapping для Description/Dialog.
5. Сохранить QuestListbox и TriggerRouteListbox как TextListboxWidget.
6. Сохранить route logic.
7. Сохранить кнопки.
8. Проверить DayZ_layout viewer compatibility.
9. Если viewer не показывает ScrollWidgetClass корректно — минимально обновить viewer.
10. Вернуть отчёт в чат.

Агент не должен:

- менять QuestJournal;
- менять JSON;
- менять server;
- менять RPC/sync;
- менять Quest Editor;
- менять quest logic;
- менять Offer / Completion / Reward архитектуру;
- менять @Trader;
- переписывать весь UI;
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

Дополнительно разрешено менять только если DayZ_layout viewer не показывает актуальный QuestMenu.layout корректно:

3. P:\Silver_77_Quests\DayZ_layout\*

Но DayZ_layout можно менять только минимально:
- поддержка ScrollWidgetClass;
- preview scroll-контейнера;
- корректное отображение MultilineTextWidgetClass внутри ScrollWidgetClass;
- визуализация clip/scroll-зоны.

Если viewer уже показывает достаточно корректно:
- DayZ_layout не менять;
- только указать проверку в отчёте.

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
- перенос Trader gameplay/server logic;
- перенос Trader RPC;
- перенос Trader currency/trade logic.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Изменить DescriptionPanel в QuestMenu.layout

Заменить текущий:

TextListboxWidgetClass DescriptionText

на структуру:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Требования:

- DescriptionScroll должен занимать область бывшего DescriptionText;
- DescriptionText должен быть внутри DescriptionScroll;
- DescriptionText должен иметь ширину контейнера;
- DescriptionText должен иметь wrap 1;
- DescriptionText должен быть read-only display text;
- не должно быть listbox row-highlight поведения.

2. Изменить DialogPanel в QuestMenu.layout

Заменить текущий:

TextListboxWidgetClass DialogText

на структуру:

ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Требования:

- DialogScroll должен занимать область бывшего DialogText;
- DialogText должен быть внутри DialogScroll;
- DialogText должен иметь ширину контейнера;
- DialogText должен иметь wrap 1;
- DialogText должен быть read-only display text;
- не должно быть listbox row-highlight поведения.

3. Изменить QuestUI.c

Нужно заменить типы:

Было:
- private TextListboxWidget m_QuestDescription;
- private TextListboxWidget m_DialogText;

Стало:
- private ScrollWidget m_DescriptionScroll;
- private ScrollWidget m_DialogScroll;
- private MultilineTextWidget m_QuestDescription;
- private MultilineTextWidget m_DialogText;

В Init():

- найти DescriptionScroll;
- найти DialogScroll;
- найти DescriptionText;
- найти DialogText;
- кастовать DescriptionText/DialogText как MultilineTextWidget.

4. Упростить SetDescriptionText / SetDialogText

Больше не использовать FillScrollableTextList для Description/Dialog.

Сделать:

- SetDescriptionText(text):
  - если m_QuestDescription есть — SetText(text);
  - если есть безопасный способ reset scroll на m_DescriptionScroll — использовать его;
  - не вызывать scroll API на m_QuestDescription.

- SetDialogText(text):
  - если m_DialogText есть — SetText(text);
  - если есть безопасный способ reset scroll на m_DialogScroll — использовать его;
  - не вызывать scroll API на m_DialogText.

5. Удалить или оставить неиспользуемым ручной wrap

Если BuildWrappedTextLines / FillScrollableTextList больше нигде не используются:
- можно удалить связанные helper-методы и constants:
  - QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE
  - QUEST_UI_DIALOG_MAX_CHARS_PER_LINE
  - GetQuestUiCharWeight
  - GetQuestUiTextWeight
  - SplitQuestUiWordByWeight
  - BuildWrappedTextLines
  - FillScrollableTextList

Если есть риск лишней правки:
- можно оставить их временно, но Description/Dialog не должны их использовать.
- в отчёте указать, остались ли они неиспользуемыми.

6. Line breaking override

Если в Enforce Script доступен метод:

SetLineBreakingOverride(LinebreakOverrideMode.LINEBREAK_WESTERN)

то применить его к:
- m_QuestDescription
- m_DialogText

Если компилятор ругается или enum недоступен:
- убрать этот вызов;
- оставить wrap 1 в layout;
- указать в PROBLEMS.

7. Scroll reset

Если ScrollWidget безопасно поддерживает:

VScrollToPos01(0)

то можно использовать его только на:
- m_DescriptionScroll
- m_DialogScroll

Если компилятор ругается:
- не использовать;
- оставить scroll как есть;
- указать в PROBLEMS.

Важно:
запрещено вызывать VScrollToPos01 / VScrollStep / IsScrollbarVisible на MultilineTextWidget.

8. Проверить DayZ_layout viewer

Проверить, может ли viewer корректно показать:

- ScrollWidgetClass DescriptionScroll
- MultilineTextWidgetClass DescriptionText
- ScrollWidgetClass DialogScroll
- MultilineTextWidgetClass DialogText
- границы scroll-зоны
- внутренний текстовый widget
- размеры контейнеров

Если viewer показывает это как generic widget и диагностика недостаточна:
- минимально обновить viewer;
- добавить поддержку ScrollWidgetClass preview;
- не переписывать viewer полностью.

9. Проверить compile-safety

Проверить:

- QuestUI.c синтаксически целый;
- QuestMenu.layout синтаксически целый;
- нет cast mismatch;
- нет вызовов listbox methods на MultilineTextWidget:
  - ClearItems
  - AddItem
  - SetItemColor
  - SelectRow
- нет вызовов unsupported API на MultilineTextWidget;
- QuestListbox и TriggerRouteListbox продолжают быть TextListboxWidget.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout, .c и возможно viewer-файлы.

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

Это только тестовая замена Description/Dialog text stack.

Разрешено исправить только:

- DescriptionPanel text widget;
- DialogPanel text widget;
- QuestUI.c casts/types для Description/Dialog;
- SetDescriptionText;
- SetDialogText;
- минимальный DayZ_layout viewer support для ScrollWidgetClass, если нужен.

Нельзя:

- менять QuestJournal;
- менять JSON;
- менять server;
- менять Quest Editor;
- менять RPC/sync;
- менять progress logic;
- менять Offer / Completion / Reward;
- менять структуру квестового контракта;
- менять @Trader;
- менять QuestListbox;
- менять TriggerRouteListbox;
- делать большой редизайн QuestMenu;
- чинить другие ошибки, если они не связаны напрямую с Description/Dialog scroll + native wrap.

Если ScrollWidget + MultilineTextWidget не компилируется:
- не переходить на RichTextWidget без отдельного разрешения;
- не переходить на MultilineEditBoxWidget без отдельного разрешения;
- описать проблему в PROBLEMS;
- предложить next task.

Если компилируется, но runtime плохо показывает auto-height:
- не изобретать большой костыль;
- описать проблему;
- предложить next task на RichTextWidget или MultilineEditBoxWidget как отдельную задачу.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. QuestUI.c синтаксически целый.
3. DescriptionText теперь MultilineTextWidgetClass.
4. DialogText теперь MultilineTextWidgetClass.
5. DescriptionScroll существует как ScrollWidgetClass.
6. DialogScroll существует как ScrollWidgetClass.
7. m_QuestDescription кастуется как MultilineTextWidget.
8. m_DialogText кастуется как MultilineTextWidget.
9. m_DescriptionScroll кастуется как ScrollWidget.
10. m_DialogScroll кастуется как ScrollWidget.
11. SetDescriptionText использует SetText.
12. SetDialogText использует SetText.
13. Description/Dialog больше не используют maxCharsPerLine.
14. TextListboxWidget сохранён для QuestListbox.
15. TextListboxWidget сохранён для TriggerRouteListbox.
16. Вызовы ClearItems/AddItem/SetItemColor не применяются к Description/Dialog.
17. Scroll API не вызывается на MultilineTextWidget.
18. Page fallback не возвращён.
19. Кнопки < / > не возвращены.
20. JSON не менялся.
21. Server не менялся.
22. QuestJournal не менялся.
23. @Trader не менялся.
24. DayZ_layout viewer проверен.
25. Кириллица не повреждена.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. Description/Dialog переведены на ScrollWidget + MultilineTextWidget.
2. Native wrap включён через layout wrap 1.
3. Description/Dialog больше не используют ручной maxCharsPerLine.
4. QuestListbox и TriggerRouteListbox не изменены.
5. Quest logic не изменена.
6. JSON не изменён.
7. Server не изменён.
8. QuestJournal не изменён.
9. DayZ_layout viewer проверен и при необходимости минимально обновлён.
10. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что изменено в QuestMenu.layout;
- что изменено в QuestUI.c;
- как теперь устроены Description/Dialog;
- используется ли native wrap;
- использовался ли ScrollWidget API;
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
- нет listbox methods на MultilineTextWidget;
- нет scroll API на MultilineTextWidget;
- QuestListbox/TriggerRouteListbox сохранены.

DAYZ_LAYOUT CHECK:
- как viewer показывает ScrollWidgetClass;
- как viewer показывает MultilineTextWidgetClass;
- обновлялся ли viewer;
- какие ограничения viewer остаются.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- если ScrollWidget/MultilineTextWidget имеет ограничения — описать.

QUESTIONS:
- только реальные вопросы, если есть.

RECOMMENDED NEXT TASK:
- если подход работает — runtime polish;
- если не работает — отдельная задача на RichTextWidget или MultilineEditBoxWidget.

CONCLUSION:
- краткий вывод: тестовая архитектура ScrollWidget + MultilineTextWidget внедрена или почему не внедрена.

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

TASK 089 REVIEW

Статус:
Принято условно.

Что принято:
- TextListboxWidget scroll сохранён.
- Scroll-архитектура не откатывалась.
- Page fallback не возвращался.
- Кнопки < / > не возвращались.
- Запрещённый scroll API не возвращался:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible
- DescriptionText расширен.
- DialogText расширен.
- DescriptionPanel / DialogPanel расширены минимально и безопасно.
- Лимиты переноса увеличены:
  - Description: 72
  - Dialog: 74
- Грубый перенос по Length() заменён на более умный weighted-wrap.
- Source-line-aware поведение сохранено:
  - исходные строки по \n остаются логическими границами;
  - предметы/цели не должны склеиваться;
  - длинный пункт переносится внутри себя.
- DayZ_layout viewer обновлён по новому правилу.
- Viewer теперь лучше понимает TextListboxWidgetClass.
- Viewer получил listbox preview со scrollbar-зоной.
- Sanity-checks QuestMenu обновлены под актуальные размеры.
- JSON не трогался.
- Server не трогался.
- QuestJournal не трогался.
- @Trader не трогался.
- Documentation не трогалась.

Что нужно проверить вручную в игре:
1. Компилируется ли QuestUI.c после добавления float helper-методов.
2. Нет ли проблем с ref array<string> в местах:
   - SplitQuestUiWordByWeight;
   - BuildWrappedTextLines.
3. Не ругается ли Enforce Script на сравнение / сложение float в этих helper-методах.
4. Открывается ли QuestMenu без runtime error.
5. DialogText теперь действительно идёт почти на всю ширину.
6. DescriptionText теперь действительно идёт почти на всю ширину.
7. Строки не залезают под scrollbar.
8. Длинные слова / className не обрезаются.
9. Цели и предметы не склеиваются.
10. Scroll работает как раньше.
11. HUD/prompt поведение из TASK 087 не сломалось.
12. Кнопки ВЗЯТЬ / СДАТЬ / Закрыть работают.
13. RoutePanel не пострадал.
14. Кириллица отображается нормально.

Риски:
- Weighted-wrap всё ещё приближённый, не точная метрика DayZ-шрифта.
- Если `72/74` окажутся слишком большими, часть строк может подлезать под scrollbar или обрезаться справа.
- Если окажутся всё ещё маленькими, нужна будет только тонкая подстройка чисел и weight-table.
- Viewer теперь показывает listbox лучше, но это preview, а не точная эмуляция DayZ runtime.
- Локальный file:// viewer не был открыт агентом из-за browser policy, значит визуальную проверку лучше сделать руками.

Вывод:
TASK 089 выполнена правильно по направлению. Это именно тот тип правки, который был нужен: не менять scroll заново, а расширить область, улучшить перенос и синхронно подтянуть DayZ_layout viewer.

Следующий шаг:
Пересобрать клиентский PBO и проверить в игре:
- ширину DialogText;
- ширину DescriptionText;
- отсутствие обрезания справа;
- сохранение scroll;
- сохранение структуры предметов.

Если визуально стало нормально:
TASK 089 можно закрывать.

Если строки всё ещё не доходят до правого края:
следующая задача должна быть только на увеличение лимитов `72/74` и/или настройку weight-table.

Если строки уходят слишком далеко вправо:
следующая задача должна быть только на уменьшение лимитов или резервирование места под scrollbar.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
