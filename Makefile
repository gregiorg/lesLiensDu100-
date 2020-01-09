CC = gcc
CFLAGS = -g -Wall -Werror
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))
OBJECTS = test_displayLegolas test_etape8 test_etape7 test_etape6 test_etape5 test_etape4 test_etape3 test_etape2 test_etape1

all: $(OBJECTS)

test_displayLegolas: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o displayLegolas.o legolas.o util.o
test_etape8: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape8.o legolas.o util.o
test_etape7: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape7.o legolas.o util.o
test_etape6: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape6.o legolas.o util.o
test_etape5: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape[1-5].o util.o
test_etape4: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape[1-4].o util.o
test_etape3: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape[1-3].o util.o
test_etape2: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape[1-2].o util.o
test_etape1: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o etape1.o util.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o test_etape?
