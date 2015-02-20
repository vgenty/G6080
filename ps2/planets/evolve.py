#!/usr/bin/python
import numpy as np
from ROOT import TCanvas, TAxis, TGraph, TMultiGraph
import planet

def main():
    nsteps = 500
    
    NP  = 3                # num planets
    
    p = planet.Planet(nsteps,NP)
    
    p.evolve()
    tg1 = TGraph()
    tg2 = TGraph()
    tmg = TMultiGraph()
    
    tmg.Add(tg1)
    tmg.Add(tg2)
    
    for i in xrange(len(p.r)):
        tg1.SetPoint(i,p.r[i][0][0],p.r[i][0][1])
        tg2.SetPoint(i,p.r[i][1][0],p.r[i][1][1])
 
    tg1.SetLineColor(3)
    tg2.SetLineColor(2)
    tmg.Draw("AL")
    raw_input('')

    # print "==r=="
    # print p.r
    # print "==v=="
    # print p.v
    # print "==fr=="
    # print p.fr
    # print "==fv=="
    # print p.fv
if __name__ == '__main__':
    main()
