# Настройки Silver_77_Migrate_server

Мод использует один runtime-конфиг:

`$profile:Silver_77_Migrate/MigrationConfig.json`

Исходный образец находится в `Silver_77_Migrate_server/config/MigrationConfig.json`. JSON не поддерживает комментарии, поэтому пояснения хранятся только в этом README.

Верхняя часть файла содержит общие настройки запуска и погоды. Ниже расположены три самостоятельных массива: `groups`, `events` и `activations`. В текущем default находятся две группы `MIGRATION_TEST_001` / `MIGRATION_TEST_002`, одно событие `EVENT_GLOBAL_MIGRATION` и две точки активации.

## Архитектура GROUP / EVENT / ACTIVATION

- `GROUP` в массиве `groups` — полный blueprint одной migration-группы. Стабильный уникальный `groupId` используется программно, а отдельное `name` является только человекочитаемой подписью и может содержать кириллицу.
- `EVENT` в массиве `events` — устойчивый `eventId`, `name` и список `groupIds`, запускаемых одним batch.
- `ACTIVATION` в массиве `activations` — устойчивый `activationId`, информационное `name`, тип `STARTUP` либо `EXTERNAL`, а также `targetType` (`GROUP`/`EVENT`) и `targetId`.

Canonical startup activation `ACT_STARTUP_GLOBAL_MIGRATION` направлена на `EVENT_GLOBAL_MIGRATION`; это событие включает обе текущие группы. `ACT_EXTERNAL_GLOBAL_MIGRATION` само ничего не запускает и ждёт явного server-side вызова.

Минимальная public facade для будущих интеграций:

- `S77MigrateAPI.Activate(activationId)`;
- `S77MigrateAPI.StartGroup(groupId)`;
- `S77MigrateAPI.StartEvent(eventId)`.

Все операции возвращают `bool`: `true` означает, что batch принят для подготовки, `false` — запрос отклонён. Причина фиксируется в server log маркерами `NOT_FOUND`, `DISABLED`, `INVALID_TARGET`, `NO_RUNNABLE_GROUPS` либо `LAUNCH_BUSY`. Во время delay/weather preparation новая заявка не ставится в очередь и получает `false`; уже живые runtime-группы блокировкой не являются. Каждый новый принятый запуск создаёт новый `runtimeGroupId`, не удаляя предыдущие группы.

Quest-мод, trigger/zone, scheduler и random в TASK 154 к facade не подключены.

## Сборка и эталонный Support

Канонический repository entry point для сборки server-only мода:

`P:\Silver_77_Quests\build_migrate_server.bat`

Wrapper запускает установленный DayZ Addon Builder в подтверждённом для этого script-мода режиме pack-only, подписывает результат существующим ключом `Silver777` и создаёт PBO по фактическому пути:

`P:\Mods_DONE\@Silver_77_Migrate_server\addons\Silver_77_Migrate_server.pbo`

Подпись `Silver_77_Migrate_server.pbo.Silver777.bisign` обновляется той же успешной сборкой; существующий публичный ключ остаётся в папке `key` готового мода.

Только после успешной сборки wrapper создаёт внешнюю папку `Support` в корне готового мода и с overwrite копирует туда canonical source:

`P:\Mods_DONE\@Silver_77_Migrate_server\Support\MigrationConfig.json`

Единственным редактируемым source of truth остаётся `Silver_77_Migrate_server/config/MigrationConfig.json`. Файл `Support/MigrationConfig.json` — эталонная копия для ручного использования и сравнения; он находится вне PBO и автоматически обновляется при каждом успешном запуске wrapper. При ошибке PBO build Support-копия не обновляется, а wrapper завершается с ошибкой.

Runtime-логика не использует Support-файл. Рабочий конфиг по-прежнему находится только в `$profile:Silver_77_Migrate/MigrationConfig.json` и автоматически не перезаписывается из готового мода.

## Общие параметры события

- `enabled` — включает всё событие: `1` — включено, `0` — выключено.
- `loggingEnabled` — включает обычные INFO/event/lifecycle-записи Migration одновременно в RPT и `$profile:Silver_77_Migrate/Migration.log`. Default: `1`. Критические ERROR остаются в RPT независимо от этого флага и пишутся в profile log, если файл удалось открыть.
- `stuckDebugLoggingEnabled` — при одновременно включённом `loggingEnabled` добавляет подробные `STUCK_SAMPLE_START`, `STUCK_SAMPLE_CHECK` и `STUCK_SAMPLE_RESET`. Default: `0`; для диагностики TASK 146 пользователь вручную ставит `1` в runtime JSON.
- `eventDelaySeconds` — задержка от принятия каждого launch batch до начала погодного перехода либо непосредственного запуска групп, в секундах. Default: `30.0`.

