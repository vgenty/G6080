#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('../lib/libLArgon')

# Call largon constructor, if you change second two values 
# immediately go to _restart() function

b = ROOT.LArgon(int(150),int(1000),float(0.75),float(1.069))

outfile = ROOT.TFile('out.root','recreate')
outtree = ROOT.TTree('outtree','physics')

outtree.Branch("LArgon",b);

b.evolve(1)

outtree.Fill()
outtree.Write()
outfile.Close()


