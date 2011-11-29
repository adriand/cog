#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "cog.h"

/* list: list records that have a 'name' field in the database */
void list_named_records(char *table, char *condition, char *order)
{
  char query[100];
  sprintf(query, "SELECT * FROM %s WHERE %s ORDER BY %s", table, condition, order);
  sqlite3_reset(stmt);
  if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) { /* at least one result */
      do {
        printf("%d: %s\n", sqlite3_column_int(stmt, 0), (char *) sqlite3_column_text(stmt, 1));
      } while (sqlite3_step(stmt) == SQLITE_ROW);
    } else {
      printf("No records found.");
    }
  }
}
