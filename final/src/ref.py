#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np

tfile = TFile("outfile.root","READ")
ttree = tfile.Get("data")

c1 = TCanvas()
c1.cd()

tg = TGraph()
tg.SetLineWidth(2)
c = 0

for e in ttree:
    tg.SetPoint(c,c,e.ref)
    c+=1

tg.Draw("AL")
tg.GetXaxis().SetTitle("time")
tg.GetYaxis().SetTitle("Reflection")
tg.GetXaxis().CenterTitle()
tg.GetYaxis().CenterTitle()
c1.Update()
c1.Modified()



