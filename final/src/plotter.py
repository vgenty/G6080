#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
import numpy as np
#from looks import   *
#from methods import *
from root_numpy import root2array, root2rec, tree2rec

#looks_minos()

tfile = TFile("outfile.root","READ")
ttree = tfile.Get("data")

rec = tree2rec(ttree)

# for e in ttree:
#     for o in xrange(len(e.y)):
#         k.append(e.y[o])
#         x.append(e.x[o])
c = 0

gStyle.SetPalette(1);
c1 = TCanvas("c1","c1",800,600)
c1.cd()

tg = TGraph()
tg.SetTitle("")

for i in xrange(len(rec['y'])):
    for j in xrange(len(rec['y'][0])):
        tg.SetPoint(c,rec['x'][i][j],rec['y'][i][j]);
        print "x: %f y: %f" % (rec['x'][i][j],rec['y'][i][j])
        c += 1

    tg.Draw("AP")
    raw_input('') 
    c = 0
