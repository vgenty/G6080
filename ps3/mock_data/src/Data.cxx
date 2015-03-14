//vgenty@nevis.columbia.edu

#include "Data.h"


void Data::calcPop() {

  auto N_ = double{static_cast<double>(_data.size())};
  double t = 0.0;
  
  std::for_each(_data.begin(),_data.end(),[&t] (const double& v) {
      t += v;
    });
  
  _mean = t/N_;

  t = 0.0;
  
  std::for_each(_data.begin(),_data.end(),[&] (const double& v) {
      t += (v-_mean)*(v-_mean);
    });
  
  _var = t/N_;
  
}
