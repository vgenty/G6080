//vgenty@nevis.columbia.edu

#ifndef ANALYZER_H
#define ANALYZER_H

// Usual shit
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <memory>

// ROOT
#include "TObject.h"

// Mine
#include "Data.h"

class Analyzer : public TObject {
  
public:

  Analyzer() {}
  Analyzer(std::string location);
  
  virtual ~Analyzer() {}
  
private:
  std::vector<std::shared_ptr<Data> > _content;
  
  ClassDef(Analyzer,1)
};
#endif