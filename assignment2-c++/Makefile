CXX = g++
CC  = $(CXX)

DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

CPPFLAGS = -std=c++11
CXXFLAGS = -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += $(DEPFLAGS)

PROGS = DecisionTrees

all: $(PROGS) permissions
	
DecisionTrees: DecisionTrees.o Attribute.o Example.o DTL.o Tree.o

permissions:
	chmod 755 *
	chmod 755 data/*

clean:
	rm -f *.o $(PROGS) *.d

SRC = $(wildcard *.cpp)
-include $(SRC:.cpp=.d)