## Логирование и диагностика stuck/recovery

Server-only manager использует подтверждённые DayZ FileIO `OpenFile(..., FileMode.APPEND)`, `FPrintln()` и `CloseFile()`. Один file handle открывается при инициализации сессии и закрывается в штатном `Stop()`; файл не открывается заново на каждом movement tick. Каждая строка получает безопасный server-runtime timestamp `timeMs` через `GetGame().GetTime()` и одновременно отправляется через `Print()` в RPT.

Profile log:

`$profile:Silver_77_Migrate/Migration.log`

Основные уровни:

- INFO — важные event, spawn, route, aggro, recovery, hold, release/death и периодические unit-state записи; зависят от `loggingEnabled`;
- STUCK DEBUG — одна запись начала sample, одна проверка после полного interval и event-based reset с причиной; требует обоих флагов `loggingEnabled = 1` и `stuckDebugLoggingEnabled = 1`;
- ERROR — всегда остаётся в RPT; после успешного открытия profile log дублируется и туда даже при `loggingEnabled = 0`.

Диагностическая цепочка одного короткого stuck incident:

`STUCK_SAMPLE_START -> STUCK_SAMPLE_CHECK result=STUCK -> STUCK_DETECTED -> STUCK_RECOVERY_RELEASE -> STUCK_RECOVERY_STIMULUS_ATTEMPT -> STUCK_RECOVERY_STIMULUS_OK/FAILED/SKIPPED -> STUCK_RECOVERY_CALM -> STUCK_RECOVERY_RESUME`

`STUCK_SAMPLE_CHECK` содержит `positionA`, `positionB`, `elapsedSeconds`, реальное `movedXZ`, threshold и `MOVING`/`STUCK`. `STUCK_SAMPLE_RESET` показывает event-причины вроде `AGGRO`, `BUILD_PATH`, `ROUTE_POINT_CHANGED`, `WAYPOINT_CHANGED`, `STUCK_RECOVERY`, `HOLD_FREE`, `DEATH` или release. Recovery marker содержит `cause=STUCK` либо `cause=ROUTE_PROGRESS_LOST`. Результат stimulus формируется по фактическому bool, возвращённому `EmitAIStimulus()`.

Для текущего runtime-теста вручную добавить/выставить в верхней части существующего `$profile:Silver_77_Migrate/MigrationConfig.json`:

    "loggingEnabled": 1,
    "stuckDebugLoggingEnabled": 1,

Параметры scenario остаются без изменения:

    "stuckDetectionSeconds": 6.0,
    "stuckMinMovementMeters": 2.0

Сложная rotation/архивация в TASK 146 не реализована. Пока используется один append-файл; вне целевой диагностики `stuckDebugLoggingEnabled` следует вернуть в `0`. Production size limit/rotation можно добавить отдельной задачей после измерения фактического роста файла.

## Погода и гроза

- `weatherEnabled` — глобальный master switch погодного предвестника. При `0` ни один scenario не может запросить смену погоды, и группы запускаются после `eventDelaySeconds` без weather transition.
- `weatherTransitionSeconds` — длительность плавного перехода overcast, fog и rain, в секундах. Default: `180.0`.
- `weatherOvercast` — целевая облачность от `0.0` до `1.0`. Default: `1.0`.
- `weatherFog` — целевая плотность тумана от `0.0` до `1.0`. Default: `0.8`.
- `weatherWindMagnitude` — требуемая скорость ветра в метрах в секунду. JSON default: `0.0`; vanilla API фактически получает безопасный минимум `0.1 m/s`.
- `weatherRain` — целевая интенсивность дождя от `0.0` до `1.0`. Default: `0.0`.
- `weatherStormEnabled` — включает нарастающую грозу. При `0` storm density остаётся `0`.
- `weatherStormDensity` — финальная плотность грозы от `0.0` до `1.0`. Default: `0.8`.
- `weatherStormThreshold` — порог облачности для vanilla storm от `0.0` до `1.0`. Default: `0.8`.
- `weatherStormTimeoutSeconds` — минимальный интервал между молниями, в секундах. Default: `30.0`.
- `weatherStormRampSeconds` — длительность нарастания storm density в конце погодного перехода, в секундах. Default: `60.0`.

При defaults погодный переход начинается через `30` секунд, длится `180` секунд, а storm ramp идёт последние `60` секунд. После завершения перехода запускаются только runnable-группы текущего GROUP/EVENT batch. Rain остаётся равным `0.0`.

