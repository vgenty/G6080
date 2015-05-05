%CXX := clang++
CXXFLAGS := -O3 -std=c++11 -m64
#INCFLAGS := -I /usr/local/opt/eigen/include/eigen3
INCFLAGS := -I /usr/include/eigen3

all: aye

aye: lanczos.o
	$(CXX) -Wall  `root-config --glibs --cflags --libs` -o aye lanczos.o

lanczos.o: lanczos.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) `root-config --glibs --cflags --libs` -Wall -c $^


clean:
	rm -rf aye lanczos.o outfile.root
#g++ -m64 `root-config --libs --cflags` -I /usr/include/eigen3 poisson.cpp -o aye -std=c++11
