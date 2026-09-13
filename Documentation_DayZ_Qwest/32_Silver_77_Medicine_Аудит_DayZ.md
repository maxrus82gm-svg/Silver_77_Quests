# Silver_77_Medicine — аудит возможностей DayZ

Технический справочник подтверждённых возможностей vanilla DayZ для будущего `Silver_77_Medicine`. Это результаты анализа, а не готовая архитектура медицинского мода.

Статусы утверждений:

- `CONFIRMED SOURCE` — механизм подтверждён официальным `BohemiaInteractive/DayZ-Script-Diff`;
- `NEEDS RUNTIME TEST` — требуется отдельный prototype или проверка в игре;
- `DESIGN POSSIBILITY` — допустимое направление дизайна, ещё не утверждённое реализацией.

## CONFIRMED SOURCE

### Agents и накопление состояния

- `PlayerAgentPool` поддерживает накопление агентов, рост и угасание, persistence, временную сопротивляемость и воздействие drugs.
- `PluginTransmissionAgents` и `AgentBase` описывают transmission, invasibility, potency, die-off и drug resistance.
- Vanilla `eAgents` включает `Cholera`, `Influenza`, `Salmonella`, `Brain`, `Food Poison`, `Chemical Poison`, `Wound Agent`, `Nerve Agent` и `Heavy Metal`.
- Vanilla agent IDs являются bitmask; доступно не более 32 отдельных agent bits.

Подтверждающие пути:

- `scripts/3_game/enums/eagents.c`
- `scripts/4_world/classes/transmissionagents/agents/agentbase.c`
- `scripts/4_world/classes/transmissionagents/playeragentpool/playeragentpool.c`
- `scripts/4_world/plugins/pluginbase/plugintransmissionagents.c`

### Modifiers и стадии

- Болезнь может использовать threshold, стадии и периодический `OnTick`.
- `ModifiersManager` связывает условия, активацию и обновление modifiers.

Подтверждающий путь:

- `scripts/4_world/classes/playermodifiers/modifiersmanager.c`

### Symptoms

`SymptomManager` предоставляет существующие симптомы и состояния:

- cough;
- vomit;
- blindness;
- blood loss;
- sneeze;
- fever blur;
- laughter;
- unconscious;
- freeze;
- hot;
- pain light;
- pain heavy;
- hand shiver;
- deafness;
- heavy-metal severe effect;
- gasp.

Подтверждающий путь:

- `scripts/4_world/classes/playersymptoms/statemanager.c`

У `SymptomManager` ограничена очередь primary symptoms; конкуренцию и расписание симптомов нельзя считать решёнными без runtime-проверки.

### Fever, Thermometer и HeatComfort

- Vanilla `Fever` активирует feverish state, `FeverBlur`, дополнительную потерю воды и `HOT` symptom.
- Vanilla `Thermometer` не хранит полноценную физиологическую температуру как отдельную непрерывную disease-variable: при Fever возвращается значение из high-temperature range, иначе — normal range.
- `HeatComfort` является отдельной от Fever системой и может влиять на вероятность или скорость развития influenza.

Подтверждающие пути:

- `scripts/4_world/classes/playermodifiers/modifiers/conditions/fever.c`
- `scripts/4_world/entities/itembase/thermometer.c`

### Bleeding и contaminated exposure

- Можно создавать настоящие `BleedingSource`.
- Можно отдельно уменьшать Blood без внешней bleeding source.
- Vanilla Contaminated Area умеет передавать `CHEMICAL_POISON`, накапливать exposure, вызывать cough, создавать contaminated bleeding source, учитывать chemical protection и усиливать поступление агента через существующие bleeding sources.

Подтверждающие пути:

- `scripts/4_world/classes/playermodifiers/modifiers/conditions/areaexposure.c`
- `scripts/4_world/classes/bleedingsources/bleedingsourcesmanagerserver.c`

### Примеры многостадийных болезней

- `Heavy Metal` демонстрирует shiver, pain, временную или полную deafness, ghost/PPE effect, vomiting, water loss и health loss.
- `Pneumonia` демонстрирует health loss, stamina penalties и gasp.

Подтверждающие пути:

- `scripts/4_world/classes/playermodifiers/modifiers/diseases/heavymetal.c`
- `scripts/4_world/classes/playermodifiers/modifiers/diseases/pneumonia.c`

### PPE, camera и drugs

- Доступны existing PPE requesters; возможно регистрировать дополнительные requester.
- `CameraShake` существует как отдельный client-side механизм.
- Antibiotics и chelation уже представлены типами медицинского воздействия; agent может иметь drug resistance.

Подтверждающие пути:

- `scripts/3_game/ppemanager/pperequesterbank.c`
- `scripts/4_world/classes/camerashake.c`

## NEEDS RUNTIME TEST

- безопасная регистрация custom disease agent;
- выбор свободного ID без конфликта с vanilla и другими модами;
- persistence собственного agent state;
- реакция custom modifier на disease load;
- отдельная синхронизация клиентского симптома;
- поведение нескольких симптомов при ограниченной primary-очереди;
- конкретное различение «укус» и «царапина» у сторонних мутантов по их damage/ammo/attack реализации;
- совместимость дополнительных PPE requester с другими модами.

## DESIGN POSSIBILITY

- Использовать vanilla `AgentPool` как основу, если custom registration и persistence подтвердятся.
- Использовать собственный `S77 DiseaseState Manager`, если bitmask, конфликты ID или модель vanilla Agents окажутся ограничивающими.
- Сочетать server-side disease state с client-side PPE, camera и sound presentation.
- Разделять настоящие bleeding sources и внутреннюю потерю Blood по смыслу заболевания.

Vanilla-медицину нельзя автоматически считать медицински точной. Она подтверждает игровые механизмы, но не заменяет отдельное решение о правдоподобии, балансе и диагнозах `Silver_77_Medicine`.
