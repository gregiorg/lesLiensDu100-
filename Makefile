CC = gcc
CFLAGS = -g -Wall -Werror
OBJECTS = test_etape5 test_etape4 test_etape3 test_etape2 test_etape1

all: $(OBJECTS)

# test_etape6: test_etape6.o etape6.o etape5.o etape4.o etape3.o etape2.o etape1.o util.o
# 	$(CC) $(CFLAGS) -o test_etape6 test_etape6.o etape6.o etape5. etape4.o etape3.o etape2.o etape1.o util.o

# test_etape6.o:
# 	$(CC) $(CFLAGS) -c test_etape6.c

# etape6.o:
# 	$(CC) $(CFLAGS) -c etape6.c

test_etape5: test_etape5.o etape5.o etape4.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape5 test_etape5.o etape5.o etape4.o etape3.o etape2.o etape1.o util.o

test_etape5.o: test_etape5.c
	$(CC) $(CFLAGS) -c test_etape5.c

etape5.o: etape5.c etape5.h
	$(CC) $(CFLAGS) -c etape5.c

test_etape4: test_etape4.o etape4.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape4 test_etape4.o etape4.o etape3.o etape2.o etape1.o util.o

test_etape4.o:
	$(CC) $(CFLAGS) -c test_etape4.c

etape4.o:
	$(CC) $(CFLAGS) -c etape4.c

test_etape3: test_etape3.o etape3.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape3 test_etape3.o etape3.o etape2.o etape1.o util.o

test_etape3.o:
	$(CC) $(CFLAGS) -c test_etape3.c

etape3.o:
	$(CC) $(CFLAGS) -c etape3.c

test_etape2: test_etape2.o etape2.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape2 test_etape2.o etape2.o etape1.o util.o

test_etape2.o:
	$(CC) $(CFLAGS) -c test_etape2.c

etape2.o:
	$(CC) $(CFLAGS) -c etape2.c

test_etape1: test_etape1.o etape1.o util.o
	$(CC) $(CFLAGS) -o test_etape1 test_etape1.o etape1.o util.o

test_etape1.o:
	$(CC) $(CFLAGS) -c test_etape1.c

etape1.o:
	$(CC) $(CFLAGS) -c etape1.c

util.o:
	$(CC) $(CFLAGS) -c util.c

clean:
	rm etape?.o util.o test_etape?
