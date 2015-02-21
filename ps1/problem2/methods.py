from ROOT import gStyle, TCanvas, TGraph, TAxis
from ROOT import TGraph, gROOT, TH1D, TLegend, TPaveText
from looks import *

def Plotter(data3,data0,n,n_loops):
    looks_minos()
    #looks()
    #looks_atlas()
    #looks_CmsTDR()
    maxx = 5
    
    if(n == 2): 
        n_loops = n_loops*2+20
    
    if(n == 7):
        maxx = 10
    if(n == 5 or n ==6):
        maxx = 50
        
    if(n == 8) :
        n_loops = 1
        maxx = 100
        
    th3 = TH1D("gcc -O3", ";;", maxx*100,0,maxx)
    th0 = TH1D("gcc -O0", ";;", maxx*100,0,maxx)
    
    for k in xrange(len(data3)):
        th3.Fill(float(data3[k][n])/float(n_loops))
        th0.Fill(float(data0[k][n])/float(n_loops))

    c1 = TCanvas()
    
    th3.SetFillColor(4)
    th3.SetFillStyle(3345)
    #th3.SetTitle(";Time [ns];Count")
    
    th0.SetFillColor(2)
    th0.SetFillStyle(3354)
    th0.SetTitle(";Time [ns];Count")
    th0.GetXaxis().CenterTitle()
    th0.GetYaxis().CenterTitle()

    
    title = TPaveText(0.30,.91,.70,1,"nbNDC")
    title.SetFillColor(0);
    title.SetFillStyle(0);
    title.SetLineColor(0);
    title.AddText(getTitle(n))
    
    
    tl = TLegend(0.6,0.4,0.9,0.8)
    tl.AddEntry(th0,th0.GetName(),"f")
    tl.AddEntry(0,"Mean: %0.2f ns" % th0.GetMean(),"")
    tl.AddEntry(th3,th3.GetName(),"f")
    tl.AddEntry(0,"Mean: %0.2f ns" % th3.GetMean(),"")
    th0.Draw()
    th3.Draw("SAMES")
    tl.Draw()
    title.Draw()
    
   
    c1.Update()
    c1.Modified()
    raw_input('')

    
def getTitle(n):
    if  ( n == 0 ) :
        return "c[i] = a[i] * b[i]"
    elif( n == 1 ) :
        return "c[i] += a[i] * b[i]"
    elif( n == 2 ) :
        return "c[i] += a[i]*b[2*i + 20]"
    elif( n == 3 ) :
        return "c[i] = a[i] / b[i]"
    elif( n == 4 ) :
        return "c[i] += a[i] / b[i]"
    elif( n == 5 ) :
        return "c[i] = std::sin(a[i])"
    elif( n == 6 ) :
        return "c[i] = std::exp(a[i])"
    elif( n == 7 ) :
        return "c[i] = std::sqrt(a[i])"
    elif( n == 8 ) :
        return "Stop and start clock"

