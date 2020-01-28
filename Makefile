#------------------------------------------------------------------------------

FLAGS   = -std=c99 -Wall
SOURCES = id_library.c test_id_library.c
OBJECTS = id_library.o test_id_library.o
HEADERS = id_library.h
EXEBIN  = id_library

all: $(EXEBIN)

$(EXEBIN) : $(OBJECTS) $(HEADERS)
	gcc -ggdb -o $(EXEBIN) $(OBJECTS)

$(OBJECTS) : $(SOURCES) $(HEADERS)
	gcc -ggdb -c $(FLAGS) $(SOURCES)

clean :
	rm -f $(EXEBIN) $(OBJECTS)

check:
	valgrind --leak-check=full $(EXEBIN) 
