#include "LArgon.h"

void LArgon::evolve(const bool r) {
  r ? _restart() : _from_file();

  for(auto i : boost::irange(0,_nsteps-1)) {

    for(auto j : boost::irange(0,_nparticles)) {
      for(int k = 0; k < 3; ++k) {
	_r[i+1][j][k] = _r[i][j][k] + _v[i][j][k]*_t + 0.5*_f[i][j][k]*_t*_t; 
	
	// periodic boundary conditions
      	if(_r[i+1][j][k] >= _L)
	  _r[i+1][j][k] = fmod(_r[i+1][j][k],_L);
	
	if( _r[i+1][j][k] < 0 )
	  _r[i+1][j][k] = _L - fmod(fabs(_r[i+1][j][k]),_L);
      }
    }
    
    _F(i+1);
    std::cout << "r[" << i << "][" << 0 << "] = {" << _r[i][0][0] << ","
     	      << _r[i][0][1] << "," << _r[i][0][2] << "}\n";  
    
    
    for(auto j : boost::irange(0,_nparticles))
      for(int k = 0; k < 3; ++k)
	_v[i+1][j][k] = _v[i][j][k] + 0.5*(_f[i][j][k] + _f[i+1][j][k])*_t;
    
    _K(i+1);
  }
  
  // for(auto cnt_ : boost::irange(0,_nparticles)) {
  //   std::cout << "r[99][" << cnt_ << "] = {" << _v[99][cnt_][0] << ","
  // 	      << _v[99][cnt_][1] << "," << _v[99][cnt_][2] << "}\n";  
  // }
  
  // for i in xrange(self.nsteps-1):

  //     # Predict each planets r, v
  //     for j in xrange(self.NP):
  //         for k in self.xyz:
  //             self.r[i+1][j][k] = self.r[i][j][k] + self.dx * self.fr[i][j][k]
  //             self.v[i+1][j][k] = self.v[i][j][k] + self.dx * self.fv[i][j][k]

  //     # Calculate the force with updated values of r, v
  //     for j in xrange(self.NP):
  //         for k in self.xyz:
  //             self.F(i+1,k)

  //     # Correct each planet r,v with update force
  //     for j in xrange(self.NP):
  //         for k in self.xyz:
  //             self.r[i+1][j][k] = self.r[i][j][k] + self.dx/2.0 * (self.fr[i][j][k]
  //                                                                  + self.fr[i+1][j][k])
  //             self.v[i+1][j][k] = self.v[i][j][k] + self.dx/2.0 * (self.fv[i][j][k]
  //                                                                  + self.fv[i+1][j][k])
  //     # Update the force one more time for the next iteration
  //     for j in xrange(self.NP):
  //         for k in self.xyz:
  //             self.F(i+1,k)
                    
  //     # Calculate total KE, PE, E
  //     self.K(i+1)
  //     self.P(i+1)
  //     self.E(i+1)
  
  
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

  std::cout << "Initializing force\n" << std::endl;
  
  _F(0);
  _K(0);
}

void LArgon::_from_file() {
  std::cout << "from_file\n";
  
}

void LArgon::_F(const int& i) {
  
  for(int k = 0; k < 3; ++k) // for each direction
    for(auto j : boost::irange(0,_nparticles)) // for each particle
      _f[i][j][k] = _force(i,j,k);

  // for j in xrange(self.NP): # planet number
  // 	  self.fr[i][j][k] = self.v[i][j][k] 
  // 	  self.fv[i][j][k] = self.force(i,j,k) #calculate force on j'th planet along k direction

}

double LArgon::_force(const int& i, const int& j, const int& k) {
  double ff_ = 0.0;
  double de_ = 0.0;
  boost::array<double, 3> img_ = {0.0,0.0,0.0};
  
  for(auto l : boost::irange(0,_nparticles)) { // for each particle
    if(j != l) {
      //compute the ``image" distance, feed it the two particles
      img_ = _image(_r[i][j],_r[i][l]);
      
      for(int b = 0; b < 3; ++b) {
	de_ +=  ((img_[b] - _r[i][j][b]) *
		 (img_[b] - _r[i][j][b]));
      }
      
      ff_ += (_r[i][j][k] - img_[k])*(pow(de_,-7) - 0.5*pow(de_,-4));
    }
  }
  
  //return _m*ff_;
  return ff_;

}

boost::array<double, 3> LArgon::_image(const boost::array<double,3>& one_,
				       const boost::array<double,3>& two_) {
  
  boost::array<double, 3> img_ = {0.0,0.0,0.0};
  
  //loop over the three directions
   for(auto i: boost::irange(0,3)) {
     //decide which is shorter, the image or the real
     if((fabs(two_[i]-one_[i]) <= fabs(two_[i]-_L-one_[i]))  &&
	(fabs(two_[i]-one_[i]) <= fabs(two_[i]+_L-one_[i]))) {
       img_[i] = two_[i];
     } else if(fabs(two_[i]-_L-one_[i]) < fabs(two_[i]+_L-one_[i])){
       img_[i] = two_[i]-_L;
     }else {
       img_[i] = two_[i]+_L;
       
     }
   }
   
   return img_;
}

void LArgon::_K(const int& i) { //for now this is like kinetic energy per mass
  for(auto l : boost::irange(0,_nparticles)) { // for each particle  
    _KEtot[i] += 0.5*(_v[i][l][0]*_v[i][l][0] +
		      _v[i][l][1]*_v[i][l][1] +
		      _v[i][l][2]*_v[i][l][2] );
  }
}
