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

TASK 098 — QuestMenu: тест clipping через ContentParent SizeToChild для ScrollWidget + MultilineTextWidget

Статус:
Узкая практическая UI-задача после аналитики TASK 097.

--------------------------------------------------------------------------------
КОНТЕКСТ
--------------------------------------------------------------------------------

Текущая рабочая база QuestMenu:

5ccb7c4d24de2ac0691136e64ad29e2789708874

Это состояние считается рабочим baseline.

В этом baseline:

1. QuestMenu визуально целый.
2. DescriptionPanel находится на правильном месте.
3. DialogPanel находится на правильном месте.
4. RoutePanel находится на правильном месте.
5. QuestListbox находится на правильном месте.
6. Кнопки находятся на правильном месте.
7. Description/Dialog текст виден.
8. Native wrap работает.
9. Scrollbar работает.
10. QuestListbox и TriggerRouteListbox работают как раньше.

Текущая проблема одна:

При прокрутке текст `DescriptionText` / `DialogText` выходит за границы своего scroll-окна и визуально накладывается на другие области.

Текущий рабочий стек:

Description:
ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Dialog:
ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Аналитика TASK 097 показала, что самый безопасный следующий тест — не менять общую геометрию QuestMenu, а добавить промежуточный vanilla-like content parent между ScrollWidget и MultilineTextWidget:

ScrollWidgetClass
  FrameWidgetClass ContentParent
    MultilineTextWidgetClass

Reference-направление:
vanilla pattern с промежуточным content parent, например `FrameWidgetClass ContentParent` с `scriptclass "SizeToChild"`.

--------------------------------------------------------------------------------
ЦЕЛЬ TASK 098
--------------------------------------------------------------------------------

Проверить, решает ли промежуточный `ContentParent` проблему clipping/viewport для текущей рабочей схемы:

ScrollWidgetClass
  MultilineTextWidgetClass

Нужно тестово заменить direct-child схему на:

Description:
ScrollWidgetClass DescriptionScroll
  FrameWidgetClass DescriptionContent
    MultilineTextWidgetClass DescriptionText

Dialog:
ScrollWidgetClass DialogScroll
  FrameWidgetClass DialogContent
    MultilineTextWidgetClass DialogText

Главная цель:

- сохранить текущую рабочую геометрию QuestMenu;
- сохранить MultilineTextWidget;
- сохранить native wrap;
- сохранить ScrollWidget;
- проверить, начнёт ли scroll viewport корректно ограничивать видимость текста;
- не менять расположение окон.

--------------------------------------------------------------------------------
ЖЁСТКИЕ РАМКИ
--------------------------------------------------------------------------------

Агент обязан:

- делать только то, что прямо указано в этой задаче;
- не чинить “заодно” соседние проблемы;
- не менять архитектуру квестов;
- не менять JSON-контракт;
- не трогать server;
- не трогать Quest Editor;
- не трогать QuestJournal;
- не трогать @Trader;
- не делать большой редизайн QuestMenu;
- не менять размеры основных панелей;
- не менять позиции основных панелей;
- не делать git commit / push / reset / clean;
- если `scriptclass "SizeToChild"` вызывает проблемы — не уходить в новую архитектуру, а описать в PROBLEMS.

Запрещено:

- переходить на RichTextWidget;
- переходить на MultilineEditBoxWidget;
- переходить на HtmlWidget;
- возвращать TextListboxWidget для Description/Dialog;
- менять QuestListbox;
- менять TriggerRouteListbox;
- менять RoutePanel;
- менять кнопки;
- менять Offer / Completion / Reward logic;
- менять JSON;
- менять server;
- менять QuestJournal;
- менять @Trader;
- перепаковывать PBO;
- запускать Addon Builder.

--------------------------------------------------------------------------------
ЧТО НУЖНО ПРОЧИТАТЬ ПЕРЕД НАЧАЛОМ
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

Можно читать vanilla/reference на диске D:

13. D:\Dayz\gui\layouts\
14. D:\Dayz\scripts\5_mission\gui\
15. D:\Dayz\scripts\

