objects = main.o dbfunctions.o person.o place.o

cog : $(objects)
	cc -lsqlite3 -o cog $(objects)

main.o : main.c main.h person.h place.h cog.h
	cc -c -g main.c
dbfunctions.o : dbfunctions.c cog.h
	cc -c -g dbfunctions.c
place.o : place.c place.h cog.h
	cc -c -g place.c
person.o : person.c person.h cog.h
	cc -c -g person.c

clean :
	rm cog $(objects)
