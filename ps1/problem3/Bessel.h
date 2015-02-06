#ifndef BESSEL_H
#define BESSEL_H

#include <cmath>
#include <iostream>

#include "boost/math/special_functions/bessel.hpp"
#include "TH1D.h"


class Bessel { 

private:

    
public:


  Bessel() {}
  
  virtual ~Bessel() {}
  
  void value(int v, double x) { std::cout << boost::math::cyl_bessel_i(v,x) << std::endl; }  
};

// template <class T> inline T     std::three_vector<T>::X()    const { return fX;    }
// template <class T> inline T     std::three_vector<T>::Y()    const { return fY;    }
// template <class T> inline T     std::three_vector<T>::Z()    const { return fZ;    }
// template <class T> inline float std::three_vector<T>::R()    const { return fR;    }
// template <class T> inline float std::three_vector<T>::dRdx() const { return fX/fR; }
// template <class T> inline float std::three_vector<T>::dRdy() const { return fY/fR; }
// template <class T> inline float std::three_vector<T>::dRdz() const { return fZ/fR; }

#endif
