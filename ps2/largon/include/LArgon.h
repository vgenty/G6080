#ifndef LARGON_H
#define LARGON_H

#include <cmath>
#include <iostream>
#include <iomanip>

#include "boost/range/irange.hpp"
#include "boost/random.hpp"

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
  std::vector<std::vector<std::array<double, 3> > > _r;

  // velocity
  std::vector<std::vector<std::array<double, 3> > > _v;

  // force
  std::vector<std::vector<std::array<double, 3> > > _f;

  // kinetic energy 
  std::vector<double> _KEtot;
  std::vector<double> _PEtot;

  void _restart();
  void _from_file();

  double _force(const int&i,const int& j, const int& k);
  double _pe(const int&i,const int& j);
  
  void _F(const int& i);
  void _K(const int& i);
  void _P(const int& i);
  
  std::array<double, 3> _image(const std::array<double,3>& first,
			       const std::array<double,3>& second);

   //image location placeholder...
  std::vector<std::array<double, 3> > _img;

  
  //Random number object
  //std::random_device _rd;
  //std::mt19937 _rangen;
  double _get_ran_double(double min, double max);
  boost::mt19937 _rng;

public:
  
  LArgon() {}

  LArgon(int ns, int np, double p) : _m(48*_epsilon/(_sigma*_sigma))
  {
    _nparticles = np;
    _nsteps     = ns;
    
    _L = std::cbrt(np/p);
    //_t = 0.032; // from Verlet paper
    _t = 0.01; // from Bob

    // Prellocate vectors for speed
    _r.resize(ns);
    _v.resize(ns);
    _f.resize(ns);  
    
    _KEtot.resize(ns);
    _PEtot.resize(ns);
    
    //Resize all vectors in the step
    for(auto k : boost::irange(0,ns)) {
      _r[k].resize(np);
      _v[k].resize(np);
      _f[k].resize(np);
    }
  
    _img.resize(np);

  }
  
  virtual ~LArgon() {}
  
  void evolve(const bool r);

  //Getters
  std::vector<double> KE() const; // KE
  std::vector<double> PE() const; // PE

  std::vector<std::vector<std::array<double, 3> > > R() const;
  std::vector<std::vector<std::array<double, 3> > > V() const;
  
};
inline std::vector<double> LArgon::KE() const { return _KEtot; }
inline std::vector<double> LArgon::PE() const { return _PEtot; }
inline std::vector<std::vector<std::array<double, 3> > > LArgon::R() const { return _r; }
inline std::vector<std::vector<std::array<double, 3> > > LArgon::V() const { return _v; }



#endif
