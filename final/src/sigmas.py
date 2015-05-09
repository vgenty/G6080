#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
from looks import   *
from methods import *
import os,sys
import subprocess32 as sb

spacing = np.linspace(0.1, 0.5, num=50)
looks_minos()

for l in spacing:
    print "Calling sigma=%s" %l
    sb.call(["./ev",str(1.25),"hey_%s.root" % l, str(l)])


files  = [x for x in os.listdir(".") if x.startswith("hey") and x.endswith(".root")]

tfiles = [TFile.Open("%s" % f,"READ") for f in files]
ttrees = [k.Get("data") for k in tfiles]

c1 = TCanvas()
c1.cd()

tgR = TGraph()
tgT = TGraph()

tgTRUER= TGraph()
tgTRUET= TGraph()

tgR.SetLineWidth(2)
tgT.SetLineWidth(2)
tgTRUER.SetLineWidth(2)
tgTRUET.SetLineWidth(2)

pp = 0
c  = 0

time = 300

for tree in ttrees:
    c = 0
    for e in tree:
        if(c == time):
            tgR.SetPoint(pp,spacing[pp],e.ref)
            tgT.SetPoint(pp,spacing[pp],1.0-e.ref)
            tgTRUER.SetPoint(pp,spacing[pp],0.14589803375031546)
            tgTRUET.SetPoint(pp,spacing[pp],1-0.14589803375031546)
            print "%f %f" % (spacing[pp],e.ref)
            break
        c+=1
    pp+=1

tgR.SetName("Reflection")
tgT.SetName("Transmission")
tgTRUET.SetName("k=#sqrt{2E} Transmission")
tgTRUER.SetName("k=#sqrt{2E} Reflection")

zz = tgs([tgR,tgT,tgTRUET,tgTRUER],"V=1500, E=1.25V")
zz[0].Draw("AL")
zz[1].Draw()
zz[2].Draw()

setaxis(zz[0],"#sigma","Coefficient")


c1.Update()
c1.Modified()



