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
  void calcAuto(int n); // n autocorrelation spacing, keep it n because we could kill it 
  void calcAutoIntegrated();
  
  //Getters and setter
  double  at(int i) const  { return _data[i]; }
  double& at(int i)        { return _data[i]; }


  inline const std::map<int, std::vector<std::pair<double,double> > >& samples() const;
  inline const std::vector<double>& data() const;
  inline const std::vector<double>& correlation() const;
  inline const double& integrated_correlation() const;
  
  //Friends
  friend std::ostream& operator<< (std::ostream& o, const Data& d) { o << d._name << " " << d._mean << " " << std::sqrt(d._var) <<  "\n";
    return o; }

  
  inline const double& mean() const { return _mean; }
  inline const double& var () const { return _var; }
  inline const double stdev () const { return std::sqrt(_var); }

  
private:
  std::string _name;
  std::vector<double> _data;

  //Population calculations
  double _mean;
  double _var;
  
  //Sample calculations
  //maps N samples to a vector of each sample means, and stdevs.
  std::map<int, std::vector<std::pair<double,double> > > _samps;

  // Autocorrelation
  std::vector<double> _corrs;
  double _integrated_corrs;
  
protected:
  // no inheritance 
  
};


inline const double& Data::integrated_correlation() const { return _integrated_corrs;}

inline const std::map<int, std::vector<std::pair<double,double> > >& Data::samples() const { return _samps; }
inline const std::vector<double>& Data::correlation() const { return _corrs; }
inline const std::vector<double>& Data::data() const { return _data; }
#endif
