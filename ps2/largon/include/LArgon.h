//vgenty@nevis.columbia.edu

#ifndef LARGON_H
#define LARGON_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

#include "boost/range/irange.hpp"
#include "boost/random.hpp"

#include "TObject.h"

class LArgon : public TObject { // I guess we will derive from
                                // TObject, speeds up ROOT IO 

 private:
  
  //LAr Constants
  const double _epsilon = 119.8; // Kelvin
  const double _sigma   = 3.405; // Angstroms
  double _m;
  
  //Simulation Constants
  int _nparticles;
  int _nsteps;
  double _L;
  double _t;
  double _Tinit;
  double _Tfinal;
  int _start;

  // int step, int particle number, double 3 vector
  
  // positions
  std::vector<std::vector<std::array<double, 3> > > _r;
  
  // velocity
  std::vector<std::vector<std::array<double, 3> > > _v;
  
  // force
  std::vector<std::vector<std::array<double, 3> > > _f;

  // energy
  std::vector<double> _KEtot;
  std::vector<double> _PEtot;
  std::vector<double> _Ttot;
  std::vector<double> _Ptot;


  // double _PEnow = 0.0;
  // double _PEaft = 0.0;

  // double _KEnow = 0.0;
  // double _KEaft = 0.0;

  // double _Tnow = 0.0;
  // double _Taft = 0.0;

  
  //Simulation directors
  void _routine ();
  void _routine2();
  void _routine3();
  
  void _restart();
  void _from_file(int more_steps);
  
  void _F(const int& i);
  void _K(const int& i);
  void _T(const int& i);
  
  std::array<double, 3> _image(const std::array<double,3>& first,
			       const std::array<double,3>& second);
  
  //image location placeholder...
  std::vector<std::array<double, 3> > _img;

  //Random number object - use boost even though it's in C++11
  //std::random_device _rd;
  //std::mt19937 _rangen;
  
  double _get_ran_double(double min, double max);
  boost::mt19937 _rng;
  
  void _scale_velocities(const int& i, const double& T);
    
  void _resize_all(const int& ns,const int& np); //For new run/continue
  
 public:
  
  LArgon() {} //Default constructor for ROOT
  
  LArgon(int ns, int np, double p, double Ti, double Tf) : 
  _m(48.0*_epsilon/(_sigma*_sigma))
    {
      _nparticles = np;
      _nsteps     = ns;
      
      _L = std::cbrt(np/p);
      //_t = 0.032; // from Verlet paper
      _t = 0.01; // from Bob
      _Tinit = Ti;
      _Tfinal = Tf;
      
      _resize_all(ns,np);
      
      _rng.seed(static_cast<unsigned int>(std::time(0))); // set seed
      
    }
  
  virtual ~LArgon() {}
  
  // Public evolve
  void evolve();
  void evolve(int i);
  void monte();
  
  //Getters
  std::vector<double> KE() const; // KE
  std::vector<double> PE() const; // PE
  std::vector<double> T()  const; // T
  std::vector<double> P()  const; // P (Virial...)

  std::vector<std::vector<std::array<double, 3> > > R() const;
  std::vector<std::vector<std::array<double, 3> > > V() const;
  
  ClassDef(LArgon,1)
};
inline std::vector<double> LArgon::KE() const { return _KEtot; }
inline std::vector<double> LArgon::PE() const { return _PEtot; }
inline std::vector<double> LArgon::T()  const { return _Ttot; }
inline std::vector<double> LArgon::P()  const { return _Ptot; }

inline std::vector<std::vector<std::array<double, 3> > > LArgon::R() const { return _r; }
inline std::vector<std::vector<std::array<double, 3> > > LArgon::V() const { return _v; }

#endif
