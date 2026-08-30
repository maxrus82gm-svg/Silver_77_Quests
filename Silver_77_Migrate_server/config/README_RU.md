# Настройки Silver_77_Migrate_server

Мод использует один runtime-конфиг:

`$profile:Silver_77_Migrate/MigrationConfig.json`

Исходный образец находится в `Silver_77_Migrate_server/config/MigrationConfig.json`. JSON не поддерживает комментарии, поэтому пояснения хранятся только в этом README.

Верхняя часть файла содержит общие настройки одного migration-события и погоды. Ниже расположен массив `scenarios`; каждый его объект описывает отдельную migration-группу. В текущем default находятся два объекта: `MIGRATION_TEST_001` и `MIGRATION_TEST_002`.

## Общие параметры события

- `enabled` — включает всё событие: `1` — включено, `0` — выключено.
- `eventDelaySeconds` — задержка от инициализации мода до начала погодного перехода, в секундах. Default: `30.0`.

## Погода и гроза

- `weatherEnabled` — включает общий погодный предвестник. При `0` группы запускаются после `eventDelaySeconds` без изменения погоды.
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

При defaults погодный переход начинается через `30` секунд, длится `180` секунд, а storm ramp идёт последние `60` секунд. После завершения перехода все включённые объекты массива `scenarios` запускаются в один общий момент. Rain остаётся равным `0.0`.

## Массив scenarios

Количество сценариев не ограничено двумя и не задаётся в коде. Manager последовательно перебирает весь массив `scenarios`. Чтобы добавить третью группу, нужно добавить в массив ещё один полный JSON-объект с уникальным `scenarioId`; отдельный файл и метод вида `LoadScenario003()` не требуются.

Порядок объектов в массиве сохраняется и определяет порядок их запуска в рамках одного события.

Параметры каждого scenario:

- `enabled` — включает конкретную группу: `1` — создавать, `0` — пропустить.
- `scenarioId` — уникальный текстовый идентификатор группы для состояния infected и серверных логов.
- `infectedCount` — количество infected в группе.
- `infectedTypes` — список vanilla class names infected. Если infected больше элементов списка, классы повторяются по кругу.
- `spawnPosition` — центр появления группы как `[X, Y, Z]` в метрах мира.
- `targetPosition` — центр конечной точки маршрута как `[X, Y, Z]`.
- `routePoints` — произвольный массив промежуточных мировых точек `[X, Y, Z]`. Допустимо любое количество, включая пустой массив. Default: `[]`.
- `routePointReachRadius` — горизонтальный радиус X/Z, в котором индивидуальная промежуточная точка считается достигнутой. Default: `6.0` метра.
- `routeActivationEnabled` — включает group-level AI stimulus на промежуточных точках. Default: `1`.
- `routeActivationTriggerPercent` — процент текущих живых active members конкретной runtime-группы, необходимый для импульса. Default: `30.0`.
- `routeActivationRadius` — общий горизонтальный радиус X/Z зоны подсчёта возле raw route point. Default: `12.0` метров.
- `routeActivationRadii` — optional-массив индивидуальных радиусов с теми же индексами, что у `routePoints`. Отсутствующее, лишнее или неположительное значение использует `routeActivationRadius`.
- `routeStimulusLifetimeSeconds` — время существования AI-only stimulus промежуточной точки. Default: `1.0` секунда.
- `routeStimulusStrengthMultiplier` — множитель силы stimulus промежуточной точки. Default: `1.0`.
- `spawnFormationSpacing` — базовый интервал сетки возле spawn, в метрах. Default: `4.5`.
- `spawnFormationJitter` — случайное отклонение spawn-позиции по X/Z в обе стороны, в метрах. Default: `0.5`.
- `targetFormationSpacing` — базовый интервал сетки индивидуальных целей возле target, в метрах. Default: `4.5`.
- `targetFormationJitter` — случайное отклонение индивидуальной цели по X/Z в обе стороны, в метрах. Default: `0.5`.
- `logIntervalSeconds` — интервал диагностического лога infected, в секундах. Default: `10.0`.
- `finalActivationEnabled` — включает group-level AI stimulus в финальной зоне. При `0` infected просто доходит до цели и получает `RELEASED`. Default: `1`.
- `finalActivationTriggerPercent` — процент текущих живых active members runtime-группы, необходимый для финального импульса. Default: `30.0`.
- `finalActivationDistance` — горизонтальный радиус X/Z финальной зоны подсчёта вокруг raw `targetPosition`. Default: `12.0` метров.
- `finalStimulusLifetimeSeconds` — время существования AI-only stimulus. Default: `1.0` секунда.
- `finalStimulusStrengthMultiplier` — множитель силы AI stimulus. Default: `1.0`; фактический радиус и реакцию нужно подтвердить runtime-тестом.

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

