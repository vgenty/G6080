#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
import os,sys
files  = [x for x in os.listdir(".") if x.startswith("hey") and x.endswith(".root")]

tfiles = [TFile.Open("%s" % f,"READ") for f in files]
ttrees = [k.Get("data") for k in tfiles]

c1 = TCanvas()
c1.cd()

tg = TGraph()
tg.SetLineWidth(2)
l=0
c = 0

time = 175

for tree in ttrees:
    c = 0
    for e in tree:
        if(c == 174):
            tg.SetPoint(l,float(l+1)/10.0,e.ref)
            print "%f %f" % (float(l+1)/10.0,e.ref)
            break
        c+=1
    l+=1

tg.Draw("AL")
tg.GetXaxis().SetTitle("V/E")
tg.GetYaxis().SetTitle("Reflection")
tg.GetXaxis().CenterTitle()
tg.GetYaxis().CenterTitle()
c1.Update()
c1.Modified()



