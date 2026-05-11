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

TASK 089 — QuestMenu: использовать всю ширину Description/Dialog текста и проверить совместимость с DayZ_layout viewer

Статус:
Новая активная UI-polish задача после ручной проверки QuestMenu в игре.

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
- не расширять задачу самостоятельно;
- менять только явно разрешённые файлы;
- не трогать JSON, server, editor, RPC/sync и quest logic;
- не менять @Trader;
- не возвращать fallback-пагинацию с кнопками < / >;
- не возвращать MultilineTextWidget scroll;
- не использовать неподдерживаемый API:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible;
- при изменении QuestMenu/layout/UI обязательно проверить совместимость с DayZ_layout viewer.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 086 настоящий scroll в QuestMenu заработал через TextListboxWidget.

После TASK 087 перенос строк был улучшен, но ручная проверка в игре показала, что текст всё ещё не использует всю доступную ширину окна.

Проблема:

- DialogText переносится слишком рано.
- DescriptionText тоже не использует всю ширину.
- Правая часть текстовой области остаётся пустой.
- Из-за раннего переноса появляется больше строк, чем нужно.
- Из-за этого игроку приходится больше скроллить.
- Текст должен идти в ширину окна и переноситься только тогда, когда реально не помещается.

Важное требование пользователя:

“Диалог должен идти во все окно, только если он не влазит в это окно — он должен переноситься.”

По предметам / целям:

- каждая предметная строка должна начинаться с новой строки;
- если предметная строка не влазит — она переносится внутри себя;
- следующий предмет обязательно начинается с новой строки;
- нельзя склеивать несколько предметов в одну строку;
- нельзя превращать список предметов в кашу;
- но внутри каждой логической строки нужно использовать доступную ширину максимально.

Актуальная база для задачи:

40a4dd54d1d442867873b196c950fc2d6326594a

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 089
--------------------------------------------------------------------------------

Сделать так, чтобы DescriptionText и DialogText в QuestMenu использовали почти всю доступную ширину текстовых областей.

Нужно:

1. Оставить рабочий scroll через TextListboxWidget.
2. Убрать преждевременный перенос строк.
3. Сделать DialogText максимально широким в пределах окна.
4. Сделать DescriptionText максимально широким в пределах окна.
5. Сохранить логическую структуру целей / предметов.
6. Уменьшить количество лишних строк и лишнего scroll.
7. Проверить, что DayZ_layout viewer корректно показывает актуальный QuestMenu.layout.
8. Если DayZ_layout viewer не поддерживает новые/актуальные элементы — обновить viewer только в рамках разрешённых файлов.
9. Не трогать QuestJournal.
10. Не трогать JSON / server / quest logic.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:

Сначала не менять архитектуру scroll.

TextListboxWidget уже подтвердился в игре как рабочий способ настоящего scroll.

Проблема сейчас в двух местах:

1. Layout width.
   Нужно проверить, какую реальную ширину имеют:
   - DescriptionPanel
   - DescriptionText
   - DialogPanel
   - DialogText

   Если справа есть свободное место внутри панели — расширить TextListboxWidget до максимально безопасной ширины.

2. Script wrap limit.
   Сейчас перенос управляется вручную через:
   - QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE
   - QUEST_UI_DIALOG_MAX_CHARS_PER_LINE
   - BuildWrappedTextLines(...)
   - FillScrollableTextList(...)

   Эти значения нужно подобрать так, чтобы текст занимал всю ширину текущего TextListboxWidget.

Важно:
- не просто поставить огромное число;
- если поставить слишком большое число, строка может уйти под scrollbar или обрезаться;
- нужно подобрать разумный лимит под текущую ширину и шрифт.

Предпочтительный порядок:

1. Увеличить полезную ширину DescriptionText / DialogText в QuestMenu.layout, если возможно.
2. Увеличить QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE и QUEST_UI_DIALOG_MAX_CHARS_PER_LINE.
3. Сохранить перенос по словам.
4. Сохранить source-line-aware поведение.
5. Проверить отображение в DayZ_layout viewer.
6. Если viewer не показывает TextListboxWidget / размеры / style корректно — обновить viewer минимально.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это UI-polish задача только для QuestMenu и DayZ_layout compatibility.

Агент должен:

1. Исправить ширину DescriptionText.
2. Исправить ширину DialogText.
3. Исправить/подобрать лимиты переноса строк.
4. Сохранить TextListboxWidget scroll.
5. Сохранить логическую структуру строк.
6. Проверить QuestMenu.layout в DayZ_layout viewer.
7. Если viewer не отображает актуальные widget classes / размеры / styles — обновить viewer минимально.
8. Вернуть отчёт в чат.

Агент не должен:

- менять @Trader;
- менять QuestJournal;
- менять JSON;
- менять server;
- менять RPC/sync;
- менять Quest Editor;
- менять quest logic;
- менять Offer / Completion / Reward архитектуру;
- делать commit/push/reset/clean;
- перепаковывать PBO;
- запускать Addon Builder.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

2. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

Дополнительно разрешено менять только если DayZ_layout viewer не показывает актуальный QuestMenu.layout корректно:

3. P:\Silver_77_Quests\DayZ_layout\*

Но менять DayZ_layout можно только минимально и только для совместимости с актуальными UI/layout-изменениями:
- TextListboxWidgetClass;
- ButtonWidgetClass QuestPanelBackdrop;
- style rover_sim_colorable;
- размеры listbox;
- отображение scroll/listbox-зоны;
- корректный preview границ DescriptionText / DialogText.

Если viewer уже корректно показывает актуальный QuestMenu.layout:
- DayZ_layout не менять;
- просто указать в отчёте, что проверка пройдена.

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

Можно читать DayZ_layout viewer:

9. P:\Silver_77_Quests\DayZ_layout\

Можно читать для сравнения, но не менять:

10. P:\Silver_77_Quests\@Trader\

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

1. Проверить текущую ширину QuestMenu.layout

Проверить:

- DescriptionPanel
- DescriptionText
- DialogPanel
- DialogText
- RoutePanel
- TriggerRouteListbox
- scrollbar-зону TextListboxWidget

Нужно понять:

- есть ли свободная ширина внутри DescriptionPanel;
- есть ли свободная ширина внутри DialogPanel;
- не мешает ли scrollbar;
- можно ли расширить DescriptionText / DialogText;
- не пересекается ли это с RoutePanel;
- не ломаются ли кнопки.

2. Расширить полезную область текста

Если возможно, расширить:

- DescriptionText;
- DialogText.

Требование:

- текстовые области должны использовать почти всю ширину своих панелей;
- не оставлять пустую половину справа;
- не залезать на scrollbar;
- не выходить за границы panel.

3. Исправить maxCharsPerLine

В QuestUI.c проверить и подобрать:

- QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE
- QUEST_UI_DIALOG_MAX_CHARS_PER_LINE

Текущие значения могут быть недостаточны.

Нужно добиться:

- DialogText идёт почти на всю ширину окна;
- DescriptionText идёт почти на всю ширину окна;
- перенос только когда строка реально не помещается;
- меньше лишних строк;
- меньше лишнего scroll.

4. Сохранить логическую структуру предметов / целей

Нельзя склеивать разные предметы.

Если исходный текст содержит:

- Принести: PotatoSeed x120
  Сдано: 23 / 120
  Можно сдавать частями
- Принести: другой предмет x2

то результат должен сохранять:

- первый предмет с новой строки;
- дополнительные строки первого предмета отдельными строками;
- если строка длинная — она переносится внутри себя;
- второй предмет начинается с новой строки.

5. Сохранить нормальное поведение DialogText

DialogText — это обычный диалог NPC.

Для него нужно:

- не резать слишком коротко;
- не превращать в стихи;
- использовать всю ширину;
- переносить только при необходимости.

6. Не трогать scroll-архитектуру

Сохранить:

- TextListboxWidgetClass DescriptionText
- TextListboxWidgetClass DialogText
- TextListboxWidget m_QuestDescription
- TextListboxWidget m_DialogText
- FillScrollableTextList
- BuildWrappedTextLines

Не возвращать:

- MultilineTextWidget scroll;
- page fallback;
- кнопки < / >;
- VScrollToPos01;
- VScrollStep;
- IsScrollbarVisible.

7. Проверить DayZ_layout viewer

После изменения QuestMenu.layout проверить, может ли DayZ_layout viewer корректно показать:

- QuestMenuRoot;
- BackgroundOverlay;
- QuestPanelBackdrop;
- QuestPanel;
- DescriptionPanel;
- DescriptionText как TextListboxWidgetClass;
- DialogPanel;
- DialogText как TextListboxWidgetClass;
- RoutePanel;
- TriggerRouteListbox;
- размеры текстовых областей;
- style rover_sim_colorable;
- границы listbox/scroll-зон.

Если viewer показывает корректно:
- DayZ_layout не менять;
- указать это в отчёте.

Если viewer не показывает корректно:
- минимально обновить DayZ_layout viewer;
- добавить поддержку нужных widget classes / styles / размеров;
- не делать большой редизайн viewer;
- не переписывать viewer полностью.

8. Проверить compile-safety

Проверить:

- QuestUI.c синтаксически целый;
- QuestMenu.layout синтаксически целый;
- если менялся DayZ_layout — viewer открывается / не ломается;
- нет ссылок на page fallback;
- нет unsupported scroll API.

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

Это только QuestMenu text-width polish + DayZ_layout compatibility.

Разрешено исправить только:

