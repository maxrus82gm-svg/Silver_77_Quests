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

TASK 084 — Hotfix compile error после TASK 082: убрать неподдерживаемый scroll API из Quest UI

Статус:
Новая активная срочная задача для агента.

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
- не трогать JSON, server, editor, layout и documentation;
- если найден другой способ scroll или другая UI-проблема — записать в PROBLEMS / RECOMMENDED NEXT TASK, но не исправлять в этой задаче.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

После TASK 082 были собраны моды, обновлены на сервере и выполнен запуск.

При старте сервер/клиент получил compile error:

Can't compile "Mission" script module!

Silver_77_Quests/scripts/5_Mission/questjournalui.c(182):
Undefined function 'MultilineTextWidget.VScrollToPos01'

Актуальный commit / hash:

296532875c6ab908cc1f20c2bb4f2f7ab3eb1904

Ошибка связана с изменениями TASK 082, где была добавлена экспериментальная scroll-логика для MultilineTextWidget через методы:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible

В текущем DayZ runtime эти методы у MultilineTextWidget не доступны, поэтому Mission script module не компилируется.

Важно:
TASK 082 была принята условно, и в REVIEW уже был отмечен риск, что scroll через VScrollStep / VScrollToPos01 требует проверки в runtime.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 084
--------------------------------------------------------------------------------

Вернуть компиляцию Mission script module.

Нужно убрать или безопасно отключить неподдерживаемый scroll API из Quest UI кода.

Главная цель:
мод должен снова компилироваться и запускаться.

Это не задача на полноценную реализацию scroll.

--------------------------------------------------------------------------------
МОЁ МНЕНИЕ / ПРЕДПОЧТИТЕЛЬНОЕ РЕШЕНИЕ
--------------------------------------------------------------------------------

Предпочтительное решение:

Сделать минимальный hotfix.

Не откатывать всю TASK 082.
Не трогать overlay.
Не трогать layout.
Не трогать подпись “Список квестов”.
Не трогать цвета, панели, размеры и mapping Description/Dialog.

Нужно убрать только ту часть, которая ломает компиляцию:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible
- helper-функции, которые их вызывают;
- OnMouseWheel-логику, если она зависит от этих helper-функций.

Лучший безопасный вариант:

1. В QuestJournalUI.c:
   - сделать ResetScrollableText безопасной пустой функцией или удалить её вызовы;
   - убрать вызовы VScrollToPos01;
   - отключить HandleScrollableTextWheel, если он использует VScrollStep / IsScrollbarVisible;
   - OnMouseWheel должен снова работать только для выбора квестов в списке, без scroll MultilineTextWidget.

2. В QuestUI.c:
   - сделать ResetScrollableText безопасной пустой функцией или удалить её вызовы;
   - убрать вызовы VScrollToPos01;
   - отключить HandleScrollableTextWheel, если он использует VScrollStep / IsScrollbarVisible;
   - OnMouseWheel не должен вызывать неподдерживаемый scroll API.

3. Если после удаления этих вызовов длинные тексты снова обрезаются:
   - это ожидаемый временный откат scroll-функции;
   - записать это в PROBLEMS;
   - предложить отдельную следующую задачу на DayZ-compatible scroll.

--------------------------------------------------------------------------------
SCOPE ЗАДАЧИ
--------------------------------------------------------------------------------

Это срочный compile hotfix.

Агент должен:
- найти все использования неподдерживаемых методов:
  - VScrollToPos01
  - VScrollStep
  - IsScrollbarVisible
- убрать или безопасно отключить эти вызовы;
- проверить оба UI-файла, потому что ошибка показалась в QuestJournalUI.c, но похожая логика есть и в QuestUI.c;
- сохранить остальную UI-логику без изменений;
- вернуть отчёт в чат.

Агент не должен:
- реализовывать новый scroll;
- менять layout;
- менять JSON;
- менять server-side код;
- менять Quest Editor;
- менять RPC/sync;
- менять Documentation;
- чинить другие найденные проблемы;
- делать commit/push/reset/clean.

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c

2. D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

--------------------------------------------------------------------------------
ЗАПРЕЩЁННЫЕ ФАЙЛЫ И ДЕЙСТВИЯ
--------------------------------------------------------------------------------

Запрещено менять:

- D:\GitHub\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout
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

1. Проверить QuestJournalUI.c

Найти все использования:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible
- HandleScrollableTextWheel
- ResetScrollableText
- OnMouseWheel

Исправить так, чтобы файл больше не вызывал неподдерживаемые методы MultilineTextWidget.

Допустимый безопасный вариант:

- ResetScrollableText оставить как пустую no-op функцию;
- HandleScrollableTextWheel удалить или сделать всегда return false;
- OnMouseWheel оставить для переключения квестов в списке, если эта логика уже была рабочей;
- не использовать VScrollToPos01 / VScrollStep / IsScrollbarVisible.

Пример безопасной no-op функции:

void ResetScrollableText(MultilineTextWidget widget)
{
    // Scroll reset disabled: MultilineTextWidget does not support VScrollToPos01 in current DayZ runtime.
}

Пример безопасного отключения scroll helper:

