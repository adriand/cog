objects = main.o db_functions.o person.o place.o

cog : $(objects)
	cc -lsqlite3 -o cog $(objects)

main.o : main.c main.h person.h place.h cog.h
	cc -c -g main.c
db_functions.o : db_functions.c cog.h
	cc -c -g db_functions.c
place.o : place.c place.h cog.h
	cc -c -g place.c
person.o : person.c person.h cog.h
	cc -c -g person.c

clean :
	rm cog $(objects)
