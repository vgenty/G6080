#!/usr/bin/python -i

from ROOT import TCanvas, TGraph, TLatex

import numpy as np

import sys

from methods import *
from looks   import *

looks_minos()
tt = TGraph()

c1 = TCanvas()
try:
    sys.argv[1]
except IndexError:
    print "./mag_viewer.py [filename]"
    sys.exit()
else:
    datfile = sys.argv[1]
    

file = open(datfile)

count = 0

for line in file:
    tt.SetPoint(count,count,float(line.strip()))
    count += 1;

t = tgs([tt],"B = 5#times10^{-5}")


t[0].Draw("AL")

t[0].GetYaxis().SetTitle("Magnetization")
t[0].GetXaxis().SetTitle("Step")
t[0].GetYaxis().CenterTitle()
t[0].GetXaxis().CenterTitle()

t[2].Draw()

c1.Update()
c1.Modified()
