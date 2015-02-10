#ifndef RECURSION_H
#define RECURSION_H

#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

class Recursion {
  
public:
  Recursion();
  virtual ~Recursion();
  
  double value(const double& N);
  double isvalue(const double& N);
  double rsvalue(const double& N);
  
private:
  const double x_ = pow(10,-6); // Initial angle value
  double c_;                    // Stores the value of cosine
  int precision_;               // Precision Value
  
  //Round to precision_ decimal places
  double roundd(double num) {
    return floor(num * pow(10.0,precision_) + 0.5)/pow(10.0,precision_);
  }
  
protected:

};
  

#endif
