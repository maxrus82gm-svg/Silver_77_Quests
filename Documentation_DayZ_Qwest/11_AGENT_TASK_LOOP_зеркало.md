# AGENT TASK LOOP

Это рабочий shell-документ текущего агентского цикла.

Он нужен для того, чтобы хранить:

- текущую задачу в `БЛОКЕ 1`;
- последний `AGENT REPORT` в `БЛОКЕ 2`;
- последний `REVIEW / STATUS` в `БЛОКЕ 3`;
- короткую карту чтения `SplitDoc`.

Правила:

- Постоянные правила: `Documentation/SplitDoc/START.md`
- Тематические правила: по матрице `SplitDoc` ниже

## Что читать всегда

Агент всегда читает:

- `Documentation/AGENT_TASK_LOOP.md`
- `Documentation/SplitDoc/START.md`

## Матрица чтения SplitDoc

- `UI / QuestMenu / QuestJournal` -> `QUEST_UI_RULES.md` + `QUEST_JSON_CONTRACT.md` + `ENCODING_RULES.md`
- `Quest Editor / JSON_Quvest` -> `QUEST_EDITOR_RULES.md` + `QUEST_JSON_CONTRACT.md` + `SERVER_PROFILE_RULES.md`
- `JSON contract / quest logic` -> `QUEST_JSON_CONTRACT.md` + `Documentation/QUEST_LOGIC_SPEC.md`
- `DayZ layout viewer` -> `DAYZ_LAYOUT_VIEWER_RULES.md` + `ENCODING_RULES.md`
- `RPC / client-server sync` -> `DAYZ_RPC_SYNC_RULES.md`
- `Git / REVIEW / commit / push check / история Git` -> `GIT_REVIEW_RULES.md`
- `Encoding / кириллица` -> `ENCODING_RULES.md`
- `Server profile / dev JSON / перенос на сервер` -> `SERVER_PROFILE_RULES.md`
- `History / status / прошлые решения` -> `TASK_HISTORY.md`

================================================================================
# 🟦 БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА ДЛЯ АГЕНТА
================================================================================

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

TASK 109 — Перенести нужные SplitDoc и проектные документы в Obsidian

Цель:  
Продолжить переход на Obsidian-базу `Documentation_DayZ_Qwest`.

Нужно перенести в Obsidian нужные рабочие документы из `Documentation` и `Documentation/SplitDoc` по принципу:

- не пересказ;
    
- не краткое зеркало;
    
- а полная копия содержимого в Obsidian;
    
- старые оригиналы пока не удалять.
    

Важно:  
Старые рабочие документы пока остаются источником правды.  
Obsidian-копии пока являются зеркалами для навигации, чтения, связей и будущего переключения системы.

Читать:

- Documentation/AGENT_TASK_LOOP.md
    
- Documentation/SplitDoc/START.md
    
- Documentation/SplitDoc/TASK_HISTORY.md
    
- Documentation_DayZ_Qwest/00_Главная.md
    
- Documentation_DayZ_Qwest/10_Правила_агента.md
    
- Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md
    
- Documentation_DayZ_Qwest/12_START_зеркало.md
    

Сделать:

1. Создать внутри Obsidian-базы папку:
    
    Documentation_DayZ_Qwest/20_SplitDoc
    
2. В папку `Documentation_DayZ_Qwest/20_SplitDoc` перенести полными копиями следующие документы из `Documentation/SplitDoc`:
    
    - Documentation/SplitDoc/GIT_REVIEW_RULES.md
        
    - Documentation/SplitDoc/TASK_HISTORY.md
        
    - Documentation/SplitDoc/QUEST_UI_RULES.md
        
    - Documentation/SplitDoc/QUEST_JSON_CONTRACT.md
        
    - Documentation/SplitDoc/ENCODING_RULES.md
        
    - Documentation/SplitDoc/QUEST_EDITOR_RULES.md
        
    - Documentation/SplitDoc/SERVER_PROFILE_RULES.md
        
    - Documentation/SplitDoc/DAYZ_LAYOUT_VIEWER_RULES.md
        
    - Documentation/SplitDoc/DAYZ_RPC_SYNC_RULES.md
        
    
    Имена файлов в Obsidian оставить такими же:
    
    - GIT_REVIEW_RULES.md
        
    - TASK_HISTORY.md
        
    - QUEST_UI_RULES.md
        
    - QUEST_JSON_CONTRACT.md
        
    - ENCODING_RULES.md
        
    - QUEST_EDITOR_RULES.md
        
    - SERVER_PROFILE_RULES.md
        
    - DAYZ_LAYOUT_VIEWER_RULES.md
        
    - DAYZ_RPC_SYNC_RULES.md
        
