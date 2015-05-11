#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
from looks import   *
from methods import *
import subprocess32 as sp
import sys
#from root_numpy import root2array, root2rec, tree2rec

looks_minos()

tfile = TFile(sys.argv[1],"READ")
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
    for o in xrange(len(e.y)):
        tg.SetPoint(c,e.x[o],e.y[o])
        c+=1
    tg.Draw("AL")
    tg.GetYaxis().SetRangeUser(0,20.0)
    tg.GetXaxis().SetTitle("x at t=%d" % t)
    tg.GetYaxis().SetTitle("|#psi|^{2}")
    tg.GetXaxis().CenterTitle()
    tg.GetYaxis().CenterTitle()
    c1.Update()
    c1.Modified()

    raw_input('')
    c = 0

# for i in xrange(len(rec['y'])):
#     for j in xrange(len(rec['y'][0])):
#         tg.SetPoint(c,rec['x'][i][j],rec['y'][i][j]);
#         print "x: %f y: %f" % (rec['x'][i][j],rec['y'][i][j])
#         c += 1

#     tg.Draw("AP")
#     raw_input('') 
#     c = 0
