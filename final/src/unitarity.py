#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
from looks import   *
from methods import *

looks_minos()

tfile = TFile("outfile.root","READ")
ttree = tfile.Get("data")

#rec = tree2rec(ttree)

c1 = TCanvas()
c1.cd()

tg = TGraph()

tg.SetLineWidth(2)
c = 0
t = -1
for e in ttree:
    t+=1
    tg.SetPoint(c,c,e.psi2)
    c+=1

tg.Draw("AL")
tg.GetYaxis().SetRangeUser(0,1.1)
tg.GetXaxis().SetTitle("t")
tg.GetYaxis().SetTitle("#int dx |#psi|^{2}")
tg.GetXaxis().CenterTitle()
tg.GetYaxis().CenterTitle()
c1.Update()
c1.Modified()

raw_input('')

