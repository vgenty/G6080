#!/usr/bin/python

import numpy as np
from looks import *
from ROOT import TCanvas, TAxis, TGraph, TMultiGraph, TLatex
import planet


def main():
    nsteps = 1000
    
    NP  = 3                # num planets
    
    p = planet.Planet(nsteps,NP)
    
    p.evolve()
    tg1 = TGraph()
    tg2 = TGraph()
    tg3 = TGraph()
    tmg = TMultiGraph()
    
    c1 = TCanvas()
    c1.cd()
    looks_minos()
    tmg.Add(tg1,"L")
    tmg.Add(tg2,"L")
    tmg.Add(tg3,"L")
    
    for i in xrange(len(p.r)):
        tg1.SetPoint(i,p.r[i][0][0],p.r[i][0][1])
        tg2.SetPoint(i,p.r[i][1][0],p.r[i][1][1])
        tg3.SetPoint(i,p.r[i][2][0],p.r[i][2][1])
        
    tg1.SetLineColor(3)
    tg2.SetLineColor(2)
    tg3.SetLineColor(1)
    
    tmg.SetTitle(";x/r_{earth};y/r_{earth}")
    tmg.Draw("A")
    tmg.GetYaxis().CenterTitle()
    tmg.GetXaxis().CenterTitle()
    
    c1.Update()
    c1.Modified()
    
    c2 = TCanvas()
    c2.cd()
    tke = TGraph()
    tpe = TGraph()
    te  = TGraph()
    tme = TMultiGraph()
    tme.Add(tke)
    tme.Add(tpe)
    tme.Add(te)
    
    for i in xrange(len(p.KEtot)):
        tke.SetPoint(i,i,p.KEtot[i])
        tpe.SetPoint(i,i,p.PEtot[i])
        te.SetPoint(i, i,p.Etot[i] )

    tke.SetLineColor(3)
    tpe.SetLineColor(2)
    te.SetLineColor(1)
    tme.Draw("AL")

    raw_input('')

    # print "==r=="
    # print p.r
    # print "==v=="
    # print p.v
    # print "==fr=="
    # print p.fr
    # print "==fv=="
    # print p.fv
    # print p.PEtot

if __name__ == '__main__':
    main()
