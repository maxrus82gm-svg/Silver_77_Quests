# Silver_77_Migrate — приоритетная задача

Этот файл содержит только следующую фактическую цель ветки `Silver_77_Migrate`. После смены приоритета текущее содержимое обновляется, а история здесь не накапливается.

## ПРИОРИТЕТ №1

Подключить Migration к существующей Web Map как первый настоящий редактируемый spatial layer.

Будущая техническая задача должна позволить:

- читать canonical `Silver_77_Migrate_server/config/MigrationConfig.json`;
- показывать `GROUP`, spawn, ordered routePoints, target и существующие spatial radii;
- выбирать `GROUP` или точку и перемещать пространственные точки непосредственно на карте;
- изменять принадлежащие Migration config радиусы;
- сохранять изменённые spatial values обратно в canonical Migration config.

Map является spatial editor и представлением данных. Она не владеет Migration-данными и не создаёт второй источник истины. Вся не-spatial логика остаётся собственностью `Silver_77_Migrate`.

После завершения Map integration этот файл обновляется следующей фактической задачей Migration, включая продолжение runtime, persistence, director или других незавершённых частей.
