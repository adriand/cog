/* cog
 * is a tool desiged to extend the mind's capacity to remember
 * people and details about their lives within a particular context.
 *
 * Because I am just getting started with this language, I am going to iterate in a deliberate
 * fashion, starting out with something very simple and going from there.
 *
 * Iteration 1:
 * - enter a new person
 * - lookup a person
 * Requires SQLite for data storage.
 * See http://www.sqlite.org/cintro.html, http://www.sqlite.org/quickstart.html
 * Also: http://wang.yuxuan.org/blog/2007/5/22/simple_sqlite_test_c_program_without_callbacks
 *
 * Iteration 2:
 * - enter a new place
 * - improve the menu handling and file organization of the project
 * - attach a person to that place
 * - find people associated with that place
 * Requires a database for data storage (SQLite)
 *
 * Iteration 3:
 * - delete a person
 * - delete a place
 * - edit a person
 * - edit a place
 * - associate a person with another person or people
 *
 *
 * Database Structure (you can get this by typing .schema in the sqlite shell)
 * CREATE TABLE people( id INTEGER PRIMARY KEY, name TEXT );
 * CREATE TABLE people_places(person_id INTEGER, place_id INTEGER);
 * CREATE TABLE places( id INTEGER PRIMARY KEY, name TEXT );
 * 
 */

#include <stdio.h>
#include <stdlib.h> /* required for exit() */
#include <string.h>
#include <sqlite3.h>
#include "cog.h"
#include "person.h"
#include "place.h"
#include "main.h"

int main()
{
  int choice;
  int response;

  response = sqlite3_open(DATABASE_NAME, &db);

  if(response) {
    fprintf(stderr, "sql error #%d: %s\n", response, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }

  printf("Welcome to cog.");
  /* some help for this menu provided by http://cboard.cprogramming.com/c-programming/64154-how-make-menu-c-dos.html */
  do
  {
    choice = get_choice("(n)ew person, (g)et person, (l)ist people, new (p)lace, l(i)st places");
    switch(choice)
    {
      case 'n':
        choose_new_person();
        break;
      case 'g':
        choose_get_person();
        break;
      case 'l':
        choose_list_people();
        break;
      case 'p':
        choose_new_place();
        break;
      case 'i':
        choose_list_places();
        break;
      case 'q':
        printf("Quitting cog...\n");
        break;
      default:
        printf("Invalid choice.\n");
        break;
    }
  } while (choice != 'q');
  sqlite3_close(db);
}

void choose_new_person()
{
  /* code for the way this works helpfully adapted from http://www.gidnetwork.com/b-56.html */
  char name[MAXNAME];
  struct person p;

  printf("Enter the person's name: ");

  fgets(name, MAXNAME, stdin);
  limit_text_input(name);
  chomp(name);
  p = new_person(name);
}

void choose_get_person()
{
  char query[MAXNAME];
  int result_count;
  struct person *selection;
  struct person results[MAX_SEARCH_RESULTS];
  printf("Enter part of the person's name: ");

  fgets(query, MAXNAME, stdin);
  limit_text_input(query);
  chomp(query);

  result_count = find_person(query, results); 
  switch (result_count) {
    case -1:
      printf("Sorry, no people were found.");
      break;
    case 1:
      work_with_person(results);
      break;
    default:
      printf("\n%d matches found: \n", result_count);
      selection = choose_from_list_of_people(results, result_count);
      if (selection != NULL)
        work_with_person(selection);
      break;
  }
}

void work_with_person(struct person *individual)
{
  char choice;
  printf("Selected ");
  print_person(individual);
  printf("\n(e}dit %s, (d)elete %s, Enter to cancel: ", individual->name, individual->name);
  choice = get_one_letter_response_on_a_line();
  switch (choice) {
    case 'e':
      // edit
      break;
    case 'd':
      if (destroy_person(individual->id))
        printf("\n%s was deleted successfully.", individual->name);
      else
        printf("\nFailed to delete %s.", individual->name);
      break;
    default:
      break;
  }
}

/* choose_from_list_of_people: given an array of people and the number of people to be concerned with,
 * asks the user to select one from the list, and returns a pointer to the person or NULL */
struct person *choose_from_list_of_people(struct person *results, int result_count)
{
  char choice[10];  /* seems unlikely we'll ever have more than 9999999999 records to choose from */
  int ichoice;
  int i = 0;

  while (1) {
    while (i < result_count) {
      print_person(results + i);
      printf("\n");
      i++;
    }
    printf("\nPlease enter the number of the person or press Enter to cancel: ");
    fgets(choice, 10, stdin);
    limit_text_input(choice);
    if (strlen(choice) == 1) { // pressed enter
      printf("Cancelled.\n");
      return NULL;
    } else {
      ichoice = atoi(choice);
      for (i = 0; i < result_count; i++)
        if ((*(results + i)).id == ichoice) // if the id equals the choice
          return results + i;               // return a pointer to that result
      printf("Invalid choice.");          // if they get this far, it's an invalid choice
    }
  }
}

void choose_list_people()
{
  printf("Listing everyone:\n\n");
  /* TODO: make this accept a list of everyone and do the printing here.  In fact, that's really the
   * best organization overall - separate the views from the database access layer!
   */
  list_named_records("people", "1", "name");
}

void choose_new_place()
{
  char name[MAXNAME];
  struct place p;

  printf("Enter the place's name: ");

  fgets(name, MAXNAME, stdin);
  limit_text_input(name);
  p = new_place(name);
}

void choose_list_places()
{
  printf("Listing places:\n\n");
  list_named_records("places", "1", "name");
}

/* get_choice: displays a menu and choices, returns an integer representing the choice */
int get_choice(char *choices)
{
  char choice;
  printf("\n%s, (q)uit:\n", choices);
  choice = get_one_letter_response_on_a_line();
  return choice;
}

/* from: http://www.velocityreviews.com/forums/t318260-peek-at-stdin-flush-stdin.html */
int get_one_letter_response_on_a_line() {
  int c, first;

  /* get and save the first character */
  first = c = getchar();

  /* if it was not newline, keep getting more (but stop on EOF) */
  while (c != '\n' && c != EOF)
    c = getchar();

  /* in any case, return the first one, which may be '\n' or EOF */
  return first;
}

/* we expect a newline or end of string character at the end of appropriate length strings.
 * If one does not exist, add one and then eat the rest of the input, because the user is probably
 * entering strings that are too long.
 */
void limit_text_input(char *input)
{
  int c;
  input += strlen(input);   /* move to end of string */
  if (*input != '\0') {     /* if the last character is not an end-of-line character */
    c = *input;               /* store end of string */
    *input = '\0';            /* make the last character an end-of-line character */
    if (c != '\n') {          /* if the original last character is not end-of-line, there are extra characters */
      while (c != '\n' && c != EOF)   /* so chomp them */
        c = getchar();
    }
  }
}

/* chomp: removes \n on the end of a string */
void chomp(char *input)
{
  while (*input != '\0', input++) {  /* traverse the string */
    if (*input == '\n') {            /* if we find a \n, kill it */
      *input = '\0';
      return;
    }
  }
}
