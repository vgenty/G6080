#ifndef LARGON_H
#define LARGON_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>

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
  double _Tinit;

  // int step, int particle number, double 3 vector

  // positions
  std::vector<std::vector<std::array<double, 3> > > _r;

  // velocity
  std::vector<std::vector<std::array<double, 3> > > _v;

  // force
  std::vector<std::vector<std::array<double, 3> > > _f;

  // Energy
  std::vector<double> _KEtot;
  std::vector<double> _PEtot;
  std::vector<double> _Ttot;

  void _restart();
  void _from_file();

  double _force(const int&i,const int& j, const int& k);
  //double _pe(const int&i,const int& j);
  
  void _F(const int& i);
  void _K(const int& i);
  void _T(const int& i);
  //void _P(const int& i); // Force will now to _P
  
  std::array<double, 3> _image(const std::array<double,3>& first,
			       const std::array<double,3>& second);

   //image location placeholder...
  std::vector<std::array<double, 3> > _img;

  //Random number object - use boost even though it's in C++11
  //std::random_device _rd;
  //std::mt19937 _rangen;
  
  double _get_ran_double(double min, double max);
  boost::mt19937 _rng;

public:
  
  LArgon() {} //Default constructor for ROOT

  LArgon(int ns, int np, double p, double T) : _m(_epsilon/(_sigma*_sigma))
  {
    _nparticles = np;
    _nsteps     = ns;
    
    _L = std::cbrt(np/p);
    //_t = 0.032; // from Verlet paper
    _t = 0.01; // from Bob
    _Tinit = T;
    // Prellocate vectors for speed
    _r.resize(ns);
    _v.resize(ns);
    _f.resize(ns);  
    
    _KEtot.resize(ns);
    _PEtot.resize(ns);
    _Ttot.resize(ns);

    //Resize all vectors in the step
    for(auto k : boost::irange(0,ns)) {
      _r[k].resize(np);
      _v[k].resize(np);
      _f[k].resize(np);
    }
  
    _img.resize(np);

  }
  
  virtual ~LArgon() {}
  
  // Public evolve
  void evolve(const bool r);

  //Getters
  std::vector<double> KE() const; // KE
  std::vector<double> PE() const; // PE
  std::vector<double> T() const; // T

  std::vector<std::vector<std::array<double, 3> > > R() const;
  std::vector<std::vector<std::array<double, 3> > > V() const;
  
};
inline std::vector<double> LArgon::KE() const { return _KEtot; }
inline std::vector<double> LArgon::PE() const { return _PEtot; }
inline std::vector<double> LArgon::T() const { return _Ttot; }

inline std::vector<std::vector<std::array<double, 3> > > LArgon::R() const { return _r; }
inline std::vector<std::vector<std::array<double, 3> > > LArgon::V() const { return _v; }



#endif
