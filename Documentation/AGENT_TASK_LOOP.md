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

TASK 085 — QuestMenu readability: рабочий непрозрачный фон и DayZ-compatible способ длинного текста

Статус:
Новая активная UI-задача после ручной проверки в игре.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
--------------------------------------------------------------------------------

Перед выполнением задачи агент обязан прочитать:

1. D:\GitHub\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md
2. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\AGENT_RULES.md
3. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\QUEST_UI_RULES.md
4. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\ENCODING_RULES.md
5. D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md

Агент обязан соблюдать принцип жёстких рамок:

- делать только то, что прямо указано в этом БЛОКЕ 1;
- не чинить “заодно” соседние проблемы;
- не расширять задачу самостоятельно;
- менять только явно разрешённые файлы;
- не трогать JSON, server, editor, RPC/sync и quest logic;
- не возвращать неподдерживаемый MultilineTextWidget scroll API:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible;
- если найден лучший, но более крупный способ scroll — описать его в RECOMMENDED NEXT TASK, а не переписывать весь UI самовольно.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 084 сервер/клиент снова запускаются нормально.

TASK 084 закрыла compile error:

Undefined function 'MultilineTextWidget.VScrollToPos01'

Но ручная проверка QuestMenu в игре показала две проблемы:

1. Фон QuestMenu фактически не работает.
   Окно квестов выглядит почти прозрачным.
   Игровой мир и action prompt вида “Проверить пульс [УДЕРЖИВАЙТЕ]” просвечивают сквозь интерфейс и мешают читать.

2. Scroll длинных текстов отсутствует.
   DescriptionPanel и DialogPanel обрезают текст.
   После TASK 084 экспериментальный scroll был временно отключён, потому что использовал неподдерживаемый API MultilineTextWidget.

По текущему QuestMenu.layout уже есть:
- BackgroundOverlay;
- QuestPanel;
- DescriptionPanel;
- DialogPanel;
- RoutePanel.

Но текущий способ через PanelWidgetClass color визуально не дал нужной непрозрачности в DayZ runtime.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 085
--------------------------------------------------------------------------------

Сделать QuestMenu читаемым в игре.

Минимальная цель:

1. Реально непрозрачный / почти непрозрачный тёмный фон QuestMenu.
2. Игровой мир и action prompt за меню не должны мешать чтению.
3. Длинный текст DescriptionPanel должен иметь безопасный способ чтения.
4. Длинный текст DialogPanel должен иметь безопасный способ чтения.
5. Не использовать неподдерживаемый MultilineTextWidget API.
6. Не ломать Offer / Completion / Reward.
7. Не менять JSON-контракт.
8. Не трогать серверную логику.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:

Разделить задачу на две части внутри одного UI-hotfix:

1. Фон / readability — обязательно сделать в этой задаче.
2. Длинный текст — сделать безопасным DayZ-compatible способом.

По фону:

Текущий PanelWidgetClass color оказался недостаточным или не рендерится как реальный фон в runtime.

Нужно найти DayZ-compatible способ сделать настоящий затемняющий слой.

Предпочтительные варианты по порядку:

A. Использовать ImageWidgetClass как тёмный фон/подложку, если DayZ layout стабильно рендерит ImageWidget с цветом/текстурой.
B. Использовать FrameWidgetClass / ImageWidgetClass с реальной фоновой текстурой/заливкой.
C. Если общий fullscreen overlay не работает, сделать отдельные непрозрачные backing-виджеты под:
   - QuestPanel;
   - DescriptionPanel;
   - DialogPanel;
   - RoutePanel;
   - button area.

Важно:
фон должен быть проверяемым визуально. Если alpha/color у PanelWidget не работает — не продолжать усиливать тот же неработающий способ.

По длинному тексту:

Не возвращать:
- VScrollToPos01;
- VScrollStep;
- IsScrollbarVisible.

Предпочтительные варианты по порядку:

A. Найти рабочий DayZ-compatible ScrollWidget / ScrollPanel pattern в существующих layout/script примерах и применить минимально.
B. Если ScrollWidget требует неизвестного API или рискован — сделать безопасную page-систему:
   - текст делится на страницы/куски;
   - добавить кнопки “Описание ↑”, “Описание ↓” или “Страница + / -”;
   - для DialogPanel аналогично;
   - это менее красиво, но надёжнее, чем неподдерживаемый scroll API.

Моё мнение:
лучше сначала гарантированно сделать непрозрачный фон, а scroll решить самым безопасным способом. Если настоящий ScrollWidget не удастся подтвердить по примерам, использовать page/fallback-механику.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это UI-задача только для QuestMenu readability.

