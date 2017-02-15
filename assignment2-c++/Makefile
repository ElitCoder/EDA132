CXX = g++
CC  = $(CXX)

DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

CPPFLAGS = -std=c++11
CXXFLAGS = -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += $(DEPFLAGS)

PROGS = DecisionTrees

all: $(PROGS)
	
DecisionTrees: DecisionTrees.o Attribute.o Example.o DTL.o Tree.o

clean:
	rm -f *.o $(PROGS) *.d

SRC = $(wildcard *.cpp)
-include $(SRC:.cpp=.d)
