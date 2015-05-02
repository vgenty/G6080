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
    print "./extrapolate.py [filename]"
    sys.exit()
else:
    datfile = sys.argv[1]
    

file = open(datfile)

count = 0

for line in file:
    line = line.split()
    tt.SetPoint(count,float(line[0]),float(line[1]))
    count += 1;

t = tgs([tt],"Extrapolation")


t[0].Draw("AP")

t[0].GetYaxis().SetTitle("V(x,y)")
t[0].GetXaxis().SetTitle("a")
t[0].GetYaxis().CenterTitle()
t[0].GetXaxis().CenterTitle()

t[2].Draw()

c1.Update()
c1.Modified()
