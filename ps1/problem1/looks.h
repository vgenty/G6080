#include "TROOT.h"
#include "RConfig.h"
#include "TStyle.h"

TStyle* looks() {
  TStyle* vstyle = new TStyle();
  //gROOT->ProcessLine("TGaxis::SetMaxDigits(3)");
  vstyle->SetFrameBorderMode(0);
  vstyle->SetFrameFillColor(0);
  vstyle->SetCanvasBorderMode(0);
  vstyle->SetCanvasColor(0);
  vstyle->SetPadBorderMode(0);
  vstyle->SetPadColor(0);
  vstyle->SetStatColor(0);
  vstyle->SetTitleFont(42,"");
  vstyle->SetLabelFont(42,"x");
  vstyle->SetTitleFont(42,"x");
  vstyle->SetLabelFont(42,"y");
  vstyle->SetTitleFont(42,"y");
  vstyle->SetLabelFont(42,"z");
  vstyle->SetTitleFont(42,"z");
  vstyle->SetLabelSize(0.048,"x");
  vstyle->SetTitleSize(0.048,"x");
  vstyle->SetLabelSize(0.048,"y");
  vstyle->SetTitleSize(0.048,"y");
  vstyle->SetLabelSize(0.048,"z");
  vstyle->SetTitleSize(0.048,"z");
  vstyle->SetTitleSize(0.048,"");
  vstyle->SetTitleAlign(23);
  vstyle->SetTitleX(0.5);
  vstyle->SetTitleBorderSize(0);
  vstyle->SetTitleFillColor(0);
  vstyle->SetTitleStyle(0);
  vstyle->SetOptStat("emr");
  vstyle->SetOptStat(0);
  vstyle->SetStatBorderSize(0);
  vstyle->SetStatFont(42);
  vstyle->SetStatFontSize(0.048);
  vstyle->SetStatY(0.9);
  vstyle->SetStatX(0.86);
  vstyle->SetHistLineColor(kBlue+2);
  vstyle->SetLegendBorderSize(0);
  vstyle->SetLegendFillColor(0);
  vstyle->SetFuncWidth(2);
  vstyle->SetFuncColor(2);
  vstyle->SetPadTopMargin(0.08);
  vstyle->SetPadBottomMargin(0.12);
  vstyle->SetPadLeftMargin(0.12);
  vstyle->SetPadRightMargin(0.06);  
  vstyle->SetCanvasDefX(400);
  vstyle->SetCanvasDefY(20);
  vstyle->SetCanvasDefH(420);
  vstyle->SetCanvasDefW(610);
  vstyle->SetFrameBorderMode(0);
  vstyle->SetFrameLineWidth(2);
  vstyle->SetHistLineWidth(2);
  vstyle->SetTitleOffset(1.16,"y");
  vstyle->SetTitleOffset(1.20,"x");
  return vstyle;
}
