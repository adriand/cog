#define MAXNAME         20 /* maximum length of a name */
#define DATABASE_NAME   "cog.sqlite3"
#define DB_ERROR        0
#define DB_SUCCESS      1

#define MAX_SEARCH_RESULTS    10

/* database handle */
sqlite3 *db;
/* a good example SQLite program is at http://wang.yuxuan.org/blog/2007/5/22/simple_sqlite_test_c_program_without_callbacks
 * actually, there are some issues with this that tripped me up.
 * */
sqlite3_stmt *stmt;

/* db functions */
void list_named_records();

/* other functions */
int get_one_letter_response_on_a_line();
void limit_text_input(char *);
