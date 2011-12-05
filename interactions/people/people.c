#include "people.h"

void choose_people()
{
  int choice;

  printf("People\n------\n\n");
  choose_list_people();
  do
  {
    choice = get_choice("(n)ew, (g)et, return (h)ome", 0);
    switch(choice)
    {
      case 'n':
        choose_new_person();
        break;
      case 'g':
        choose_get_person();
        break;
      case 'q':
        printf("Quitting cog...\n");
        break;
      case 'h':
        break;
      default:
        printf("Invalid choice.\n");
        break;
    }
  } while (choice != 'h');
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
  /* printf("Enter the person's ID or part of the person's name: "); */
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
  printf("\n(e)dit %s, (d)elete %s, (a)ssociate %s with place, Enter to cancel: ", individual->name, individual->name, individual->name);
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
    case 'a':
      associate_person_with_place(individual);
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
  /* TODO: make this accept a list of everyone and do the printing here.  In fact, that's really the
   * best organization overall - separate the views from the database access layer!
   */
  list_named_records("people", "1", "name");
}

