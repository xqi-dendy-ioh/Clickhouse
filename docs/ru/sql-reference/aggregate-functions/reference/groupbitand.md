---
slug: /ru/sql-reference/aggregate-functions/reference/groupbitand
sidebar_position: 125
---

# groupBitAnd {#groupbitand}

Применяет побитовое `И` для последовательности чисел.

``` sql
groupBitAnd(expr)
```

**Аргументы**

`expr` – выражение, результат которого имеет тип данных `UInt*`.

**Возвращаемое значение**

Значение типа `UInt*`.

**Пример**

Тестовые данные:

``` text
binary     decimal
00101100 = 44
00011100 = 28
00001101 = 13
01010101 = 85
```

Запрос:

``` sql
SELECT groupBitAnd(num) FROM t
```

Где `num` — столбец с тестовыми данными.

Результат:

``` text
binary     decimal
00000100 = 4
```
