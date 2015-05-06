%CXX := clang++
CXXFLAGS := -O3 -std=c++1y -m64
INCFLAGS := -I /usr/local/opt/eigen/include/eigen3
#INCFLAGS := -I /usr/include/eigen3

all: ev

ev: evolve.o
	$(CXX) -Wall  `root-config --glibs --cflags --libs` -o ev evolve.o

evolve.o: evolve.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) `root-config --glibs --cflags --libs` -Wall -c $^


clean:
	rm -rf ev evolve.o outfile.root
#g++ -m64 `root-config --libs --cflags` -I /usr/include/eigen3 poisson.cpp -o ev -std=c++11
