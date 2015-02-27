#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('../lib/libLArgon')

# Call largon constructor, if you change second two values 
# immediately go to _restart() function

# constructor is nsteps, nparticles, density, initial temp, desired final temp
b = ROOT.LArgon(int(500),int(500),float(0.35),float(1.0),float(1.069))
#b = ROOT.LArgon(int(5000),int(27),float(.9063),float(1.069),float(1.069))

outfile = ROOT.TFile('out.root','recreate')
outtree = ROOT.TTree('outtree','physics')

outtree.Branch("LArgon",b);

b.evolve(1)

outtree.Fill()
outtree.Write()
outfile.Close()

ROOT.gSystem.Unload('../lib/libLArgon')
