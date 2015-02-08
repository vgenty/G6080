#include "Bessel.h"

double Bessel::recursion(const double x,const int nn) {
  
  std::vector<double> *v = new std::vector<double>();

  v->resize(nn+2);
  
  v->at(nn+1) = -4.765;
  v->at(nn) = 20.45;
  
  for(auto i : boost::irange(1,nn+1) | boost::adaptors::reversed) {
    v->at(i-1) = v->at(i+1) + 2*i*v->at(i)/x;
  }
  
  // Normalization
  double s = 0.5*v->at(0);
  for(auto i: boost::irange(2,nn,2)) {
    s += pow(-1,i/2)*v->at(i);
  }
  s *= 2;
  
  return v->at(2)/s;
}