3. Создать индекс:
    
    Documentation_DayZ_Qwest/20_SplitDoc/00_INDEX.md
    
    В нём сделать список ссылок на все перенесённые SplitDoc:
    
    # 20_SplitDoc — зеркала правил
    
    Это Obsidian-зеркала рабочих документов из `Documentation/SplitDoc`.
    
    Пока источником правды остаётся старая папка:  
    `Documentation/SplitDoc`
    
    ## Документы
    
    - [[GIT_REVIEW_RULES]]
        
    - [[TASK_HISTORY]]
        
    - [[QUEST_UI_RULES]]
        
    - [[QUEST_JSON_CONTRACT]]
        
    - [[ENCODING_RULES]]
        
    - [[QUEST_EDITOR_RULES]]
        
    - [[SERVER_PROFILE_RULES]]
        
    - [[DAYZ_LAYOUT_VIEWER_RULES]]
        
    - [[DAYZ_RPC_SYNC_RULES]]
        
4. Создать внутри Obsidian-базы папку:
    
    Documentation_DayZ_Qwest/21_Project_Docs
    
5. В папку `Documentation_DayZ_Qwest/21_Project_Docs` перенести полными копиями следующие важные проектные документы из `Documentation`:
    
    - Documentation/QUEST_LOGIC_SPEC.md
        
    - Documentation/RUSSIAN_ENCODING.md
        
    - Documentation/README_JSON_CONFIG.md
        
    - Documentation/SPLIT_CLIENT_SERVER.md
        
    - Documentation/BUILD.md
        
    - Documentation/README_INSTALLATION.md
        
    - Documentation/README.md
        
    - Documentation/CHANGELOG.md
        
    
    Имена файлов в Obsidian оставить такими же:
    
    - QUEST_LOGIC_SPEC.md
        
    - RUSSIAN_ENCODING.md
        
    - README_JSON_CONFIG.md
        
    - SPLIT_CLIENT_SERVER.md
        
    - BUILD.md
        
    - README_INSTALLATION.md
        
    - README.md
        
    - CHANGELOG.md
        
6. Создать индекс:
    
    Documentation_DayZ_Qwest/21_Project_Docs/00_INDEX.md
    
    В нём сделать список ссылок:
    
    # 21_Project_Docs — зеркала проектных документов
    
    Это Obsidian-зеркала важных проектных документов из `Documentation`.
    
    Пока источником правды остаётся старая папка:  
    `Documentation`
    
    ## Документы
    
    - [[QUEST_LOGIC_SPEC]]
        
    - [[RUSSIAN_ENCODING]]
        
    - [[README_JSON_CONFIG]]
        
    - [[SPLIT_CLIENT_SERVER]]
        
    - [[BUILD]]
        
    - [[README_INSTALLATION]]
        
    - [[README]]
        
    - [[CHANGELOG]]
        
7. В `Documentation_DayZ_Qwest/00_Главная.md` добавить раздел:
    
    ## Перенесённые документы
    
    - [[20_SplitDoc/00_INDEX|20_SplitDoc — зеркала правил]]
        
    - [[21_Project_Docs/00_INDEX|21_Project_Docs — проектные документы]]
        
8. В `Documentation_DayZ_Qwest/10_Правила_агента.md` добавить уточнение:
    
    - `20_SplitDoc` содержит Obsidian-зеркала тематических правил.
        
    - `21_Project_Docs` содержит Obsidian-зеркала важных проектных документов.
        
    - Пока источником правды остаются старые пути:
        
        - `Documentation/SplitDoc`
            
        - `Documentation`
            
    - Удаление старых документов и переключение системы на Obsidian будет отдельной задачей.
        