Особенно свериться с reference из TASK 097:

- `D:\Dayz\gui\layouts\inventory_new\wide\left_area.layout`
- `D:\Dayz\scripts\5_mission\gui\inventorynew\areas\leftarea.c`
- любые места, где есть `FrameWidgetClass ContentParent`
- любые места, где используется `scriptclass "SizeToChild"`
- любые места, где `ScrollWidget` содержит промежуточный content parent

--------------------------------------------------------------------------------
РАЗРЕШЁННЫЕ ФАЙЛЫ ДЛЯ ПРАВОК
--------------------------------------------------------------------------------

Разрешено менять только:

1. P:\Silver_77_Quests\Silver_77_Quests_Client\gui\QuestMenu.layout

2. P:\Silver_77_Quests\Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c

3. P:\Silver_77_Quests\DayZ_layout\*

DayZ_layout менять только если текущий viewer явно устарел относительно рабочей базы и мешает проверить структуру.

Если viewer менять:
- только минимально;
- вернуть sanity-check к текущей рабочей схеме `ScrollWidget + MultilineTextWidget`;
- добавить проверку `DescriptionContent/DialogContent`, если они появятся;
- не переписывать viewer полностью;
- не делать redesign viewer.

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
- переход на HtmlWidget;
- возврат к TextListboxWidget для Description/Dialog.

--------------------------------------------------------------------------------
БАЗОВАЯ ГЕОМЕТРИЯ, КОТОРУЮ НЕЛЬЗЯ ЛОМАТЬ
--------------------------------------------------------------------------------

Не менять эти позиции и размеры:

DescriptionPanel:
- position 300 74
- size 430 204

DescriptionScroll:
- position 8 34
- size 414 156

DialogPanel:
- position 300 286
- size 430 122

DialogScroll:
- position 8 34
- size 414 74

RoutePanel:
- position 740 74
- size 210 334

QuestListbox:
- position 30 82
- size 250 450

AcceptButton:
- position 300 420
- size 210 48

CompleteButton:
- position 530 420
- size 210 48

CloseButton:
- position 300 490
- size 650 48

--------------------------------------------------------------------------------
ЧТО ИМЕННО НУЖНО СДЕЛАТЬ
--------------------------------------------------------------------------------

1. Изменить Description stack в QuestMenu.layout

Было:

ScrollWidgetClass DescriptionScroll
  MultilineTextWidgetClass DescriptionText

Нужно тестово сделать:

ScrollWidgetClass DescriptionScroll
  FrameWidgetClass DescriptionContent
    MultilineTextWidgetClass DescriptionText

Требования:

- `DescriptionScroll` остаётся `ScrollWidgetClass`.
- `DescriptionScroll position` не менять.
- `DescriptionScroll size` не менять.
- `DescriptionText` остаётся `MultilineTextWidgetClass`.
- `DescriptionText` имя не менять.
- `DescriptionText` должен остаться шириной примерно 394.
- `DescriptionText` должен сохранить:
  - wrap 1
  - "size to text v" 0
  - font "gui/fonts/metron16"
  - текущий цвет
  - text halign left
  - text valign top
- `DescriptionText size 394 600` можно оставить как в baseline для первого теста.
- Между `DescriptionScroll` и `DescriptionText` добавить `FrameWidgetClass DescriptionContent`.
- Для `DescriptionContent` проверить vanilla-like настройки:
  - position 0 0
  - size примерно 394 600
  - clipchildren 1 или 0 — выбрать по reference и объяснить в отчёте
  - scriptclass "SizeToChild", если это подтверждено reference и синтаксис безопасен
- Не менять расположение `DescriptionPanel`.

2. Изменить Dialog stack в QuestMenu.layout

Было:

ScrollWidgetClass DialogScroll
  MultilineTextWidgetClass DialogText

Нужно тестово сделать:

ScrollWidgetClass DialogScroll
  FrameWidgetClass DialogContent
    MultilineTextWidgetClass DialogText

Требования:

- `DialogScroll` остаётся `ScrollWidgetClass`.
- `DialogScroll position` не менять.
- `DialogScroll size` не менять.
- `DialogText` остаётся `MultilineTextWidgetClass`.
- `DialogText` имя не менять.
- `DialogText` должен остаться шириной примерно 394.
- `DialogText` должен сохранить:
  - wrap 1
  - "size to text v" 0
  - font "gui/fonts/metron16"
  - текущий цвет
  - text halign left
  - text valign top
- `DialogText size 394 300` можно оставить как в baseline для первого теста.
- Между `DialogScroll` и `DialogText` добавить `FrameWidgetClass DialogContent`.
- Для `DialogContent` проверить vanilla-like настройки:
  - position 0 0
  - size примерно 394 300
  - clipchildren 1 или 0 — выбрать по reference и объяснить в отчёте
  - scriptclass "SizeToChild", если это подтверждено reference и синтаксис безопасен
- Не менять расположение `DialogPanel`.

3. QuestUI.c

Проверить, нужно ли добавить ссылки:

- Widget m_DescriptionContent
- Widget m_DialogContent

Если для `SizeToChild` и статичного child-size это не нужно:
- не добавлять лишний код.

Если нужно обновлять content parent после SetText:
- добавить минимально;
- не менять общую логику;
- не вызывать scroll API на MultilineTextWidget;
- reset scroll оставить только через ScrollWidget.

Сохранить:

- private MultilineTextWidget m_QuestDescription;
- private MultilineTextWidget m_DialogText;
- касты `MultilineTextWidget.Cast(...)`;
- SetLineBreakingOverride для Description/Dialog, если он сейчас есть;
- SetText;
- Update;
- ResetScrollWidgetToTop(m_DescriptionScroll);
- ResetScrollWidgetToTop(m_DialogScroll).

4. DayZ_layout viewer

Проверить viewer по правилу SplitDoc.

Сейчас viewer мог остаться частично под старый RichText experiment.

Если viewer sanity-check ожидает `RichTextWidgetClass` или `DescriptionContent/DialogContent` от старого эксперимента неправильно:
- минимально обновить sanity-check под текущую задачу;
- viewer должен понимать:
  - DescriptionScroll
  - DialogScroll
  - DescriptionContent, если добавлен
  - DialogContent, если добавлен
  - MultilineTextWidgetClass DescriptionText/DialogText

Важно:
viewer не обязан идеально симулировать runtime clipping, но должен не вводить в заблуждение по структуре.

--------------------------------------------------------------------------------
ПРОВЕРКИ
--------------------------------------------------------------------------------

После правок проверить:

1. QuestMenu.layout синтаксически целый.
2. QuestUI.c синтаксически целый.
3. DescriptionPanel position/size не изменились.
4. DialogPanel position/size не изменились.
5. RoutePanel position/size не изменились.
6. QuestListbox position/size не изменились.
7. AcceptButton/CompleteButton/CloseButton position/size не изменились.
8. DescriptionScroll остался ScrollWidgetClass.
9. DialogScroll остался ScrollWidgetClass.
10. DescriptionText остался MultilineTextWidgetClass.
11. DialogText остался MultilineTextWidgetClass.
12. DescriptionContent добавлен как FrameWidgetClass, если выбран этот вариант.
13. DialogContent добавлен как FrameWidgetClass, если выбран этот вариант.
14. RichTextWidget не используется.
15. MultilineEditBoxWidget не используется.
16. HtmlWidget не используется.
17. TextListboxWidget для Description/Dialog не возвращён.
18. QuestListbox не изменён.
19. TriggerRouteListbox не изменён.
20. JSON не менялся.
21. Server не менялся.
22. QuestJournal не менялся.
23. @Trader не менялся.
24. DayZ_layout viewer проверен.
25. Кириллица не повреждена.

--------------------------------------------------------------------------------
КОДИРОВКА
--------------------------------------------------------------------------------

Задача затрагивает .layout, возможно .c и возможно viewer html.

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