Для текущего batch-запуска действует агрегированное правило: если `weatherEnabled = 1` и хотя бы одна runnable GROUP имеет `weatherChangeEnabled = 1`, выполняется ровно один общий weather transition. Если у всех групп текущего batch значение `0`, transition пропускается и пачка запускается после обычного `eventDelaySeconds`. При `weatherEnabled = 0` transition всегда пропускается независимо от group flags.

Погода DayZ глобальна для мира. Поэтому GROUP с `weatherChangeEnabled = 0`, запущенная в одной пачке с GROUP, у которой стоит `1`, сама не инициирует transition, но всё равно находится в той же изменённой погоде. Решение всегда относится только к текущему batch; ранее созданные runtime-группы не учитываются.

## Массив groups

Количество GROUP не ограничено двумя и не задаётся в коде. Чтобы добавить третью группу, нужно добавить в массив ещё один полный JSON-объект с уникальным `groupId`, после чего при необходимости сослаться на него из `events[].groupIds`. Отдельный файл или отдельный loader-метод не требуется.

Порядок `groupIds` внутри EVENT определяет порядок запуска групп в его batch.

Параметры каждой GROUP:

- `enabled` — включает конкретную группу: `1` — создавать, `0` — пропустить.
- `weatherChangeEnabled` — определяет, запрашивает ли эта GROUP существующий глобальный weather transition: `1` — запрашивает, `0` — сама не инициирует. Default: `1`. Это не локальная защита от уже изменённой погоды мира.
- `spawnChancePercent` — базовый GROUP-level шанс выбора на одну fresh попытку запуска, `float` от `0.0` до `100.0`; дробные значения разрешены. `100.0` всегда выбирает GROUP, `0.0` всегда пропускает её, а промежуточное значение независимо разыгрывает `Silver_77_Migrate`. Chance miss является штатным успешным результатом request и может оставить EVENT без выбранных групп; в этом случае weather, delay и runtime group не создаются. Отсутствующее поле старого profile получает default `100.0` только в памяти без перезаписи файла.
- `groupId` — обязательный стабильный и уникальный технический идентификатор GROUP для EVENT, ACTIVATION, API, runtime state и логов.
- `name` — отдельное информационное имя GROUP; не используется как ID и не обязано быть уникальным.
- `infectedCount` — количество infected в группе.
- `groupLifetimeSeconds` — максимальное время жизни каждого infected, созданного этой конкретной migration-группой. Default: `14400.0` секунд (4 часа). Значение задаётся отдельно для каждой GROUP, поэтому разные группы могут иметь разные сроки: `86400.0` — сутки, `604800.0` — неделя. Искусственного верхнего application-level ограничения нет. Неположительное значение заменяется default `14400.0` через `Normalize()`.
- `infectedTypes` — список vanilla class names infected. Если infected больше элементов списка, классы повторяются по кругу.
- `spawnPosition` — центр появления группы как `[X, Y, Z]` в метрах мира.
- `targetPosition` — центр конечной точки маршрута как `[X, Y, Z]`.
- `routePoints` — произвольный массив промежуточных мировых точек `[X, Y, Z]`. Допустимо любое количество, включая пустой массив. Default: `[]`.
- `routePointReachRadius` — горизонтальный радиус X/Z, в котором индивидуальная промежуточная точка считается достигнутой. Default: `6.0` метра.
- `routeActivationEnabled` — включает legacy group-level AI stimulus на промежуточных точках. Default: `0`; код и остальные параметры сохранены для специальных сценариев, но стандартная migration-схема их не использует.
- `routeActivationTriggerPercent` — процент текущих живых active members конкретной runtime-группы, необходимый для импульса. Default: `30.0`.
- `routeActivationRadius` — общий горизонтальный радиус X/Z зоны подсчёта возле raw route point. Default: `12.0` метров.
- `routeActivationRadii` — optional-массив индивидуальных радиусов с теми же индексами, что у `routePoints`. Отсутствующее, лишнее или неположительное значение использует `routeActivationRadius`.
- `routeStimulusLifetimeSeconds` — время существования AI-only stimulus промежуточной точки. Default: `1.0` секунда.
- `routeStimulusStrengthMultiplier` — множитель силы stimulus промежуточной точки. Default: `1.0`.
- `spawnFormationSpacing` — базовый интервал сетки возле spawn, в метрах. Default: `4.5`.
- `spawnFormationJitter` — случайное отклонение spawn-позиции по X/Z в обе стороны, в метрах. Default: `0.5`.
- `targetFormationSpacing` — базовый интервал сетки индивидуальных целей возле target, в метрах. Default: `4.5`.
- `targetFormationJitter` — случайное отклонение индивидуальной цели по X/Z в обе стороны, в метрах. Default: `0.5`.
- `logIntervalSeconds` — интервал обычной unit-state записи infected, в секундах. Default: `10.0`; применяется только при глобальном `loggingEnabled = 1`.
- `finalActivationEnabled` — включает legacy group-level AI stimulus в финальной зоне. Это не release-механика: итоговое поведение actual arrival определяется `finalHoldEnabled`. Default: `0`; стандартная migration-схема отключает этот процентный trigger.
- `finalActivationTriggerPercent` — процент текущих живых active members runtime-группы, необходимый для финального импульса. Default: `30.0`.
- `finalActivationDistance` — горизонтальный радиус X/Z финальной зоны подсчёта вокруг raw `targetPosition`. Default: `12.0` метров.
- `finalStimulusLifetimeSeconds` — время существования AI-only stimulus. Default: `1.0` секунда.
- `finalStimulusStrengthMultiplier` — множитель силы AI stimulus. Default: `1.0`; фактический радиус и реакцию нужно подтвердить runtime-тестом.
- `stuckRecoveryEnabled` — включает индивидуальное восстановление застрявших infected через временную передачу vanilla AI и направленный AI-only stimulus. Default: `1`.
- `stuckDetectionSeconds` — период индивидуального контрольного stuck-sample. Default: `6.0` секунд.
- `stuckMinMovementMeters` — минимальное итоговое горизонтальное X/Z-смещение world position между sample и текущей позицией. Default: `2.0` метра; ровно `2.0` считается достаточным движением.
- `stuckReverseEnabled` — включает второй уровень recovery, если первый stimulus/free-period не дал физического смещения. Default: `1`.
- `stuckReverseDistanceMeters` — расстояние локальной reverse-цели в направлении, противоположном сохранённому направлению на route/control target. Default: `10.0` метров.
- `stuckReverseMaxSeconds` — максимальная продолжительность одной reverse-попытки. Default: `5.0` секунд.
- `stuckReverseRetrySeconds` — задержка перед повторной попыткой начать прерванный vanilla-busy reverse. Default: `10.0` секунд.
- `stuckPostReverseFreeSeconds` — свободное vanilla-время после окончания reverse и перед новым `BuildPath()`. Default: `10.0` секунд.
- `routeProgressWatchdogEnabled` — включает второй индивидуальный уровень контроля полезного прогресса к текущей логической маршрутной цели. Default: `1`.
- `routeProgressCheckSeconds` — длительность одного comparison window watchdog. Default: `30.0` секунд.
- `routeProgressMinProgressMeters` — минимальное уменьшение дистанции до logical route target за окно, считающееся хорошим прогрессом. Default: `5.0` метров.
- `routeProgressMaxBacktrackMeters` — увеличение дистанции за одно окно, которое немедленно подтверждает уход назад. Default: `10.0` метров.
- `routeProgressBadCheckLimit` — число последовательных окон с недостаточным прогрессом до подтверждения проблемы. Default: `2`.
- `stuckRecoveryFreeSeconds` — минимальное свободное время vanilla AI после stuck recovery. Default: `30.0` секунд.
- `stuckRecoveryStatusCheckSeconds` — редкий интервал проверки, можно ли вернуть спокойного infected под migration control после recovery. Default: `3.0` секунды.
- `stuckStimulusForwardDistance` — расстояние вперёд от infected по направлению к текущему waypoint/route target для recovery stimulus. Default: `10.0` метров.
- `stuckStimulusShareRadius` — legacy-поле, сохраняемое в JSON/parser для backward compatibility. После TASK 144 manager его не использует: соседние infected не переводятся в recovery принудительно. Default в parser: `20.0` метров.
- `stuckStimulusRetrySeconds` — legacy-поле, сохраняемое в JSON/parser для backward compatibility. После TASK 144 manager его не использует: spatial cooldown не подавляет собственный stimulus другого infected. Default в parser: `5.0` секунд.
- `stuckStimulusLifetimeSeconds` — время существования recovery AI-only stimulus. Default: `1.0` секунда.
- `stuckStimulusStrengthMultiplier` — множитель силы recovery stimulus. Default: `1.0`; точная эффективная дальность остаётся предметом runtime-подбора.
- `finalHoldEnabled` — включает удержание infected возле финальной зоны вместо терминального `RELEASED` после actual arrival. Default: `1`.
- `finalHoldRadius` — большой горизонтальный X/Z радиус свободной жизни возле `targetPosition`; внутри него manager не направляет infected. Default: `200.0` метров.
- `finalHoldReturnRadius` — радиус возвращения к индивидуальной финальной цели `targetPosition + routeOffset`, после которого `RETURN_TO_HOLD` снова переходит в `HOLD_FREE`. Default: `30.0` метров.
- `finalHoldCheckSeconds` — редкий интервал проверки расстояния в `HOLD_FREE`. Default: `5.0` секунд.

