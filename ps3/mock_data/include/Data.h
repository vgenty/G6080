//vgenty@nevis.columbia.edu

#ifndef DATA_H
#define DATA_H

#include <map>
#include <vector>
#include <utility>
#include <iostream>

#include "boost/range/irange.hpp"

#include "Constants.h"

class Data {

public:

  Data(std::string name) : _name(name) { _data.resize(NUMVALS); } //Default constructor for ROOT

  virtual ~Data() {}
  
  void calcPop(); //Calculate population parameters
  void calcSam(int i); //Calculate sample parameters
  
  
  //Getters and setter
  double  at(int i) const  { return _data[i]; }
  double& at(int i)        { return _data[i]; }


  //Friends
  friend std::ostream& operator<< (std::ostream& o, const Data& d) { o << d._name << " " << d._mean << " " << std::sqrt(d._var) <<  "\n";
    return o; }
  
private:
  std::string _name;
  std::vector<double> _data;

  //Population calculations
  double _mean;
  double _var;

  //Sample calculations
  //maps N samples to a vector of each sample means, and stdevs.
  std::map<int, std::vector<std::pair<double,double> > > _samps;

  inline const double& mean() const { return _mean; }
  inline const double& var () const { return _var; }
  
protected:
  // no inheritance 
  
};

#endif