9. В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавить запись о TASK 109 как о переносе нужных документов в Obsidian.
    
10. В `Documentation/SplitDoc/TASK_HISTORY.md` добавить короткую запись о TASK 109.
    
11. Обновить AGENT REPORT в `Documentation/AGENT_TASK_LOOP.md`.
    
12. После успешного выполнения очистить текущий БЛОК 1 в `Documentation/AGENT_TASK_LOOP.md` по правилу из START.md.
    

Разрешено менять:

- Documentation/AGENT_TASK_LOOP.md
    
- Documentation/SplitDoc/TASK_HISTORY.md
    
- Documentation_DayZ_Qwest/00_Главная.md
    
- Documentation_DayZ_Qwest/10_Правила_агента.md
    
- Documentation_DayZ_Qwest/05_Очередь_задач.md
    

Разрешено создавать:

- Documentation_DayZ_Qwest/20_SplitDoc/00_INDEX.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/GIT_REVIEW_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/QUEST_UI_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/QUEST_JSON_CONTRACT.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/QUEST_EDITOR_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/SERVER_PROFILE_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/DAYZ_LAYOUT_VIEWER_RULES.md
    
- Documentation_DayZ_Qwest/20_SplitDoc/DAYZ_RPC_SYNC_RULES.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/00_INDEX.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/QUEST_LOGIC_SPEC.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/RUSSIAN_ENCODING.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/README_JSON_CONFIG.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/SPLIT_CLIENT_SERVER.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/BUILD.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/README_INSTALLATION.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/README.md
    
- Documentation_DayZ_Qwest/21_Project_Docs/CHANGELOG.md
    

Запрещено:

- не удалять старые документы из `Documentation`
    
- не удалять старые документы из `Documentation/SplitDoc`
    
- не переключать систему на Obsidian-путь
    
- не менять код мода
    
- не менять JSON квестов
    
- не менять layout-файлы
    
- не менять server profile/runtime-данные
    
- не трогать PBO/моды
    
- не делать git commit / git push
    
- не менять `.obsidian/workspace.json`, `.obsidian/graph.json`, `.trash`, если они изменились только от открытия Obsidian
    
- не переносить CODEX_* документы в этой задаче
    
- не переносить `STARTER_QUEST_CONFIG.json` в этой задаче
    

Проверить:

- все указанные документы скопированы полностью, без пересказа;
    
- имена файлов сохранены;
    
- индексы `20_SplitDoc/00_INDEX.md` и `21_Project_Docs/00_INDEX.md` созданы;
    
- `00_Главная.md` содержит ссылки на новые индексы;
    
- старые оригиналы остались на местах;
    
- `TASK_HISTORY.md` содержит запись TASK 109;
    
- БЛОК 1 в оригинальном `AGENT_TASK_LOOP.md` очищен после выполнения;
    
- документация сохранена в UTF-8 без BOM;
    
- моды обновлять не нужно.
    

Отчёт:

- DONE
    
- RULES PATH
    
- CHANGED FILES
    
- CREATED FILES
    
- DIFF
    
- CHECKS
    
- PROBLEMS
    
- CONCLUSION





================================================================================
# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AGENT REPORT

DONE:
- В Obsidian-базе `Documentation_DayZ_Qwest` исправлены старые упоминания `DayZ_Qwest` и старые пути на актуальное имя/путь.
- В `Documentation_DayZ_Qwest/02_Решения.md` исправлена сломанная Markdown-строка пути.
- В заметках `02`-`06` добавлены нормальные первые заголовки.
- В `Documentation_DayZ_Qwest/00_Главная.md` добавлен раздел `Агентская система`.
- Созданы компактные Obsidian-зеркала агентских правил.
- В `Documentation_DayZ_Qwest/05_Очередь_задач.md` добавлена запись о подготовке Obsidian-перехода.
- В `Documentation/SplitDoc/TASK_HISTORY.md` добавлена короткая запись по `TASK 107`.
- Текущий `БЛОК 1` очищен по правилу из `START.md`.

