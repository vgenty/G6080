#!/usr/bin/python -i

from ROOT import    * #import it all who gives a hoot
#from looks import   *
#from methods import *


#looks_minos()

tfile = TFile("outfile.root","READ")
ttree = tfile.Get("data")

k = []

for e in ttree:
    for o in xrange(len(e.y)):
        k.append(e.y[o])

c = 0

gStyle.SetPalette(1);
c1 = TCanvas()
tg2d = TGraph2D()

p = 101.0

for i in xrange(int(p)):
    for j in xrange(int(p)):
        tg2d.SetPoint(c,i/p,j/p,k[c])
        c += 1

tg2d.SetTitle("")
tg2d.Draw("SURF1")
#tg2d.Draw("P")
#tg2d.Draw("CONTZ")
c1.Update()
c1.Modified()

