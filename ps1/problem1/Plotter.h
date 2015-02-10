#ifndef PLOTTER_H
#define PLOTTER_H

#include <map>
#include <cmath>
#include <iomanip>
#include "TGraph.h"
#include "TMultiGraph.h"

class Plotter {

public:
  Plotter();
  virtual ~Plotter();
  
  TGraph* plot(std::map<double,double> & pts);
  TGraph* ratio(std::map<double,double> & pts);
  TGraph* error();

  //Last couple functions are ghetto i'm sorry
  TGraph* errorC(std::map<double,double>& ptsI);  
  TGraph* error3();  

  private:
  TGraph *tg_;
  TGraph *err_;
  TGraph *rat_;
  
protected:
  
};

#endif
