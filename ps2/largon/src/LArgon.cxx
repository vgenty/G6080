//vgenty@nevis.columbia.edu

#include "LArgon.h"
void LArgon::evolve() {
  _restart();
  _routine();
}

void LArgon::evolve(int i) {
  _from_file(i);
  _routine();
}

void LArgon::_routine() {
  
  for(auto i : boost::irange(_start,_nsteps-1)) {

    if(!(i%1)){
      std::cout << "Step: " << i << std::endl;
      std::cout << "Temp: " << _Ttot[i] << "\n";
      std::cout << "Vir : " << _Ptot[i] << "\n";
      //_scale_velocities(i,0.0000001);
    } 

    
    // if( i == floor(0.5*_nsteps))
    //   for(auto j : boost::irange(0,_nparticles))
    // 	for(int k = 0; k < 3; ++k)
    // 	  _v[i][j][k] *= -1.0;
      
    
    //slowly lower the temperature of the system.
    
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

    for(auto j : boost::irange(0,_nparticles))
      for(int k = 0; k < 3; ++k)
	_v[i+1][j][k] = _v[i][j][k] + 0.5*(_f[i][j][k] + _f[i+1][j][k])*_t;
    
    _T(i+1);
    _K(i+1);
    _P(i+1);
  
  }
  std::cout << "Finished\n";
  
}



void LArgon::_restart() {
  
  _start = 0;
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
  
  
  //remove CM motion
  for(const auto& j : boost::irange(0,_nparticles))
    for(int b = 0; b < 3; ++b)
      _v[0][j][b] -= totV_[b];
  
  
  
  
  //Scale all velocities to initial temperature
  _scale_velocities(0,_Tinit);
  
  // Figure out how we will partition all the particles
  // we can put a cube root of the number of particles in each direction
 
  // TODO: tweak nside_ definition it's scary, lets get it working for a few _p
  auto nside_ = int{static_cast<int>(std::floor(std::cbrt(_nparticles)))}; 
  auto inc_   = double{_L/nside_};
  auto cnt_   = int{0};
  auto extra_ = double{_nparticles - pow(nside_,3.0)};


  std::cout << "Extra_: " << extra_ << " particles not in cubic" << std::endl;
  std::cout << "Pre density: " << pow(nside_/_L,3.0) << std::endl;
  std::cout << "nside_: " << nside_ << " inc_: " << inc_ << " _L: " << _L << std::endl;

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
  std::cout << "Post density: " << cnt_/pow(_L,3.0) << " on face centered cubic" << std::endl;
  
  if(_nparticles != cnt_){
    std::cout << "Couldn't get the required density :- ( on partial FCC" << std::endl;
    std::exit(0);
  }
  std::cout << "I put: " << cnt_ << " particles in the box just fine" << std::endl;
  std::cout << "Initializing T,K,F,P\n" << std::endl;
  
  _T(0);
  _K(0);
  _F(0);
  _P(0);

}

void LArgon::_from_file(int more_steps) {
  std::cout << "Reading in data from the ROOT file and beginning simulation.\n";

  _start   = _nsteps-1;
  _nsteps += more_steps;

  _resize_all(_nsteps,_nparticles);
  
}

void LArgon::_F(const int& i) {
  
  //don't change this order will mess up image calculation
  //for(auto j : boost::irange(0,_nparticles)) // for each particle
  //#pragma omp parallel
#pragma omp parallel for collapse(2)
  for(int j = 0 ; j  < _nparticles; ++j){ // for each particle
    for(int k = 0; k < 3; ++k) { // for each direction
      _f[i][j][k] = _force(i,j,k);
    }
  }
  
}

