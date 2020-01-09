CC = gcc
CFLAGS = -g -Wall -Werror
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))
LEGOLAS_OBJ=legolas.o legolas_header.o legolas_section.o legolas_string_table.o legolas_symbole_table.o legolas_relocation_table.o
OBJECTS = test_displayLegolas fusion test_etape7 test_etape6 readElfRelTab readElfSymTab readElfSecRawD readElfSecTab readElfHeader

all: $(OBJECTS)

test_displayLegolas: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) displayLegolas.o util.o
fusion: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape8.o util.o
test_etape7: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape7.o util.o
test_etape6: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape6.o util.o
readElfRelTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape[1-5].o util.o
readElfSymTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape[1-4].o util.o
readElfSecRawD: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape[1-3].o util.o
readElfSecTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape[1-2].o util.o
readElfHeader: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(LEGOLAS_OBJ) etape1.o util.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o test_etape?
