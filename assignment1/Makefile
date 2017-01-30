CXX = g++
CC  = g++

CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -Wold-style-cast -std=c++11 -g

PROGS = Search

all: $(PROGS)

Search: Search.o Board.o

clean:
	rm -f *.o $(PROGS) *.d*

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

SRC = $(wildcard *.cpp)
include $(SRC:.cpp=.d)
