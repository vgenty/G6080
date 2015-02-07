#include "Bessel.h"

double Bessel::recursion(const double x) {
  
  std::vector<double> *v = new std::vector<double>();

  v->resize(52);
  
  v->at(51) = -4.765;
  v->at(50) = 20.45;
  
  for(auto i : boost::irange(1,51) | boost::adaptors::reversed) {
    //std::cout << "v->at(" << i-1 << ") = v->at(" << i+1 << ") + 2*" << i << "*v->at(" << i << ")/" << x << " ~~~  ";
    v->at(i-1) = v->at(i+1) + 2*i*v->at(i)/x;
    //std::cout << v->at(i-1) << " = " << v->at(i+1) << " + " <<  2*i*v->at(i)/x << std::endl;
  }
  
  double s = 0.5*v->at(0);
  for(auto i: boost::irange(2,40,2)) {
    s += pow(-1,i/2)*v->at(i);
  }
  s *= 2;
  std::cout << v->at(2)/s << std::endl;
}
