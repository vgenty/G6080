#include "Plotter.h"

Plotter::Plotter() {}

Plotter::~Plotter() {}

TGraph* Plotter::plot(std::map<double,double>& pts) {
  
  std::map<double,double>::iterator itr;
  
  for (itr = pts.begin(); itr != pts.end(); ++itr)
    std::cout << "(" << itr->first << "," << std::setprecision (15) << itr->second << ")" << std::endl;
  
  
  
  
   

}


  // std::cout << "cos(" << N << "*" << x_ << ") =" << std::endl;
  // std::cout << "_cc:     " << std::setprecision (15) << _cc->at(_cc->size()-1) << std::endl;
  // std::cout << "builtin: " << std::setprecision (15) << cos(N*x_) << std::endl;

