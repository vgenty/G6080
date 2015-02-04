#ifndef PLOTTER_H
#define PLOTTER_H

#include <map>

#include "TGraph.h"
#include "TMultiGraph.h"

class Plotter {
  
public:
  Plotter();
  virtual ~Plotter();
  
  void plot(std::map<double,double>& values);
  
private:
  TGraph *tg;
  
protected:

};
  

#endif
