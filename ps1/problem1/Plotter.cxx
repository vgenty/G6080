#include "Plotter.h"

Plotter::Plotter() {}

Plotter::~Plotter() {}

TGraph* Plotter::plot(std::map<double,double>& pts) {
  TGraph *t = new TGraph();
  
  std::map<double,double>::iterator itr;
  
  for (itr = pts.begin(); itr != pts.end(); ++itr) {
    t->SetPoint((int)(itr->first - 2.0), 
		pow(10,itr->first), 
		itr->second);
  }
  
  return t;  
}

TGraph* Plotter::errorC(std::map<double,double>& ptsI) {
  TGraph *t_ = new TGraph();
  
  double _x0 = pow(10,-6);
  
  for(int i = 2; i <= 8; ++i) {
    t_->SetPoint(i-2,pow(10,i),fabs(2* (cos(pow(10,i)*_x0) - ptsI[i])/(cos(pow(10,i)*_x0) + ptsI[i])));
  }
  
  return t_;
  
}

TGraph* Plotter::error() {
  err_ = new TGraph();
  
  double _x0 = pow(10,-6);

  for(int i = 2; i <= 8; ++i) {
    err_->SetPoint(i-2,
		   (double)pow(10,i),
		   fabs(pow(10,i)*sin(_x0*pow(10,i))/sin(_x0)*pow(10,-15)));
  }
  
  return err_;
}

TGraph* Plotter::error3() {
  TGraph *tg__ = new TGraph();
  
  double _x0 = pow(10,-6);
  
  for(int i = 2; i <= 8; ++i) {
    
    double yvalue = 0.0;

    for(int k = 1; k <= pow(10,i)-1; ++k) {
      yvalue += (1/tan(k*_x0))/k;
    }
    
    yvalue = yvalue * pow(10,i)*sin(pow(10,i)*_x0)*pow(10,-15);
    

    tg__->SetPoint(i-2,
		   (double)pow(10,i),
		   fabs(yvalue));
    
  }
  return tg__;
}

TGraph* Plotter::ratio(std::map<double,double>& pts) {

  rat_ = new TGraph();
  double _x0 = pow(10,-6);

  std::map<double,double>::iterator itr;
  
  for (itr = pts.begin(); itr != pts.end(); ++itr) {
    rat_->SetPoint((int)(itr->first - 2.0), 
		   pow(10,itr->first), 
		   fabs((pow(10,itr->first)*sin(_x0*pow(10,itr->first))/sin(_x0))*pow(10,-15)
			/ 
			(2* (cos(pow(10,itr->first)*_x0) - itr->second)/(cos(pow(10,itr->first)*_x0) + itr->second))));
		  }
  
  return rat_;
}
