from ROOT import gStyle, TCanvas, TGraph, TAxis, TGraph, gROOT, TH1D
from looks import *

def Plotter(data3,data0,n,n_loops):
    looks()
    maxx = 5
    
    if(n == 2): 
        n_loops = n_loops*2+20
    
    if(n == 7):
        maxx = 10
    if(n == 5 or n ==6):
        maxx = 50
        maxx = 50
        
    th3 = TH1D("O3", ";;", maxx*100,0,maxx)
    th0 = TH1D("O0", ";;", maxx*100,0,maxx)
    
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

    
    
    th0.Draw()
    th3.Draw("SAMES")
    

    c1.Update()
    c1.Modified()
    raw_input('')

    
