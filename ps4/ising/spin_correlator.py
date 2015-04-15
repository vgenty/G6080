#!/usr/bin/python -i

import subprocess32 as sp
import numpy as np
import os, sys
import ROOT
from ROOT import TCanvas, TGraph, gStyle, TF1, TLatex,TPaveText
from looks import *
from methods import *



file = open("the_spins.dat")


x=0
y = 0

N=50
NN = N*N

ss = []

for line in file:
    sp = np.zeros((N,N))
    spins = line.split(' ')
    
    for x in xrange(N):
        if ( x%N == 0 ): y += 1
        sp[x%N][y-1] = int(spins[x])

    x = 0
    y = 0
    ss.append(sp)


avgs = []

for n in xrange(0,9):
        
    f = 0.0
    fs = []

    for s in ss:
        for x in xrange(N):
            for y in xrange(N):
                q = (x+n)%N
                w = (y+n)%N
                e = (x-n)%N
                r = (y-n)%N
                
                f += (s[x][y]*s[q][y] +
                      s[x][y]*s[e][y] +
                      s[x][y]*s[x][w] +
                      s[x][y]*s[x][r] )
                  
            
        fs.append(f/NN)
        f = 0.0
    #Average the final 100 values?
    avgs.append(np.mean(fs[len(fs)-100:len(fs)]))
         
c1 = TCanvas()
tg = TGraph()


for i in xrange(len(avgs)) :
    tg.SetPoint(i,i,avgs[i])


looks_minos()

tf = TF1("tf","[0]*exp([1]*x)+[2]",0,10)
title =  TPaveText(0.5014368,0.7478814,0.9022989,0.8177966,"nbNDC");
title.SetFillColor(0);
title.SetFillStyle(0);
title.SetLineColor(0);

title.AddText("p0 exp(p1*n) + p2");


tg.SetMarkerStyle(6)
tg.Fit("tf")
gStyle.SetOptFit(1111)
tg.SetLineWidth(2)
tg.Draw("AL")
title.Draw("SAMES");

tg.GetYaxis().SetTitle("<#sigma_{x} #sigma_{y}>")
tg.GetXaxis().SetTitle("n")
tg.GetYaxis().CenterTitle()
tg.GetXaxis().CenterTitle()


c1.Update()
c1.Modified()
