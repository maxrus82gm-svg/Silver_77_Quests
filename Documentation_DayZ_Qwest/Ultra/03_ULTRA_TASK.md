# ULTRA LAUNCH PAYLOAD — TASK 182 CORRECTIVE PASS

## Режим и baseline

- `EXECUTOR: ULTRA`
- `CODEX MODE: PAYLOAD BUILD + DISPATCH ONLY`
- `ULTRA MODE: DOCUMENTATION PATCH PREPARATION`
- `AUTO-DELEGATION: OFF`
- Канонический проект: `P:\Silver_77_Quests`
- Каноническая документация: `P:\Silver_77_Quests\Documentation_DayZ_Qwest`
- Последний независимо проверенный GitHub baseline: commit message `100`, SHA `9e9ecbb72206c42da4e1e3f170fd7ce7e3da5b3f`

Локальные документы могли быть изменены пользователем после baseline. Ниже передано их фактическое текущее локальное состояние. Ничего не откатывать.

Это корректирующий второй проход TASK 182 после независимого review первого ответа Ultra. Первый patch нельзя применять как есть и нельзя считать текущей истиной. Используй только current state и установленные ниже результаты review.

Текущий bridge передаёт Ultra только текст payload и принимает текстовый ответ. Файловых инструментов у Ultra нет. Не утверждай, что открывала repository, проверяла filesystem, выполняла Git-команды, меняла файлы или физически применяла patch.

## Исходная цель TASK 182

Сделать однозначный workflow для трёх режимов:

1. `EXECUTOR: CODEX`
2. `EXECUTOR: ULTRA`
3. `EXECUTOR: CODEX + ULTRA`

БЛОК 1 документа `Documentation_DayZ_Qwest/11_Задача_агенту.md` — единственный пользовательский источник текущей TASK. `Ultra/03_ULTRA_TASK.md` — одноразовый технический payload конкретного запуска, полностью сформированный Codex. `Ultra/04_ULTRA_REPORT.md` — результат вызова.

Нужно сохранить safety, scope, Git-контроль, encoding safety, review и тематические правила, устранить lifecycle/functional-allowlist неоднозначность и сделать будущий dispatch-header системным результатом TASK PREP.

Ultra должна подготовить новый минимальный patch/replacement, а не физически менять repository.

## Канонический контракт, уже реализованный current state

### EXECUTOR: CODEX

- Codex читает БЛОК 1 и REQUIRED-контекст, выполняет TASK сам.
- Ultra не запускается; `03` и `04` не используются.
- Стандартный lifecycle применяется, если TASK его не отключает.

### EXECUTOR: ULTRA

- Codex — технический dispatcher.
- Читает body и REQUIRED только для полного payload, содержательную TASK не выполняет.
- Полностью пересобирает новый непустой `03`; старое содержимое не используется.
- Вызывает только `03 + 04`.
- После вызова не открывает/не читает `04`, не анализирует результат и не применяет patch.

### EXECUTOR: CODEX + ULTRA

- Codex остаётся основным исполнителем, формирует отдельную подзадачу в новом `03` и вызывает `03 + 04`.
- После успешного вызова обязан прочитать и критически проверить `04` до использования результата.

Invalid/unknown `EXECUTOR` не угадывается; dispatch не начинается. `AUTO-DELEGATION: OFF`.

Канонический bridge:

- `task_file`: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\03_ULTRA_TASK.md`
- `report_file`: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\04_ULTRA_REPORT.md`

Bridge текстовый; Ultra возвращает анализ, текст, replacement, diff или patch, но не считается физически изменившей repository.

## CONTEXT PROVENANCE

Следующие документы и их актуальное содержание переданы Codex внутри payload. Не пиши, что Ultra лично открыла эти файлы. Допустимые формулировки:

- `CONTEXT PROVIDED IN PAYLOAD`;
- `передано Codex в launch-payload`;
- `по переданному актуальному контексту`.

