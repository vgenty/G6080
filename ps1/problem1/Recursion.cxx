#include "Recursion.h"

Recursion::Recursion() {
  c_ = cos(x_);
  precision_ = 6;
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
  
  //Reduce precision for initial guess say 0.2
  _cc->at(0) = c_ - (double)pow(10,-8);
  _cc->at(1) = 2.0 * c_ * c_ - 1.0;
  
  for( double  _k = 1; _k <  N-1; ++_k)
    _cc->at(_k+1) = 2.0 * c_ * _cc->at(_k) - _cc->at(_k-1);
  
  return _cc->at(N-1);
  
}

double Recursion::rsvalue(const double& N) {
  std::vector<double> *_cc = new std::vector<double>();
  _cc->resize(N,0);
  
  //Keep the initial guess kosher
  _cc->at(0) = roundd(c_);
  
  //Round 
  _cc->at(1) = 2.0* roundd(c_ * c_) - 1.0;
  
  for( double  _k = 1; _k <  N-1; ++_k) {
    _cc->at(_k+1) = roundd(2.0 * roundd(c_ * _cc->at(_k)) - _cc->at(_k-1));
  }

  return _cc->at(N-1);
  
}

