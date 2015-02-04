//Builtin includes
#include <iostream>
#include <memory>
#include <vector>
#include <map>

//ROOT Framework
#include "TApplication.h"

//Local includes
#include "Recursion.h"


//  An example of the propagation of errors in the trigonometric
//  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

int main(int argc, char *argv[]) {
  
  //std::shared_ptr<Recursion> r = std::make_shared<Recursion>();
  
  Recursion *r = new Recursion();
  
  std::map<double,double> points;
  std::map<double,double>::iterator itr;
  
  for(int i = 2; i<=8; ++i)
    points[i] = r->value(pow(10,i));
  
   for (itr = points.begin(); itr != points.end(); ++itr)
     std::cout << "(" << itr->first << "," << std::setprecision (15) << itr->second << ")" << std::endl;
     
  
  //  TApplication tapp("tapp",&argc,argv);
  //tapp.Run();
  
  return 0;
  
}
