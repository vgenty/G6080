//vgenty@nevis.columbia.edu

#include "LArgon.h"

#include "TFile.h"
#include "TTree.h"

#include "boost/program_options.hpp"


int main(int argc, char *argv[]) {

  namespace po = boost::program_options;
  po::options_description desc("\nLiquid Argon Simulation\nvgenty@nevis.columbia.edu\n\n\nOptions");
  desc.add_options()
    ("help,h","Print help message")
    ("restart,r",po::value<std::vector<double> >()->multitoken(),
     "-r [steps] [particles] [density] [Ti] [Tf] ~ Restart (destroy out.root)")
    ("continue,c",po::value<int>(),
     "-c [more events] Continue from out.root");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  
  if ( vm.count("help") || argc == 1) {
    std::cout << desc << std::endl;
    return 0;
  }
  else if ( vm.count("restart") ) {
    
    auto the_file = std::make_shared<TFile>("output/out.root","RECREATE");
    auto the_tree = std::make_shared<TTree>("outtree","physics");
    
    LArgon b(vm["restart"].as<std::vector<double> >()[0],
	     vm["restart"].as<std::vector<double> >()[1],
	     vm["restart"].as<std::vector<double> >()[2],
	     vm["restart"].as<std::vector<double> >()[3],
	     vm["restart"].as<std::vector<double> >()[4]);
    

    the_tree->Branch("LArgon",&b);
    

    b.monte();
    
    //b.evolve();

    the_tree->Fill();
    the_tree->Write();
    the_file->Close();

    //Explicit exit ensures ROOT doesn't hang the program.
    std::exit(0);
  
  }
  
  else if (vm.count("continue")) { // Not supported for monte carlo
    
    LArgon *b = new LArgon();
    
    auto in_file = std::make_shared<TFile>("output/out.root","READ");
    auto in_tree  = dynamic_cast<TTree*>(in_file->Get("outtree"));
    
    in_tree->SetBranchAddress("LArgon",&b);
    in_tree->GetEntry(0); // There should be only one entry
    
    b->evolve(vm["continue"].as<int>());

    auto out_file = std::make_shared<TFile>("output/out.root","RECREATE");
    auto out_tree = std::make_shared<TTree>("outtree","physics");
    
    out_tree->Branch("LArgon",&b);
    
    out_tree->Fill();
    out_tree->Write();

    out_file->Close();
    in_file->Close();

    std::exit(0);


  }

    return 0;
 
}
