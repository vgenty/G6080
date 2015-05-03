#!/usr/bin/python -i

from ROOT import *
from looks import   *
from methods import *
import sys

looks_minos()

gStyle.SetPalette(1);
c1 = TCanvas()
tg2d = TGraph2D()
k = []

try:
    sys.argv[1]
except IndexError:
    print "./surfer.py [filename]"
    sys.exit()
else:
    datfile = sys.argv[1]

file = open(datfile)

for line in file:
    line = line.split()
    k.append(float(line[0]))


p = 101.0
c = 0
for i in xrange(int(p)):
    for j in xrange(int(p)):
        tg2d.SetPoint(c,i/p,j/p,k[c]*k[c])
        c += 1

tg2d.SetTitle(";;")
tg2d.GetXaxis().SetTitle("x")
tg2d.GetYaxis().SetTitle("y")
tg2d.GetYaxis().CenterTitle()
tg2d.GetXaxis().CenterTitle()

tg2d.Draw("SURF1")
#tg2d.Draw("P")
#tg2d.Draw("CONTZ")

c1.Update()
c1.Modified()
