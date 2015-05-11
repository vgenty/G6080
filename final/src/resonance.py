#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
from looks import   *
from methods import *
import os,sys
import subprocess32 as sb
import multiprocessing

looks_minos()
jobs = []

def worker(l):
    sb.call(["./ev",str(l),"hey_%s.root" % l, "0.1"])

spacing = np.linspace(3.0, 5.0, num=50)
for l in spacing:
    print "Calling E=%s*V0" %l
    #worker(l)
    p = multiprocessing.Process(target=worker,args=(l,))
    jobs.append(p)
    p.start()
    
for j in jobs:
    j.join()

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
            tgR.SetPoint(pp,spacing[pp],1.0-e.psi2res)
            tgT.SetPoint(pp,spacing[pp],e.psi2res)
            print "%f %f" % (spacing[pp],e.psi2res)
            break
        c+=1
    pp+=1

tgR.SetName("Reflection")
tgT.SetName("Transmission")

zz = tgs([tgR,tgT],"V=odd, #sigma=0.05")
zz[0].Draw("AL")
zz[1].Draw()
zz[2].Draw()

setaxis(zz[0],"E/V","Coefficient")


c1.Update()
c1.Modified()



