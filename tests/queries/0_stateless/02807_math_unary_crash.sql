DROP TABLE IF EXISTS t10;
CREATE TABLE t10 (`c0` Int32) ENGINE = MergeTree ORDER BY tuple();
INSERT INTO t10 (c0) FORMAT Values (-1);
SELECT 1 FROM t10 GROUP BY erf(-sign(t10.c0));
SELECT 1 FROM t10 GROUP BY -sign(t10.c0);
DROP TABLE t10;
