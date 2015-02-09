//Builtin includes
#include <iostream>
#include <memory>
#include <vector>
#include <map>

//ROOT Framework
#include "TApplication.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TPaveText.h"

//Local includes
#include "Recursion.h"
#include "Plotter.h"
#include "looks.h"

//  An example of the propagation of errors in the trigonometric
//  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

int main(int argc, char *argv[]) {
  
  Recursion *r = new Recursion();
  Plotter *p = new Plotter();
  
  std::map<double,double> pts;
  std::map<double,double>::iterator itr;
  
  for(int i = 2; i<=8; ++i)
    pts[i] = r->value(pow(10,i));
  

  auto style = looks();
  style->cd();
  //Doing hardocred plots like this is idiocy
  TApplication tapp("tapp",&argc,argv);
  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  TCanvas *c3 = new TCanvas();

  c1->cd();
  c1->SetLogx();
  auto pp = p->plot(pts);
  (pp)->Draw("AL");
 
  pp->SetTitle(";N;cos(Nx)");
  pp->GetYaxis()->CenterTitle();
  pp->GetXaxis()->CenterTitle();
 
 
  c1->Modified();
  c1->Update();

  c2->cd();
  c2->SetLogx();
  c2->SetLogy();
  
  //so I need an l value to do this i'm so confused
  auto err = p->error();
  err->GetYaxis()->SetRangeUser(pow(10,-14),pow(10,0));
  (err)->Draw("AL");
  
  err->SetTitle(";N;Nsin(Nx)/sin(x) #epsilon");
  err->GetYaxis()->CenterTitle();
  err->GetXaxis()->CenterTitle();
  c2->Modified();
  c2->Update();
  
  c3->cd();
  //  c3->SetLogy();
  c3->SetLogx();
  
  auto ratt = p->ratio(pts);
  //  ratt->GetYaxis()->SetRangeUser(pow(10,1),pow(10,15));
  ratt->Draw("AL");
  
  ratt->SetTitle(";N;Ratio");
  ratt->GetYaxis()->CenterTitle();
  ratt->GetXaxis()->CenterTitle();
  TPaveText *tp = new TPaveText(0.5676568,0.2893401,0.8844884,0.5634518,"nbNDC");
  tp->AddText("Ratio = #left|#frac{Nsin(Nx)/sin(x)}{#Delta cos(Nx)} #epsilon #right|");
  tp->SetFillColor(0);
  tp->Draw();
  
  c3->Modified();
  c3->Update();

  tapp.Run();
  
  return 0;
  
}