## Многоточечный маршрут

Каждый infected проходит маршрут последовательно:

`spawnPosition -> routePoints[0] -> routePoints[1] -> ... -> targetPosition`

Для каждого отрезка manager отдельно вызывает vanilla `AIWorld::FindPath()`. Число промежуточных точек не зафиксировано в коде. Пустой `routePoints` сохраняет прямой маршрут от spawn к target.

Один индивидуальный formation offset рассчитывается для infected один раз и без нового jitter применяется ко всем `routePoints` и к его конечной цели. Поэтому группа сохраняет разлёт вдоль маршрута и не пытается собраться всеми entity в одной координате. Переход к следующему отрезку выполняется независимо для каждого infected после входа в `routePointReachRadius` текущей точки.

Для узких ворот и проходов пользователь может уменьшить `targetFormationSpacing` и `targetFormationJitter`: именно эти два параметра формируют стабильный offset вдоль ручного маршрута. Подходящее расстояние нужно подобрать runtime-тестом с учётом размера группы и navmesh.

Пример массива из трёх промежуточных точек:

    "routePoints": [
        [13180.0, 85.0, 13310.0],
        [13150.0, 95.0, 13240.0],
        [13125.0, 108.0, 13160.0]
    ],
    "routePointReachRadius": 6.0

