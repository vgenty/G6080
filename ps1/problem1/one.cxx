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
  
  std::map<double,double> pts;
  std::map<double,double>::iterator itr;
  
  for(int i = 2; i<=8; ++i)
    pts[i] = r->value(pow(10,i));
  

  
  //Doing hardocred plots like this is idiocy
  TApplication tapp("tapp",&argc,argv);
  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  TCanvas *c3 = new TCanvas();

  c1->cd();
  c1->SetLogx();
  auto pp = p->plot(pts);
  (pp)->Draw("AL");
  
  c1->Modified();
  c1->Update();

  c2->cd();
  c2->SetLogx();
  c2->SetLogy();
  
  //so I need an l value to do this i'm so confused
  auto err = p->error();
  err->GetYaxis()->SetRangeUser(pow(10,1),pow(10,15));
  (err)->Draw("AL");
  
  c2->Modified();
  c2->Update();
  
  c3->cd();
  c3->SetLogy();
  c3->SetLogx();
  
  auto ratt = p->ratio(pts);
  ratt->GetYaxis()->SetRangeUser(pow(10,1),pow(10,15));
  ratt->Draw("AL");
 
  c3->Modified();
  c3->Update();

  tapp.Run();
  
  return 0;
  
}
