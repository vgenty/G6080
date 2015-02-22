#include "LArgon.h"

void LArgon::evolve(const bool r) {
  r ? _restart() : _from_file();

  
}

void LArgon::_restart() {

  // Set initial velocity to zero
  for(auto j : boost::irange(0,_nparticles))
    _v[0][j] = {0.0,0.0,0.0};
  
  // Figure out how we will partition all the particles
  // we can put a cube root of the number of particles in each direction

  // TODO: tweak nside_ definition it's scary, lets get it working for _p = 1.0
  int nside_ = std::cbrt(_nparticles); 
  double inc_   = _L/nside_;
  int    cnt_   = 0;
  
  std::cout << " nside_: " << nside_ << " inc_: " << inc_ << " _L: " << _L << std::endl;
  for(auto x : boost::irange(0,nside_)) { // x direction
    for(auto y : boost::irange(0,nside_)) { // y direction
      for(auto z : boost::irange(0,nside_)) { // z direction
  	_r[0][cnt_] = {x*inc_, y*inc_, z*inc_};
	// std::cout << "r[0][" << cnt_ << "] = {" << _r[0][cnt_][0] << ","
	// 	  << _r[0][cnt_][1] << "," << _r[0][cnt_][2] << "}\n";
	cnt_++;
      }
    }
  }
  
  std::cout << "I put: " << cnt_ << " particles in the box" << std::endl;
}

void LArgon::_from_file() {
  std::cout << "from_file\n";
  
}
