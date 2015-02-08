//Builtin includes
#include <iostream>
#include <memory>
#include <vector>
#include <map>

//ROOT Framework
#include "TApplication.h"
#include "TCanvas.h"
#include "TAxis.h"

//Local includes
#include "Recursion.h"
#include "Plotter.h"


//  An example of the propagation of errors in the trigonometric
//  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

int main(int argc, char *argv[]) {
  
  Recursion *r = new Recursion();
  Plotter *p = new Plotter();
  
  std::map<double,double> ptsInitialStep;
  std::map<double,double>::iterator itrInitialStep;

  std::map<double,double> ptsReducedStep;
  std::map<double,double>::iterator itrReducedStep;
  
  for(int i = 2; i<=8; ++i) {
    ptsInitialStep[i] = r->isvalue(pow(10,i));
    ptsReducedStep[i] = r->rsvalue(pow(10,i));
  }
  
   for(auto& p : ptsInitialStep) 
     std::cout << p.first << " " << p.second<< std::endl;

  // for(auto& p : ptsReducedStep) 
  //   std::cout << p.second<< std::endl;
  
  //Doing hardocred plots like this is idiocy
   TApplication tapp("tapp",&argc,argv);
   TCanvas *c1  = new TCanvas();
   TCanvas *c11 = new TCanvas();
   TCanvas *c2  = new TCanvas();
   TCanvas *c3  = new TCanvas();
   
   c1->cd();
   c1->SetLogx();
   (p->plot(ptsInitialStep))->Draw("AL");
  
   c1->Modified();
   c1->Update();
  
   c11->cd();
   c11->SetLogx();
   (p->plot(ptsReducedStep))->Draw("AL");
  
   c11->Modified();
   c11->Update();

   c2->cd();
   c2->SetLogx();
   c2->SetLogy();
  
   //so I need an l value to do this i'm so confused
   auto err = p->errorC(ptsInitialStep);
   err->GetYaxis()->SetRangeUser(pow(10,-14),pow(10,2));
   (err)->Draw("AL");
  
   c2->Modified();
   c2->Update();
  
   c3->cd();
    c3->SetLogy();
    c3->SetLogx();
    
    auto err2 = p->errorC(ptsReducedStep);
    err2->GetYaxis()->SetRangeUser(pow(10,-14),pow(10,2));
    (err2)->Draw("AL");
    
    c3->Modified();
    c3->Update();

   tapp.Run();
  
   return 0;
  
}