Только реально доступный Ultra инструмент мог бы считаться прямым чтением файла; в этом запуске такого инструмента нет.

Переданный current state:

- `Documentation_DayZ_Qwest/11_Задача_агенту.md`
- `Documentation_DayZ_Qwest/10_Правила_агента.md`
- `Documentation_DayZ_Qwest/12_Старт_агента.md`
- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_PREPARATION_RULES.md`
- `Documentation_DayZ_Qwest/20_SplitDoc/CONTEXT_ROUTING.md`
- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_LIFECYCLE_RULES.md`
- `Documentation_DayZ_Qwest/Ultra/00_Главная.md`
- `Documentation_DayZ_Qwest/Ultra/01_Старт_Ultra.md`
- `Documentation_DayZ_Qwest/Ultra/02_Правила_Ultra.md`
- дополнительно по доказанной документационной/encoding-зависимости: `Documentation_DayZ_Qwest/20_SplitDoc/DOCUMENTATION_RULES.md` и `Documentation_DayZ_Qwest/20_SplitDoc/ENCODING_RULES.md`.

Используй точные канонические пути. Не сокращай, например, `Documentation_DayZ_Qwest/20_SplitDoc/TASK_LIFECYCLE_RULES.md` до несуществующего `Documentation_DayZ_Qwest/TASK_LIFECYCLE_RULES.md`.

## Фактический current state

### `11_Задача_агенту.md`

TASK 182 уже имеет dispatch-header в правильном месте: в начале содержимого БЛОКА 1, после shell-заголовка `# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА`. Header уже содержит:

- `EXECUTOR: ULTRA`;
- `CODEX MODE: PAYLOAD BUILD + DISPATCH ONLY`;
- `ULTRA MODE: DOCUMENTATION PATCH PREPARATION`;
- `USER TASK SOURCE`;
- `ULTRA REPORT TARGET`;
- `ULTRA LAUNCH PAYLOAD`;
- `AUTO-DELEGATION: OFF`;
- `END DISPATCH HEADER`.

Не предлагай второй header перед shell-структурой всего документа 11 и не перемещай текущий header. БЛОКИ 2/3 менять нельзя.

### `12_Старт_агента.md`

Уже правильно указано:

> Исполнителя задаёт поле `EXECUTOR` в dispatch-header БЛОКА 1. Канонический алгоритм режимов `CODEX`, `ULTRA` и `CODEX + ULTRA`, включая сборку launch-payload и правила чтения результата Ultra, находится только в `20_SplitDoc/CONTEXT_ROUTING.md`.

START также уже содержит жёсткий scope: менять только явно разрешённые TASK файлы/разделы; чтение не расширяет право записи; `ANALYSIS ONLY` не запускает AGENT REPORT/историю/очистку. Перед успешной implementation/documentation-задачей требуется `TASK_LIFECYCLE_RULES.md`.

Не предлагай повторно переносить полный алгоритм в START или менять START без конкретного отсутствующего инварианта.

### `CONTEXT_ROUTING.md`

Уже содержит:

- БЛОК 1 как единственный пользовательский источник TASK;
- три режима CODEX / ULTRA / CODEX + ULTRA;
- `03` формирует только Codex непосредственно перед вызовом;
- старый `03` не переиспользуется, новый проверяется на непустоту;
- каноническую пару `03 + 04` с абсолютными путями;
- ULTRA: Codex не читает `04`;
- CODEX + ULTRA: Codex обязан прочитать и проверить `04`;
- text-only bridge и отсутствие физического изменения repository Ultra;
- invalid executor не угадывается;
- `AUTO-DELEGATION: OFF`.

Не предлагай повторно добавлять эти правила или менять routing без конкретного нового gap.

### `Ultra/00_Главная.md`

Уже содержит цепочку `00 -> 01 -> 02 -> 03 -> 04` и правила: `03` — одноразовый payload, сформированный Codex; пользователь вручную его не поддерживает; `04` — результат запуска. Изменение не требуется без конкретного gap.

