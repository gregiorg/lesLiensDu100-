CC = gcc
CFLAGS = -g -Wall -Werror
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))
LEGOLAS_OBJ=legolas.o legolas_header.o legolas_section.o legolas_string_table.o legolas_symbole_table.o legolas_relocation_table.o
OBJECTS = test_etape8 test_etape7 test_etape6 test_etape5 test_etape4 test_etape3 test_etape2 test_etape1

all: $(OBJECTS)

test_etape8: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape8.o util.o
test_etape7: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape7.o util.o
test_etape6: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape6.o util.o
test_etape5: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape5.o etape4.o etape3.o etape2.o etape1.o util.o
test_etape4: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape4.o etape3.o etape2.o etape1.o util.o
test_etape3: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape3.o etape2.o etape1.o util.o
test_etape2: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape2.o etape1.o util.o
test_etape1: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape1.o util.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o test_etape?

#etape?.o: etape?.h

#etape1.o: util.c util.h
#etape2.o: etape1.h util.h
#etape3.o: etape2.h etape1.h util.h
#etape4.o: etape3.h etape2.h etape1.h util.h
#etape5.o: etape4.h etape3.h etape2.h etape1.h util.h
#etape6.o: legolas.h legolas.c
#etape7.o: legolas.h util.h
#etape8.o: legolas.h util.h

#util.o: util.h
#legolas.o: legolas.h