bool HandleScrollableTextWheel(Widget w, MultilineTextWidget textWidget, int wheel)
{
    return false;
}

2. Проверить QuestUI.c

Найти все использования:

- VScrollToPos01
- VScrollStep
- IsScrollbarVisible
- HandleScrollableTextWheel
- ResetScrollableText
- OnMouseWheel

Исправить так, чтобы файл больше не вызывал неподдерживаемые методы MultilineTextWidget.

Допустимый безопасный вариант:

void ResetScrollableText(MultilineTextWidget widget)
{
    // Scroll reset disabled: MultilineTextWidget does not support VScrollToPos01 in current DayZ runtime.
}

Для варианта helper-а с panelWidget:

bool HandleScrollableTextWheel(Widget w, Widget panelWidget, MultilineTextWidget textWidget, int wheel)
{
    return false;
}

3. Не трогать layout

Не менять:

- QuestMenu.layout
- QuestJournal.layout

Overlay и визуальные изменения TASK 082 оставить как есть.

4. Не реализовывать новый scroll

Если агент знает другой DayZ-compatible способ scroll:
- не внедрять его сейчас;
- описать в RECOMMENDED NEXT TASK.

5. Проверить синтаксис

После правок проверить, что в разрешённых файлах:
- нет вызовов VScrollToPos01;
- нет вызовов VScrollStep;
- нет вызовов IsScrollbarVisible;
- нет очевидных синтаксических ошибок;
- нет незакрытых скобок;
- нет ссылок на удалённые функции.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .c файлы с русским текстом.

Нужно соблюдать ENCODING_RULES.md:

- не делать массовую перекодировку;
- не ломать кириллицу;
- не менять текстовые строки без необходимости;
- не сохранять файл в неправильной кодировке;
- если файл был UTF-8 без BOM — сохранить UTF-8 без BOM;
- в отчёте указать ENCODING CHECK.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ ДЛЯ ЭТОЙ ЗАДАЧИ
--------------------------------------------------------------------------------

Это только hotfix compile error.

Разрешено исправить только ошибку неподдерживаемого scroll API.

Нельзя:
- переделывать UI;
- улучшать scroll;
- менять layout;
- менять размеры панелей;
- менять фон;
- менять список квестов;
- менять кнопки;
- менять квестовую логику;
- менять server sync;
- менять JSON;
- менять документацию;
- чинить другие ошибки, если они не связаны напрямую с текущим compile error.

Если после исправления появится новая compile error в этих же двух разрешённых файлах и она напрямую связана с удалением scroll helper-ов:
- можно исправить её в рамках этих же двух файлов.

Если новая ошибка находится в другом файле:
- не исправлять;
- записать в PROBLEMS;
- предложить следующую задачу.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. В QuestJournalUI.c больше нет:
   - VScrollToPos01
   - VScrollStep
   - IsScrollbarVisible

2. В QuestUI.c больше нет:
   - VScrollToPos01
   - VScrollStep
   - IsScrollbarVisible

3. OnMouseWheel не вызывает неподдерживаемый MultilineTextWidget scroll API.

4. ResetScrollableText не вызывает неподдерживаемый API.

5. HandleScrollableTextWheel не вызывает неподдерживаемый API или удалён безопасно.

6. Код синтаксически целый.

7. Кириллица не повреждена.

8. CHANGED FILES содержит только:
   - QuestJournalUI.c
   - QuestUI.c

--------------------------------------------------------------------------------
КРИТЕРИИ ГОТОВНОСТИ
--------------------------------------------------------------------------------

Задача считается выполненной, если:

1. Compile error по MultilineTextWidget.VScrollToPos01 устранён.
2. В коде больше нет вызовов VScrollToPos01.
3. В коде больше нет вызовов VScrollStep.
4. В коде больше нет вызовов IsScrollbarVisible.
5. Изменены только два разрешённых файла.
6. Layout не менялся.
7. JSON не менялся.
8. Server не менялся.
9. Documentation не менялась.
10. Агент вернул отчёт в чат.
11. В отчёте явно указано, что scroll временно отключён / отложен в отдельную задачу.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что исправлено;
- какие unsupported API вызовы удалены/отключены;
- что QuestJournalUI.c проверен;
- что QuestUI.c проверен.

CHANGED FILES:
- Silver_77_Quests_Client\scripts\5_Mission\QuestJournalUI.c
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

DIFF:
- кратко описать, что изменено;
- например:
  - ResetScrollableText стал no-op;
  - HandleScrollableTextWheel отключён / удалён;
  - OnMouseWheel больше не вызывает MultilineTextWidget scroll API.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- длинные тексты снова могут обрезаться, потому что экспериментальный scroll отключён;
- полноценный scroll требует отдельной DayZ-compatible задачи.

QUESTIONS:
- только реальные вопросы, если есть.

RECOMMENDED NEXT TASK:
- TASK 085 — найти DayZ-compatible способ scroll для QuestMenu / QuestJournal без неподдерживаемых методов MultilineTextWidget.

CONCLUSION:
- compile hotfix выполнен;
- неподдерживаемый scroll API убран;
- мод должен пройти дальше этапа Mission script compile.

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
