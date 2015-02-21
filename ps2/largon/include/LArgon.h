#ifndef LARGON_H
#define LARGON_H

#include <cmath>
#include <iostream>

#include "boost/range/irange.hpp"
#include "boost/multi_array.hpp"  //Eh might use or not
#include "boost/array.hpp"

class LArgon { 

private:
  
  //LAr Constants
  const double _epsilon = 119.8; // Kelvin
  const double _sigma   = 3.405; // Angstroms
  double _m;
  int _nparticles;
  
  //Simulation Constants
  int _nsteps;
  double _L;
  double _t;

  // int step, int particle number, double 3 vector

  // positions
  std::vector<std::vector<boost::array<double, 3> > > _r;

  // velocity
  std::vector<std::vector<boost::array<double, 3> > > _v;

  // force
  std::vector<double> _f;

  // kinetic energy
  std::vector<double> _KEtot;

  void _restart();
  void _from_file();

public:

  LArgon(int ns, int np, double p) : _m(48*_epsilon/(_sigma*_sigma))
  {
    _nparticles = np;
    _nsteps     = ns;
    
    _L = std::cbrt(np/p);
    _t = 0.032; // from Verlet paper

    // Allocate vectors
    _list.resize(ns);
    _KEtot.resize(ns);
    
    for(auto& step : _list)
      step.resize(np);
  }

  virtual ~LArgon() {}
  
  void evolve(const bool r);
};

#endif
