#!/usr/bin/python -i

from ROOT import TCanvas, TH2D

from looks import *

import sys
import time

looks_minos()
file = open("the_spins.dat")


c1   = TCanvas()

l = 50


th2  = TH2D("","",l,1,l+1,l,1,l+1)
th2.Draw("COLZ")

x = 0
y = 0

N = l * l

th2.GetZaxis().SetRangeUser(-1,1)
th2.GetZaxis().SetNdivisions(2)

th2.GetXaxis().SetTitle("Sx")
th2.GetYaxis().SetTitle("Sy")

th2.GetXaxis().CenterTitle()
th2.GetYaxis().CenterTitle()

for line in file:
    spins = line.split(' ')
    th2.Reset()
    for x in xrange(N):
        if ( x%l == 0 ): y += 1
        th2.Fill(x%l+1,y,int(spins[x]))
        
    c1.Update()
    c1.Modified()
    time.sleep(0.1)
    x = 0
    y = 0
    
