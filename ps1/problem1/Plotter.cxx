#include "Plotter.h"

Plotter::Plotter() {}

Plotter::~Plotter() {}

TGraph* Plotter::plot(std::map<double,double>& pts) {
  tg_ = new TGraph();
  
  std::map<double,double>::iterator itr;
  
  for (itr = pts.begin(); itr != pts.end(); ++itr) {
    //std::cout << "(" << itr->first << "," << std::setprecision (15) << itr->second << ")" << std::endl;
    tg_->SetPoint((int)(itr->first - 2.0), 
		  pow(10,itr->first), 
		  itr->second);
  }
  
  return tg_;  
}


TGraph* Plotter::error() {
  err_ = new TGraph();
  
  double _x0 = pow(10,-6);

  for(int i = 2; i <= 8; ++i) {
    err_->SetPoint(i-2,
		   (double)pow(10,i),
		   fabs(pow(10,i)*sin(_x0*pow(10,i))/sin(_x0)));
    //std::cout << "~~\n" << pow(10,i)*sin(_x0*pow(10,i))/sin(_x0) << std::endl;
  }
  
  return err_;
}

TGraph* Plotter::ratio(std::map<double,double>& pts) {

  rat_ = new TGraph();
  double _x0 = pow(10,-6);

  std::map<double,double>::iterator itr;
  
  for (itr = pts.begin(); itr != pts.end(); ++itr) {
    //std::cout << "(" << itr->first << "," << std::setprecision (15) << itr->second << ")" <<  " ratio " <<  (itr->second)/(pow(10,itr->first)*sin(_x0*pow(10,itr->first))/sin(_x0)) << std::endl;
    rat_->SetPoint((int)(itr->first - 2.0), 
		   pow(10,itr->first), 
		   fabs((pow(10,itr->first)*sin(_x0*pow(10,itr->first))/sin(_x0))/(itr->second)));
  }
  
  return rat_;
}

  // std::cout << "cos(" << N << "*" << x_ << ") =" << std::endl;
  // std::cout << "_cc:     " << std::setprecision (15) << _cc->at(_cc->size()-1) << std::endl;
  // std::cout << "builtin: " << std::setprecision (15) << cos(N*x_) << std::endl;

