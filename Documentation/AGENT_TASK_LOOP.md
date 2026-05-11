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

TASK 088 — Documentation rule: при изменении UI/menu обновлять совместимость с DayZ_layout viewer

Статус:
Новая документационная задача для агента.

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

- выполнить только эту документационную задачу;
- не менять код;
- не менять layout;
- не менять JSON;
- не менять server;
- не менять Quest Editor;
- не менять DayZ_layout viewer code;
- не делать git commit / push / reset / clean;
- менять только явно разрешённые документационные файлы.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

В проекте активно меняется UI QuestMenu / QuestJournal.

Недавно в QuestMenu были изменения:
- MultilineTextWidget заменялся на TextListboxWidget;
- добавлялся настоящий scroll;
- менялись размеры панелей;
- добавлялся фон / backing;
- добавлялись новые widget types и styles.

Пользователь хочет, чтобы при любых будущих изменениях меню / layout / UI агент сразу учитывал совместимость с инструментом:

P:\Silver_77_Quests\DayZ_layout

Нужно закрепить постоянное правило:

Если меняется меню, layout или UI-код, то агент обязан проверить, не нужно ли одновременно обновить / проверить совместимость с DayZ_layout viewer.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 088
--------------------------------------------------------------------------------

Внести постоянное правило в документацию:

При изменениях UI/menu/layout:
- проверять совместимость с DayZ_layout viewer;
- если добавлены новые widget classes, styles или layout-паттерны — убедиться, что DayZ_layout может их распознать;
- если viewer не поддерживает новые элементы — указать это в отчёте и предложить отдельную задачу на обновление viewer;
- не оставлять ситуацию, когда игровой UI изменился, а DayZ_layout больше не даёт полезный просмотр.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:

Внести правило в два места:

1. QUEST_UI_RULES.md  
   Потому что правило относится ко всем задачам, где меняется QuestMenu / QuestJournal / layout / UI.

2. DAYZ_LAYOUT_VIEWER_RULES.md  
   Потому что правило относится к поддержке самого viewer-а и его совместимости с актуальными layout-файлами.

Смысл правила:

Если задача меняет:
- .layout файл;
- UIScriptedMenu;
- QuestMenu;
- QuestJournal;
- widget class;
- style;
- scroll-паттерн;
- TextListboxWidget / MultilineTextWidget / ButtonWidget / PanelWidget;
- размеры или структуру меню;

то агент должен:
- проверить, понимает ли DayZ_layout эти изменения;
- если не понимает — не чинить viewer самовольно, если это не входит в scope;
- записать в PROBLEMS / RECOMMENDED NEXT TASK;
- если задача прямо разрешает — обновить viewer.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это только документационная задача.

Агент должен:
- внести правило в подходящий SplitDoc;
- не менять код;
- не менять UI;
- не менять viewer;
- не менять JSON;
- вернуть отчёт в чат.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. P:\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md

2. P:\Silver_77_Quests\Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md

Дополнительно разрешено менять только если в проекте принято фиксировать историю задач:

3. P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- P:\Silver_77_Quests\Silver_77_Quests_Client\
- P:\Silver_77_Quests\Silver_77_Quests_Server\
- P:\Silver_77_Quests\JSON_Quvest\
- P:\Silver_77_Quests\DayZ_layout\
- P:\Silver_77_Quests\Support\
- P:\Silver_77_Quests\@Trader\
- P:\Silver_77_Quests\Doors and Barricades Fixed\
- любые .layout файлы;
- любые .c файлы;
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
- запуск Addon Builder;
- перепаковку PBO.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО ДОБАВИТЬ
--------------------------------------------------------------------------------

Добавить правило примерно такого смысла:

“Если задача меняет меню, UI или layout-файлы, агент обязан проверить совместимость изменений с DayZ_layout viewer. Новые widget classes, styles, scroll-паттерны, размеры панелей и структура меню должны быть видимы и понятны в viewer-е. Если viewer не поддерживает новый элемент, агент не должен молча игнорировать это: нужно указать проблему в отчёте и предложить отдельную задачу на обновление DayZ_layout. Если задача прямо разрешает правку viewer-а, агент должен обновить его вместе с UI-изменением.”

Важно закрепить:

