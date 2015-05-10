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
    tg.SetPoint(c,c,e.psi2inside)
    c+=1

tg.Draw("AL")
tg.GetYaxis().SetRangeUser(0,1.0)
tg.GetXaxis().SetTitle("t")
tg.GetYaxis().SetTitle("#int dx |#psi|^{2}")
tg.GetXaxis().CenterTitle()
tg.GetYaxis().CenterTitle()

tit = TPaveText(0.2988506,0.934322,0.6997126,1,"nbNDC")
tit.SetFillColor(0)
tit.SetFillStyle(0)
tit.SetLineColor(0)
tit.AddText("f(t) = a + b exp(c t)")
tit.Draw()

tf = TF1("tf","[0] + [1]*exp([2]*x)",50,700)

tf.SetParNames("a","b","c")
tg.Fit("tf")

gStyle.SetOptFit(1111)

c1.Update()
c1.Modified()

raw_input('')

