//vgenty@nevis.columbia.edu

#ifndef DATA_H
#define DATA_H

#include "boost/range/irange.hpp"
#include "Constants.h"

class Data {

public:

  Data(std::string name) : _name(name) { _data.resize(NUMVALS); } //Default constructor for ROOT

  virtual ~Data() {}
  
  double  at(int i) const  { return _data[i]; }
  double& at(int i)        { return _data[i]; }

private:
  std::string _name;
  std::vector<double> _data;
  
protected:
  // no inheritance 
  
};

#endif
