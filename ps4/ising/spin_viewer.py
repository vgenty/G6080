#!/usr/bin/python -i

from ROOT import TCanvas, TH2D

from looks import *

import sys
import time

looks_minos()
file = open("the_spins.dat")


c1   = TCanvas()

th2  = TH2D("","",16,1,17,16,1,17)
th2.Draw("COLZ")

x = 0
y = 0

N = 256

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
        if ( x%16 == 0 ): y += 1
        th2.Fill(x%16+1,y,int(spins[x]))
        
    c1.Update()
    c1.Modified()
    time.sleep(0.01)
    x = 0
    y = 0
    
