#!/usr/bin/env bash

CURDIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../shell_config.sh
. "$CURDIR"/../shell_config.sh

{ printf "select "; for _ in {1..1000}; do printf "coalesce(null, "; done; printf "1"; for _ in {1..1000}; do printf ")"; done; } > "${CLICKHOUSE_TMP}"/query

echo '-- 1.'
cat "${CLICKHOUSE_TMP}"/query | $CLICKHOUSE_CLIENT 2>&1 | grep -o -m1 -F 'Code: 167'
echo '-- 2.'
cat "${CLICKHOUSE_TMP}"/query | $CLICKHOUSE_LOCAL 2>&1 | grep -o -m1 -F 'Code: 167'
echo '-- 3.'
cat "${CLICKHOUSE_TMP}"/query | $CLICKHOUSE_CURL --data-binary @- -vsS "$CLICKHOUSE_URL" 2>&1 | grep -o -m1 -F 'Code: 167'