- UI/menu changes и DayZ_layout compatibility связаны;
- viewer должен оставаться актуальным инструментом проверки;
- если меняется QuestMenu / QuestJournal, нужно думать о viewer;
- если добавлен новый widget type, viewer должен уметь хотя бы безопасно его показать;
- если добавлен scroll через TextListboxWidget, viewer должен корректно показывать область, scrollbar-зону или хотя бы размеры listbox;
- если viewer не может эмулировать DayZ runtime, это нужно явно писать.

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. В QUEST_UI_RULES.md добавлено правило про DayZ_layout compatibility при UI/menu/layout changes.
2. В DAYZ_LAYOUT_VIEWER_RULES.md добавлено встречное правило про необходимость поддерживать актуальные UI/layout-паттерны.
3. Код не менялся.
4. Layout не менялся.
5. Viewer code не менялся.
6. JSON не менялся.
7. Server не менялся.
8. Агент вернул отчёт в чат.
9. В отчёте указано, в какие документы внесено правило.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- какое правило добавлено;
- в какие SplitDoc-файлы внесено;
- что правило теперь требует при UI/menu/layout changes.

CHANGED FILES:
- Documentation\SplitDoc\QUEST_UI_RULES.md
- Documentation\SplitDoc\DAYZ_LAYOUT_VIEWER_RULES.md
- Documentation\SplitDoc\TASK_HISTORY.md, если менялся

DIFF:
- кратко описать добавленные пункты.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- если были.

QUESTIONS:
- если были.

CONCLUSION:
- правило закреплено: при изменении меню / UI / layout нужно проверять актуальность DayZ_layout viewer.

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

TASK 087 REVIEW

Статус:
Принято условно.

Что принято:
- TextListboxWidget-scroll сохранён.
- Page fallback не возвращался.
- Кнопки < / > не возвращались.
- Неподдерживаемый MultilineTextWidget API не возвращался:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible
- Перенос строк ослаблен:
  - DescriptionText = 60 символов;
  - DialogText = 62 символа.
- Логика source-line-aware wrapping сохранена.
- Предметы / цели должны оставаться отдельными логическими строками.
- DescriptionText и DialogText расширены в layout.
- Попытка усилить фон через rover_sim_colorable сделана.
- Prompt / background проблема проанализирована правильно:
  - prompt, скорее всего, рисуется не нашим QuestHintWidget;
  - обычный фон QuestMenu может его не перекрыть;
  - поэтому временное скрытие HUD на время QuestMenu — разумный тестовый вариант.
- @Trader не трогался.
- QuestJournal не трогался.
- JSON не трогался.
- Server не трогался.
- Editor не трогался.
- Documentation не трогалась.

Что требует ручной проверки в игре:
1. Компилируется ли клиент после `GetHud().Show(false)` / `GetHud().Show(true)`.
2. Не возникает ли runtime error, если `GetHud()` в момент открытия/закрытия меню равен null.
3. Уходит ли prompt “Проверить пульс [УДЕРЖИВАЙТЕ]” при открытом QuestMenu.
4. Возвращается ли HUD после закрытия QuestMenu.
5. Не остаётся ли HUD скрытым, если меню закрыто нестандартно.
6. Стал ли перенос строк нормальным, без “стихов”.
7. Не слишком ли широкие строки теперь для русского текста.
8. Не клипуются ли длинные className / предметы.
9. Сохраняется ли правило:
   - каждый предмет с новой строки;
   - длинный предмет переносится внутри себя;
   - следующий предмет не приклеивается к предыдущему.
10. Работает ли scroll колесом.
11. Работает ли drag scrollbar.
12. Не сломались ли:
   - ВЗЯТЬ КВЕСТ;
   - СДАТЬ КВЕСТ;
   - Закрыть рот и уйти;
   - RoutePanel;
   - Offer / Completion / Reward.
13. Кириллица на месте.

Риски:
- `GetHud().Show(false)` скрывает весь HUD, не только action prompt.
- Если меню закроется аварийно или через нестандартный путь, HUD может остаться скрытым до следующего события/перезахода.
- Желательно потом заменить это на более точечное скрытие action prompt, если найдём конкретный widget/layer.
- `maxCharsPerLine` всё ещё приблизительный параметр, потому что перенос считается по символам, а не по реальной ширине шрифта.
- `rover_sim_colorable` может помочь фону, но реальный результат подтверждается только в игре.

Вывод:
TASK 087 выполнена корректно по scope. Принимается условно до ручной проверки в игре.

Следующий шаг:
Пересобрать клиентский PBO и проверить в игре:
- исчезает ли prompt;
- возвращается ли HUD после закрытия;
- стал ли текст читаться нормально;
- не сломались ли кнопки и квестовая логика.

Если всё работает:
TASK 087 можно закрывать.

Если HUD скрывается слишком грубо:
следующая задача — точечно найти и скрывать именно action prompt widget/layer, а не весь HUD.

Если перенос всё ещё не идеален:
следующая задача — только подбор `QUEST_UI_DESCRIPTION_MAX_CHARS_PER_LINE`, `QUEST_UI_DIALOG_MAX_CHARS_PER_LINE` и размеров listbox.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
