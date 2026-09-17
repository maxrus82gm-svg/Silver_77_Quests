ULTRA REPORT

TASK: TASK 182 — CORRECTIVE PASS
STATUS: COMPLETED

### CONTEXT PROVENANCE
- Документы переданы Codex в launch-payload. Прямого доступа к файловой системе у Ultra нет.
- Канонические пути:
  - `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_LIFECYCLE_RULES.md`
  - `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_PREPARATION_RULES.md`
  - `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
  - `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\02_Правила_Ultra.md`
  - `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\01_Старт_Ultra.md` (опционально)

### CURRENT STATE
- Dispatch-header БЛОКА 1 (`11_Задача_агенту.md`) корректен и находится на своём месте.
- Алгоритм трёх режимов, text-only bridge, запрет авто-делегирования зафиксированы в `CONTEXT_ROUTING.md`.
- Цепочка документов Ultra (`00->01->02->03->04`) существует; роли определены.
- Lifecycle-правила существуют, но не содержат узкого служебного разрешения для CODEX.
- Правила подготовки TASK не требуют системного dispatch-header.
- Общие правила агента допускают некорректную трактовку очистки для ULTRA.
- В правилах Ultra отсутствуют постоянные provenance/self-reporting-инварианты.

### ALREADY SATISFIED / NO CHANGE
- `12_Старт_агента.md`: scope, ссылка на routing верны.
- `CONTEXT_ROUTING.md`, `Ultra/00_Главная.md`: полная маршрутизация есть.
- Текущий header в `11_Задача_агенту.md`: менять/дублировать не нужно.

### REAL REMAINING GAPS
1. `TASK_LIFECYCLE_RULES.md`: отсутствует постоянное узкое разрешение трёх lifecycle-операций для успешного EXECUTOR: CODEX с явным запретом расширения функционального скоупа и исключением для ULTRA/ANALYSIS ONLY.
2. `TASK_PREPARATION_RULES.md`: будущие TASK не обязаны получать системный dispatch-header.
3. `10_Правила_агента.md`: формулировка очистки универсальна, должна быть ограничена разрешённым lifecycle.
4. `Ultra/02_Правила_Ultra.md`: нет постоянных правил происхождения контекста и self-reporting при text-only bridge.
5. `Ultra/01_Старт_Ultra.md`: может потребоваться один короткий инвариант о том, что без файлового инструмента сведения поступают только через payload.

### PROPOSED CHANGES
1. Файл: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_LIFECYCLE_RULES.md`
   Причина: добавить узкое служебное разрешение трёх операций после успешной CODEX-задачи и явно исключить автоматический lifecycle для ULTRA/ANALYSIS ONLY.

2. Файл: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_PREPARATION_RULES.md`
   Причина: сделать наличие dispatch-header обязательным элементом БЛОКА 1 будущей TASK со всеми полями.

3. Файл: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md`
   Причина: ограничить очистку режимом/задачей, где lifecycle разрешён.

4. Файл: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\02_Правила_Ultra.md`
   Причина: закрепить permanent rules A–F по текстовому мосту и происхождению контекста.

5. Файл: `P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\01_Старт_Ultra.md` (только если нужен быстрый ориентир)
   Причина: зафиксировать short invariant об отсутствии прямого чтения repository.

### ULTRA BEHAVIOR RULES
A. Если тексты переданы в payload — писать «CONTEXT PROVIDED IN PAYLOAD», а не утверждать прямое чтение файла.
B. Не выдумывать доступ: не заявлять открытие репозитория, проверку ФС, выполнение Git-команд или применение патча без соответствующего инструмента.
C. Использовать точные канонические пути из payload; не сокращать их.
D. Сначала проверять current state; если правило уже есть — фиксировать ALREADY SATISFIED / NO CHANGE.
E. Разделять три блока: CURRENT FACT / GAP / PROPOSED CHANGE.
F. Не объявлять историческую проблему текущей, если она уже исправлена в переданном состоянии.

### EXACT PATCH / REPLACEMENT

#### Patch 1: P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_LIFECYCLE_RULES.md
```diff
@@
 После успешного выполнения TASK исполнитель останавливается. Запись в Git write запрещена.
+
+## Постоянное служебное разрешение для успешно завершённой задачи (EXECUTOR: CODEX)
+
+- Для обычной успешно завершённой implementation/documentation TASK под EXECUTOR: CODEX ровно три операции являются разрешёнными как служебные действия без дополнительного перечисления в functional allowlist:
+  1) Замена содержимого БЛОКА 2 новым AGENT REPORT;
+  2) Добавление одной записи о текущей TASK в `20_SplitDoc/TASK_HISTORY.md`;
+  3) Штатная очистка содержимого завершённого БЛОКА 1.
+- Это разрешение НЕ расширяет функциональный скоуп: изменение любых других функциональных файлов запрещено без отдельного явного разрешения в TASK.
+- БЛОК 3 остаётся запрещённым для изменения без прямого разрешения в TASK.
+- TASK может явно отключить lifecycle; ANALYSIS ONLY его не запускает.
+- При статусе BLOCKED содержимое БЛОКА 1 сохраняется.
+- Для EXECUTOR: ULTRA стандартный Codex-lifecycle автоматически НЕ выполняется: БЛОКИ 1/2/3 и TASK_HISTORY остаются неизменными; закрытие производится отдельным явным шагом.
```

#### Patch 2: P:\Silver_77_Quests\Documentation_DayZ_Qwest\20_SplitDoc\TASK_PREPARATION_RULES.md
```diff
@@
 ## Обязательные поля БЛОКА 1
 
