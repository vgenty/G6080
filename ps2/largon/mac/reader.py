from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend, TH1D
from looks import *

import sys,math

ROOT.gSystem.Load('../lib/libLArgon')

from ROOT import LArgon

infile = ROOT.TFile('out.root','read')
intree = infile.Get('outtree')

intree.GetEntry(0) #only one entry get first entry

b = intree.LArgon

looks_minos()

# Ghetto way of looking at the output...

tR = [TH1D("X",";x pos;",100,0,10),
      TH1D("Y",";y pos;",100,0,10),
      TH1D("Z",";z pos;",100,0,10)]

vmin = -0.002
vmax = 0.002
tV = [TH1D("Vx",";x pos;",100,vmin,vmax),
      TH1D("Vy",";y pos;",100,vmin,vmax),
      TH1D("Vz",";z pos;",100,vmin,vmax)]


c = 0
for i in b.R():
    if(c == 0):
        for j in i:
            tR[0].Fill(j[0])
            tR[1].Fill(j[1])
            tR[2].Fill(j[2])
    c +=1
  
c = 0
for i in b.V():
    if(c == 0):
        for j in i:
            tV[0].Fill(j[0])
            tV[1].Fill(j[1])
            tV[2].Fill(j[2])
    c +=1
  
      
c1 = TCanvas()
c1.cd()    
tV[0].Draw()

c2 = TCanvas()
c2.cd()    
tV[1].Draw()

c3 = TCanvas()
c3.cd()    
tV[2].Draw()

raw_input('')

#for x in xrange(len(b._r
#intree.Branch("LArgon",b);
