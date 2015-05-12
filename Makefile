CC=gcc

INCFLAGS=-I.
LIBFLAGS=-L.

REL_FLAGS=-Ofast -DNDEBUG
DBG_FLAGS=-g -DDEBUG
PROF_FLAGS=-pg

SRCS=$(shell ls *.c)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
REL_OBJS=$(patsubst %.cpp,%_release.o,$(SRCS))
DBG_OBJS=$(patsubst %.cpp,%_debug.o,$(SRCS))
PROF_OBJS=$(patsubst %.cpp,%_prof.o,$(SRCS))

program=main_release 
all: $(program)
main_release: $(REL_OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(REL_FLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CXXFLAGS) $(INCFLAGS) $(LIBFLAGS)
%_release.o: %.c
	$(CC) -o $@ -c $< $(CXXFLAGS) $(REL_FLAGS) $(INCFLAGS) $(LIBFLAGS)
%_debug.o: %.c
	$(CC) -o $@ -c $< $(CXXFLAGS) $(DBG_FLAGS) $(INCFLAGS) $(LIBFLAGS)
%_prof.o: %.c
	$(CC) -o $@ -c $< $(CXXFLAGS) $(PROF_FLAGS) $(INCFLAGS) $(LIBFLAGS)

clean:
	rm -rf *.o main_release
