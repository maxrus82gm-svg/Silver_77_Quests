# TASK LIFECYCLE RULES

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
Необязательный source_files фиксирует repository-relative пути, size/sha256
исходников; при его наличии утилита проверяет их версию и повторное изменение.
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
координатор может повторить с --apply. output-dir — новый каталог внутри report package (каталога manifest); его
родитель уже существует в разрешённой отчётной области. Все inputs/outputs внутри project root.
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

В отчёте явно писать, требуется ли обновление модов.

- Менялись клиентские/shared структуры или клиентские скрипты — указать `Silver_77_Quests_Client`.
- Менялась серверная логика/runtime — указать `Silver_77_Quests_Server`.
- Менялись обе стороны — указать оба мода.
- Менялись только документация, JSON или локальный редактор `JSON_Quvest` — написать: `Моды обновлять не нужно.`

Не выполнять build, packing, deployment или перенос output без прямого разрешения.

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
