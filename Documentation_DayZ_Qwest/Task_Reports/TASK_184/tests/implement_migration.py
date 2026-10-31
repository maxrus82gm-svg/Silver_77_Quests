"""TASK 184 one-shot local documentation migration; snapshots must pre-exist."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path('P:/Silver_77_Quests')
D = ROOT / 'Documentation_DayZ_Qwest'
OUT = D / 'Task_Reports/TASK_184'
BASE = json.loads((OUT / 'snapshot/baseline.json').read_text(encoding='utf-8'))
SNAP = OUT / 'snapshot/files'
MAPPING = {'11_Задача_агенту': '11_Задачи_Codex', '12_Старт_агента': '12_Старт'}


def original(rel):
    return (SNAP / rel).read_bytes()


def old(name):
    return original('Documentation_DayZ_Qwest/' + name).decode('utf-8')


def remap(text):
    for a, b in MAPPING.items():
        text = text.replace(a, b)
    return text


changes = {}


def document(name, text, remap_paths=True):
    rel = 'Documentation_DayZ_Qwest/' + name
    if remap_paths:
        text = remap(text)
    source = SNAP / rel
    nl = '\r\n' if source.exists() and b'\r\n' in source.read_bytes() else '\n'
    changes[rel] = text.replace('\r\n', '\n').replace('\n', nl).encode('utf-8')


def section(text, heading, replacement, next_heading):
    a = text.index(heading)
    b = text.index(next_heading, a + len(heading))
    return text[:a] + replacement.rstrip() + '\n\n' + text[b:]


# Check the whole approved input set again before generating any replacement.
for rel in BASE['mutable']:
    assert hashlib.sha256((ROOT / rel).read_bytes()).hexdigest() == BASE['files'][rel]['sha256'], rel

task = (OUT / 'snapshot/block_1_task.md').read_bytes()
assert b'\r' not in task
card = '''# Задачи Codex — Silver_77_Quests

## Правила

Персональная карточка Codex. Назначение — пользователь; factual report — Codex;
независимый БЛОК 4 и постоянный архив — ChatGPT-координатор.
Старт: [[12_Старт]]. Архив: [[001_История_задач_Codex]].
Текущее состояние: [[42_TACTICAL_CONTEXT]]. Реестр: [[05_Очередь_задач]].
Канон стадий: `20_SplitDoc/TASK_LIFECYCLE_RULES.md`; маршруты: `CONTEXT_ROUTING.md`.
Git/GitHub Codex запрещены, включая чтение, без отдельного прямого разрешения.
Claude отложен. Ultra / CODEX + ULTRA не используются в действующем маршруте.
Ни отчёт, ни механическая ротация не создают независимый PASS.
Точные постоянные маркеры сохраняются. Пользователь вставляет только тело TASK
между маркерами БЛОКА 1. После запуска постановка заморожена; поправки отдельно.
Ротацию инициирует ChatGPT после получения финального опубликованного пакета;
Codex не выполняет её над собственной активной задачей.

# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА

<!-- TASK-SLOT:BLOCK-1:BEGIN -->
'''.encode() + task + '''<!-- TASK-SLOT:BLOCK-1:END -->

# БЛОК 1.1 — ПОСЛЕДНЯЯ СДАННАЯ ПОСТАНОВКА

<!-- TASK-SLOT:BLOCK-1.1:BEGIN -->
**НЕ ЗАПОЛНЕН — ПЕРВЫЙ ПЕРЕНОС ЕЩЁ НЕ ВЫПОЛНЕН.**
<!-- TASK-SLOT:BLOCK-1.1:END -->

# БЛОК 2 — КОМПАКТНЫЙ FACTUAL HANDOFF

TASK-184: локальная реализация; итоговый авторский пакет после тестов —
`Task_Reports/TASK_184/report.md`, `manifest.json`, `migration_matrix.md`, `rollback.md`.
Авторский статус читать из report; независимой приёмки TASK-184 пока нет.
Предыдущий полный отчёт TASK 181 сохранён byte-exact:
`Task_Reports/TASK_184/snapshot/block_2_legacy.md`.
Результат 181 не переносится как авторский результат 184.

# БЛОК 3 — КОНТЕКСТ И НЕЗАКРЫТЫЕ ВОПРОСЫ

- Текущая TASK-184 остаётся в БЛОКЕ 1; реальная ротация в этой реализации запрещена.
- Сверка статусов TASK 107/108 и independent verdict 181 — в №05/FOLLOWUP.
- Историческое ограничение TASK 101 (длинный текст) требует адресной проверки,
  а не автоматического объявления новым runtime-дефектом.
- Полный прежний REVIEW TASK 101: `Task_Reports/TASK_184/snapshot/block_3_legacy.md`.
- Пользовательские настройки Obsidian не изменяются. Их цветовые фильтры могут
  сохранять старые имена; это визуальное ограничение, не потеря документов.
- Утилита вызывается только уполномоченным локальным инструментом координатора.
  При отсутствии такого доступа — EXECUTION GATE BLOCKED; GitHub сам Python не запускает.

# БЛОК 4 — НЕЗАВИСИМЫЙ VERIFIER

READ-ONLY для Codex. Инициализированный пустой слот, независимого verdict TASK-184 нет.
Заполняет ChatGPT после пользовательской публикации и проверки точной версии.
Прежний review TASK 101 остаётся в snapshot и не является приёмкой TASK 181/184.
'''.encode()
changes['Documentation_DayZ_Qwest/11_Задачи_Codex.md'] = card

start = old('12_Старт_агента.md')
start = start.replace('# СТАРТ АГЕНТА', '# Старт — Silver_77_Quests', 1)
start = start.replace('`БЛОК 2` и `БЛОК 3` автоматически не читать.',
                      '`БЛОКИ 1.1–4` автоматически не читать.')
start = start.replace('Исполнителя задаёт поле `EXECUTOR` в dispatch-header БЛОКА 1. Канонический алгоритм режимов `CODEX`, `ULTRA` и `CODEX + ULTRA`, включая сборку launch-payload и правила чтения результата Ultra, находится только в `20_SplitDoc/CONTEXT_ROUTING.md`.',
                      'Действующий исполнитель — `EXECUTOR: CODEX`. Claude отложен; Ultra и CODEX + ULTRA не активированы. Выбор чтения — `20_SplitDoc/CONTEXT_ROUTING.md`. При неизвестном исполнителе не угадывать режим.')
start = start.replace('Git контролирует пользователь. Без прямого разрешения запрещены любые Git write-команды и изменения веток, tags, remotes, config, индекса или истории.',
                      'Git контролирует пользователь. Codex запрещены все Git/GitHub-операции, включая чтение CLI/UI/API, без отдельного прямого разрешения на конкретный scope. Исторический SHA не является разрешением.')
start = start.replace('Он является единственным владельцем правил AGENT REPORT, TASK_HISTORY, BLOCKED, очистки БЛОКА 1 и сообщения об обновлении модов.',
                      'Он владеет правилами report/manifest, архива, FOLLOWUP и защищённой ротации координатором. Исполнитель не очищает свою задачу и не пишет независимый БЛОК 4.')
start = start.replace('При `BLOCKED`, неполном результате или нерешённой критичной ошибке БЛОК 1 не очищать.',
                      'При DRAFT/BLOCKED или неполном пакете БЛОК 1 сохраняется. Финальные PARTIAL/REVISE допускают только координируемую ротацию по lifecycle, без продуктового PASS. TASK 184 не ротируется исполнителем.')
document('12_Старт.md', start)

# Small noncompeting compatibility nodes preserve old external/history targets.
for name, title, target, snap in [
    ('11_Задача_агенту.md', 'ЗАДАЧА АГЕНТУ', '11_Задачи_Codex', '11_Задача_агенту.md'),
    ('12_Старт_агента.md', 'СТАРТ АГЕНТА', '12_Старт', '12_Старт_агента.md')]:
    legacy = f'''# {title}

LEGACY LOCATOR — здесь нет активной постановки или второго регламента.
Действующий документ: [[{target}]]. Не выполнять TASK по этому старому имени.
Byte-exact источник до TASK 184:
`Task_Reports/TASK_184/snapshot/files/Documentation_DayZ_Qwest/{snap}`.
Исторические пути/якоря ведут к сохранённому источнику; текущие правила — в новом документе.
'''
    # Preserve headings as address compatibility, not executable historical bodies.
    for line in old(name).splitlines():
        if re.match(r'^#{1,6} ', line) and line != '# ' + title:
            legacy += '\n' + line + '\n\nСохранённый раздел — в byte-exact snapshot выше; это исторический адрес.\n'
    document(name, legacy, False)

document('001_История_задач_Codex.md', '''# История задач Codex

[[11_Задачи_Codex|Персональная карточка Codex]]

## Владелец и сохранность

Append-only архив. Заполняет ChatGPT-координатор после публикации, независимого
review и проверки исходной постановки. Codex в TASK 184 создаёт только структуру.
Архив хранит также отрицательные verdict/PARTIAL/REVISE; наличие записи не равно VERIFIED.
Не дублировать старый `20_SplitDoc/TASK_HISTORY.md` и не угадывать исполнителей прошлых TASK.

## Формат новой записи

- event_id / TASK ID / iteration / дата / источник назначения;
- точный полный текст TASK и принятых AMENDMENT либо постоянный snapshot + SHA-256;
- source/result revision (UNKNOWN, если не проверена), report/manifest/evidence locator;
- factual execution, publication, independent verdict/scope/NOT RUN раздельно;
- решения пользователя, OPEN/FOLLOWUP-ID, successor и условия закрытия;
- previous_slot snapshot + hash, readback, неизменность старого архивного префикса.

Литеральные TASK/AMENDMENT сохранять без изменения байтов. Если исходник уже
сохранён в постоянном файле, дать точный locator/hash. Не создавать случайные
wiki-рёбра копированием цитат. Перед вытеснением слота проверить весь предыдущий
1.1 (включая AMENDMENT), отчёт и verdict. Один подвижный main или summary недостаточны.

## Исторические источники до переключения

Это указатели происхождения, не новые принятые записи:
- Общая прежняя история: `20_SplitDoc/TASK_HISTORY.md` — сохранена без изменения.
- Полный старый №11: `Task_Reports/TASK_184/snapshot/files/Documentation_DayZ_Qwest/11_Задача_агенту.md`.
- Отчёт 181: `Task_Reports/TASK_184/snapshot/block_2_legacy.md`.
- Review 101: `Task_Reports/TASK_184/snapshot/block_3_legacy.md`.

## Принятые записи

Пока не заполнено координатором. TASK 184 остаётся активной и не закрыта этим файлом.
''', False)

register = '''# Реестр задач, состояний и открытых продолжений

## Назначение и чтение

Этот вход действует после локального переключения TASK 184; независимая приёмка
миграции ещё ожидается. Ниже сохранён полный прежний backlog byte-exact.
Его метка LEGACY относится к старому разделу, а не к этому активному входу.
№39 остаётся глобальным roadmap, №40 — архивом принятых глобальных этапов.
Координатор перечитывает все OPEN перед новым назначением, после review и при смене чата.
Исполнитель использует только явно назначенные записи; история не становится TASK.

## Текущий реестр TASK

| record_id / TASK | execution / publication | review / verifier | owner / evidence / next |
| --- | --- | --- | --- |
| REG-184 / TASK-184 | LOCAL IMPLEMENTATION; публикация UNKNOWN | REVIEW PENDING / ChatGPT | Codex; `Task_Reports/TASK_184/report.md`, `manifest.json`; публикация пользователем, затем независимая проверка |
| REG-181 / TASK 181 | AUTHOR COMPLETED по прежнему отчёту | independent verdict НЕ ПОДТВЕРЖДЁН текущим №11 | ChatGPT; snapshot/block_2_legacy.md, TASK_HISTORY → TASK 181; адресная сверка |
| REG-107 / TASK 107 | STATUS CONFLICT: legacy «Выполняется», история описывает выполненную подготовку | UNKNOWN / ChatGPT | Сверить конкретный результат и принятие; старые статусы сохранены |
| REG-108 / TASK 108 | STATUS CONFLICT: legacy «В очереди», результат выноса правил описан TASK 181 | UNKNOWN / ChatGPT | Проверить соответствие scope, не присваивать VERIFIED автоматически |

Публикация текущей версии не проверялась: Git/GitHub в TASK 184 запрещены.
Полный пакет REG-181 находится в `Task_Reports/TASK_184/snapshot/block_2_legacy.md`;
это отчёт автора, не независимое принятие. Полные старые записи107/108 — ниже.

## Открытые продолжения (FOLLOWUP)

| FOLLOWUP-ID | Источник и тип | owner / gate | Действие и независимый критерий | status / successor |
| --- | --- | --- | --- | --- |
| FU-DOC-107-STATUS | legacy TASK107 ↔ TASK_HISTORY TASK107; STATUS_CONFLICT | ChatGPT / согласованность реестра | Установить результат и фактический verdict по точному источнику | OPEN / UNASSIGNED |
| FU-DOC-108-STATUS | legacy TASK108 ↔ TASK181; STATUS_CONFLICT | ChatGPT / согласованность реестра | Сверить требование выноса подготовки с реализацией и review | OPEN / UNASSIGNED |
| FU-DOC-181-REVIEW | прежний БЛОК2 TASK181, БЛОК3 TASK101; VERIFICATION_GAP | ChatGPT / подтверждение181 | Найти проверенный verdict именно181 либо выполнить отдельный review | OPEN / UNASSIGNED |
| FU-UI-101-LONG-TEXT | snapshot/block_3_legacy.md; историческое ограничение, VERIFICATION_GAP | ChatGPT / актуальность UI-ограничения | Адресно установить, остаётся ли ограничение; runtime только отдельным назначением | OPEN / UNASSIGNED |
| FU-DOC-SPLIT-QUESTION | №03: вопрос о дроблении правил ↔ TASK180/181; STATUS_CONFLICT | ChatGPT / решение вопроса | Проверить принятое решение; исходный №03 не переписан | OPEN / UNASSIGNED |
| FU-DOC-COPY-PATH | `copy_documentation.bat` использует старый Documentation; FACT_OBSERVATION | Пользователь / отдельный tooling scope | Отдельно решить судьбу скрипта; в184 не запускать и не менять | OPEN / UNASSIGNED |
| FU-184-COORDINATOR-EXEC | локальная utility; VERIFICATION_GAP | ChatGPT / реальная ротация | Подтвердить доступ координатора к опубликованной рабочей копии; иначе EXECUTION GATE BLOCKED | OPEN / UNASSIGNED |
| FU-184-OBSIDIAN-VISUAL | пользовательские настройки сохраняют старые имена; VERIFICATION_GAP | Пользователь + ChatGPT / GUI приёмка | Открыть новый START/карточку, проверить backlinks/graph; настройки не менять автоматически | OPEN / UNASSIGNED |

Источники для строк snapshot выше находятся в `Task_Reports/TASK_184/snapshot/`.
Это проверенные примеры незавершённого; реестр не объявляет полный исторический
аудит всех долгов. Каждая новая находка получает свой источник и owner.
Новую TASK назначает пользователь. Назначенный successor не закрывает OPEN.
VERIFIED_CLOSED требует независимого evidence; ACCEPTED_BY_USER — явного решения
с остаточным риском/датой пересмотра. Закрытие итерации REVISE не даёт продуктовый PASS.
Канон — `20_SplitDoc/TASK_LIFECYCLE_RULES.md`; история — №001/TASK_HISTORY.

## Исторический backlog — исходные байты сохранены

'''.encode()
changes['Documentation_DayZ_Qwest/05_Очередь_задач.md'] = register + original('Documentation_DayZ_Qwest/05_Очередь_задач.md')

lifecycle = '''# TASK LIFECYCLE RULES

Канонический цикл сдачи, сохранности и координируемой ротации DayZ.

## 1. Когда читать

Перед финальной сдачей implementation/documentation TASK и перед ротацией.
ANALYSIS ONLY не разрешает запись report/истории, ротацию или очистку.
Штатная сдача code/FIX не требует автоматически всего DOCUMENTATION_RULES.

## 2. Условия успешного завершения

Различать execution / publication / independent review / iteration closeout.
Codex сохраняет финальный report/manifest/evidence в разрешённом scope и сообщает
RESULT READY/PARTIAL/REVISE либо DRAFT/BLOCKED. Указать реальные проверки и NOT RUN.
Публикация и Git — пользователь; доступный инструмент не передаёт права Codex.
TASK 184: реальную активную задачу не ротировать, БЛОК1.1 остаётся uninitialized.

## 3. AGENT REPORT

Пакет: report.md + manifest.json + исходный task snapshot, AMENDMENT при наличии,
evidence. Формат report содержит отдельные строки `TASK ID: <ID>` и
`STATUS: RESULT READY` (либо PARTIAL/REVISE/DRAFT/BLOCKED), затем BASELINE, DONE,
CHANGED/CREATED, CHECKS/EXIT, ENCODING, NOT RUN, RISKS/QUESTIONS, DOC IMPACT, NEXT.
Manifest schema_version=1, task_id, final=true только для окончательной сдачи,
status равен report; artifacts — относительные к manifest пути без ../absolute,
для каждого size/sha256. Report и точный task snapshot обязательны в artifacts.
AMENDMENT — отдельные JSON-файлы task_id/amendment_id/text/source с реальным
происхождением и digest в manifest. Не выдумывать отсутствующие уточнения.
Автоматические метаданные/шаблон файла не доказывают принятие или публикацию.

БЛОК2 — компактный factual handoff, БЛОК3 — контекст/OPEN, БЛОК4 — независимый
verifier, READ-ONLY для автора. Перед вытеснением старого handoff сохранить
полный прежний текст и проверить readback. Автор не пишет финальный verdict.

## 4. TASK_HISTORY

Прежний TASK_HISTORY сохраняется. Новый №001 — append-only архив координатора:
полные TASK/AMENDMENT или проверенный постоянный snapshot с SHA, report/manifest,
independent verdict/scope/NOT RUN, OPEN и происхождение. Ни summary, ни подвижный
main, ни локальный TEMP не заменяют доказанный постоянный архив.
После публикации ChatGPT проверяет исходники и пакет, добавляет новые события
истории, не переписывая прежние verdict. Codex не закрывает себя самостоятельно.

## 5. Очистка БЛОКА 1

Ротацию инициирует ChatGPT-координатор при получении/приёмке финального пакета,
через доступный разрешённый локальный инструмент либо уполномоченный запуск.
Python не запускается от GitHub автоматически. При недоступности инструмента —
EXECUTION GATE BLOCKED, показать команду, не имитировать ROTATED.

Минимальный проверяемый запуск из project root (полные реальные пути подставляет
координатор; пример — инструкция, не разрешение ротировать TASK184 сейчас):

    python -B Tools/task_slot_rotate.py check --card "Documentation_DayZ_Qwest/11_Задачи_Codex.md" --task-id TASK-184
    python -B Tools/task_slot_rotate.py rotate --card "<card>" --task-id <ID> --snapshot "<exact-task.md>" --report "<report.md>" --manifest "<manifest.json>" --output-dir "<NEW handoff directory>"

rotate без --apply — полностью read-only dry-run. После проверки результата
координатор может повторить с --apply. output-dir — новый, его родитель уже
существует в разрешённой отчётной области. Все inputs/outputs внутри project root.
Непустой предыдущий1.1 требует --previous-archive <byte-exact previous_slot.md>
и --previous-sha256 <SHA256 всего предыдущего слота>; архив постоянный и не карточка.
Каждый принятый AMENDMENT передать отдельным --amendment <JSON>, включённым в manifest.

Успех: точный TASK в handoff_task.md и1.1, amendments сохранены отдельно без
пересказа; SUBMITTED / REVIEW PENDING, а1 содержит ОЖИДАНИЕ НОВОЙ ЗАДАЧИ.
БЛОКИ2–4 и байты вне слотов не меняются. JSON receipt сохраняет hashes/readback.
Это не PASS и не документальное закрытие: source/evidence проверяются отдельно.
PARTIAL/REVISE допустимы только как финальный сданный пакет. DRAFT/BLOCKED,
недостающий архив/evidence, повторная ротация, повреждённые границы/кодировка —
NO ROTATE, exit3; до commit point карточка и ранее существовавшие файлы неизменны.
Скрипт использует lock для своих запусков, reread/hash/identity перед atomic replace.
Несохранённый редактор не является источником: сохранить и закрыть карточку перед
запуском. Он не даёт абсолютной изоляции от несотрудничающего процесса между
последней проверкой и replace; это открытая граница, не серверная защита.
При аварии после commit point сохранить recovery evidence/card_before.md;
не восстанавливать поверх внешних изменений вслепую. Stale lock/частичный output
после остановки процесса требуют сверки координатором, не автоматического удаления.

## 6. Моды, build и runtime

''' + old('20_SplitDoc/TASK_LIFECYCLE_RULES.md').split('## 6. Моды, build и runtime', 1)[1].split('## 7.', 1)[0].strip() + '''

## 7. Остановка после отчёта

Codex сдаёт пакет и останавливается. Git/GitHub запрещены без отдельного
прямого разрешения. ChatGPT после пользовательской публикации независимо
проверяет exact TASK/AMENDMENT → report → source/diff/evidence по GIT_REVIEW_RULES.
Поле verdict другой TASK/ревизии не распространяется на текущую.

## 8. FOLLOWUP и закрытие итерации

№05 — постоянный владелец OPEN, рисков и задач-преемников. Координатор до closeout
сохраняет каждый невыполненный пункт: стабильный ID, origin TASK/версию/evidence,
тип (дефект/гипотеза/verification gap/не реализовано/конфликт), owner, gate,
действие, независимый критерий, successor=UNASSIGNED до назначения пользователем.
ASSIGNED/FIX REPORTED не равно VERIFIED_CLOSED. ACCEPTED_BY_USER требует явного
решения и условий остаточного риска. Возраст/ротация/смена чата не закрывают OPEN.
После независимого review допустимо закрыть итерацию с REVISE/PARTIAL, сохранив
непринятый продукт и все OPEN. DRAFT/BLOCKED остаются до полного пакета/решения
пользователя; скрипт их не ротирует. Нет архива/readback — DOC CLOSEOUT BLOCKED.
Порядок: полная история →05/FOLLOWUP →42/NEXT →БЛОК4/компактный handoff.
Состояния обновлять согласованно. Ротация1→1.1 — самостоятельная механическая
операция координатора, не замена этим стадиям. TASK+digest повторной доставки
дедуплицировать; прерванный closeout продолжать по event_id, без повторного переноса.
'''
document('20_SplitDoc/TASK_LIFECYCLE_RULES.md', lifecycle)

rules = old('10_Правила_агента.md').split('## Очистка выполненных задач')[0] + '''## Очистка выполненных задач

Codex сохраняет factual пакет и останавливается; свою TASK не очищает.
Ротация БЛОКА1→1.1 — ChatGPT по `20_SplitDoc/TASK_LIFECYCLE_RULES.md`, раздел5.
Точные маркеры/заголовки не удалять. Архив №001 и independent БЛОК4 — координатор.
Текущая184 остаётся в1 до отдельного координируемого действия после публикации.
Полная прежняя оболочка/отчёты/review — в `Task_Reports/TASK_184/snapshot/`.
'''
rules = rules.replace('рабочим shell', 'персональной карточкой')
document('10_Правила_агента.md', rules)

route = old('20_SplitDoc/CONTEXT_ROUTING.md')
route = section(route, '#### Маршрутизация по EXECUTOR', '''#### Маршрутизация по EXECUTOR

Единственная активная карточка — `11_Задачи_Codex.md`, только БЛОК1.
EXECUTOR: CODEX. Неизвестный/отсутствующий исполнитель — STOP, не угадывать.
Claude отложен, Ultra / CODEX + ULTRA не активированы. `Ultra/*` остаются
историческими источниками, не заданиями и не launch-payload текущего цикла.
Прежний dispatcher-контракт сохранён в snapshot/CONTEXT_ROUTING.md внутри
`Task_Reports/TASK_184/snapshot/files/Documentation_DayZ_Qwest/20_SplitDoc/`.
Ни история, ни NEXT не запускают новую работу. Ротация принадлежит ChatGPT.
''', '### REVIEW')
route = route.replace('БЛОК 2, БЛОК 3, TASK_HISTORY', 'БЛОКИ 1.1–4, TASK_HISTORY')
route = route.replace('Штатная запись AGENT REPORT, одной строки TASK_HISTORY и очистка БЛОКА 1', 'Штатная сдача report/manifest и разрешённая координируемая ротация')
document('20_SplitDoc/CONTEXT_ROUTING.md', route)

prep = old('20_SplitDoc/TASK_PREPARATION_RULES.md') + '''
## 9. Интерфейс защищённого цикла TASK 184

Полная постановка содержит единственное поле `**TASK ID:**` с неизменяемым ID,
EXECUTOR: CODEX, MODE и явный scope report/manifest/snapshot. Маркеры вставляет
карточка, не копируемая TASK. Пользователь назначает; координатор даёт полный
текст, короткий запуск и понятное объяснение результата. Заморозка сохраняется.
Принятые поправки отдельно с task_id/amendment_id/text/source; не подменять TASK.
Перед новым назначением проверить №05/FOLLOWUP и №42. Не считать задачу свободной
только из старой метки WAITING. Условие сдачи и роль ротации — TASK_LIFECYCLE_RULES.
'''
document('20_SplitDoc/TASK_PREPARATION_RULES.md', prep)

doc = old('20_SplitDoc/DOCUMENTATION_RULES.md')
doc = doc.replace('документ 11 — рабочим shell TASK / REPORT / REVIEW',
                  'документ 11 — персональной карточкой Codex с отдельными слотами1/1.1/2/3/4')
doc = doc.replace('AGENT REPORT, одной записи TASK_HISTORY и очистка БЛОКА 1', 'report/manifest и разрешённая координируемая ротация')
doc += '''
## 12. Карта владельцев после TASK 184

- №12 — короткий старт; №10/CONTEXT_ROUTING — адреса/маршрут без копирования норм.
- №11 Codex — назначение и компактный handoff;1.1 — точная последняя сданная TASK;
  БЛОК4 — независимый verifier, READ-ONLY для автора.
- №001 — append-only полные TASK/AMENDMENT и результаты/вердикты координатора.
- №05 — задачи/статусы/FOLLOWUP; старый backlog остаётся byte-exact.
- №42 — текущее состояние/решения/OPEN/NEXT; не полный архив и не квота3–6тыс.
- TASK_HISTORY — прежняя общая история; №30/38/39/40 — продуктовые владельцы.
- Task_Reports — report/manifest/evidence/snapshots; локальная запись не публикация.
Новых Claude/43/общего журнала нет. Структура001 сама не закрывает TASK184.
При переименовании проверять весь vault: wiki/Markdown/embeds/anchors и входящие
адреса; новые рёбра определяет реальная навигация. Compatibility locator не
второй активный вход. `.obsidian` не менять; отличать target resolution от GUI.
'''
document('20_SplitDoc/DOCUMENTATION_RULES.md', doc)

git_rules = old('20_SplitDoc/GIT_REVIEW_RULES.md')
git_rules = git_rules.replace('## 3. Read-only Git', '''## 3. Read-only Git

После TASK184 Codex запрещены также read-only Git/GitHub без отдельного прямого
разрешения на конкретные операции. Ни TASK ID, ни SHA, ни роль исполнителя не дают
такого разрешения. Команды ниже — справочник для пользователя/уполномоченного
reviewer, не автоматический маршрут Codex. Публикацию выполняет пользователь.
''')
git_rules += '''
## 14. Привязка независимого review и ротация

Reviewer фиксирует TASK ID/iteration, base/result SHA или точный диапазон,
report digest и область проверки. Latest main сверяется, но не подменяет
результат TASK, если поверх опубликованы другие изменения. REVIEW PENDING,
авторские тесты и ROTATED не являются независимым PASS. БЛОК4 — ChatGPT,
все OPEN сохраняются в05; механика ротации — TASK_LIFECYCLE_RULES, раздел5.
Если локальный запуск координатором недоступен — EXECUTION GATE BLOCKED.
'''
document('20_SplitDoc/GIT_REVIEW_RULES.md', git_rules)

# Navigation-only edits; preserve subsystem content.
for name in ['01_Текущее_состояние.md', '04_Архитектура.md',
             '21_Project_Docs/PROJECT_PATHS.md', '21_Project_Docs/README.md',
             '20_SplitDoc/ENCODING_RULES.md']:
    document(name, old(name))
home = old('00_Главная.md').replace('- [[12_Старт_агента]]', '''- [[12_Старт]]
- [[001_История_задач_Codex|Постоянная история Codex]]
- [[05_Очередь_задач|Реестр TASK и FOLLOWUP]]
- [[42_TACTICAL_CONTEXT|Тактический контекст]]''')
home = home.replace('## Аналитические основания и вспомогательные агенты',
                    '## Аналитические основания и неактивные исторические материалы')
home = home.replace('Ultra — документационная основа', 'Ultra — исторические материалы, не активный маршрут')
document('00_Главная.md', home)
document('20_SplitDoc/00_INDEX.md', old('20_SplitDoc/00_INDEX.md').replace('отчёт, история, BLOCKED и очистка БЛОКА 1', 'сдача пакета, история/FOLLOWUP и ротация координатором'))
document('21_Project_Docs/00_INDEX.md', old('21_Project_Docs/00_INDEX.md') +
         '\n## Организационный вход\n\n[[12_Старт]] → [[11_Задачи_Codex]]. Текущее: [[42_TACTICAL_CONTEXT]], OPEN: [[05_Очередь_задач]].\n')
for rel in ['README.md', 'JSON_Quvest/README.md']:
    text = remap(original(rel).decode('utf-8'))
    changes[rel] = text.encode('utf-8')

# Generate tactical context from verified documentary sources, not invented runtime facts.
tactical = '''# Тактический контекст Silver_77_Quests

Срез: 31.10.2026, TASK-184, локальные файлы до пользовательской публикации.
Автор: Codex. Независимая приёмка TASK184 — REVIEW PENDING.
Этот документ помогает возобновить координацию; не назначает работу автоматически.

## 1. Цель проекта и владельцы

DayZ: квесты, Migration, Zones, server events/world/map и отдельная Medicine.
Архитектура: [[30_Единая_система_проекта]]. Интеграция карты:
[[38_Интеграция_модов_на_карте]]. Крупные этапы: [[39_Глобальный_план]].
Их предметные контракты не менялись при документационной миграции.
Мелкий FIX не становится глобальным этапом, а implementation TASK не закрывает
этап39 без критериев/пользовательского принятия. №40 сохраняет отдельную роль.

## 2. Текущая работа и точка возобновления

Единственный действующий исполнитель — Codex, персональная карточка
[[11_Задачи_Codex]]. Активна TASK-184; БЛОК1 сохранён дословно, БЛОК1.1 пуст.
Исполнитель разрабатывает/тестирует механику на фикстурах, реальную184 не ротирует.
Пакет: `Task_Reports/TASK_184/report.md`, `manifest.json`, `migration_matrix.md`,
`rollback.md`, `snapshot/`, `tests/`. Окончательный авторский статус — вreport.
Следующая стадия: публикация пользователем → независимый ChatGPT source/evidence
review → защищённая ротация координатором и согласованный closeout.
Путь к utility: `Tools/task_slot_rotate.py` от project root. Никакого автозапуска
от GitHub, Git hooks, watcher или скрытого bridge не устанавливалось.
При недоступности локального запуска ChatGPT: EXECUTION GATE BLOCKED; точная
команда и контракт находятся в TASK_LIFECYCLE_RULES, раздел5, и rollback/report.

## 3. Проверяемые источники и ограничения baseline

Канонический root: `P:\\Silver_77_Quests`. Пользователь сообщил опубликованный
baseline `main@b98d082c7eb66b882aaed27efca5b0231bfeba85`, message100.
В184 Git/GitHub запрещены: HEAD/main, незакоммиченный diff и публикация заново
не проверялись. Источник реализации — текущие локальные байты, полный inventory
и hashes в `Task_Reports/TASK_184/snapshot/baseline.json`.
Нельзя называть этот локальный снимок идентичным опубликованному main.
Исходный №11 включает пользовательскую TASK184, отчёт181 и review101. Полный
byte-exact snapshot и отдельные B1/B2/B3 доступны в постоянной отчётной области.
Несохранённые буферы Obsidian не доступны из filesystem; не считать их снятыми
в snapshot. Перед координируемой ротацией сохранить/закрыть карточку.

## 4. Действующие решения перехода

1. Персональная карточка называется11_Задачи_Codex;12_Старт — короткое ядро.
2. Старые11/12 — compatibility locators с прежними заголовками, не вторые TASK.
3. БЛОК1 — текущее назначение;1.1 — ровно последняя точная сдача, не PASS.
4. БЛОК2 — factual handoff;3 — контекст/OPEN;4 — independent verifier.
5. Координатор инициирует проверяемый механический перенос. Codex не закрывает себя.
6. №001 создан как структура; принятые записи оформляет ChatGPT после публикации/review.
7. №05 — активный реестр и владелец FOLLOWUP; весь прежний backlog сохранён.
8. №42 не ограничивается3–6тыс. символов. Ориентир около20тыс. не требует
   дописывать пустой текст или скрывать OPEN ради квоты; полнота и locators важнее.
9. Claude и отдельная43 отложены. Ultra/CODEX+ULTRA не входят в активный маршрут;
   прежние файлыUltra не изменяются и не становятся новыми задачами.
10. `.obsidian/graph.json`/workspace не меняются. Связи строятся по реальной навигации.
11. `copy_documentation.bat` — старое копирование, не ротация, вне184.
12. Код DayZ, конфиги, runtime/PBO и технический Workshop не меняются.

## 5. Последние существенные TASK — адресный документальный срез

Это результаты из старой истории/отчётов, не повторный runtime-тест и не новый PASS.
'''
history = (D/'20_SplitDoc/TASK_HISTORY.md').read_text(encoding='utf-8')
for number in [177, 178, 179, 181]:
    line = next(line for line in history.splitlines() if line.startswith(f'- `TASK {number}`'))
    tactical += '\n### TASK ' + str(number) + '\n\n' + line + '\n'
tactical += '''
### TASK 184

Текущая миграция процесса: точный source/B1 сохранён, новая карточка/START,
структура архива, реестр/FOLLOWUP и utility с негативными тестами. Всё ещё
LOCAL ONLY / REVIEW PENDING. Не записано как принятое событие TASK_HISTORY/001.
Независимую сверку тестов и опубликованного результата выполняет ChatGPT позже.

## 6. Все текущие OPEN из нового реестра

Постоянный владелец — [[05_Очередь_задач]] → «Открытые продолжения (FOLLOWUP)».
Этот список — краткая проекция; перед новым назначением перечитать исходный реестр.

- FU-DOC-107-STATUS: legacy «Выполняется» против записи о выполненной подготовке.
  Owner ChatGPT; сверить результат/приёмку, не переписать исходные статусы.
- FU-DOC-108-STATUS: старая очередь против вынесенных правилTASK181.
  Owner ChatGPT; проверить соответствиеscope и наличие независимого принятия.
- FU-DOC-181-REVIEW: текущий старый отчёт181 соседствовал сreview101.
  Owner ChatGPT; нужен verdict именно181, не наследование чужого PASS.
- FU-UI-101-LONG-TEXT: старый review принимал компромиссUI с ограничением длинного
  текста. Требуется установить современную актуальность; runtime не проверялся.
- FU-DOC-SPLIT-QUESTION: в03 открыт вопрос дробления; TASK180/181 могут отвечать
  на него, но отдельное принятое решение здесь не выдумано.
- FU-DOC-COPY-PATH: script ссылается наDocumentation; отдельный scope пользователя,
  в184 не запускался и не исправлялся.
- FU-184-COORDINATOR-EXEC: подтвердить доступ reviewer к локальному запуску после
  публикации. Команда не равна исполнению; при отсутствии доступа явныйBLOCKED.
- FU-184-OBSIDIAN-VISUAL: статические ссылки проверяются отдельно от GUI/backlinks.
  Личные фильтры цвета/открытые вкладки могут по-прежнему показывать старые имена.

Каждый OPEN имеет successor UNASSIGNED до назначения пользователем. Передача
преемнику или ROTATED не закрывает пункт. Авторский FIX_REPORTED требует verifier.
Непроверенное отмечаетсяUNKNOWN/NOT RUN. Перечень не объявлен полным аудитом всех
старых ограничений за всю историю; новые факты добавляются с точным источником.

## 7. Безопасная сдача, архив и новые задачи

Исполнитель получает одну небольшую задачу с проверяемым итогом и точной матрицей
чтения/записи. Постановка заморожена; поправки — отдельно, с ID и происхождением.
TASK PREP принадлежитTASK_PREPARATION_RULES, выбор чтения —CONTEXT_ROUTING.
Сдача и механика —TASK_LIFECYCLE_RULES, independent review —GIT_REVIEW_RULES.
Не копировать полные нормы вSTART и не читать архивы рекурсивно при каждом входе.
Report/manifest содержат точные пути/sha/counts/exit/FAIL/NOT RUN; публикация —
самостоятельная стадия. Координатор сравниваетTASK→REPORT→source/evidence.
При ротации1.1 хранит дословныйTASK, amendments отдельно/с hash; старый1.1 нельзя
перезаписать без проверенного полного архива. Неподтверждённый архив блокирует
операцию. Скрипт проверяет маркеры/EOL/taskID, package и повторное изменение inputs.
DRY RUN ничего не пишет. --apply — только отдельный разрешённый запуск координатора.
PARTIAL/REVISE — финальная сдача, не продуктовый PASS; BLOCKED/DRAFT не ротируются.
БЛОК4 независим от автора. Послеreview все существенные OPEN идут в05; результат
и решения — в001, краткая история по необходимости вTASK_HISTORY. Старыеverdict
не переписываются; новая запись объясняет, что и в какой области supersedes.

## 8. Следующий разрешённый шаг

Пользователь публикует локальный пакетTASK184 и документационные изменения.
ChatGPT независимо сверяет конкретный опубликованный result, сохранность B1,
архивов, utilities/tests, пути, кодировки, scope и защищённые файлы. Публикация
не запускает ротацию автоматически и не объявляет внедрение принятым.
Только координатор при наличии локального канала может инициировать dry-run,
проверитьJSON, затем отдельно --apply в согласованной рабочей копии.
После приёмки/сохраненияOPEN — новаяTASK по назначению пользователя.
Следующий продуктовый этап выбирается по39, не из этого документационногоNEXT.

## 9. Навигация и сохранность

[[12_Старт]]; [[11_Задачи_Codex]]; [[001_История_задач_Codex]];
[[05_Очередь_задач]]; `20_SplitDoc/TASK_HISTORY.md`.
Исходные файлы миграции и77 ссылочных вхождений инвентаризированы в snapshot/baseline.
Точная матрица изменений/оставленных исторических адресов —migration_matrix.md.
Snapshot не является второй активной карточкой: полные Markdown-источники там
могут отображаться дополнительными узлами Obsidian. Это архивное свидетельство,
не скрытая перестройка пользовательского graph layout. Существующие ссылки вне
scope и сами settings сохраняются; визуальный результат проверяет пользователь.
При rollback нельзя восстановить старые байты поверх новых пользовательских правок.
Сначала hashes/адресная обратная дельта, инструкция вTask_Reports/TASK_184/rollback.md.
'''
document('42_TACTICAL_CONTEXT.md', tactical, False)

# All replacements use byte-checked snapshots; all new paths are create-only.
for rel, data in changes.items():
    p = ROOT / rel
    if rel in BASE['files']:
        assert hashlib.sha256(p.read_bytes()).hexdigest() == BASE['files'][rel]['sha256'], rel
    else:
        assert not p.exists(), rel
    data.decode('utf-8', errors='strict')
for rel, data in changes.items():
    p = ROOT / rel
    p.write_bytes(data)
    assert p.read_bytes() == data
(OUT/'tests/migration_writes.json').write_text(json.dumps(list(changes), ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'written': len(changes), 'card_task_exact': task in card,
                  'tactical_characters': len(tactical), 'real_rotation': False}, ensure_ascii=False))
