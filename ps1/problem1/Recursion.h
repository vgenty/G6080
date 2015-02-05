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
  
private:
  const double x_ = pow(10,-6); // Initial angle value
  double c_;       // Stores the value of cosine
  
protected:

};
  

#endif
