#include <iostream>
#include <memory>
#include <vector>

#include "Recursion.h"


//  An example of the propagation of errors in the trigonometric
//  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

int main() {

  // Needs constructor
  //std::shared_ptr<Recursion> r = std::make_shared<Recursion>();
  Recursion *r = new Recursion();
  
  const double NN = pow(10,8);

  r->value(NN);

  return 0;
}