## Волновая активация route points (опциональная legacy-механика)

В стандартном source-конфиге `routeActivationEnabled = 0` и `finalActivationEnabled = 0`. При одновременном отключении manager пропускает runtime-группу до подсчёта `aliveCount`, поэтому percentage accumulation не участвует в текущем baseline и не создаёт лишний group count. Ниже описана сохранённая code-path, которую можно явно включить для специального сценария.

Каждый фактический запуск GROUP создаёт собственную runtime-группу с новым уникальным `runtimeGroupId` и списком только успешно созданных этим запуском infected. Поэтому повторные запуски одинакового `groupId` не смешивают members. Runtime group ID пока не сохраняется между рестартами.

Denominator — текущее количество живых, ещё не `RELEASED` members именно этой runtime-группы. Убитые сразу перестают учитываться. Vanilla infected, infected других модов и members другой runtime-группы в процент не входят. Требуемое количество рассчитывается так:

`required = ceil(aliveActiveMembers * triggerPercent / 100)`

При наличии живых members минимум всегда равен `1`. Например, при `alive=7` и `30%` требуется `3` infected.

`routePointReachRadius` и activation radius выполняют разные задачи:

- `routePointReachRadius = 6.0` — когда конкретный infected завершает navigation segment;
- `routeActivationRadius = 12.0` — зона подсчёта группы возле raw route point;
- `routeActivationRadii[index]` — optional override counting radius конкретной точки.

Counting zone центрируется по raw `routePoints[index]`, без individual formation offset, и использует только X/Z. При достижении threshold ARMED-точка создаёт один world AI stimulus и становится TRIGGERED. Пока occupancy остаётся не ниже текущего threshold, повторного импульса нет. Когда occupancy падает ниже threshold, точка снова ARMED; следующий рост до threshold создаёт новый импульс. Обязательного времени ожидания или cooldown для этого нет — проверка использует существующий update cycle.

Stimulus создаётся через `NoiseSystem::AddNoiseTarget()` с `NoiseParams.LoadFromPath("CfgVehicles SurvivorBase NoiseShout")`. Это AI-only perception target: мод не создаёт `SoundSet`, `SoundObject` или слышимый игроку fake sound и не устанавливает native mind state. Поэтому посторонние nearby infected не учитываются в percentage, но могут естественно воспринять мировой stimulus. Migration infected при vanilla busy-состоянии отдают управление native AI, а после `CALM` и существующего cooldown продолжают текущий маршрут.

Короткий пример:

    "routePointReachRadius": 6.0,
    "routeActivationEnabled": 0,
    "routeActivationTriggerPercent": 30.0,
    "routeActivationRadius": 12.0,
    "routeActivationRadii": [8.0, 15.0, 12.0],
    "routeStimulusLifetimeSeconds": 1.0,
    "routeStimulusStrengthMultiplier": 1.0

