#!/usr/bin/python -i

import subprocess32 as sp
import numpy as np
import os, sys
import ROOT
from ROOT import TCanvas, TGraph, gStyle
from looks import *
from methods import *



file = open("the_spins.dat")


x=0
y = 0

N=16
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
                  
            
        fs.append(f/(4*NN))
        f = 0.0
    #Average the final 100 values?
    avgs.append(np.mean(fs[len(fs)-100:len(fs)]))
         
c1 = TCanvas()
tg = TGraph()


for i in xrange(len(avgs)) :
    tg.SetPoint(i,i,avgs[i])


tg.SetMarkerStyle(6)
tg.Fit("expo")
gStyle.SetOptFit(1111)
tg.Draw("AL")

