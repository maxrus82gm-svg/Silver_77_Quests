# История задач Codex

`11_Задачи_Codex`

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
