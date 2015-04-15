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
    print "./bob_viewer.py [filename]"
    sys.exit()
else:
    datfile = sys.argv[1]
    

file = open(datfile)

count = 0
c = 0

for line in file:
    tt.SetPoint(count,(2.2-.1*float(count)),-1.0/(float(line.strip())))
    count += 1;
    cc = .1;

t = tgs([tt],"1/|T-T_c| Expected")


t[0].Draw("AL")

t[0].GetYaxis().SetTitle("a(T)")
t[0].GetXaxis().SetTitle("T")
t[0].GetYaxis().CenterTitle()
t[0].GetXaxis().CenterTitle()

t[2].Draw()

c1.Update()
c1.Modified()
