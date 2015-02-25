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
    
    //std::cout << std::setprecision(15) << "r[" << i << "][" << 0 << "] = {" << _r[i][50][0] << ","
    // 	      << _r[i][50][1] << "," << _r[i][50][2] << "}\n";  
        
    for(auto j : boost::irange(0,_nparticles))
      for(int k = 0; k < 3; ++k)
	_v[i+1][j][k] = _v[i][j][k] + 0.5*(_f[i][j][k] + _f[i+1][j][k])*_t;
    
    _K(i+1);
    _T(i+1);
    //_P(i+1);
  }
  std::cout << "Simulation finished calculating final pressure value" << std::endl;
  double p_ = 0.0;
  int f_ = _nsteps-1;
  std::array<double , 3> img_;
  double de_ = 0.0;
  double fe_ = 0.0;
  //Sim is done lets print to the screen the pressure...
  for(auto j : boost::irange(0,_nparticles)){
    for(auto l : boost::irange(0,_nparticles)){
      de_ = 0.0;
      fe_ = 0.0;
      if(j != l) {
	img_ = _image(_r[f_][j],_r[f_][l]);
	for(int b = 0; b < 3; ++b) {
	  de_ +=  ((img_[b] - _r[f_][j][b]) *
		   (img_[b] - _r[f_][j][b]));
	}
	// for(int b = 0; b<3; ++b)
	//   fe_ += (_r[f_][j][b] - img_[b])*(_r[f_][j][b] - img_[b]);
	
	//p_ += sqrt(de_) * sqrt(fe_) * fabs(48*(pow(de_,-7) - 0.5*pow(de_,-4))); 
	p_ += sqrt(de_)*fabs(48*(pow(sqrt(de_),-13) - 0.5*pow(sqrt(de_),-7))); 
	
      }
    }
  }

  //p_ *= (1/(6*_nparticles*_Ttot[f_]));
  p_ *= (1/(6*_nparticles*_Ttot[f_]));
  p_ = 1 - p_;
  std::cout << "p_: " << p_ << std::endl;
}

void LArgon::_restart() {

  // Set initial velocity to zero
  std::array<double,3> totV_ = {0.0,0.0,0.0};

  //Some range of initial velocities
  for(auto j : boost::irange(0,_nparticles)) {
    _v[0][j] = {_get_ran_double(-1.0,1.0),
		_get_ran_double(-1.0,1.0),
		_get_ran_double(-1.0,1.0)};
    
    totV_[0] += _v[0][j][0];
    totV_[1] += _v[0][j][1];
    totV_[2] += _v[0][j][2];
  }

  // Make the cm velocity zero
  for(int b = 0; b < 3; ++b)
    totV_[b] /= _nparticles;


  double sumsqs_ = 0.0;
  //remove CM motion
  for(auto j : boost::irange(0,_nparticles)) {
    for(int b = 0; b < 3; ++b) {
      _v[0][j][b] -= totV_[b];
      sumsqs_ += _v[0][j][b]*_v[0][j][b];
    }
  }
  
  double scale_ = sqrt( 3 * (_nparticles) * _Tinit / sumsqs_ );
  for(auto j : boost::irange(0,_nparticles))
    for(int b = 0; b < 3; ++b)
      _v[0][j][b] *= scale_;
    
  
  //Scale all velocities to initial temperature
  

  // Figure out how we will partition all the particles
  // we can put a cube root of the number of particles in each direction

  // TODO: tweak nside_ definition it's scary, lets get it working for _p = 1.0
  int nside_ = floor(std::cbrt(_nparticles)); 
  double inc_   = _L/nside_;
  int    cnt_   = 0;
  double extra_ = _nparticles - pow(nside_,3.0);
  
  std::cout << "extra_: " << extra_ << std::endl;
  std::cout << "pre density: " << pow(nside_/_L,3.0) << std::endl;
  std::cout << " nside_: " << nside_ << " inc_: " << inc_ << " _L: " << _L << std::endl;
  for(auto x : boost::irange(0,nside_)) { // x direction
    for(auto y : boost::irange(0,nside_)) { // y direction
      for(auto z : boost::irange(0,nside_)) { // z direction
	_r[0][cnt_] = {x*inc_,y*inc_,z*inc_};
	//std::cout << "r[0][" << cnt_ << "] = {" << _r[0][cnt_][0] << ","
	//	  << _r[0][cnt_][1] << "," << _r[0][cnt_][2] << "}\n";
	cnt_++;
      }
    }
  }

  //put the extras in like in fcc
  if(extra_) {
    for(auto x : boost::irange(0,nside_-1)) { // x direction
      for(auto y : boost::irange(0,nside_-1)) { // y direction
 	for(auto z : boost::irange(0,nside_-1)) { // z direction
 	  _r[0][cnt_] = {(0.5+x)*inc_,(0.5+y)*inc_,(0.5+z)*inc_};
 	  //std::cout << "cnt_ " << cnt_;
 	  //std::cout << "r[0][" << cnt_ << "] = {" << _r[0][cnt_][0] << ","
	    //<< _r[0][cnt_][1] << "," << _r[0][cnt_][2] << "}\n";
 	  
	  if(_nparticles == cnt_) 
 	    goto LABEL;
	  
  	  cnt_++;
  	}
       }
    }
  }

  LABEL:
  std::cout << "post density: " << cnt_/pow(_L,3.0) << std::endl;
  
  if(_nparticles != cnt_){
    std::cout << "couldn't get the required density :- (" << std::endl;
    std::exit(0);
  }
  std::cout << "I put: " << cnt_ << " particles in the box" << std::endl;
  std::cout << "Initializing force\n" << std::endl;
  
  _F(0);
  _K(0);
  _T(0);
  //_P(0);
}

