//Builtin includes
#include <iostream>
#include <memory>
#include <vector>
#include <map>

//ROOT Framework
#include "TApplication.h"

//Local includes
#include "Recursion.h"
#include "Plotter.h"


//  An example of the propagation of errors in the trigonometric
//  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

int main(int argc, char *argv[]) {
  
  //std::shared_ptr<Recursion> r = std::make_shared<Recursion>();
  
  Recursion *r = new Recursion();
  Plotter *p = new Plotter();
  
  std::map<double,double> pts;
  std::map<double,double>::iterator itr;
  
  for(int i = 2; i<=8; ++i)
    pts[i] = r->value(pow(10,i));
  
  
  TApplication tapp("tapp",&argc,argv);
  
  (p->plot(pts))->Draw("ALP");
  
  tapp.Run();
  
  return 0;
  
}
