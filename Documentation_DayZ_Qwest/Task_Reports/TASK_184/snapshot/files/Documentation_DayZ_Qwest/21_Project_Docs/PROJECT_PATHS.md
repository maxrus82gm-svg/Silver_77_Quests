# PROJECT PATHS

Каноническая карта рабочих roots проекта `Silver_77_Quests`. Документ читается при выборе физической цели, работе с несколькими подпроектами, build/output или риске перепутать source и runtime-копию. Он не входит в постоянный agent-core.

## 1. Канонический repository

- Рабочий проект: `P:\Silver_77_Quests`
- Активная документация: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`
- Активная задача: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\11_Задача_агенту.md`
- Тематические правила: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc`
- Проектные справочники: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\21_Project_Docs`

Случайные worktree, старые копии на `C:`/`D:` и legacy-каталоги не являются источником истины без прямого указания TASK.

## 2. Рабочие исходники и инструменты

- Клиентский мод: `P:\Silver_77_Quests\Silver_77_Quests_Client`
- Серверный мод: `P:\Silver_77_Quests\Silver_77_Quests_Server`
- Migration server: `P:\Silver_77_Quests\Silver_77_Migrate_server`
- Zones: `P:\Silver_77_Quests\Silver_77_Zones`
- Quest Editor / Web Workshop / dev JSON: `P:\Silver_77_Quests\JSON_Quvest`
- DayZ layout viewer: `P:\Silver_77_Quests\DayZ_layout`
- Server reference snapshots: `P:\Silver_77_Quests\DayZ_Server_Reference`
- Исходники отдельного cooking/build fix-мода: `P:\Silver_77_Quests\Silver_77_fix_Builds_Cooking`

`JSON_Quvest` и `DayZ_layout` — локальные инструменты, а не runtime-код игрового мода.

## 3. Source и packed output

Рабочие изменения вносятся в разрешённые исходники внутри `P:\Silver_77_Quests`.

`P:\Mods_DONE` — packed/build output. Например:

- source: `P:\Silver_77_Quests\Silver_77_fix_Builds_Cooking`
- packed output: `P:\Mods_DONE\@Silver_77_fix_Builds_Cooking`

Packed output не считать исходниками и не менять без отдельной build/deployment-задачи.

Корневые `scripts/`, `gui/`, `config.cpp`, `mod.cpp`, а также `SplitMods/` и `Support/` не считать автоматически актуальной целью: сначала сверить текущую TASK и профильный контракт.

## 4. Dev, snapshot и runtime target

Repository dev JSON, reference snapshots, export-файлы и реальные server profile/runtime targets имеют разные роли.

Перед записью определить:

1. источник данных;
2. dev/editor-копию;
3. snapshot/reference;
4. export/build result;
5. реальный runtime/production target.

Известный путь не даёт разрешения на запись. Для server profile, dev JSON, export и runtime дополнительно читать `20_SplitDoc/SERVER_PROFILE_RULES.md` и следовать allowlist текущей TASK.

## 5. Проверка перед записью

- `Resolve-Path` указывает внутрь ожидаемого канонического root;
- выбран source, а не packed output или snapshot;
- путь прямо разрешён TASK;
- старая или внешняя копия не подменяет repository на `P:`;
- build/runtime/deployment не выполняются без отдельной команды.
