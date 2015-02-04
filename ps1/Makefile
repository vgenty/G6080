CXX := c++
CXXFLAGS := -std=c++11
INCFLAGS := -I.

all: one

one: one.o Recursion.o Plotter.o
	$(CXX) -Wall  `root-config --glibs --cflags --libs` -o one one.o Recursion.o

one.o: one.cxx
	$(CXX) $(CXXFLAGS) $(INCFLAGS) `root-config --glibs --cflags --libs` -Wall -c $^

Recursion.o: Recursion.cxx
	$(CXX) `root-config --glibs --cflags --libs` -c $^

Plotter.o: Plotter.cxx
	$(CXX) `root-config --glibs --cflags --libs` -c $^

clean:
	rm -rf *.o one
