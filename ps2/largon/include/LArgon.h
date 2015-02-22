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
  std::vector<std::vector<boost::array<double, 3> > > _f;

  // kinetic energy
  std::vector<double> _KEtot;

  void _restart();
  void _from_file();
  double _force(const int&i,const int& j, const int& k);

  void _F(const int& i);
  void _K(const int& i);
  
  boost::array<double, 3> _image(const boost::array<double,3>& first,
				 const boost::array<double,3>& second);
  
public:

  LArgon(int ns, int np, double p) : _m(48*_epsilon/(_sigma*_sigma))
  {
    _nparticles = np;
    _nsteps     = ns;
    
    _L = std::cbrt(np/p);
    _t = 0.032; // from Verlet paper

    // Prellocate vectors for speed
    _r.resize(ns);
    _v.resize(ns);
    _f.resize(ns);  
    _KEtot.resize(ns);
    
    //Resize all vectors in the step
    for(auto k : boost::irange(0,ns)) {
      _r[k].resize(np);
      _v[k].resize(np);
      _f[k].resize(np);
    }
  
  }
  
  virtual ~LArgon() {}
  
  void evolve(const bool r);

  //Getters
  std::vector<double> KE() const; // KE
};

inline std::vector<double> LArgon::KE() const { return _KEtot; }

#endif
