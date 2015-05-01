CXX := g++
CXXFLAGS := -std=c++11 -m64
INCFLAGS := -I /usr/include/eigen3


all: aye

aye: poisson.o
	$(CXX) -Wall  `root-config --glibs --cflags --libs` -o aye poisson.o

poisson.o: poisson.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) `root-config --glibs --cflags --libs` -Wall -c $^


clean:
	rm -rf aye poisson.o outfile.root
#g++ -m64 `root-config --libs --cflags` -I /usr/include/eigen3 poisson.cpp -o aye -std=c++11
