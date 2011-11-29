#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "cog.h"
#include "place.h"

struct place new_place(char *n)
{
  int i = 0; 
  struct place temp;

  while (temp.name[i++] = *n++)
    ;

  sqlite3_reset(stmt);
  sqlite3_prepare_v2(db, "INSERT INTO places (name) VALUES(?)", -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, temp.name, -1, SQLITE_TRANSIENT);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    return temp;
  } else {
    temp.id = sqlite3_last_insert_rowid(db);
    printf("Inserted %s (id:%d) successfully.\n", temp.name, temp.id);
    return temp;
  }
}
