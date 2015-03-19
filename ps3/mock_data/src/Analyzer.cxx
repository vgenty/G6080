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
    _content[i]->calcAutoIntegrated();

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