### `Ultra/01_Старт_Ultra.md`

Уже содержит:

- роль Ultra и передачу решения о чтении результата dispatcher-маршруту;
- канонические пути на `P:`;
- `03` как технический payload, полностью сформированный Codex перед текущим вызовом;
- запрет считать старый payload текущей задачей;
- необходимость боевых документов предметной подсистемы;
- неканоничность legacy, worktree и D:-копии.

Текущая формулировка ещё может создавать впечатление, что Ultra сама читает боевые документы. Допускается предложить только один короткий инвариант о text-only bridge, если он действительно нужен и не дублирует полный self-reporting-контракт.

### `Ultra/02_Правила_Ultra.md`

Уже содержит: выполнять только текущий payload; соблюдать scope; `ANALYSIS ONLY` запрещает изменения; чтение не разрешает правку; не чинить соседние проблемы; отделять факты/требования/проверки/историю/гипотезы; не угадывать конфликт; результат только в `04`; предметные выводы требуют актуального контекста; недостаток сведений указывать честно.

Но постоянные provenance/self-reporting-правила text-only bridge отсутствуют. Это реальный gap и основной владелец для исправления.

### `TASK_PREPARATION_RULES.md`

Канонический владелец подготовки новых TASK. Сейчас обязательные поля БЛОКА 1 включают номер/тип, результат, baseline, root, обязательное чтение, действия, scope, запреты, проверки, формат отчёта и build/runtime, но НЕ требуют dispatch-header и его полей.

Это реальный gap. Нужно добавить обязательное системное определение:

- `EXECUTOR`;
- роли Codex;
- `USER TASK SOURCE`;
- `ULTRA LAUNCH PAYLOAD` и `ULTRA REPORT TARGET`, если используется Ultra;
- `AUTO-DELEGATION`.

Не копировать сюда полный алгоритм трёх режимов; сослаться на канонический `CONTEXT_ROUTING.md`. Header должен быть в начале содержимого БЛОКА 1, а не в начале всего файла 11.

### `TASK_LIFECYCLE_RULES.md`

Сейчас lifecycle требует до завершения проверить, что разрешение TASK включает lifecycle-обновления. После успеха он требует:

1. заменить БЛОК 2 новым AGENT REPORT;
2. добавить ровно одну запись текущей TASK в `20_SplitDoc/TASK_HISTORY.md`;
3. штатно очистить завершённый БЛОК 1.

БЛОК 3 без прямого разрешения не меняется. `ANALYSIS ONLY` lifecycle не запускает. При BLOCKED БЛОК 1 не очищается. После отчёта исполнитель останавливается; Git write не разрешён.

Реальный gap: документ не содержит чётко сформулированного постоянного узкого служебного разрешения для успешного `EXECUTOR: CODEX`, независимого от functional allowlist.

Нужно минимальное правило: для обычной успешно завершённой CODEX implementation/documentation TASK ровно три перечисленные lifecycle-операции разрешены как служебные без отдельного перечисления в functional allowlist. Это не разрешает менять любые соседние функциональные файлы. БЛОК 3 остаётся запрещён без прямого разрешения. TASK может явно отключить lifecycle. `ANALYSIS ONLY` его не запускает. Для `EXECUTOR: ULTRA` стандартный Codex lifecycle автоматически не выполняется; БЛОКИ 1/2/3 и TASK_HISTORY остаются неизменными, закрытие — отдельным явным шагом.

### `10_Правила_агента.md`

Сейчас широко сказано: после выполнения задачи агент переносит результат в отчёт и историю, затем очищает содержимое БЛОКА 1; подробный алгоритм — в lifecycle.

Это реальный gap, потому что формулировку можно трактовать как универсальную и для `EXECUTOR: ULTRA`. Нужна минимальная замена: завершение выполняется только по `TASK_LIFECYCLE_RULES.md` и только в режиме/задаче, где lifecycle разрешён. Полный алгоритм в документ 10 не копировать.

