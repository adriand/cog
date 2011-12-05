#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "place.h"

void choose_places()
{

}

/* now I'm not sure if I should go down this route... */
void associate_person_with_place(struct person *individual)
{
  char choice;

  choose_list_places();
  printf("\nEnter the ID of a place to associate %s with, Enter to cancel: ", individual->name);
  choice = get_one_letter_response_on_a_line();
  

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