-- Номер и тип задачи
+- Исполнитель (EXECUTOR): одно из значений {CODEX, ULTRA, CODEX + ULTRA}. Invalid/unknown значения блокируют dispatch.
+- Режим Codex (CODEX MODE): например, "PAYLOAD BUILD + DISPATCH ONLY".
+- Режим Ultra (ULTRA MODE), если используется Ultra: например, "DOCUMENTATION PATCH PREPARATION".
+- USER TASK SOURCE: абсолютный путь к источнику пользовательской задачи.
+- ULTRA LAUNCH PAYLOAD: абсолютный путь к файлу `Ultra/03_ULTRA_TASK.md`.
+- ULTRA REPORT TARGET: абсолютный путь к файлу `Ultra/04_ULTRA_REPORT.md`.
+- AUTO-DELEGATION: OFF.
+- END DISPATCH HEADER.
+
+- Остальные обязательные поля:
+- Номер и тип задачи
 - Ожидаемый результат
 - Baseline/commit
 - Root cause / context
```

#### Patch 3: P:\Silver_77_Quests\Documentation_DayZ_Qwest\10_Правила_агента.md
```diff
@@
 После выполнения задачи агент переносит результат в отчёт и историю, затем очищает содержимое БЛОКА 1; подробный алгоритм — в lifecycle.
+Завершение действий (очистка БЛОКА 1, запись отчёта и истории) выполняется строго по `TASK_LIFECYCLE_RULES.md` и только в режиме/задаче, где lifecycle разрешён.
```

#### Patch 4: P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\02_Правила_Ultra.md
```diff
@@
 ...соблюдать scope; `ANALYSIS ONLY` запрещает изменения; чтение не разрешает правку; не чинить соседние проблемы; отделять факты/требования/проверки/историю/гипотезы; не угадывать конфликт; результат только в `04`; предметные выводы требуют актуального контекста; недостаток сведений указывать честно.
+
+## Provenance и self-reporting при text-only bridge
+
+A. Происхождение контекста: если тексты документов переданы в payload, Ultra использует формулировки «CONTEXT PROVIDED IN PAYLOAD» или «передано Codex в launch-payload». Прямым чтением считается только действие реально доступного файлового инструмента.
+B. Не выдумывать доступ: не утверждать, что Ultra открыла файл, проверила filesystem, выполнила Git-команду или физически применила patch, если такого инструмента не было.
+C. Точные пути: использовать канонические пути из payload; не придумывать расположение и не сокращать существующие пути.
+D. Сначала current state, затем patch: перед изменением проверить текущий переданный текст; если правило уже присутствует — фиксировать ALREADY SATISFIED / NO CHANGE.
+E. Разделение фактов и изменений: всегда разделять блоки CURRENT FACT, GAP, PROPOSED CHANGE.
+F. Исторические vs текущие проблемы: не считать исторические дефекты текущими, если текущее состояние их не содержит.
```

#### Patch 5 (опционально): P:\Silver_77_Quests\Documentation_DayZ_Qwest\Ultra\01_Старт_Ultra.md
```diff
@@
 ...канонические пути на `P:`; `03` как технический payload, полностью сформированный Codex перед текущим вызовом...
+
+## Text-only bridge
+
+Без файлового инструмента Ultra получает все сведения исключительно из текста, переданного Codex в launch-payload; прямого открытия боевых документов или repository не происходит.
```

### SCENARIO CHECKS
- **CODEX**: Header читается Codex; функциональные права жёсткие; lifecycle-разрешение позволяет ровно три операции.
- **ULTRA**: Codex формирует новый непустой `03`, вызывает пару `03+04`; report не читается; lifecycle не выполняется.
- **CODEX + ULTRA**: Codex основной, обязательно читает и проверяет `04` до использования результата.
- **Invalid header**: Dispatch не начинается.
- **Lifecycle + narrow service permission**: Только три операции плюс отдельно разрешённый `migration.js`; другие файлы недоступны.
- **ANALYSIS ONLY**: Lifecycle не запускается.
- **Provenance при text-only bridge**: Отчёты ссылаются на контекст из payload, без имитации FS/Git-доступа.

### PROBLEMS
Фактических незакрытых проблем после применения данных замен не остаётся.

### CONCLUSION
Изменить пять файлов согласно предложенным точным патчам. Повторных изменений START/routing/header не требуется. Патчи готовы к отдельному физическому применению методом apply_patch.