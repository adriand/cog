struct person {
  int id;
  char name[MAXNAME];
};

struct person new_person(char *);
int find_person(char *, struct person *);
struct person get_person(int);
int associate_with_place(int, int);
struct person make_person(int, char *);
void print_person(struct person *);
int destroy_person(int);
