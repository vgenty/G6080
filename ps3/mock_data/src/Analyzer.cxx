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
  std::cout << "\t Part 1a\n";
  
#pragma omp parallel for
  for(int i = 0; i < NUMFILES; ++i)
    _content[i]->calcPop();
  
  for_each(_content.begin(),_content.end(),[] (std::shared_ptr<Data> d) {
      std::cout << (*d);
    });
}
