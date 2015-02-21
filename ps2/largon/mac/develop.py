#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('../lib/libLArgon')

#Call largon constructor
b = ROOT.LArgon(int(100),int(1000),float(1.0))

b.evolve(1)
b.evolve(0)
