//vgenty@nevis.columbia.edu

#ifndef ANALYZER_H
#define ANALYZER_H

// Usual shit
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

// ROOT
#include "TObject.h"

// Mine
#include "Data.h"

class Analyzer : public TObject {
  
public:

  Analyzer();
  
  virtual ~Analyzer() {}
  
private:
  
  ClassDef(Analyzer,1)
};
#endif