double LArgon::_force(const int& i, const int& j, const int& k) {
  double ff_ = 0.0;
  double de_;

  //delete if not openMP
  std::array<double, 3> img_ = {0.0,0.0,0.0};
  
  for(auto l : boost::irange(0,_nparticles)) { // for each particle
  //for(int l = 0 ; l < _nparticles; ++l) { // for each particle
    de_ = 0.0; // reset the denominator before next particle...
    if(j != l) {
      //compute the ``image" distance, feed it the two particles,

      //imcompatable with OPENMP each thread tries to fuck with global var scary
      //if(k == 0) { _img[l] = _image(_r[i][j],_r[i][l]); }
      img_ = _image(_r[i][j],_r[i][l]);

      for(int b = 0; b < 3; ++b) {
	de_ +=  ((img_[b] - _r[i][j][b]) *
		 (img_[b] - _r[i][j][b]));
      }
      
      if(k == 0){ //update the potential only when x coordinate is seen
	_PEtot[i]  += 4*(0.5)*(pow(de_,-6) - pow(de_,-3));
	_Ptot[i]   += (48)*(pow(de_,-6) - 0.5*pow(de_,-3)); 
      }
      ff_ += 48 * (_r[i][j][k] - img_[k])*(pow(de_,-7) - 0.5*pow(de_,-4));

    }
  }

  return ff_;
  
}
void LArgon::_P(const int& i) { // Force will now to _P

  //For now let _P calculate just the virial 
  
  //_Ptot[i] *= 1/(6*_nparticles*_Ttot[i]);
  //_Ptot[i]  = 1 - _Ptot[i];
}
  
std::array<double, 3> LArgon::_image(const std::array<double,3>& one_,
				     const std::array<double,3>& two_) {
  
  std::array<double, 3> img_ = {0.0,0.0,0.0};
  
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

void LArgon::_T(const int& i) {
  
  auto squarev_ = double{0.0};
  for(auto j : boost::irange(0,_nparticles))
    squarev_ += (_v[i][j][0]*_v[i][j][0] +
		 _v[i][j][1]*_v[i][j][1] +
		 _v[i][j][2]*_v[i][j][2]);
  
  //_Ttot[i] = squarev_ / (3* (_nparticles ));
  _Ttot[i] = 16.0 * squarev_ / ( _nparticles );
  
}

double LArgon::_get_ran_double(double min, double max)
{
  boost::uniform_real<double> u(min, max);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > gen(_rng, u);
  return gen();
}
 
void LArgon::_scale_velocities(const int& i, const double& T){
   
  double sumsqs_ = 0.0;
  
  for(auto j : boost::irange(0,_nparticles))
    for(int b = 0; b < 3; ++b)
      sumsqs_ += _v[i][j][b]*_v[i][j][b];
   
  //double scale_ = sqrt( 3 * (_nparticles) * T / sumsqs_ );
  double scale_ = sqrt( (_nparticles) * T / (sumsqs_ * 16.0));
   
  for(auto j : boost::irange(0,_nparticles))
    for(int b = 0; b < 3; ++b)
      _v[i][j][b] *= scale_;
}

void LArgon::_resize_all(const int& ns, const int& np) {

  // Prellocate vectors for speed
  _r.resize(ns);
  _v.resize(ns);
  _f.resize(ns);  
  
  _KEtot.resize(ns);
  _PEtot.resize(ns);
  _Ptot.resize(ns);
  _Ttot.resize(ns);

  //Resize all vectors in the step
  
  for(auto k : boost::irange(0,ns)) {
  //  for(int k = 0; k< ns; ++k) {
    _r[k].resize(np);
    _v[k].resize(np);
    _f[k].resize(np);
  }
  
  _img.resize(np);

}


//Misc shit:


      // if(j == 0)
      // 	std::cout << k << " ~ dir ~ " << de_ << " ~ dist ~  => p " << j << " {" << _r[i][j][0] << "," << _r[i][j][1] << "," << _r[i][j][2] << "} on p " << l << " {" << _r[i][l][0] << "," << _r[i][l][1] << "," << _r[i][l][2] << "} " << " at image: " << " {" << _img[l][0] << "," << _img[l][1] << "," << _img[l][2] << "} \ntotal f is: " << std::setprecision(15) << ff_ << "\n~~\n";


  //  std::cout << "I calculated the image at " << img_[0] << "," << img_[1] << "," << img_[2] << "\n";


  // std::cout << "have the two particles: (" << one_[0] << "," << one_[1] << "," << one_[2] << ") - (" 
  // 	    << two_[0] << "," << two_[1] << "," << two_[2] << ")\n";
  
