CC = gcc
CFLAGS = -g -Wall -Werror
OBJECTS = test_etape8 test_etape7 test_etape6 test_etape5 test_etape4 test_etape3 test_etape2 test_etape1

all: $(OBJECTS)

test_etape8: test_etape8.o etape8.o legolas.o util.o
	$(CC) $(CFLAGS) -o test_etape8 test_etape8.o etape8.o legolas.o util.o

test_etape8.o: test_etape8.c
	$(CC) $(CFLAGS) -c test_etape8.c

etape8.o: etape8.c legolas.h util.h
	$(CC) $(CFLAGS) -c etape8.c

test_etape7: test_etape7.o etape7.o legolas.o util.o
	$(CC) $(CFLAGS) -o test_etape7 test_etape7.o etape7.o legolas.o util.o

test_etape7.o: test_etape7.c
	$(CC) $(CFLAGS) -c test_etape7.c

etape7.o: etape7.c legolas.h util.h
	$(CC) $(CFLAGS) -c etape7.c

test_etape6: test_etape6.o etape6.o legolas.o util.o
	$(CC) $(CFLAGS) -o test_etape6 test_etape6.o etape6.o legolas.o util.o

test_etape6.o: test_etape6.c
	$(CC) $(CFLAGS) -c test_etape6.c

etape6.o: etape6.c etape6.h legolas.h legolas.c
	$(CC) $(CFLAGS) -c etape6.c

test_etape5: test_etape5.o etape5.o etape4.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape5 test_etape5.o etape5.o etape4.o etape3.o etape2.o etape1.o util.o

test_etape5.o: test_etape5.c
	$(CC) $(CFLAGS) -c test_etape5.c

etape5.o: etape5.c etape5.h etape4.c etape4.h etape3.c etape3.h etape2.c etape2.h etape1.c etape1.h util.c util.h
	$(CC) $(CFLAGS) -c etape5.c

test_etape4: test_etape4.o etape4.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape4 test_etape4.o etape4.o etape3.o etape2.o etape1.o util.o

test_etape4.o: test_etape4.c
	$(CC) $(CFLAGS) -c test_etape4.c

etape4.o: etape4.c etape4.h etape3.c etape3.h etape2.c etape2.h etape1.c etape1.h util.c util.h
	$(CC) $(CFLAGS) -c etape4.c

test_etape3: test_etape3.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape3 test_etape3.o etape3.o etape2.o etape1.o util.o

test_etape3.o: test_etape3.c
	$(CC) $(CFLAGS) -c test_etape3.c

etape3.o: etape3.c etape3.h etape2.c etape2.h etape1.c etape1.h util.c util.h
	$(CC) $(CFLAGS) -c etape3.c

test_etape2: test_etape2.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape2 test_etape2.o etape2.o etape1.o util.o

test_etape2.o: test_etape2.c
	$(CC) $(CFLAGS) -c test_etape2.c

etape2.o: etape2.c etape2.h etape1.c etape1.h util.c util.h
	$(CC) $(CFLAGS) -c etape2.c

test_etape1: test_etape1.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape1 test_etape1.o etape1.o util.o

test_etape1.o: test_etape1.c
	$(CC) $(CFLAGS) -c test_etape1.c

etape1.o: etape1.c etape1.h util.c util.h
	$(CC) $(CFLAGS) -c etape1.c

legolas.o: legolas.c legolas.h
	$(CC) $(CFLAGS) -c legolas.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm etape?.o util.o test_etape?
