# TASK184 — rollback и повторная проверка

Реальный rollback не выполнялся. На фикстуре проверены dry-run, отказ при чужой
правке и восстановление точных байтов с сохранением новых кандидатов.
Никакие команды ниже не вызывают Git/GitHub.

## Read-only проверка

Из `P:\Silver_77_Quests`:

    python -B Documentation_DayZ_Qwest/Task_Reports/TASK_184/tests/validate_migration.py
    python -B Documentation_DayZ_Qwest/Task_Reports/TASK_184/tests/rollback_migration.py --root P:/Silver_77_Quests --state P:/Silver_77_Quests/Documentation_DayZ_Qwest/Task_Reports/TASK_184/migration_state.json

Вторая команда по умолчанию только проверяет hashes и сообщает ROLLBACK_DRY_RUN.
Ожидаемый exit0. Чужая правка/snapshot mismatch/link — ROLLBACK_BLOCKED, exit3.
Проверка отчётных tests без `--record` не перезаписывает сохранённые результаты.
На временных фикстурах создаются/удаляются только файлы в tests/fixture-*.

## Разрешённое восстановление

Только после отдельного решения пользователя/координатора, на сохранённой
неизменяемой в этот момент рабочей копии, та же rollback-команда с `--apply`.
Сначала утилита проверяет весь migration_state.json и все snapshot hashes.
Затем сохраняет каждый текущий кандидат в
`Task_Reports/TASK_184/rollback_saved/<исходный repository path>`.
После readback возвращает существовавшие документы из snapshot, а новые
функциональные файлы удаляет только если их hash равен принятому after-hash.
Отчётный пакет и snapshots не удаляются. Настройки .obsidian не восстанавливаются.
Изменение любого файла после состояния миграции блокирует операцию до записи.
При частичном сбое после начала восстановления не повторять вслепую: сравнить
сохранённые кандидаты, восстановленные bytes и оставшиеся файлы, согласовать
адресную обратную дельту. Это не многотомная атомарная транзакция.

Вернувшийся старый №11 содержит точную TASK184 из исходного snapshot. Новый
№11 после удаления всё равно сохранён в rollback_saved; уникальные данные не теряются.
После реальной ротации или нового назначения эта инструкция сначала откажет поhash;
это требуемая защита, а не повод обходить guard.

## Будущая ротация координатором — отдельная операция

В TASK184 она не выполнялась. После пользовательской публикации ChatGPT должен
проверить точную версию, локальный доступ и разрешение запуска. При недоступном
инструменте — EXECUTION GATE BLOCKED, не поручение пользователю вручную переносить слоты.
Пример для текущего первоначально пустого1.1, только после такого разрешения:

    python -B Tools/task_slot_rotate.py check --card Documentation_DayZ_Qwest/11_Задачи_Codex.md --task-id TASK-184
    python -B Tools/task_slot_rotate.py rotate --card Documentation_DayZ_Qwest/11_Задачи_Codex.md --task-id TASK-184 --snapshot Documentation_DayZ_Qwest/Task_Reports/TASK_184/snapshot/block_1_task.md --report Documentation_DayZ_Qwest/Task_Reports/TASK_184/report.md --manifest Documentation_DayZ_Qwest/Task_Reports/TASK_184/manifest.json --output-dir Documentation_DayZ_Qwest/Task_Reports/TASK_184/handoff_by_coordinator

Вторая команда — dry-run без записей. Только координатор после проверки JSON
повторяет её с `--apply`. В этом отчёте она не запускалась даже в dry-run на184.
При следующей ротации непустого1.1 обязательны --previous-archive и
--previous-sha256. Accepted AMENDMENT передаются явно через --amendment и manifest.
Output-dir должен быть новым каталогом внутри report package; parent уже существует.

Exit0: CHECK_OK/DRY_RUN/ROTATED. Exit3: NO ROTATE до commit point; ранее существовавшие
файлы не изменены, собственные временные artifacts удалены. Exit4: COMMIT_UNCERTAIN,
ошибка/внешнее изменение после atomic replace; recovery evidence сохраняется,
восстановление поверх внешнего редактирования автоматически не выполняется.
Ротация не создаёт independent PASS. Shared scope защищается lock между своими
запусками и повторной проверкой bytes/stat перед replace. Несотрудничающий редактор
после последней проверки и аппаратный сбой не покрываются абсолютной транзакцией.
Сохранить/закрыть редактор перед координируемым запуском.
