#!/usr/bin/python -i

import subprocess32 as sp
import numpy as np
import os, sys
import ROOT
from ROOT import TCanvas, TGraph
from looks import *
from methods import *

temps = np.linspace(0,5.0,1000,endpoint=False)

for t in temps:
    sp.call(["./cluster",str(t),"0.00", "1000"])


dats = [ j for j in os.listdir(".") if j.startswith("cmdat") and j.endswith(".dat")]

dats = sorted(dats)

files = [open(d) for d in dats]

n   = 0
z   = 0
avg = 0.0

mags = []

print "Scanning through files"

for f in files:
    for l in f:
        n += 1
        if( n >= 900):
            z += 1
            l.strip()
            avg += float(l)
    mags.append(avg/float(z))
    z   = 0
    avg = 0
    n   = 0

looks_minos()

c1 = TCanvas()
tg = TGraph()
for x in xrange(len(mags)):
    tg.SetPoint(x,temps[x],mags[x])


tg.SetMarkerStyle(6)
tg.Draw("AP")
c1.Update()
c1.Modified()
