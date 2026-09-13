# Silver_77_Medicine — приоритетная задача

Короткий living-document. Историю изменений здесь не хранить.

## ПРИОРИТЕТ №1

До написания полноценного медицинского движка проверить минимальным prototype, можно ли безопасно добавить собственный disease agent / modifier поверх существующей vanilla `AgentPool` / `Modifier` architecture.

Будущий prototype должен ответить:

- работает ли custom agent registration;
- не конфликтует ли ID;
- переживает ли состояние persistence;
- реагирует ли custom modifier на disease load;
- можно ли отдельно синхронизировать client symptom;
- стоит ли использовать vanilla `AgentPool` как основу или нужен собственный `S77 DiseaseState Manager`.

В рамках документационного baseline prototype не реализуется.