### Documentation/encoding

Использовать существующего канонического владельца, не создавать дубли; изменения должны быть минимальными. Markdown — UTF-8 без BOM, нормальные переносы, сохранённая кириллица, без mojibake, `�`, literal `\n` и массового пересохранения. Для реального применения предпочтителен минимальный `apply_patch`.

## Установленные ошибки первого ULTRA REPORT

Первый ответ Ultra нельзя применять как есть. Считать установленными ошибки:

1. Он предлагал повторно добавлять в START/routing/Ultra-документы правила, которые уже реализованы.
2. Он ошибочно предлагал универсальный dispatch-header в начале всего файла 11. Header относится к TASK и должен находиться в начале содержимого БЛОКА 1; текущий TASK 182 уже расположен правильно.
3. Раздел `READ` создавал впечатление прямого чтения repository-файлов Ultra, хотя bridge text-only.
4. Использовались/могли использоваться неточные сокращённые пути вместо переданных канонических.
5. Не было строгого разделения `CURRENT FACT` / `GAP` / `PROPOSED CHANGE`.
6. Исторические проблемы TASK были повторно объявлены текущими без сравнения с current state.

Не воспроизводи первый patch. Отметь уже выполненные требования как `ALREADY SATISFIED / NO CHANGE` или `ALREADY RESOLVED`.

## Реально оставшиеся gaps

1. `Documentation_DayZ_Qwest/20_SplitDoc/TASK_LIFECYCLE_RULES.md`: отсутствует узкое постоянное служебное разрешение трёх lifecycle-операций для успешного `EXECUTOR: CODEX`, с явным запретом расширять functional scope и исключением для ULTRA/ANALYSIS ONLY.
2. `Documentation_DayZ_Qwest/20_SplitDoc/TASK_PREPARATION_RULES.md`: будущие TASK не обязаны системно получать dispatch-header и его поля.
3. `Documentation_DayZ_Qwest/10_Правила_агента.md`: очистка сформулирована слишком широко и не ограничена режимом/разрешённым lifecycle.
4. `Documentation_DayZ_Qwest/Ultra/02_Правила_Ultra.md`: отсутствуют постоянные provenance/self-reporting-правила text-only bridge.
5. `Documentation_DayZ_Qwest/Ultra/01_Старт_Ultra.md`: только при доказанной необходимости допустим один короткий инвариант, что без файлового инструмента Ultra получает сведения через payload, а не открывает repository сама.

Не объявляй gap в `12_Старт_агента.md`, `CONTEXT_ROUTING.md`, `Ultra/00_Главная.md` или текущем dispatch-header, если переданный текст не показывает конкретного отсутствующего инварианта.

## Обязательные постоянные правила поведения Ultra

Основной владелец — `Documentation_DayZ_Qwest/Ultra/02_Правила_Ultra.md`.

Предложи точный минимальный текст, закрепляющий:

### A. Происхождение контекста

Если тексты документов переданы в payload, Ultra не пишет «я прочитала файл X» и не создаёт впечатление прямого доступа. Использует `CONTEXT PROVIDED IN PAYLOAD`, «передано Codex в launch-payload», «по переданному актуальному контексту». Прямым чтением считается только действие реально доступного инструмента.

### B. Не выдумывать доступ

Не утверждать, что Ultra открыла файл, проверила filesystem, изменила repository, выполнила Git-команду или физически применила patch, если соответствующего инструмента не было.

### C. Точные пути

Использовать именно канонические пути из payload. Не придумывать расположение и не сокращать `Documentation_DayZ_Qwest/20_SplitDoc/TASK_LIFECYCLE_RULES.md` до несуществующего пути.

### D. Сначала current state, затем patch

Перед изменением проверить по переданному текущему тексту, существует ли правило. Если да: `ALREADY SATISFIED / NO CHANGE`.

### E. Разделять три вещи

1. `CURRENT FACT` — что реально есть в переданном current state.
2. `GAP` — чего реально не хватает.
3. `PROPOSED CHANGE` — минимальная правка только gap.

