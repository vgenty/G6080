#!/usr/bin/python

#ROOT includes
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend, TH1D, TGraph2D, TH2D, gStyle
#User includes
from looks import *
#Python includes
import sys,math

#Load the object
ROOT.gSystem.Load('../lib/libLArgon')
from ROOT import LArgon


def main():
    infile = ROOT.TFile('out.root','read')
    intree = infile.Get('outtree')
    
    intree.GetEntry(0) #only one entry get first entry
    
    b = intree.LArgon
    
    looks_minos()
    
    rr = initialR(b)
    vv = initialV(b,-5,5)
    tt = TT(b)

    c1 = TCanvas()
    c1.cd()    
    #rr[0][0].Draw()
    vv[0][0].Draw()
    
    c2 = TCanvas()
    c2.cd()    
    #rr[0][1].Draw()
    vv[0][1].Draw()

    c3 = TCanvas()
    c3.cd()    
    vv[0][2].Draw()
    

    
    c4 = TCanvas()
    c4.cd()
    e = E(b)
    e.Draw("A")
    e.SetTitle(";Step;KE/PE")
    c4.Update()
    c4.Modified()

    c5 = TCanvas()
    c5.cd()
    rr[1].SetMarkerStyle(20)
    rr[1].Draw("P")
    

    c6 = TCanvas()
    c6.cd()
    tt.Draw("AL")

    gStyle.SetPalette(55)

    c7 = TCanvas()
    c7.cd()
    vv[1].Draw("COLZ")
    

    raw_input('')
    raw_input('')
    
def initialR(b):

    tR = [TH1D("X",";x pos;",100,0,10),
          TH1D("Y",";y pos;",100,0,10),
          TH1D("Z",";z pos;",100,0,10)]

    tg2d = TGraph2D()
    c = 0
    o = 0
    for i in b.R():
        if(c == 0):
            for j in i:
                #print j
                x = j[0]
                y = j[1]
                z = j[2]
                tR[0].Fill(x)
                tR[1].Fill(y)
                tR[2].Fill(z)
                tg2d.SetPoint(o,x,y,z)
                o +=1
                #print "(%f,%f,%f)" % (x,y,z)
        c +=1
        
    
        
    
    
    return [tR,tg2d]

def initialV(b,vmin, vmax):

    tV2D = TH2D("2d","",150,0,150,100,vmin,vmax)

    tV = [TH1D("Vx",";Vx;",100,vmin,vmax),
          TH1D("Vy",";Vy;",100,vmin,vmax),
          TH1D("Vz",";Vz;",100,vmin,vmax)]

    c = 0
    for i in b.V():
        for j in i:
            tV2D.Fill(c,j[0])

        if(c == 99):
            for j in i:
                tV[0].Fill(j[0])
                tV[1].Fill(j[1])
                tV[2].Fill(j[2])
        c +=1
  
        
    return [tV,tV2D]

def E(b):
    
    tg1 = TGraph() 
    tg2 = TGraph()
    tm  = TMultiGraph()

    tm.Add(tg1,"L")
    tm.Add(tg2,"L")

    #print len(b.KE())
    
    for i in xrange(len(b.KE())) :
        tg1.SetPoint(i,i,b.KE()[i])
        tg2.SetPoint(i,i,b.PE()[i])
       #print "TOTAL E = %f, energy now = %f, diff = %f" % (b.PE()[0]+b.KE()[0], b.PE()[i]+b.KE()[i], (b.PE()[0]+b.KE()[0]) - (b.PE()[i]+b.KE()[i]))
    
    return tm

def TT(b):
    tgT = TGraph()
    
    for i in xrange(len(b.T())):
        tgT.SetPoint(i,i,b.T()[i])

    tgT.SetTitle(";step;T")
    return tgT

if __name__ == '__main__' :
    main()
    