RULES PATH:
- Основные правила и SplitDoc находятся здесь: `P:\Silver_77_Quests\Documentation`

CHANGED FILES:
- `P:\Silver_77_Quests\Documentation\AGENT_TASK_LOOP.md`
- `P:\Silver_77_Quests\Documentation\SplitDoc\TASK_HISTORY.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\00_Главная.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\01_Текущее_состояние.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\02_Решения.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\03_Открытые_вопросы.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\04_Архитектура.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\05_Очередь_задач.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\06_Идеи.md`

CREATED FILES:
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_AGENT_TASK_LOOP_зеркало.md`
- `P:\Silver_77_Quests\Documentation_DayZ_Qwest\12_START_зеркало.md`

DIFF:
- Старые упоминания Obsidian-базы `DayZ_Qwest` заменены на `Documentation_DayZ_Qwest`.
- Актуальный путь базы указан как `P:\Silver_77_Quests\Documentation_DayZ_Qwest`.
- `02_Решения.md` получил корректный путь ``Documentation_DayZ_Qwest``.
- В `00_Главная.md` добавлены ссылки `[[10_Правила_агента]]`, `[[11_AGENT_TASK_LOOP_зеркало]]`, `[[12_START_зеркало]]`.
- Новые зеркала описывают текущие рабочие оригиналы, но не заменяют их.
- История пополнена записью о `TASK 107`.
- `БЛОК 1` заменён на стандартную заглушку.

CHECKS:
- Старые упоминания `DayZ_Qwest` в Obsidian-базе заменены на `Documentation_DayZ_Qwest`.
- Путь указан точно: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`.
- `02_Решения.md` больше не содержит сломанную Markdown-кавычку.
- Новые Obsidian-файлы созданы и связаны из `00_Главная.md`.
- Старые рабочие документы остались на своих местах.
- `БЛОК 1` очищен корректно.
- Документация сохранена в UTF-8 без BOM.
- Моды обновлять не нужно.

PROBLEMS:
- Нет.

CONCLUSION:
- TASK 107 выполнен как подготовительный Obsidian-переход: база `Documentation_DayZ_Qwest` нормализована, зеркала агентских документов созданы, старые рабочие документы не удалялись и физически не переносились.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ ОТЧЁТА
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

================================================================================
# 🟨 БЛОК 3 — REVIEW / STATUS
================================================================================

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## НАЧАЛО REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

TASK 101 REVIEW

Статус:
Принято после runtime-проверки пользователя.

Commit baseline:
`2fc3212f8a381efd7d33bdd60ff2491f5b5e3800`

Что изменено:
- QuestPanelBackdrop: 1040x620 -> 1040x710
- QuestPanel: 980x560 -> 980x650

Description:
- DescriptionPanel: 430x204 -> 430x272
- DescriptionScroll: 414x156 -> 414x224
- DescriptionText: 394x600 -> 406x224
- Scrollbar V: 1 -> 0

Dialog:
- DialogPanel: y 286 -> 358
- DialogPanel: 430x122 -> 430x164
- DialogScroll: 414x74 -> 414x116
- DialogText: 394x300 -> 406x116
- Scrollbar V: 1 -> 0

Buttons:
- AcceptButton y: 420 -> 540
- CompleteButton y: 420 -> 540
- CloseButton y: 490 -> 594

Подтверждено в runtime:
- текущий вариант визуально нормальный и пока принимается как рабочий baseline;
- `Description/Dialog` увеличены по вертикали;
- `Scrollbar V` отключён;
- используется простой стек `ScrollWidget -> MultilineTextWidget`;
- `QuestUI.c` не менялся;
- `wrapper/mask/SizeToChild/RichText/EditBox` не используются.

Known limitation:
- очень длинный текст может обрезаться, потому что scroll/clipping тема пока отложена.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## КОНЕЦ REVIEW
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

История задач вынесена в:

- `Documentation/SplitDoc/TASK_HISTORY.md`
