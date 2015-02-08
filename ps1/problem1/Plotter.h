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
  TGraph* errorC(std::map<double,double>& ptsI);  

  private:
  TGraph *tg_;
  TGraph *err_;
  TGraph *rat_;
  
protected:
  
};

#endif
