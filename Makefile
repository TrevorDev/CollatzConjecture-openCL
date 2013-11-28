CC          =	gcc
# Flags that are sent to the compiler
# Do not change these
CFLAGS      = -g -pedantic -Wall -std=c99
LDFLAGS     =

#Directories where the compiler can find things
INCLUDES    = -I/opt/AMDAPP/include
LIBDIRS     = -L/opt/AMDAPP/lib/x86_64
LIBS        = -lOpenCL

BINDIR = ./
SRCDIR = ./

#Put the names of your source code files in the lines below.  You will need both your
# Solution source code file and the source code with the main program in it

SOURCE = $(SRCDIR)openCL-allToOne.c $(SRCDIR)openCL-oneToAll.c $(SRCDIR)dlxlib.c


#The list of object files for each program is below.  
# The object file names here must match the file names of your source code 
# except with .o instead of .c.   Make changes as appropriate


OBJS    = dlxlib.o


PROGNAME = $(BINDIR)openCL-allToOne

default : all

all : prog


prog : build
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $(PROGNAME) openCL-allToOne.o $(OBJS) $(LIBS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $(BINDIR)openCL-oneToAll openCL-oneToAll.o $(OBJS) $(LIBS)

build: $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) $(INCLUDES)
omp:
	$(CC) $(CFLAGS) -o omp_allToOne -fopenmp omp_allToOne.c
clean:
	@ rm *.o

run: prog 
	$(PROGNAME)
 
val: prog
	valgrind $(PROGNAME)