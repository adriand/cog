#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "cog.h"
#include "person.h"

/* make_person: given a name, inserts the person into the database and produces a person 
 * structure. later, can be expanded for other details.
 *
 * this looks interesting: http://www.lemoda.net/sqlite/c-insert/
 *
 * Binding variables in SQLite: http://www.adp-gmbh.ch/sqlite/bind_insert.html
 * Quick tip here: http://stackoverflow.com/questions/3137808/sqlite-and-iphone-input-sanitisation 
 */
struct person new_person(char *n)
{
  struct person temp;
  temp = make_person(0, n);

  sqlite3_reset(stmt);
  sqlite3_prepare_v2(db, "INSERT INTO people (name) VALUES(?)", -1, &stmt, NULL);
  /* Args here are: the statement, index of leftmost parameter (1), the text to bind, -1 means read up to zero terminator,
   * SQLITE_TRANSIENT means make a private copy of data (some kind of mem mgmt thing) */
  sqlite3_bind_text(stmt, 1, temp.name, -1, SQLITE_TRANSIENT);
  /* not sure this behaviour makes the most sense in terms of a return value */
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    return temp;
  } else {
    /* get the ID of the inserted row */
    temp.id = sqlite3_last_insert_rowid(db);
    printf("Inserted %s (id:%d) successfully.\n", temp.name, temp.id);
    return temp;
  }
}

/* find_person: given a query, finds a person in the database.
 * Returns -1 for unsuccessful search, or the number of results for successful searches.  Also populates an array, 
 * whose pointer is passed to this function, with matching people.
 */
int find_person(char *name, struct person *results)
{
  char query_name[MAXNAME];
  sqlite3_reset(stmt);
  if (sqlite3_prepare_v2( db,
                          "SELECT * FROM people WHERE name LIKE ? LIMIT 10", 
                          -1, 
                          &stmt, 
                          NULL
                        ) == SQLITE_OK) {
    sprintf(query_name, "%%%s%%", name); /* to get '%name%' */
    sqlite3_bind_text(stmt, 1, query_name, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) { /* at least one result */
      int i = 0;
      do {
        *(results + i) = make_person(sqlite3_column_int(stmt, 0), (char *) sqlite3_column_text(stmt, 1));
      } while (i++ < MAX_SEARCH_RESULTS && sqlite3_step(stmt) == SQLITE_ROW); // the order here is important, otherwise i does not get incremented
      return i;
    } else {
      return -1;
    }
  }
}

/* destroy_person: given a person's ID, deletes them from the database */
int destroy_person(int id)
{
  char query[100];
  sqlite3_reset(stmt);
  sprintf(query, "DELETE FROM people WHERE id = %d", id);
  if ((sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) && sqlite3_step(stmt) == SQLITE_DONE)
    return 1; // TODO: delete associations, too, like in people_places
  else
    return 0;
}

/* associate_with_place: given a person's ID and a place's ID, associates that person with the place */
int associate_with_place(int person_id, int place_id)
{
  char query[100];
  sqlite3_reset(stmt);
  /* need to check to make sure they're not already associated, to avoid creating additional records */
  sprintf(query, "SELECT * FROM people_places WHERE person_id = %d AND place_id = %d", person_id, place_id);
  if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) { // already exists so do nothing
      return -1;
    } else {
      sprintf(query, "INSERT INTO people_places (person_id,place_id) VALUES(%d,%d)", person_id, place_id);
      sqlite3_reset(stmt);
      if (sqlite3_step(stmt) == SQLITE_DONE) {
        return 1;
      } else {
        return -1;
      }
    }
  }
}

/* make_person: creates a structure for a person based on passed values and returns it */
struct person make_person(int id, char *name)
{
  int i = 0;
  struct person temp;
  /* id */
  temp.id = id;
  /* name */
  while (temp.name[i++] = *name++)
    ;
  return temp;
}

void print_person(struct person *individual)
{
  printf("%d: %s", individual->id, individual->name);
}