Агент должен:
- проверить, почему текущий фон не даёт нужной непрозрачности;
- заменить или дополнить фон DayZ-compatible способом;
- сделать QuestMenu визуально читаемым поверх игрового мира;
- найти безопасный способ длинного текста для DescriptionPanel и DialogPanel;
- не использовать неподдерживаемый MultilineTextWidget scroll API;
- сохранить текущий mapping:
  - DescriptionPanel = quest.description / цели / прогресс / награды;
  - DialogPanel = triggerActions[].dialogText / NPC dialog;
- вернуть отчёт в чат.

Агент не должен:
- менять JSON;
- менять server;
- менять RPC/sync;
- менять Quest Editor;
- менять quest logic;
- менять Offer / Completion / Reward архитектуру;
- трогать Doors and Barricades Fixed;
- делать commit/push/reset/clean.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

Дополнительно разрешено читать, но не менять:

3. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout

4. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c

QuestJournal в этой задаче не править, если ошибка касается только QuestMenu.

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
- D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestJournal.layout
- D:\GitHub\Silver_77_Quests\Silver_77_Quests_Server\
- D:\GitHub\Silver_77_Quests\JSON_Quvest\
- D:\GitHub\Silver_77_Quests\DayZ_layout\
- D:\GitHub\Silver_77_Quests\Documentation\
- D:\GitHub\Silver_77_Quests\Documentation\SplitDoc\
- D:\GitHub\Silver_77_Quests\Support\
- D:\GitHub\Silver_77_Quests\Doors and Barricades Fixed\
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
- изменение логики Offer / Completion / Reward.

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Проанализировать текущий QuestMenu.layout

Проверить:

- QuestMenuRoot;
- BackgroundOverlay;
- QuestPanel;
- DescriptionPanel;
- DialogPanel;
- RoutePanel;
- кнопочные зоны.

Нужно понять:
- почему текущий BackgroundOverlay визуально не затемняет мир;
- почему QuestPanel / panels выглядят прозрачными, несмотря на color alpha;
- работает ли PanelWidgetClass color как реальная заливка в DayZ runtime;
- нужен ли ImageWidgetClass / другой widget type для реального фона.

2. Исправить фон QuestMenu

Требование:

- QuestMenu должен иметь реально тёмный читаемый фон;
- игровой мир должен быть значительно затемнён или перекрыт;
- action prompt за окном не должен мешать чтению;
- не отключать сам action prompt через input/action систему;
- решать только визуально.

Допустимые варианты:

- заменить BackgroundOverlay на DayZ-compatible ImageWidgetClass / FrameWidgetClass, если это стабильно работает;
- добавить непрозрачные backing-виджеты под панели;
- сделать QuestPanel и внутренние панели реально непрозрачными через widget type, который рендерит background;
- если fullscreen overlay не работает, сделать хотя бы непрозрачную большую подложку под всей центральной областью QuestMenu.

3. Сделать безопасный способ чтения длинного DescriptionText

Запрещено использовать:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

Вариант A:
Если найден рабочий DayZ-compatible ScrollWidget / ScrollPanel pattern:
- использовать его минимально;
- не переписывать весь UI;
- не менять mapping Description/Dialog.

Вариант B:
Если ScrollWidget pattern не подтверждён:
- реализовать page/fallback-механику для DescriptionPanel;
- например:
  - хранить полный description text в переменной;
  - показывать только часть строк/символов;
  - добавить кнопки или обработку Page Up / Page Down;
  - вывести индикатор страницы, например “Описание 1/3”.

Важно:
fallback должен быть простым и компилируемым.

4. Сделать безопасный способ чтения длинного DialogText

Аналогично DescriptionText.

Запрещено использовать:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

Вариант A:
Если найден рабочий ScrollWidget / ScrollPanel pattern:
- применить минимально.

Вариант B:
Если нет:
- реализовать page/fallback-механику для DialogPanel;
- например “Диалог 1/2”.

5. Не ломать текущие кнопки

Сохранить:

- AcceptButton;
- CompleteButton;
- CloseButton;
- текущие тексты кнопок;
- текущую логику OnClick;
- текущую логику RefreshQuestList;
- текущую логику UpdateQuestDetails;
- текущую логику BuildQuestDialogText.

6. Не менять mapping Description / Dialog

Сохранить:

DescriptionPanel / DescriptionText:
- quest.description;
- status;
- requirements;
- objectives;
- deposited progress;
- rewards.

DialogPanel / DialogText:
- triggerActions[].dialogText;
- current NPC dialog;
- selected route dialog.

Нельзя возвращать старый режим, где NPC dialogText подмешивался в DescriptionPanel.

7. Проверить отсутствие unsupported API

После правок обязательно проверить:

- нет VScrollToPos01;
- нет VScrollStep;
- нет IsScrollbarVisible.

Ни в QuestUI.c, ни в QuestMenu.layout не должно быть решений, которые снова вызовут прошлую compile error.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout и .c файлы с кириллицей.

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