## Финальная accumulation zone

Final target использует ту же percentage-механику. Внутри `finalActivationDistance` считаются только фактически находящиеся там живые active members данной runtime-группы, независимо от их логического индекса маршрута; терминальные `RELEASED` infected исключены. При достижении `finalActivationTriggerPercent` создаётся один stimulus точно в raw `targetPosition`. Финальная зона тоже имеет ARMED/TRIGGERED/rearm и может дать несколько импульсов после падения и нового роста occupancy.

Final stimulus не переводит всю группу и отдельных infected в `RELEASED` или `HOLD_FREE`. Он только даёт vanilla AI perception stimulus.

## Stuck recovery

Stuck recovery работает индивидуально для infected в режимах `MIGRATION` и `RETURN_TO_HOLD`. Он не применяется к `HOLD_FREE`, активному vanilla AI, dead infected и терминальному `RELEASED`.

Manager сохраняет индивидуальный sample реальной world position infected. Через `stuckDetectionSeconds` он сравнивает sample и текущую позицию только в горизонтальной плоскости X/Z. Если итоговое смещение меньше `stuckMinMovementMeters`, infected считается застрявшим; progress к текущему waypoint может оставаться в диагностике, но не отменяет `STUCK_DETECTED`. При смещении, равном threshold или превышающем его, sample обновляется и начинается следующий интервал.

При подтверждённом stuck manager:

1. логирует `STUCK_DETECTED`;
2. снимает `OverrideHeading` и `OverrideMovementSpeed`;
3. переводит infected в `STUCK_RECOVERY`;
4. создаёт AI-only recovery stimulus впереди по направлению к текущей управляющей цели на `stuckStimulusForwardDistance`;
5. даёт infected минимум `stuckRecoveryFreeSeconds` свободного vanilla AI.

Recovery полностью индивидуален. Только infected, который сам получил `STUCK_DETECTED` или `ROUTE_PROGRESS_LOST`, переходит в `STUCK_RECOVERY` и создаёт один собственный directed stimulus. Соседи могут естественно услышать этот world AI stimulus и начать двигаться, но manager не меняет их state и не считает их recovery выполненным. Если сосед остаётся застрявшим, его собственный detector продолжает работать и позднее создаёт отдельный stimulus без spatial/group suppression.

Пока infected находится в `STUCK_RECOVERY`, оба detector не выполняются, поэтому один incident не создаёт повторный spam. После `STUCK_RECOVERY_RESUME` и нового `BuildPath()` samples создаются заново; при новом застревании тот же infected снова имеет право на отдельный recovery stimulus без lifetime-ограничения.

`STUCK_RECOVERY_STIMULUS` содержит `cause=STUCK` либо `cause=ROUTE_PROGRESS_LOST`, infected ID и фактическую target semantics. Если control target практически совпал с позицией, manager пробует существующий logical route target. При полном отсутствии usable direction recovery всё равно начинается без бесконечного reset sample, а stimulus пропускается с диагностикой `STUCK_RECOVERY_STIMULUS_SKIPPED reason=NO_USABLE_DIRECTION`.

После free-period manager проверяет recovery status не чаще `stuckRecoveryStatusCheckSeconds`. Если у infected есть target или mind state не `CALM`, возврат не выполняется. Только после `target == null`, `CALM` и защитного cooldown строится новый путь от текущей позиции к правильному intent: `MIGRATION` или `RETURN_TO_HOLD`.

После первого free-period TASK 151 дополнительно сравнивает текущую world position с позицией начала recovery по X/Z. `STUCK_RECOVERY_STIMULUS_OK` означает только успешное создание stimulus. Физически успешным recovery считается только `movedXZ >= stuckMinMovementMeters`; это фиксирует `STUCK_RECOVERY_MOVEMENT_CHECK result=MOVED`. При `NO_MOVEMENT` и включённом `stuckReverseEnabled` начинается одна локальная reverse-попытка.

Reverse не является движением спиной и не меняет `routePoints`, `migrationTarget` или logical target. Infected разворачивается в сторону, противоположную сохранённому направлению на control/logical target, и временно получает `OverrideHeading` / `OverrideMovementSpeed` к локальной точке примерно в `stuckReverseDistanceMeters`. Попытка завершается при смещении минимум на `stuckMinMovementMeters` либо по `stuckReverseMaxSeconds`; teleport и `SetPosition` не используются.