- ширину DescriptionText;
- ширину DialogText;
- maxCharsPerLine для DescriptionText;
- maxCharsPerLine для DialogText;
- сохранение логических строк целей/предметов;
- совместимость DayZ_layout viewer с актуальным QuestMenu.layout.

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
- возвращать unsupported scroll API;
- возвращать page fallback;
- чинить другие ошибки, если они не связаны напрямую с QuestMenu text width или DayZ_layout compatibility.

Если DayZ_layout требует большой переработки:
- не делать её в этой задаче;
- описать ограничение в PROBLEMS;
- предложить отдельную задачу.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. Scroll через TextListboxWidget сохранён.
2. DescriptionText использует почти всю ширину области.
3. DialogText использует почти всю ширину области.
4. Текст не режется “стихами”.
5. Перенос идёт по словам.
6. Перенос происходит только при заполнении строки.
7. Строки целей / предметов начинаются с новой строки.
8. Длинная строка предмета переносится, если не влезает.
9. Следующий предмет начинается с новой строки.
10. В QuestUI.c нет executable-вызовов:
    - VScrollToPos01
    - VScrollStep
    - IsScrollbarVisible
11. Page fallback не возвращён.
12. Кнопки < / > не возвращены.
13. QuestMenu.layout синтаксически целый.
14. QuestUI.c синтаксически целый.
15. DayZ_layout viewer проверен на актуальном QuestMenu.layout.
16. Если DayZ_layout менялся — viewer не сломан.
17. Кнопки не сломаны:
    - ВЗЯТЬ КВЕСТ
    - СДАТЬ КВЕСТ
    - Закрыть рот и уйти
18. Mapping Description/Dialog не изменён.
19. Кириллица не повреждена.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. DescriptionText использует ширину окна нормально.
2. DialogText использует ширину окна нормально.
3. Scroll работает через TextListboxWidget.
4. Предметы / цели сохраняют логическую структуру.
5. Следующий предмет начинается с новой строки.
6. Неподдерживаемый MultilineTextWidget API не используется.
7. Page fallback не возвращён.
8. DayZ_layout viewer проверен на актуальном QuestMenu.layout.
9. Если viewer требовал минимальной правки — он обновлён.
10. JSON не менялся.
11. Server не менялся.
12. Quest Editor не менялся.
13. @Trader не менялся.
14. Documentation не менялась.
15. Агент вернул отчёт в чат.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что исправлено по ширине текста;
- какие параметры maxCharsPerLine изменены;
- как теперь работает перенос DescriptionText;
- как теперь работает перенос DialogText;
- как сохранена структура предметов / целей;
- что проверено по DayZ_layout viewer;
- менялся ли DayZ_layout или нет.

CHANGED FILES:
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c
- Silver_77_Quests_Client\gui\QuestMenu.layout
- DayZ_layout\..., если менялся

DIFF:
- кратко описать изменения QuestUI.c;
- кратко описать изменения QuestMenu.layout;
- кратко описать изменения DayZ_layout, если менялся.

DAYZ_LAYOUT CHECK:
- открывается ли актуальный QuestMenu.layout в viewer;
- видит ли viewer TextListboxWidgetClass;
- показывает ли размеры DescriptionText/DialogText;
- показывает ли style/панели достаточно корректно;
- какие ограничения viewer остаются.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- какие ограничения остались.

QUESTIONS:
- только реальные вопросы, если есть.

RECOMMENDED NEXT TASK:
- если нужно, предложить отдельную задачу на внешний polish QuestMenu;
- если нужно, предложить отдельную задачу на DayZ_layout viewer.

CONCLUSION:
- краткий вывод: QuestMenu должен использовать ширину текста нормально, scroll сохранён, DayZ_layout compatibility проверена.

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

TASK 088 REVIEW

Статус:
Принято.

Что принято:
- Правило о связке UI/menu/layout changes с DayZ_layout viewer закреплено.
- Правило внесено в два правильных SplitDoc:
  - Documentation\SplitDoc\QUEST_UI_RULES.md
  - Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
- Scope соблюдён:
  - код не менялся;
  - layout не менялся;
  - JSON не менялся;
  - server не менялся;
  - DayZ_layout viewer code не менялся;
  - @Trader не трогался.
- TASK_HISTORY.md не менялся — это допустимо, если история в этой задаче не была обязательной.
- Правило сформулировано правильно:
  - если меняется QuestMenu / QuestJournal / .layout / UIScriptedMenu / widget classes / styles / scroll-паттерны / структура UI, нужно проверять совместимость с DayZ_layout viewer;
  - если viewer не поддерживает новый элемент, это фиксируется в PROBLEMS;
  - обновление viewer делается только если прямо разрешено текущей задачей.

Вывод:
TASK 088 выполнена корректно. Новое правило принято и теперь должно учитываться во всех будущих UI/layout задачах.

Важно для нас дальше:
При следующей задаче по QuestMenu нужно сразу включать в scope проверку DayZ_layout viewer или явно писать, что viewer только проверяется, а не правится.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
