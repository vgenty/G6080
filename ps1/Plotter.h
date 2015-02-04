#ifndef PLOTTER_H
#define PLOTTER_H

#include <map>
#include <iomanip>
#include "TGraph.h"
#include "TMultiGraph.h"

class Plotter {

public:
Plotter();
virtual ~Plotter();

TGraph* plot(std::map<double,double> & pts);


private:
TGraph *tg;

protected:

};
  
#endif
