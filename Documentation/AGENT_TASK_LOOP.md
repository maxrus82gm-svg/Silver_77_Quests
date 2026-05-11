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

TASK 090 — Исследовать DayZ-native word wrap для QuestMenu вместо ручного maxCharsPerLine

Статус:
Новая аналитическая задача после проверки TASK 089 в игре.

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

- задача аналитическая;
- код не менять;
- layout не менять;
- JSON не менять;
- server не менять;
- DayZ_layout не менять;
- @Trader не менять;
- не делать git commit / push / reset / clean;
- вернуть результат анализа в чат.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

В QuestMenu сейчас работает scroll через TextListboxWidget.

Но перенос строк реализован вручную через:

- QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE
- QUEST_UI_DIALOG_MAX_CHARS_PER_LINE
- BuildWrappedTextLines(...)
- weighted-wrap helper logic

Ручная проверка показала, что такой подход всё равно остаётся неправильным как финальная архитектура.

Причина:

- перенос зависит от вручную подобранных чисел;
- ширина layout сейчас задаётся пикселями;
- на другом разрешении / масштабе UI / aspect ratio текст может обрезаться или переноситься неправильно;
- нужен не подбор чисел, а настоящий auto word wrap по фактической ширине контейнера, как в обычном текстовом документе.

Пользовательское требование:

“Диалог должен идти во всё окно. Только если он не влазит в это окно — он должен переноситься.”

Иными словами:
нужно найти DayZ-native способ переноса строк, а не симулировать перенос через maxCharsPerLine.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 090
--------------------------------------------------------------------------------

Найти правильный DayZ-compatible путь для автоматического переноса строк в QuestMenu.

Нужно исследовать:

1. Есть ли у DayZ UI виджет, который сам делает word wrap по ширине контейнера.
2. Поддерживает ли такой виджет scroll.
3. Можно ли заменить текущий TextListboxWidget-подход на более правильный.
4. Можно ли использовать MultilineTextWidget без запрещённых scroll API.
5. Есть ли layout-свойства для auto-wrap / multiline / clip / scroll.
6. Как это реализовано в vanilla DayZ UI или в reference-модах.
7. Что безопаснее применить к QuestMenu.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ НАПРАВЛЕНИЕ
--------------------------------------------------------------------------------

Нужно перестать подбирать maxCharsPerLine как финальное решение.

TextListboxWidget решил проблему scroll, но не решил проблему нормального auto-wrap.

Нужно искать один из вариантов:

Вариант A:
Нативный scrollable text widget, который сам переносит строки.

Вариант B:
MultilineTextWidget внутри scrollable container, если такой паттерн существует в DayZ и не требует неподдерживаемых методов:
- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

Вариант C:
Vanilla/Trader/другой reference использует отдельный scroll-panel pattern, который можно перенести.

Вариант D:
Если DayZ вообще не даёт нормального auto-wrap + scroll вместе, тогда оставить TextListboxWidget, но отказаться от pixel-guessing и искать способ рассчитывать перенос от реального размера widget, если такой API существует.

Но сначала нужно исследование, не правка.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это только аналитическая задача.

Агент должен:

1. Изучить текущий QuestMenu implementation.
2. Найти, какие виджеты сейчас используются.
3. Изучить DayZ / vanilla / reference layout-паттерны для:
   - multiline text;
   - scrollable text;
   - text wrap;
   - text area;
   - listbox scroll;
   - scroll panel.
4. Проверить, есть ли у layout-файлов свойства, связанные с переносом строк.
5. Проверить, есть ли в script API безопасные методы для auto-wrap / scroll.
6. Сравнить варианты.
7. Вернуть отчёт в чат с рекомендацией, какой путь выбрать.

Агент не должен:

- менять QuestUI.c;
- менять QuestMenu.layout;
- менять DayZ_layout;
- менять JSON;
- менять server;
- менять QuestJournal;
- менять @Trader;
- менять documentation;
- делать commit/push/reset/clean.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ И ПАПКИ ДЛЯ ЧТЕНИЯ
--------------------------------------------------------------------------------

Можно читать:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c
2. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
3. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
4. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout

Можно читать reference:

5. P:\Silver_77_Quests\@Trader\
6. P:\Silver_77_Quests\DayZ_layout\

Можно читать documentation:

7. P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
8. P:\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
9. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
10. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
11. P:\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
12. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Если есть доступ к распакованным vanilla scripts/layouts DayZ — можно читать их как reference, но не менять.

Искать по словам:

- MultilineTextWidgetClass
- TextWidgetClass
- TextListboxWidgetClass
- Scroll
- ScrollWidget
- Scrollbar
- SliderWidget
- wrap
- multiline
- text
- lines
- clipchildren
- size to text
- exact text
- text valign
- text halign
- OnMouseWheel
- AddItem
- SetText
- SetItem
- Update
- rich text
- description
- note
- book
- journal
- message
- chat

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- P:\Silver_77_Quests\Silver_77_Quests_Client\
- P:\Silver_77_Quests\Silver_77_Quests_Server\
- P:\Silver_77_Quests\JSON_Quvest\
- P:\Silver_77_Quests\DayZ_layout\
- P:\Silver_77_Quests\Documentation\
- P:\Silver_77_Quests\@Trader\
- P:\Silver_77_Quests\Support\
- P:\Silver_77_Quests\Doors and Barricades Fixed\
- любые .layout;
- любые .c;
- любые JSON;
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
- изменение quest logic;
- изменение UI без отдельной задачи.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ВЫЯСНИТЬ
--------------------------------------------------------------------------------

1. Текущий подход

Описать текущий подход:

- TextListboxWidgetClass DescriptionText
- TextListboxWidgetClass DialogText
- ручной BuildWrappedTextLines
- maxCharsPerLine
- weighted-wrap

Описать его минусы:

- зависит от подобранных чисел;
- не знает реальную ширину виджета;
- не знает реальную ширину шрифта;
- может вести себя плохо на другом разрешении.

2. DayZ-native text wrap

Выяснить, есть ли в DayZ UI:

- виджет, который сам переносит текст по ширине;
- layout-свойство для word wrap;
- script API для включения wrap;
- возможность scroll для такого текста.

3. MultilineTextWidget

Проверить:

- умеет ли MultilineTextWidget сам переносить строки;
- можно ли его использовать без ручного maxCharsPerLine;
- можно ли дать ему scroll через layout, а не через запрещённые методы;
- есть ли безопасный scroll-container pattern.

Важно:
не возвращать ранее сломавшие методы:
- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

4. ScrollWidget / ScrollPanel

Проверить, существует ли рабочий pattern:

- ScrollWidgetClass
- ScrollPanel
- SliderWidget
- container + text
- OnMouseWheel для container
- vanilla pattern для long text

5. Vanilla / reference examples

Найти примеры в vanilla / reference:

- книги;
- заметки;
- журнал;
- чат;
- описание предметов;
- tutorial screens;
- Trader или другие menus;
- любые long text screens.

Нужно понять:
как они делают scroll + text wrap.

6. Варианты решения

В отчёте дать варианты:

Вариант 1:
Оставить TextListboxWidget и улучшать ручной wrap.

Вариант 2:
Перейти на MultilineTextWidget с native wrap, если найден безопасный scroll.

Вариант 3:
Использовать ScrollWidget/container pattern.

Вариант 4:
Сделать свой scroll text system, если native решения нет.

Для каждого варианта указать:

- плюсы;
- минусы;
- риск compile error;
- риск runtime error;
- совместимость с DayZ_layout viewer;
- насколько хорошо решает проблему разных разрешений.

7. Рекомендация

В конце отчёта дать чёткую рекомендацию:

- какой вариант лучше;
- какие файлы надо менять в следующей задаче;
- какие файлы не надо трогать;
- нужен ли отдельный update DayZ_layout viewer.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если агент вернул аналитический отчёт, в котором есть:

1. Описание текущей проблемы ручного wrap.
2. Список найденных DayZ-native вариантов.
3. Проверка MultilineTextWidget.
4. Проверка возможного scroll-container pattern.
5. Проверка reference/vanilla примеров.
6. Сравнение вариантов.
7. Рекомендация следующего технического пути.
8. Указание, нужен ли update DayZ_layout viewer.
9. Подтверждение, что код/layout не менялись.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что изучено;
- какие файлы/папки просмотрены;
- какие native/pattern варианты найдены.

CURRENT APPROACH ANALYSIS:
- как сейчас работает QuestMenu text wrap;
- почему текущий maxCharsPerLine / weighted-wrap не является финальным решением.

NATIVE WRAP FINDINGS:
- найден ли DayZ-native auto word wrap;
- какой widget / layout property / script API за это отвечает;
- есть ли scroll.

MULTILINE TEXT FINDINGS:
- можно ли использовать MultilineTextWidget;
- есть ли безопасный scroll без VScrollToPos01 / VScrollStep / IsScrollbarVisible.

SCROLL CONTAINER FINDINGS:
- найден ли ScrollWidget / ScrollPanel / SliderWidget pattern;
- можно ли применить к QuestMenu.

REFERENCE FINDINGS:
- какие vanilla/reference меню изучены;
- как там сделан long text.

OPTIONS:
- Вариант 1
- Вариант 2
- Вариант 3
- плюсы/минусы каждого.

RECOMMENDATION:
- какой вариант выбрать для следующей задачи;
- какие файлы менять;
- какие риски.

DAYZ_LAYOUT VIEWER IMPACT:
- нужно ли менять viewer в следующей задаче.

CHANGED FILES:
- должно быть: none.

ENCODING CHECK:
- файлы не менялись.

PROBLEMS:
- что не удалось подтвердить.

QUESTIONS:
- если есть реальные вопросы.

CONCLUSION:
- краткий вывод: есть ли путь к настоящему auto-wrap, или DayZ вынуждает оставаться на ручном wrap.

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
