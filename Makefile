CC = g++
DIRECTIVES =# -D_GLIBCXX_USE_CXX11_ABI=0 # Needed for gcc 5 because of incompatible ABI between gcc 5 and whatever version of gcc libpqxx was compiled with. Never mind
CFLAGS = -g -Wall --std=c++14 $(DIRECTIVES)
LIBS = -lpqxx -lpq

ODIR = obj

OCOMPILE = $(CC) $(CFLAGS) -o $@ -c

# Main build

checklist : $(ODIR)/Date.o $(ODIR)/PGAdaptor.o $(ODIR)/Task.o $(ODIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/Task.o : Task.cpp Task.h Date.h
	$(OCOMPILE) Task.cpp

$(ODIR)/Date.o : Date.cpp Date.h 
	$(OCOMPILE) Date.cpp

$(ODIR)/main.o : main.cpp Date.h Task.h
	$(OCOMPILE) main.cpp

$(ODIR)/PGAdaptor.o : PGAdaptor.cpp PGAdaptor.h BaseDBAdaptor.h Task.h
	$(OCOMPILE) PGAdaptor.cpp

.PHONY : clean setup

setup :
	mkdir $(ODIR) $(TODIR)

clean :
	rm -f $(ODIR)/*.o
