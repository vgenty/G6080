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


  void a();
  void b();
  void c(int n);
  void d();
  void e();


  void three();
  //Getters
  inline const std::vector<std::shared_ptr<Data> >& content() const;
  inline const std::array<std::array<double,5>,5>& crosscorrelation() const;
  
private:
  std::vector<std::shared_ptr<Data> > _content;
  std::array<std::array<double,5 >,5 > _crosscorr;
  
  ClassDef(Analyzer,1)
  
};

inline const std::vector<std::shared_ptr<Data> >& Analyzer::content() const { return _content; }
inline const std::array<std::array<double,5>,5>& Analyzer::crosscorrelation() const { return _crosscorr;}

#endif
