cog
===

cog is a tool desiged to extend the mind's capacity to remember people and details about their 
lives within a particular context.

Because I am just getting started with this language, I am going to iterate in a deliberate
fashion, starting out with something very simple and going from there.

Iteration 1:
* enter a new person [done]
* lookup a person [done]

Requires SQLite for data storage.

See http://www.sqlite.org/cintro.html, http://www.sqlite.org/quickstart.html

Also: http://wang.yuxuan.org/blog/2007/5/22/simple_sqlite_test_c_program_without_callbacks

Iteration 2:
* enter a new place [done]
* attach a person to that place
* find people associated with that place
* improve the menu handling and file organization of the project

Improving the menu handling would be good, since right now it's a bit hard to work with this.  Probably
the best method would be to start by presenting a main menu consisting of:

(f)ind person (p)eople p(l)aces (q)uit

When you hit p for people, you would be brought to a list where you could select one, find one, etc. Once you
selected one, you'd be able to work with that person specifically.  The general gist of this would be the
standard index, show, new, edit, delete functionality, as in web apps.

I need to come up with a better overall pattern for this.  Let's imagine this working like an IOS app.  From the
main menu (tabs), you would select People.  At this point you would be presented with a List of people.  You would
also have the ability to Find people, and the ability to create a New person.

From the List (or via Find, which just displays a narrowed list), you would be able to select a single person,
who would then be Shown.  From Showing the person, you would be able to Edit the person, Delete the person, and
Associate the person with a place.

To relate this back to this program, this is the structure:

  Main Menu - People, Places
    People
      List (offers select by ID)
        Show (via select by ID)
          Edit
          Delete
          Associate
      Find -> List, but narrower
      New

This brings up the need for some kind of architecture.  MVC is overkill for this, since there is little in the way
of "views".  Instead, I will call my architecture the "interaction / entity" architecture.  Interactions handle
displaying menus, retrieving user input, and taking action based on interaction with the user (they are thus like
a combination of views and controllers).

Entities are the individual thing (e.g. a person) and collections of those things (e.g. people). 

I'm going to proceed by setting up folders that handle this distinction.  The structure should be:

  /
    interactions
      home
      people
      places
    entities
      person
      place

Additionally, I probably need a lib folder for stuff like dbfunctions, helper functions for menus, etc.

Iteration 3:
* delete a person
* delete a place
* edit a person
* edit a place
* associate a person with another person or people

Iteration 4:
* find (search)...

Build Cog
=========

Create The Database
-------------------

Ensure you have sqlite3, then create cog.sqlite3 with it.  Open it, and then create the
database structure (you can get this later on by typing .schema in the sqlite shell):

  CREATE TABLE people( id INTEGER PRIMARY KEY, name TEXT );
  CREATE TABLE people_places(person_id INTEGER, place_id INTEGER);
  CREATE TABLE places( id INTEGER PRIMARY KEY, name TEXT );

Run make
--------

  make

Run cog
-------

  ./cog

Watch the magic happen
----------------------

Actually, it's not all that magical at all.

Issues
======

My Makefile is clearly broken, since if you run it with nothing compiled, it doesn't work.  But if you start
from the base modules (e.g. person) and work your way up, it does work.
