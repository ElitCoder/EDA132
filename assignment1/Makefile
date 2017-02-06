CXX = g++
CC  = g++

CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -Wold-style-cast -pedantic-errors

PROGS = Search

all: $(PROGS) permission

Search: Search.o Board.o AI.o

clean:
	rm -f *.o $(PROGS) *.d*

permission:
	chmod 755 *

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

SRC = $(wildcard *.cpp)
include $(SRC:.cpp=.d)
