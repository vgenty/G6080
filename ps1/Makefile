CXX := g++
CXXFLAGS := -std=c++11
INCFLAGS := -I.

all: one

one: one.o Recursion.o
	$(CXX) -Wall -o one one.o Recursion.o

Recursion.o: Recursion.cxx
	$(CXX) `root-config --glibs` -c $^

one.o: one.cxx
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -Wall -c $^

clean:
	rm -rf *.o
