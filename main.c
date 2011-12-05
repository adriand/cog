#include <stdio.h>
#include <stdlib.h> /* required for exit() */
#include <string.h>
#include <sqlite3.h>
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

  /* some help for this menu provided by http://cboard.cprogramming.com/c-programming/64154-how-make-menu-c-dos.html */
  do
  {
    printf("cog\n---\n");
    choice = get_choice("(f)ind, (p)eople, p(l)aces", 1);
    switch(choice)
    {
      case 'f':
        choose_find();
        break;
      case 'p':
        choose_people();
        break;
      case 'l':
        choose_list_places();
        break;
      /*
      case 'n':
        choose_new_person();
        break;
      case 'g':
        choose_get_person();
        break;
      case 'p':
        choose_new_place();
        break;
      */
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

void choose_find()
{
  printf("Find people, places, strange and unknown entities: ");
}

/* get_choice: displays a menu and choices, returns an integer representing the choice */
int get_choice(char *choices, int show_quit)
{
  char choice;
  if (show_quit) {
    printf("\n%s, (q)uit:\n", choices);
  } else {
    printf("\n%s:\n", choices);
  }
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
