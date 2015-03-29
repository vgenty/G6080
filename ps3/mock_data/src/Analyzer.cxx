//vgenty@nevis.columbia.edu

#include "Analyzer.h"

Analyzer::Analyzer(std::string location) {
  
  std::ifstream file_;

  _content.resize(NUMFILES);

  for(int i = 0; i < NUMFILES; ++i) {
    
    std::string name_ = "v" + std::to_string(i+1);
    _content[i] = std::make_shared<Data>(name_);
    
    file_.open("./" + location + "/" + name_ + ".dat");


    std::cout << "Reading in: " << name_ << "\n";
    double val_;
    
    if(file_.is_open()) {
     
     for(int j : boost::irange(0,NUMVALS)) {
       file_ >> val_;
       _content[i]->at(j) = val_;
     }
     
     file_.close();
     
   } else {
     std::cout << "Could not open " << file_ << "\n";
     std::exit(0);
    }
  }

}

void Analyzer::a() {
  std::cout << "\n\t Part 1a\n";
  
#pragma omp parallel for
  for(int i = 0; i < NUMFILES; ++i)
    _content[i]->calcPop();
  
  for_each(_content.begin(),_content.end(),[] (std::shared_ptr<Data> d) {
      std::cout << (*d);
    });
}

void Analyzer::b() {
  std::cout << "\n\t Part 1b\n";


  //Populating internal map
  
#pragma omp parallel for
  for(int i = 0; i < NUMFILES; ++i) {
    _content[i]->calcSam(1000 );
    _content[i]->calcSam(10000);
  }
  
}

void Analyzer::c(int n) {
  std::cout << "\n\t Part 1c\n";
  std::cout << "Calculating correlation with n = " << n << "\n";
  
#pragma omp parallel for
  for(int i = 0; i < NUMFILES; ++i)
    for(int j = 0; j <= n; ++j) // I think we repeated 0 but whatever
    _content[i]->calcAuto(j);
  
}

void Analyzer::d() {
  std::cout << "\n\t Part 1d\n";
  std::cout << "Integrated correlation time..." << "\n";

#pragma omp parallel for
  for(int i = 0; i < NUMFILES; ++i)
    _content[i]->calcAutoIntegrated(125);

}

void Analyzer::e() {
  std::cout << "\n\t Part 1e\n";
  std::cout << "Cross correlation matrix return 5x5 something...\n";

  //Cross correlation matrix is symmetric but it's probably easier
  //to over calculate instead of being careful


  //don't know if I can pass Boost matrix to ROOT so use std::array...
#pragma omp parallel for
  for(int i = 0; i < 5; ++i){
    for(int j = 0; j < 5; ++j) {
      for(int k = 0; k < NUMVALS; ++k) {
	_crosscorr[i][j] +=
	  (_content[i]->data()[k] - _content[i]->mean()) *
	  (_content[j]->data()[k] - _content[j]->mean());
      }
      //Normalize...
      _crosscorr[i][j] /= (NUMVALS*_content[i]->stdev()*_content[j]->stdev());
    }
  }

  
}


void Analyzer::three(int nmax,int N) {

  double C_ = 0.0;

  std::vector<double> CS_;
  std::vector<double> tint_;

  // samples size is 1000
  int t_ = NUMVALS/N;
  int N_ = N;
  
  // for(auto& k : _content[0]->samples()) {
  //   std::cout << k.first << std::endl;
  //   std::cout << k.second.size() << std::endl;
  //   std::cout << k.second[0].first << std::endl;
  // }
  
  auto s = _content[0]->samples(); //what the fuck why do i have to do this??
  std::vector<std::vector<double> > d; //trick compiler into thinking d exists tehe: - )
  
  if(N == 1000)
    d = _content[0]->datasamples1k();
  else if (N == 10000)
    d = _content[0]->datasamples10k();
  else
    std::exit(0);
  
  //std::cout << _content[0]->samples()[1000].size();
  
  _samples_tint.clear();
  

  std::cout << "calling analyzer::three with nmax = " << nmax << "\n";
  //currently only doing the first file!!!
  for(int j = 0; j < t_; ++j) { //loop over all samples
    for(int n = 0; n < nmax; ++n) { //loop over n values...
      for(int i = 0; i < N_ - n; ++i) { //actually loop
	C_ += (d[j][i+n] - s[N_][j].first)*
	  (d[j][i]- s[N_][j].first);
      }
      
      C_ /= static_cast<double>((N_- n));
      CS_.push_back(C_);
      C_ = 0.0;
    } //end n
    
      //CS_ now has a vector of Cn's up to nmax, now sum them up (integrate them)
    
    double counter = 0.0;
    
    std::for_each(CS_.begin(),CS_.end(),[&counter](double& C){counter += C; });
    
    tint_.push_back(counter/CS_[0]);
    
    CS_.clear();
  } //end j
  std::cout << tint_.size() << "\n";
  _samples_tint.push_back(tint_);
  tint_.clear();
  // end files
  
}
    
      
