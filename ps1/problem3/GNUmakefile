CXX := c++
CXXFLAGS := -std=c++1y
INCFLAGS := -I.

all: executable libBessel.so

libBessel.so: BesselDict.o Bessel.o
	$(CXX) -shared -o $@ `root-config --glibs` $^

BesselDict.o: BesselDict.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c $< `root-config --cflags`

Bessel.o: Bessel.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c $< `root-config --cflags`

BesselDict.cxx: Bessel.h LinkDef.h
	rootcling -f $@ $^

executable: three.o libBessel.so
	$(CXX) -o $@ $< -L. -lBessel `root-config --glibs`

three.o: three.cxx
	$(CXX) $(CXXFLAGS) -fPIC -c $< -I. `root-config --cflags`

clean:
	rm -rf *.o *.so *Dict* executable
