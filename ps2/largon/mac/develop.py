#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('../lib/libLArgon')

# Call largon constructor, if you change second two values 
# immediately go to _restart() function

b = ROOT.LArgon(int(100),int(1000),float(1.0))

b.evolve(1)
#b.evolve(0)
tg = TGraph()


for i in xrange(len(b.KE())) :
    tg.SetPoint(i,i,b.KE()[i])

tg.Draw("AL")

raw_input('')