### F. Не объявлять исправленное проблемой

Историческая проблема не текущая автоматически. Если current state уже исправлен: `ALREADY RESOLVED`, не включать повторно в patch.

## Желаемый минимальный scope нового patch

Ожидаемый минимальный набор:

- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_LIFECYCLE_RULES.md`
- `Documentation_DayZ_Qwest/20_SplitDoc/TASK_PREPARATION_RULES.md`
- `Documentation_DayZ_Qwest/10_Правила_агента.md`
- `Documentation_DayZ_Qwest/Ultra/02_Правила_Ultra.md`

`Documentation_DayZ_Qwest/Ultra/01_Старт_Ultra.md` — только если нужен один короткий text-only инвариант.

Не менять повторно без конкретного gap:

- `Documentation_DayZ_Qwest/12_Старт_агента.md`
- `Documentation_DayZ_Qwest/20_SplitDoc/CONTEXT_ROUTING.md`
- `Documentation_DayZ_Qwest/Ultra/00_Главная.md`
- текущий dispatch-header в БЛОКЕ 1.

Любое дополнительное изменение обосновать. Не менять БЛОКИ 2/3, не очищать БЛОК 1, не писать TASK_HISTORY, не менять server.py, код, JSON, runtime, `.obsidian`, PBO/build и не выполнять Git write.

## Сценарии проверки нового patch

- `CODEX`: header/body читаются Codex, functional scope жёсткий, ровно три lifecycle-операции имеют узкое служебное разрешение.
- `ULTRA`: Codex собирает `03`, вызывает `03 + 04`, report не читает, lifecycle автоматически не выполняет.
- `CODEX + ULTRA`: Codex основной и обязательно читает/проверяет `04`.
- `invalid header`: выполнение/dispatch не начинаются.
- `lifecycle`: allowlist только `migration.js` разрешает `migration.js` плюс ровно три служебные операции и ничего функционально соседнего.
- `ANALYSIS ONLY`: lifecycle автоматически не запускается.
- `provenance`: text-only Ultra сообщает только о контексте, переданном в payload, и не выдумывает filesystem/Git/patch-доступ.

## Формат нового ULTRA REPORT

`ULTRA REPORT`

`TASK: TASK 182 — CORRECTIVE PASS`

`STATUS: COMPLETED / BLOCKED`

### CONTEXT PROVENANCE

- какие документы/фрагменты переданы в payload;
- не писать, что Ultra открывала repository.

### CURRENT STATE

- что уже реализовано;
- какие правила уже корректны.

### ALREADY SATISFIED / NO CHANGE

- отдельный список уже выполненных требований TASK 182.

### REAL REMAINING GAPS

- только фактически незакрытые проблемы.

### PROPOSED CHANGES

- минимальный список файлов;
- причина изменения каждого.

### ULTRA BEHAVIOR RULES

- точные постоянные правила provenance/self-reporting.

### EXACT PATCH / REPLACEMENT

- готовый минимальный patch;
- не повторять существующий текст;
- использовать точные пути;
- не менять запрещённые файлы/разделы.

### SCENARIO CHECKS

- CODEX;
- ULTRA;
- CODEX + ULTRA;
- invalid header;
- lifecycle + narrow service permission;
- ANALYSIS ONLY;
- provenance при text-only bridge.

### PROBLEMS

- только реальные оставшиеся проблемы.

### CONCLUSION

- какие файлы действительно нужно изменить;
- какие менять не нужно;
- готов ли patch к отдельному физическому применению.

## Финальные ограничения

- Не изменяй repository физически.
- Не утверждай прямое чтение файлов или первый patch как current state.
- Не очищай БЛОК 1, не пиши БЛОКИ 2/3 и TASK_HISTORY.
- Не выполняй Git write и не меняй server.py.
- Верни только новый корректирующий текстовый результат TASK 182 и остановись.
- Моды обновлять не нужно.