Перед любым возвратом manager control общий vanilla-busy guard требует доступный controller, отсутствие target и `MINDSTATE_CALM`. Natural target/aggro немедленно запрещает или прерывает reverse; overrides снимаются, context incident сохраняется, а новая попытка возможна только после retry, `CALM` и cooldown. После `STUCK_REVERSE_END` manager всегда снимает overrides, выдерживает `stuckPostReverseFreeSeconds`, снова ждёт vanilla idle + cooldown и лишь затем строит path к исходному intent. `stuckReverseEnabled = 0` полностью сохраняет одноуровневый recovery без reverse fallback.

## Route Progress Watchdog

Watchdog дополняет, но не заменяет короткий stuck detector `6 sec / 2 m`. Он работает только в `MIGRATION` и `RETURN_TO_HOLD` и сравнивает горизонтальную дистанцию до `GetCurrentRouteTarget(state)`: текущего `routePoint + routeOffset` либо индивидуального `m_MigrationTarget`. Текущий navmesh waypoint для этого решения не используется.

Первый baseline каждого infected назначается с индивидуальным controlled jitter внутри одного `routeProgressCheckSeconds`; последующие comparison windows имеют полную configured-длительность. Все проверки выполняются в существующем manager update cycle без отдельного `CallLater` на infected. Обычная проверка содержит только world position, logical target, X/Z distance и несколько сравнений; `FindPath()`, group scan и stimulus при нормальном результате не вызываются.

За окно рассчитывается:

`progress = previousDistance - currentDistance`

- `progress >= routeProgressMinProgressMeters` — хороший прогресс, bad-check counter сбрасывается;
- `currentDistance - previousDistance >= routeProgressMaxBacktrackMeters` — немедленный `ROUTE_PROGRESS_LOST` с причиной `BACKTRACK`;
- меньший прогресс увеличивает индивидуальный bad-check counter; при достижении `routeProgressBadCheckLimit` создаётся `ROUTE_PROGRESS_LOST` с причиной `NO_PROGRESS`.

После подтверждённого `ROUTE_PROGRESS_LOST` manager переиспользует существующий directed recovery к logical route target: снимает overrides, создаёт AI-only stimulus, даёт vanilla freedom и возвращает правильный intent только после free-period, `target == null`, `CALM` и cooldown, после чего `BuildPath()` строится от актуальной позиции. В `AGGRO`, `STUCK_RECOVERY`, `HOLD_FREE` и `RELEASED` watchdog не работает; его baseline сбрасывается при смене path, logical target или режима.

## Final hold и RETURN_TO_HOLD

Если `finalHoldEnabled = 0`, actual arrival сохраняет старое терминальное поведение: `RELEASED`, entity остаётся в мире, а manager больше не назначает route control.

Если `finalHoldEnabled = 1`, actual arrival не делает терминальный `RELEASED`. Manager снимает route overrides, очищает path и переводит конкретного infected в `HOLD_FREE`. В `HOLD_FREE` он свободно живёт под vanilla AI внутри `finalHoldRadius` от raw `targetPosition`; manager не заставляет его стоять в центре.

Если `HOLD_FREE` infected вышел дальше `finalHoldRadius`, manager сначала проверяет vanilla AI. При target или non-`CALM` mind state возврат к поселению не начинается. После `target == null`, `CALM` и cooldown только этот infected переходит в `RETURN_TO_HOLD`; путь строится к индивидуальной финальной цели `targetPosition + routeOffset`.

Когда `RETURN_TO_HOLD` входит в `finalHoldReturnRadius`, route overrides снимаются, path очищается и infected снова становится `HOLD_FREE`. На `RETURN_TO_HOLD` действует тот же stuck recovery. Если infected во время возврата увидел игрока, manager отдаёт его vanilla AI и после спокойного cooldown возвращает именно к `RETURN_TO_HOLD`, а не начинает старый маршрут заново.

Если preset или `NoiseSystem` недоступны, manager пишет ограниченное предупреждение; group counting, migration route, vanilla handoff, stuck recovery без stimulus и final hold продолжают работать без слышимого fallback-звука.

