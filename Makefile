CC = g++
CFLAGS = -Wall -std=c++11
LIBS =

TODIR = obj_test
ODIR = obj

OCOMPILE = $(CC) $(CFLAGS) -o $@ -c

# Main build

checklist : $(ODIR)/Date.o $(ODIR)/Task.o $(ODIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/Task.o : Task.h Task.cpp Date.h
	$(OCOMPILE) Task.cpp

$(ODIR)/Date.o : Date.h Date.cpp
	$(OCOMPILE) Date.cpp

$(ODIR)/main.o : main.cpp Date.h Task.h
	$(OCOMPILE) main.cpp

# Test build (unfortunately make doesn't offer a way to eliminate the repetition in rules. Look at CMake perhaps

$(TODIR)/Task.o : Task.h Task.cpp Date.h
	$(OCOMPILE) Task.cpp

$(TODIR)/Date.o : Date.h Date.cpp
	$(OCOMPILE) Date.cpp

$(TODIR)/main.o : main.cpp Date.h Task.h
	$(OCOMPILE) main.cpp

$(TODIR)/unittest.o : unittest.cpp Date.h Task.h
	$(OCOMPILE) unittest.cpp

test : CFLAGS := -D UNIT_TEST $(CFLAGS)
test : $(TODIR)/Date.o $(TODIR)/Task.o $(TODIR)/unittest.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY : clean setup

setup :
	mkdir $(ODIR) $(TODIR)

clean :
	rm -f $(ODIR)/*.o $(TODIR)/*.o
