#include "LArgon.h"

#include "TFile.h"
#include "TTree.h"

#include "boost/program_options.hpp"

int main(int argc, char *argv[]) {

  namespace po = boost::program_options;
  po::options_description desc("\nrestart,\ncontinue");
  desc.add_options()
    ("restart,r","Restart (destroy out.root)")
    ("continue,c","Continue from out.root");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  
  if ( vm.count("restart") ) {
    
    auto the_file = std::make_shared<TFile>("output/out.root","RECREATE");
    auto the_tree = std::make_shared<TTree>("outtree","physics");
    
    LArgon b(10000,300,0.75,0.1,1.069);
    
    the_tree->Branch("LArgon",&b);
    
    b.evolve(0);
    
    the_tree->Fill();
    the_tree->Write();
    the_file->Close();
  
  }
  
  if (vm.count("continue")) {
    
    LArgon *b = new LArgon();
    
    auto the_file = std::make_shared<TFile>("output/out.root","read");
    auto in_tree  = dynamic_cast<TTree*>(the_file->Get("outtree"));
    in_tree->SetBranchAddress("LArgon",&b);
    in_tree->GetEntry(0); // There should be only one entry
    
  
    //a.evolve(b);
    b->evolve(1);
    
  }

  
  return 0;
 
}
