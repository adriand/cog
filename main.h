int get_choice(char *, int);
void choose_find();

/* people */
void choose_people();
void choose_new_person();
void choose_get_person();
void choose_list_people();
void work_with_person(struct person *);
struct person *choose_from_list_of_people(struct person *, int);

/* places */
void choose_places();
void choose_new_place();
void choose_list_places();

/* misc */
void chomp(char *);
