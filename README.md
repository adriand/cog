cog
===

cog is a tool desiged to extend the mind's capacity to remember
people and details about their lives within a particular context.

Because I am just getting started with this language, I am going to iterate in a deliberate
fashion, starting out with something very simple and going from there.

Iteration 1:
- enter a new person
- lookup a person
Requires SQLite for data storage.
See http://www.sqlite.org/cintro.html, http://www.sqlite.org/quickstart.html
Also: http://wang.yuxuan.org/blog/2007/5/22/simple_sqlite_test_c_program_without_callbacks

Iteration 2:
- enter a new place
- improve the menu handling and file organization of the project
- attach a person to that place
- find people associated with that place
Requires a database for data storage (SQLite)

Iteration 3:
- delete a person
- delete a place
- edit a person
- edit a place
- associate a person with another person or people

Build Cog
=========

1. Create The Database

Ensure you have sqlite3, then create cog.sqlite3 with it.  Open it, and then create the
database structure (you can get this later on by typing .schema in the sqlite shell):

CREATE TABLE people( id INTEGER PRIMARY KEY, name TEXT );
CREATE TABLE people_places(person_id INTEGER, place_id INTEGER);
CREATE TABLE places( id INTEGER PRIMARY KEY, name TEXT );

2. Run make
