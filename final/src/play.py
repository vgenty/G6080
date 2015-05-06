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

c1 = TCanvas()
c1.cd()

tg = TGraph()
tg.SetTitle("")

c = 0

for e in ttree:
    for o in xrange(len(e.y)):
        tg.SetPoint(c,e.x[o],e.y[o])
        c+=1
    tg.Draw("AL")
    tg.GetYaxis().SetRangeUser(0,1.0)

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