Пример структуры третьего объекта, который добавляется внутрь массива `groups` после существующего объекта через запятую:

    {
        "enabled": 1,
        "weatherChangeEnabled": 0,
        "groupId": "MIGRATION_TEST_003",
        "name": "Миграционная группа 003",
        "infectedCount": 10,
        "groupLifetimeSeconds": 14400.0,
        "infectedTypes": ["ZmbM_HikerSkinny_Blue"],
        "spawnPosition": [1000.0, 100.0, 1000.0],
        "targetPosition": [1200.0, 100.0, 1200.0],
        "routePoints": [],
        "routePointReachRadius": 6.0,
        "routeActivationEnabled": 0,
        "routeActivationTriggerPercent": 30.0,
        "routeActivationRadius": 12.0,
        "routeActivationRadii": [],
        "routeStimulusLifetimeSeconds": 1.0,
        "routeStimulusStrengthMultiplier": 1.0,
        "spawnFormationSpacing": 4.5,
        "spawnFormationJitter": 0.5,
        "targetFormationSpacing": 4.5,
        "targetFormationJitter": 0.5,
        "logIntervalSeconds": 10.0,
        "finalActivationEnabled": 0,
        "finalActivationTriggerPercent": 30.0,
        "finalActivationDistance": 12.0,
        "finalStimulusLifetimeSeconds": 1.0,
        "finalStimulusStrengthMultiplier": 1.0,
        "stuckRecoveryEnabled": 1,
        "stuckDetectionSeconds": 6.0,
        "stuckMinMovementMeters": 2.0,
        "stuckReverseEnabled": 1,
        "stuckReverseDistanceMeters": 10.0,
        "stuckReverseMaxSeconds": 5.0,
        "stuckReverseRetrySeconds": 10.0,
        "stuckPostReverseFreeSeconds": 10.0,
        "routeProgressWatchdogEnabled": 1,
        "routeProgressCheckSeconds": 30.0,
        "routeProgressMinProgressMeters": 5.0,
        "routeProgressMaxBacktrackMeters": 10.0,
        "routeProgressBadCheckLimit": 2,
        "stuckRecoveryFreeSeconds": 30.0,
        "stuckRecoveryStatusCheckSeconds": 3.0,
        "stuckStimulusForwardDistance": 10.0,
        "stuckStimulusShareRadius": 20.0,
        "stuckStimulusRetrySeconds": 5.0,
        "stuckStimulusLifetimeSeconds": 1.0,
        "stuckStimulusStrengthMultiplier": 1.0,
        "finalHoldEnabled": 1,
        "finalHoldRadius": 200.0,
        "finalHoldReturnRadius": 30.0,
        "finalHoldCheckSeconds": 5.0
    }

Координаты примера условные и не являются принятым маршрутом.

## Создание и миграция runtime-конфига

Если `MigrationConfig.json` отсутствует, loader проверяет старые runtime-файлы:

- `MigrationEvent.json`;
- `MigrationScenario_001.json`;
- `MigrationScenario_002.json`.

Все существующие старые файлы сначала должны успешно прочитаться. Затем их пользовательские значения переносятся в новый единый объект, временный JSON валидируется и только после этого устанавливается как `MigrationConfig.json`. Старые runtime-файлы не удаляются и не изменяются.

Если найденный старый JSON повреждён, единый файл автоматически не создаётся: сомнительные данные не заменяются defaults. Если старых файлов нет, создаётся единый конфиг с двумя default GROUP, глобальным EVENT и STARTUP/EXTERNAL ACTIVATION.

Если `MigrationConfig.json` уже существует, активным считается только он. Старые три runtime-файла игнорируются и остаются на диске для ручного контроля или последующего удаления пользователем.

## Add-only и совместимость

Существующий `MigrationConfig.json` loader не переписывает, не пересобирает и не пересортировывает. Malformed или семантически невалидный unified JSON остаётся нетронутым и останавливает запуск.

Legacy runtime-файл с массивом `scenarios` продолжает загружаться. Для каждого старого объекта `scenarioId` становится in-memory fallback для `groupId`, отсутствующее `name` получает безопасное информационное значение, а все enabled legacy-группы объединяются в созданный только в памяти `EVENT_GLOBAL_MIGRATION`. Там же создаются совместимые STARTUP и EXTERNAL activation. Runtime JSON на диске при этом не изменяется.

Новая canonical schema использует `groups`, `events` и `activations`. Все `groupId`, `eventId` и `activationId` проверяются на непустое уникальное значение, EVENT-ссылки и ACTIVATION target должны существовать, а `type` ограничен `STARTUP`/`EXTERNAL`. Lookup по `name` не выполняется. EVENT сначала разрешает все ссылки целиком и только затем запускает batch, поэтому неизвестный `groupId` не приводит к частичному старту.

Остальные отсутствующие nested-поля старого runtime получают constructor/`Normalize()` defaults только в памяти. Поля `spawnDelaySeconds` и scenario-level weather из ещё более старых отдельных JSON поддерживаются только одноразовой миграцией; в актуальном массиве `groups` они не используются.
