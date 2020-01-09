CC = gcc
CFLAGS = -g -Wall -Werror
OBJDIR = obj
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, $(OBJDIR)/%.o, $(SOURCES))
LEGOLAS_OBJ=$(wildcard $(OBJDIR)/legolas*.o)
UTIL = $(OBJDIR)/util.o
OBJECTS = test_displayLegolas fusion test_etape7 test_etape6 readElfRelTab readElfSymTab readElfSecRawD readElfSecTab readElfHeader

all: $(OBJECTS)

test_displayLegolas: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/displayLegolas.o $(UTIL)
fusion: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape8.o $(UTIL)
test_etape7: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape7.o $(UTIL)
test_etape6: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape6.o $(UTIL)
readElfRelTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape[1-5].o $(UTIL)
readElfSymTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape[1-4].o $(UTIL)
readElfSecRawD: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape[1-3].o $(UTIL)
readElfSecTab: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape[1-2].o $(UTIL)
readElfHeader: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$@.o $(LEGOLAS_OBJ) $(OBJDIR)/etape1.o $(UTIL)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm $(OBJDIR)/*.o test_displayLegolas fusion test_etape7 test_etape6 readElfRelTab readElfSymTab readElfSecRawD readElfSecTab readElfHeader
