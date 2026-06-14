# Проверка хвостов старой Documentation

## Цель

Проверить, остались ли активные ссылки на старую папку `Documentation`, и отделить рабочие хвосты от legacy bridge, истории и архивного контекста.

## Проверенные паттерны

- `P:\Silver_77_Quests\Documentation`
- `P:\\Silver_77_Quests\\Documentation`
- `Documentation/AGENT_TASK_LOOP.md`
- `Documentation/SplitDoc/START.md`
- `Documentation/SplitDoc`
- `Documentation/QUEST_LOGIC_SPEC.md`
- `Documentation/README_JSON_CONFIG.md`
- `Documentation/RUSSIAN_ENCODING.md`
- `Documentation/SPLIT_CLIENT_SERVER.md`
- `Documentation/BUILD.md`
- `Documentation/README_INSTALLATION.md`
- `Documentation/README.md`
- `Documentation/CHANGELOG.md`
- `Documentation/`
- `Documentation\`

## Найденные активные хвосты

- `Documentation_DayZ_Qwest/20_SplitDoc/00_INDEX.md` — старый `Documentation/SplitDoc` больше не указан как источник правды; документ переименован по смыслу в активные правила.
- `Documentation_DayZ_Qwest/21_Project_Docs/00_INDEX.md` — старый `Documentation` больше не указан как источник правды; документ переименован по смыслу в активные проектные документы.
- `Documentation_DayZ_Qwest/12_START_зеркало.md` — карта проекта переведена с `Documentation/` на `Documentation_DayZ_Qwest/`.
- `Documentation_DayZ_Qwest/01_Текущее_состояние.md` — активные правила, START и loop переведены на Obsidian-пути.
- `Documentation_DayZ_Qwest/04_Архитектура.md` — раздел документации и правил переведён на Obsidian-пути.
- `Documentation_DayZ_Qwest/20_SplitDoc/*.md` — внутренние ссылки на правила и проектные документы переведены на `Documentation_DayZ_Qwest/20_SplitDoc` и `Documentation_DayZ_Qwest/21_Project_Docs`.
- `Documentation_DayZ_Qwest/21_Project_Docs/README.md` — первая точка входа для агента заменена на Obsidian-loop и Obsidian-START.
- `Documentation_DayZ_Qwest/21_Project_Docs/BUILD.md` — ссылка на чеклист split-сборки заменена на `Documentation_DayZ_Qwest/21_Project_Docs/SPLIT_CLIENT_SERVER.md`.

## Оставленные legacy bridge

- `Documentation/AGENT_TASK_LOOP.md` — оставлен как bridge и указывает на `Documentation_DayZ_Qwest/11_AGENT_TASK_LOOP_зеркало.md`.
- `Documentation/SplitDoc/START.md` — оставлен как bridge и указывает на `Documentation_DayZ_Qwest/12_START_зеркало.md`, `Documentation_DayZ_Qwest/20_SplitDoc` и `Documentation_DayZ_Qwest/21_Project_Docs`.
- Упоминания старых путей в `Documentation_DayZ_Qwest/10_Правила_агента.md` оставлены как описание legacy bridge.

## Оставленные исторические упоминания

- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md` — старые пути в TASK 104-110 оставлены как история миграции.
- Старые документы в `Documentation/*.md` и `Documentation/SplitDoc/*.md` не переписывались: это legacy-слой, архивные отчёты и старые рабочие документы.
- `Documentation_DayZ_Qwest/21_Project_Docs/RUSSIAN_ENCODING.md` — ссылки на старые CODEX-документы оставлены как legacy/исторический источник для восстановления текста.
- `Documentation_DayZ_Qwest/21_Project_Docs/BUILD.md` и `Documentation_DayZ_Qwest/21_Project_Docs/SPLIT_CLIENT_SERVER.md` — `Documentation/` в деревьях сборки оставлен как описание структуры старого/публикуемого мода, не как активный источник правил агента.

## Непонятные хвосты

- `P:\Silver_77_Quests\README.md`, `P:\Silver_77_Quests\JSON_Quvest\README.md`, `P:\Silver_77_Quests\Support\...\README.md` содержат ссылки на старую `Documentation`, но не входили в разрешённый список правок этой задачи.
- Старые CODEX-документы в `Documentation` содержат много ссылок на `Documentation/...`; они оставлены без правок как legacy/исторический контекст.
- Если нужно полностью обновить пользовательские README и старые CODEX-документы под Obsidian-канон, нужна отдельная задача с расширенным scope.

## Итог

Активные хвосты внутри `Documentation_DayZ_Qwest` исправлены или явно классифицированы. Legacy bridge и исторические записи оставлены без удаления. Старые `Documentation`-документы не считаются текущим каноном для агентских задач.

## TASK 112 — Финальная миграция хвостов

Исправлены README и пользовательские точки входа:

- `README.md`
- `JSON_Quvest/README.md`
- `Support/JSON_Quvest/README.md`
- `Support/Arh_29042026/JSON_Quvest/README.md`

Перенесены архивные документы:

- старые top-level Markdown-документы из `Documentation` скопированы в `Documentation_DayZ_Qwest/90_Legacy_Archive/Documentation`;
- старые Markdown-документы из `Documentation/SplitDoc` скопированы в `Documentation_DayZ_Qwest/90_Legacy_Archive/Documentation_SplitDoc`;
- перед удалением старых дублей выполнена SHA256-сверка архивных копий.

Удалены старые `.md`-дубли после переноса:

- из `Documentation` удалены все top-level `.md`, кроме `AGENT_TASK_LOOP.md` bridge;
- из `Documentation/SplitDoc` удалены все `.md`, кроме `START.md` bridge.

Оставлены допустимые legacy-упоминания:

- `Documentation/AGENT_TASK_LOOP.md` — bridge;
- `Documentation/SplitDoc/START.md` — bridge;
- старые пути внутри `90_Legacy_Archive` — историческое содержимое архивных копий;
- старые пути в `TASK_HISTORY` и отчётах — история миграции.

Итог TASK 112:

- активные README больше не ведут на старую `Documentation`;
- старые Markdown-документы перенесены в Obsidian-архив;
- `Documentation` больше не является рабочим каноном.
