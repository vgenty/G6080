#include "LArgon.h"

#include "TFile.h"
#include "TTree.h"

int main() {

  auto the_file = std::make_shared<TFile>("output/out.root","RECREATE");
  auto the_tree = std::make_shared<TTree>("outtree","physics");


  LArgon b(500,500,0.35,1.0,1.069);
  
  the_tree->Branch("LArgon",&b);

  b.evolve(1);

  
  the_tree->Fill();
  the_tree->Write();
  the_file->Close();
  
  return 0;
 
}