void LArgon::_from_file() {
  std::cout << "from_file\n";
  
}

void LArgon::_F(const int& i) {
  
  //don't change this order will mess up image calculation
  for(auto j : boost::irange(0,_nparticles)) // for each particle
    for(int k = 0; k < 3; ++k) // for each direction
      _f[i][j][k] = _force(i,j,k);

  // for j in xrange(self.NP): # planet number
  // 	  self.fr[i][j][k] = self.v[i][j][k] 
  // 	  self.fv[i][j][k] = self.force(i,j,k) #calculate force on j'th planet along k direction

}

double LArgon::_force(const int& i, const int& j, const int& k) {
  double ff_ = 0.0;
  double de_;
  
  for(auto l : boost::irange(0,_nparticles)) { // for each particle
    de_ = 0.0; // reset the denominator before next particle...
    if(j != l) {
      //compute the ``image" distance, feed it the two particles,
      if(k == 0) { _img[l] = _image(_r[i][j],_r[i][l]); }
      
      for(int b = 0; b < 3; ++b) {
	de_ +=  ((_img[l][b] - _r[i][j][b]) *
		 (_img[l][b] - _r[i][j][b]));
      }
      
      if(k == 0) //update the potential only when x coordinate is seen
	_PEtot[i] += 4*(0.5)*(pow(de_,-6) - pow(de_,-3)) ;

      //do i need the 48 here ~ sure why not?
      ff_ += 48 * (_r[i][j][k] - _img[l][k])*(pow(de_,-7) - 0.5*pow(de_,-4));
      // if(j == 0)
      // 	std::cout << k << " ~ dir ~ " << de_ << " ~ dist ~  => p " << j << " {" << _r[i][j][0] << "," << _r[i][j][1] << "," << _r[i][j][2] << "} on p " << l << " {" << _r[i][l][0] << "," << _r[i][l][1] << "," << _r[i][l][2] << "} " << " at image: " << " {" << _img[l][0] << "," << _img[l][1] << "," << _img[l][2] << "} \ntotal f is: " << std::setprecision(15) << ff_ << "\n~~\n";
    }
  }
  
  //return _m*ff_;
  return ff_;

}

std::array<double, 3> LArgon::_image(const std::array<double,3>& one_,
				     const std::array<double,3>& two_) {
  
  std::array<double, 3> img_ = {0.0,0.0,0.0};
  
  // std::cout << "have the two particles: (" << one_[0] << "," << one_[1] << "," << one_[2] << ") - (" 
  // 	    << two_[0] << "," << two_[1] << "," << two_[2] << ")\n";
  
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
  //  std::cout << "I calculated the image at " << img_[0] << "," << img_[1] << "," << img_[2] << "\n";
  return img_;
}

void LArgon::_K(const int& i) { //for now this is like kinetic energy per mass
  for(auto l : boost::irange(0,_nparticles)) { // for each particle  
    _KEtot[i] += 0.5*(_v[i][l][0]*_v[i][l][0] +
		      _v[i][l][1]*_v[i][l][1] +
		      _v[i][l][2]*_v[i][l][2] );
  }
}

void LArgon::_T(const int& i) {
  
  double squarev_ = 0.0;
  for(auto j : boost::irange(0,_nparticles))
    squarev_ += (_v[i][j][0]*_v[i][j][0] +
		 _v[i][j][1]*_v[i][j][1] +
		 _v[i][j][2]*_v[i][j][2]);
  
  _Ttot[i] = squarev_ / (3* (_nparticles ));

}
// void LArgon::_P(const int& i) {
  
//   for(auto j : boost::irange(0,_nparticles))
//     _PEtot[i] += (0.5)*_pe(i,j);

// }

// double LArgon::_pe(const int&i,const int& j) {

//   double pe_temp = 0.0;
//   double de_;
//   std::array<double,3> img_;
  
//   for(auto l : boost::irange(0,_nparticles)){
//     de_ = 0.0;
//     if(j != l) {
//       img_ = _image(_r[i][j],_r[i][l]);
//       for(int b = 0; b < 3; ++b) {
// 	de_ +=  ((img_[b] - _r[i][j][b]) *
// 		 (img_[b] - _r[i][j][b]));
//       }
//       pe_temp += 4*(pow(de_,-6) - pow(de_,-3)) ;
//       if(j == 0)
// 	std::cout << de_ << " ~ dist ~  => p " << j << " {" << _r[i][j][0] << "," << _r[i][j][1] << "," << _r[i][j][2] << "} on p " << l << " {" << _r[i][l][0] << "," << _r[i][l][1] << "," << _r[i][l][2] << "} " << " at image: " << " {" << img_[0] << "," << img_[1] << "," << img_[2] << "} \ntotal pe is: " << std::setprecision(15) << pe_temp << "\n~~\n";
//     }
//   }
  
//   return pe_temp;
// }

 double LArgon::_get_ran_double(double min, double max)
 {
   boost::uniform_real<double> u(min, max);
   boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > gen(_rng, u);
   return gen();
 }
 

