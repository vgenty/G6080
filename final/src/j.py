#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
from looks import   *
from methods import *
import os,sys
import subprocess32 as sb

spacing = np.linspace(0.1, 3.0, num=50)
looks_minos()

for l in spacing:
    print "Calling E=%s*V0" %l
    sb.call(["./ev",str(l),"hey_%s.root" % l, "0.05"])


files  = [x for x in os.listdir(".") if x.startswith("hey") and x.endswith(".root")]

tfiles = [TFile.Open("%s" % f,"READ") for f in files]
ttrees = [k.Get("data") for k in tfiles]

c1 = TCanvas()
c1.cd()

tgR = TGraph()
tgT = TGraph()
tgR.SetLineWidth(2)
tgT.SetLineWidth(2)

pp=0
c = 0

time = 180

for tree in ttrees:
    c = 0
    for e in tree:
        if(c == time):
            tgR.SetPoint(pp,spacing[pp],e.ref)
            tgT.SetPoint(pp,spacing[pp],1.0-e.ref)
            print "%f %f" % (spacing[pp],e.ref)
            break
        c+=1
    pp+=1

tgR.SetName("Reflection")
tgT.SetName("Transmission")

zz = tgs([tgR,tgT],"V=1500, #sigma=0.05")
zz[0].Draw("AL")
zz[1].Draw()
zz[2].Draw()

setaxis(zz[0],"E/V","Coefficient")


c1.Update()
c1.Modified()



