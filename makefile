CC = gcc
CFLAGS = -Wall -g
LFLAGS = -L$(LIBDIR)
INCLUDES = -I$(INCDIR)

# define directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
OUTPUTDIR = output
INCDIR = include

# define targets and executables
TARGET = app
EXEC = $(BINDIR)/$(TARGET)

# define sources, objects and libraries
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LIBS = -lgenetic -lprisoner_dilemma -lstrategy

all: $(EXEC)

# genetic.c and prisoner_dilemma.c and strategy.c are compiled into shared libraries
$(EXEC): $(OBJECTS) $(LIBDIR)/libgenetic.so $(LIBDIR)/libprisoner_dilemma.so $(LIBDIR)/libstrategy.so
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $(EXEC) $(OBJECTS) $(LIBS)

$(LIBDIR)/libgenetic.so: $(SRCDIR)/genetic.c
	$(CC) $(CFLAGS) $(INCLUDES) -shared -fPIC -o $(LIBDIR)/libgenetic.so $(SRCDIR)/genetic.c

$(LIBDIR)/libprisoner_dilemma.so: $(SRCDIR)/prisoner_dilemma.c
	$(CC) $(CFLAGS) $(INCLUDES) -shared -fPIC -o $(LIBDIR)/libprisoner_dilemma.so $(SRCDIR)/prisoner_dilemma.c

$(LIBDIR)/libstrategy.so: $(SRCDIR)/strategy.c
	$(CC) $(CFLAGS) $(INCLUDES) -shared -fPIC -o $(LIBDIR)/libstrategy.so $(SRCDIR)/strategy.c

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(OUTPUTDIR)/valgrind.log $(EXEC)

init:
	mkdir -p $(BINDIR) $(OBJDIR) $(LIBDIR) $(OUTPUTDIR)

clean:
	rm -f $(OBJECTS) $(EXEC)

deepclean:
	rm -f $(OBJECTS) $(EXEC)
	rm -f $(LIBDIR)/*.so
	
cleanoutput:
	rm -f -r $(OUTPUTDIR)/*

# get what arguments are passed to the make command
ARGS = $(filter-out $@,$(MAKECMDGOALS))

run: $(EXEC)
	$(EXEC) $(ARGS)