## Волновая активация route points

Каждый фактический `StartScenario()` создаёт собственную runtime-группу с уникальным ID и списком только успешно созданных этим запуском infected. Поэтому будущие повторные запуски одинакового `scenarioId` не смешивают members. Runtime group ID пока не сохраняется между рестартами.

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
    "routeActivationEnabled": 1,
    "routeActivationTriggerPercent": 30.0,
    "routeActivationRadius": 12.0,
    "routeActivationRadii": [8.0, 15.0, 12.0],
    "routeStimulusLifetimeSeconds": 1.0,
    "routeStimulusStrengthMultiplier": 1.0

## Финальная accumulation zone и RELEASED

Final target использует ту же percentage-механику. Внутри `finalActivationDistance` считаются только фактически находящиеся там живые active members данной runtime-группы, независимо от их логического индекса маршрута; уже `RELEASED` infected исключены. При достижении `finalActivationTriggerPercent` создаётся один stimulus точно в raw `targetPosition`. Финальная зона тоже имеет ARMED/TRIGGERED/rearm и может дать несколько импульсов после падения и нового роста occupancy.

Final stimulus не переводит всю группу и отдельных infected в `RELEASED`. Он только даёт vanilla AI perception stimulus. Индивидуальный infected получает `RELEASED` исключительно при фактическом arrival по существующей логике конечного navmesh path. После этого entity не удаляется, lifetime flags не меняются, route retries и возврат к migration control прекращаются, а infected остаётся vanilla AI.

Если preset или `NoiseSystem` недоступны, manager пишет ограниченное предупреждение; group counting, migration route, vanilla handoff и arrival -> `RELEASED` продолжают работать без слышимого fallback-звука.

Пример структуры третьего объекта, который добавляется внутрь массива после существующего объекта через запятую:

    {
        "enabled": 1,
        "scenarioId": "MIGRATION_TEST_003",
        "infectedCount": 10,
        "infectedTypes": ["ZmbM_HikerSkinny_Blue"],
        "spawnPosition": [1000.0, 100.0, 1000.0],
        "targetPosition": [1200.0, 100.0, 1200.0],
        "routePoints": [],
        "routePointReachRadius": 6.0,
        "routeActivationEnabled": 1,
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
        "finalActivationEnabled": 1,
        "finalActivationTriggerPercent": 30.0,
        "finalActivationDistance": 12.0,
        "finalStimulusLifetimeSeconds": 1.0,
        "finalStimulusStrengthMultiplier": 1.0
    }

Координаты примера условные и не являются принятым маршрутом.

## Создание и миграция runtime-конфига

Если `MigrationConfig.json` отсутствует, loader проверяет старые runtime-файлы:

- `MigrationEvent.json`;
- `MigrationScenario_001.json`;
- `MigrationScenario_002.json`.

Все существующие старые файлы сначала должны успешно прочитаться. Затем их пользовательские значения переносятся в новый единый объект, временный JSON валидируется и только после этого устанавливается как `MigrationConfig.json`. Старые runtime-файлы не удаляются и не изменяются.

Если найденный старый JSON повреждён, единый файл автоматически не создаётся: сомнительные данные не заменяются defaults. Если старых файлов нет, создаётся единый конфиг с двумя default-сценариями.

Если `MigrationConfig.json` уже существует, активным считается только он. Старые три runtime-файла игнорируются и остаются на диске для ручного контроля или последующего удаления пользователем.

## Add-only и совместимость

Существующий `MigrationConfig.json` loader не переписывает, не пересобирает и не пересортировывает. Существующие глобальные значения, порядок объектов `scenarios` и их пользовательские значения сохраняются. Malformed unified JSON остаётся нетронутым и останавливает запуск события.

Для отсутствующих полей применяются constructor defaults только в загруженном runtime-объекте, без записи обратно в пользовательский JSON. Вложенный массив не патчится текстово: новые scenario-объекты никогда не добавляются автоматически. Это безопасное ограничение текущей add-only совместимости.

То же правило относится к `routePoints`, route-activation и final-activation полям: существующий runtime JSON автоматически не переписывается ради новых nested fields. Missing TASK 135 fields получают constructor defaults только в памяти. Чтобы задать пользовательский маршрут или изменить активацию, поля нужно вручную добавить в нужный объект `scenarios`.

Поля `spawnDelaySeconds` и weather-поля из старого Scenario 001 поддерживаются только одноразовой миграцией старой схемы. Внутри актуального массива `scenarios` они не используются.
