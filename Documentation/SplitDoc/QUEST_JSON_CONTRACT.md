# QUEST JSON CONTRACT

Короткий агентский контракт между модом, JSON-квестами, UI и редактором.

## 1. Источник правды

Мод является источником правды для JSON-контракта.

Редактор, UI и вспомогательные инструменты должны подстраиваться под то, что реально читает мод.

Расширенный логический документ проекта:

- `Documentation/QUEST_LOGIC_SPEC.md`

Этот `SplitDoc` нужен как короткая operational-версия для агентов.

## 2. Главная модель квеста

Каждый квест может иметь:

- `requiredQuestIds[]`
- `Offer`
- `Completion[]`
- `Reward`

Рабочая модель:

```text
один Offer -> несколько Completion -> один Reward
```

Текущая runtime-модель:

- `Offer` и `Reward` фактически single-trigger роли;
- `Completion` может быть несколько.

## 3. Реальные поля, которые надо учитывать

Ключевые поля:

- `quest.description`
- `requiredQuestIds[]`
- `objectives[]`
- `rewards[]`
- `offerTriggerIds[]`
- `completionTriggerIds[]`
- `rewardTriggerIds[]`
- `triggerActions[]`
- `triggerActions[].dialogText`

## 4. Чего в контракте нет

Не вводить ложные поля.

В контракте нет отдельного поля:

- `requiredItems`
- `dialogue`
- `dialogueText` как отдельной сущности вне `triggerActions[]`
- `rewardBlock`
- `completionBlock`
- `offerBlock`

Роль `requiredItems` выполняют:

- `objectives[]`
- особенно `objectives[]` с `type == "item"`

NPC-текст хранится в:

- `triggerActions[].dialogText`

## 5. Семантика ролей

`Offer`:

- место / NPC, где игрок берёт квест;
- стартовый диалог;
- может выдать стартовые предметы через `giveItems`;
- не является финальным закрытием квеста.

`Completion`:

- промежуточный этап;
- может быть `0`, `1` или несколько;
- может иметь свой `triggerActions[].dialogText`;
- может принимать objectives;
- может выдавать промежуточную награду через `triggerActions[].rewards`.

`Reward`:

- финальное закрытие квеста;
- выдаёт итоговую награду;
- переводит квест в `completed`.

## 6. Objective-правила

Для предметных целей ориентироваться на:

- `objectives[]`
- `quantity`
- `useItemQuantity`
- `allowPartialTurnIn`
- `removeOnComplete`

Не выдумывать отдельную сущность "список обязательных предметов", если речь идёт о runtime JSON.

## 7. Связь с UI

Смысловые данные делятся так:

- описание, цели, прогресс, статус, награды -> `DescriptionPanel`
- NPC-диалог текущего действия -> `DialogPanel`

`triggerActions[].dialogText` не должен подменять основное описание квеста.

Подробные UI-правила см. в:

- `Documentation/SplitDoc/QUEST_UI_RULES.md`

## 8. Риск дублей

В проекте уже есть более широкий документ:

- `Documentation/QUEST_LOGIC_SPEC.md`

Если между ним и этим `SplitDoc` появится расхождение, это нужно считать риском двойного канона и выносить в отдельную синхронизационную задачу.
