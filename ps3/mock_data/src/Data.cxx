//vgenty@nevis.columbia.edu

#include "Data.h"


void Data::calcPop() {

  auto N_ = double{static_cast<double>(_data.size())};
  double t_ = 0.0;
  
  std::for_each(_data.begin(),_data.end(),[&t_] (const double& v) {
      t_ += v;
    });
  
  _mean = t_/N_;

  t_ = 0.0;
  
  std::for_each(_data.begin(),_data.end(),[&] (const double& v) {
      t_ += (v-_mean)*(v-_mean);
    });
  
  _var = t_/(N_-1);
  
}

void Data::calcSam(int i) {
  
  double div_   = static_cast<double>(NUMVALS)/static_cast<double>(i);
  double mean_  = 0.0;
  double var_   = 0.0;
  
  for(int j = 1; j <= div_; ++j) {
    
    //do the mean
    for(int k = i*(j-1); k < i*j; ++k) { mean_ += _data[k]; }
    mean_ /= i;
    
    // do the var
    for(int k = i*(j-1); k < i*j; ++k) { var_ += (_data[k] - mean_)*(_data[k] - mean_); }
    var_  /= i;
    
    _samps[i].push_back(std::make_pair(mean_,var_));
    
    mean_ = 0.0;
    var_  = 0.0;
  }

}

void Data::calcAuto(int n) {
  double s_ = 0.0;

  for(auto i : boost::irange(0,NUMVALS - n) )
    s_ += (_data[i+n] - _mean)*(_data[i] - _mean);
  
  _corrs.push_back(s_/(NUMVALS-n));
    
}

void Data::calcAutoIntegrated() {
  auto c_ = double{0.0};
  std::for_each(_corrs.begin(),_corrs.end(),[&c_](double& v){ c_ += v; });
  _integrated_corrs = c_/_corrs[0];
  
}