1. Текущая рабочая геометрия QuestMenu сохранена.
2. Description/Dialog получили промежуточный content parent или агент честно объяснил, почему не стал его добавлять.
3. MultilineTextWidget сохранён.
4. ScrollWidget сохранён.
5. RichTextWidget не используется.
6. MultilineEditBoxWidget не используется.
7. TextListboxWidget для Description/Dialog не возвращён.
8. QuestListbox и TriggerRouteListbox не изменены.
9. Quest logic не изменена.
10. JSON не изменён.
11. Server не изменён.
12. QuestJournal не изменён.
13. DayZ_layout viewer проверен и при необходимости минимально обновлён.
14. Агент вернул отчёт в чат.

Если `ContentParent + SizeToChild` не компилируется или ломает layout:
- задача всё равно может считаться выполненной как тест, если агент не ушёл в новую архитектуру, а честно описал проблему в PROBLEMS и предложил следующий безопасный вариант.

--------------------------------------------------------------------------------
ОЖИДАЕМЫЙ ОТЧЁТ
--------------------------------------------------------------------------------

AGENT REPORT

DONE:
- что изменено в QuestMenu.layout;
- что изменено в QuestUI.c;
- добавлен ли DescriptionContent/DialogContent;
- использован ли `scriptclass "SizeToChild"`;
- менялся ли DayZ_layout viewer.

CHANGED FILES:
- Silver_77_Quests_Client\gui\QuestMenu.layout
- Silver_77_Quests_Client\scripts\5_Mission\QuestUI.c, если менялся
- DayZ_layout\..., если менялся

DIFF:
- кратко описать изменения layout;
- кратко описать изменения script;
- кратко описать изменения viewer, если были.

BASELINE GEOMETRY CHECK:
- подтвердить, что позиции и размеры DescriptionPanel/DialogPanel/RoutePanel/QuestListbox/buttons не изменились.

COMPILE-SAFETY CHECK:
- нет cast mismatch;
- DescriptionText/DialogText остались MultilineTextWidget;
- scroll API только на ScrollWidget;
- QuestListbox/TriggerRouteListbox сохранены.

DAYZ_LAYOUT CHECK:
- показывает ли viewer текущий stack;
- обновлялся ли viewer;
- какие ограничения viewer остаются.

RUNTIME EXPECTATION:
- что должно измениться в игре;
- что нужно проверить руками.

ENCODING CHECK:
- указать, что кириллица не повреждена;
- указать, что массовая перекодировка не выполнялась.

PROBLEMS:
- что не удалось проверить без запуска игры;
- если ContentParent/SizeToChild не подходит — почему.

RECOMMENDED NEXT TASK:
- если clipping исправится — runtime polish;
- если clipping не исправится — отдельная задача на следующий вариант;
- если layout сломается — откат к baseline `5ccb7c4d24de2ac0691136e64ad29e2789708874`.

CONCLUSION:
- краткий вывод: тест ContentParent выполнен или почему остановлен.

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

TASK 098 REVIEW

Статус:
Принято условно.

Что принято:
- Description/Dialog остались на MultilineTextWidget.
- RichTextWidget не используется.
- MultilineEditBoxWidget не используется.
- TextListboxWidget для Description/Dialog не возвращён.
- QuestUI.c не трогался.
- Scroll API остаётся только на ScrollWidget.
- Базовые панели и кнопки по отчёту не сдвигались.
- Внутри DescriptionScroll добавлен DescriptionContent.
- Внутри DialogScroll добавлен DialogContent.
- Viewer обновлён под текущий тестовый stack.

Что теперь надо проверить в игре:
1. Компилируется ли layout с `scriptclass "SizeToChild"`.
2. QuestMenu открывается без runtime error.
3. Окна остались на местах.
4. Description текст виден.
5. Dialog текст виден.
6. Scrollbar работает.
7. Wrap сохранился.
8. Текст больше не выходит за границы DescriptionScroll.
9. Текст больше не выходит за границы DialogScroll.
10. Кнопки работают.
11. RoutePanel работает.
12. QuestListbox работает.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
