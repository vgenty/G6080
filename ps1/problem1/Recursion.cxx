#include "Recursion.h"

Recursion::Recursion() {
     c_ = cos(x_);
}

Recursion::~Recursion() {}

double Recursion::value(const double& N) {
  std::vector<double> *_cc = new std::vector<double>();
  _cc->resize(N,0);
  
  _cc->at(0) = c_;
  _cc->at(1) = 2.0 * c_ * c_ - 1.0;
  
  for( double  _k = 1; _k <  N-1; ++_k)
    _cc->at(_k+1) = 2.0 * c_ * _cc->at(_k) - _cc->at(_k-1);
  
  return _cc->at(N-1);
  
}

  // std::cout << "cos(" << N << "*" << x_ << ") =" << std::endl;
  // std::cout << "_cc:     " << std::setprecision (15) << _cc->at(_cc->size()-1) << std::endl;
  // std::cout << "builtin: " << std::setprecision (15) << cos(N*x_) << std::endl;

