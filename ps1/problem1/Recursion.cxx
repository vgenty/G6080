#include "Recursion.h"

Recursion::Recursion() {
  c_ = cos(x_);
  precision_ = 8;
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

double Recursion::isvalue(const double& N) {
  std::vector<double> *_cc = new std::vector<double>();
  _cc->resize(N,0);
  
  //Reduce precision for initial guess by 10^-10
  _cc->at(0) = c_ - (double)pow(10,-10);
  _cc->at(1) = 2.0 * c_ * c_ - 1.0;
  
  for( double  _k = 1; _k <  N-1; ++_k)
    _cc->at(_k+1) = 2.0 * c_ * _cc->at(_k) - _cc->at(_k-1);
  
  return _cc->at(N-1);
  
}

double Recursion::rsvalue(const double& N) {
  std::vector<double> *_cc = new std::vector<double>();
  _cc->resize(N,0);
  
  //Round the initial guess
  _cc->at(0) = roundd(c_);
  
  //Round again
  _cc->at(1) = 2.0* roundd(c_ * c_) - 1.0;
  
  //Round over and over
  for( double  _k = 1; _k <  N-1; ++_k) {
    _cc->at(_k+1) = roundd(2.0 * roundd(c_ * _cc->at(_k)) - _cc->at(_k-1));
  }

  return _cc->at(N-1);
  
}