Это только QuestMenu readability task.

Разрешено исправить только:

- рабочий тёмный фон QuestMenu;
- безопасное чтение длинного DescriptionText;
- безопасное чтение длинного DialogText.

Нельзя:
- переделывать весь UI;
- менять QuestJournal;
- менять JSON;
- менять server;
- менять Quest Editor;
- менять RPC/sync;
- менять progress logic;
- менять Offer / Completion / Reward;
- менять структуру квестового контракта;
- возвращать unsupported scroll API;
- чинить другие ошибки, если они не связаны напрямую с QuestMenu readability.

Если полноценный ScrollWidget окажется рискованным:
- не форсировать его;
- сделать безопасный fallback или описать ограничение;
- предложить следующую задачу.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. В QuestUI.c нет:
   - VScrollToPos01
   - VScrollStep
   - IsScrollbarVisible

2. QuestMenu.layout синтаксически целый.

3. QuestUI.c синтаксически целый.

4. QuestMenu имеет реальный тёмный фон / backing.

5. DescriptionText имеет безопасный способ чтения длинного текста.

6. DialogText имеет безопасный способ чтения длинного текста.

7. Кнопки не переименованы и не сломаны.

8. Mapping Description/Dialog не изменён.

9. Кириллица не повреждена.

10. CHANGED FILES содержит только:
   - QuestMenu.layout
   - QuestUI.c

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. QuestMenu получил реально рабочий тёмный фон / подложку.
2. Игровой мир и action prompt визуально не мешают чтению.
3. Длинный DescriptionText можно прочитать безопасным способом.
4. Длинный DialogText можно прочитать безопасным способом.
5. Неподдерживаемый MultilineTextWidget API не используется.
6. Изменены только разрешённые файлы.
7. JSON не менялся.
8. Server не менялся.
9. Quest Editor не менялся.
10. Documentation не менялась.
11. Агент вернул отчёт в чат.
12. В отчёте указан выбранный способ:
   - ScrollWidget / ScrollPanel;
   - или page/fallback.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что исправлено по фону;
- какой widget/type использован для реального фона;
- что сделано для DescriptionText;
- что сделано для DialogText;
- подтверждение, что unsupported API не возвращался.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

DIFF:
- кратко описать изменения layout;
- кратко описать изменения QuestUI.c;
- указать, выбран ScrollWidget/ScrollPanel или fallback/page approach.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- если ScrollWidget не применён — почему;
- какие ограничения остались.

QUESTIONS:
- только реальные вопросы, если есть.

RECOMMENDED NEXT TASK:
- если нужно, предложить отдельную задачу на QuestJournal;
- если нужно, предложить отдельную задачу на полноценный scrollbar.

CONCLUSION:
- краткий вывод: QuestMenu должен стать читаемым, фон должен перекрывать мир, длинные тексты должны иметь безопасный способ чтения.

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

TASK 082 REVIEW

Статус:
Принято условно.

Что принято:
- В QuestMenu.layout добавлен full-screen BackgroundOverlay.
- В QuestJournal.layout добавлен full-screen BackgroundOverlay.
- Усилен фон QuestPanel и QuestJournalPanel.
- В QuestMenu возвращён label “Список квестов” над QuestListbox.
- Для DescriptionPanel и DialogPanel добавлена wheel-прокрутка через widget scroll API.
- Для QuestJournal добавлена wheel-прокрутка DescriptionText.
- Action prompt не отключался и input/action-система не трогалась.
- JSON-контракт не менялся.
- Server profile не трогался.
- Mapping DescriptionPanel / DialogPanel не переписывался.
- Кириллица по отчёту не повреждена.
- Forbidden files не менялись.

Что требует ручной проверки в игре:
- Реально ли full-screen overlay затемняет мир и action prompt достаточно сильно.
- Работает ли VScrollStep / VScrollToPos01 на MultilineTextWidget в DayZ runtime.
- Работает ли wheel-scroll в QuestMenu для DescriptionText и DialogText.
- Работает ли wheel-scroll в QuestJournal.
- Видна ли подпись “Список квестов” и не перекрывает ли она QuestListbox.
- Не сломалось ли управление кнопками.
- Не сломались ли Offer / Completion / Reward.

Риски:
- Scroll реализован не через отдельный scroll-container, а через wheel-scroll на MultilineTextWidget. Это может работать, но требует проверки в runtime.
- Для QuestJournal агент сам отметил риск: курсор там по-прежнему не включается, поэтому wheel-scroll может потребовать отдельной проверки.
- Если VScrollStep окажется нестабильным или не сработает, следующая задача должна быть только про явный DayZ-compatible scroll container / scrollbar.

Вывод:
TASK 082 выполнен в рамках жёстких рамок. Следующий шаг — собрать клиентский PBO и проверить QuestMenu / QuestJournal в игре.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
