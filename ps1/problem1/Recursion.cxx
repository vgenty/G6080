#include "Recursion.h"

Recursion::Recursion() {
     c_ = cos(x_);
     precision_ = 13;
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
  _cc->at(0) = c_ - (double)0.0;
  _cc->at(1) = 2.0 * c_ * c_ - 1.0;
  
  for( double  _k = 1; _k <  N-1; ++_k)
    _cc->at(_k+1) = 2.0 * c_ * _cc->at(_k) - _cc->at(_k-1);
  
  return _cc->at(N-1);
  
}

double Recursion::rsvalue(const double& N) {
  std::vector<double> *_cc = new std::vector<double>();
  _cc->resize(N,0);
  
  //Keep the initial guess kosher
  _cc->at(0) = c_;
  
  //Round 
  _cc->at(1) = roundd(2.0 * c_ * c_ - 1.0);
  
  for( double  _k = 1; _k <  N-1; ++_k) {
    _cc->at(_k+1) = roundd(roundd(2.0 * c_ * _cc->at(_k)) - roundd(_cc->at(_k-1)));
  }
  return _cc->at(N-1);
  
}

// std::cout << "cos(" << N << "*" << x_ << ") =" << std::endl;
// std::cout << "_cc:     " << std::setprecision (15) << _cc->at(_cc->size()-1) << std::endl;
// std::cout << "builtin: " << std::setprecision (15) << cos(N*x_) << std::endl;

