#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('../lib/libLArgon')

# Call largon constructor, if you change second two values 
# immediately go to _restart() function

b = ROOT.LArgon(int(100),int(1000),float(1.0))

outfile = ROOT.TFile('out.root','recreate')
outtree = ROOT.TTree('outtree','physics')

outtree.Branch("LArgon",b);

b.evolve(1)
c1 = TCanvas()
c1.cd()
tg1 = TGraph() 
tg2 = TGraph()
tm  = TMultiGraph()

tm.Add(tg1,"L")
tm.Add(tg2,"L")

print len(b.KE())

for i in xrange(len(b.KE())) :
    tg1.SetPoint(i,i,b.KE()[i])
    tg2.SetPoint(i,i,b.PE()[i])
    print "TOTAL E = %f, energy now = %f, diff = %f" % (b.PE()[0], b.PE()[i]+b.KE()[i], b.PE()[0] - (b.PE()[i]+b.KE()[i]))

tm.Draw("A");
#tg1.Draw("AL")
outtree.Fill()

outtree.Write()
outfile.Close()

c1.Update()
c1.Modified()

raw_input('')
