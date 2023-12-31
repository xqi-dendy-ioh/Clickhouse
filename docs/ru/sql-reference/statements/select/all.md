---
slug: /ru/sql-reference/statements/select/all
sidebar_label: ALL
---

# Секция ALL {#select-all}

Если в таблице несколько совпадающих строк, то `ALL` возвращает все из них. Поведение запроса `SELECT ALL` точно такое же, как и `SELECT` без аргумента `DISTINCT`. Если указаны оба аргумента: `ALL` и `DISTINCT`, функция вернет исключение.


`ALL` может быть указан внутри агрегатной функции, например, результат выполнения запроса:

```sql
SELECT sum(ALL number) FROM numbers(10);
```

равен результату выполнения запроса:

```sql
SELECT sum(number) FROM numbers(10);
```
