#!/usr/bin/python

import numpy as np
from looks import *
from ROOT import TCanvas, TAxis, TGraph, TMultiGraph, TLatex
from ROOT import TLegend
import planet


def main():
    nsteps = 200000
    
    NP  = 3                # num planets
    
    p = planet.Planet(nsteps,NP)
    
    p.evolve()


    looks_minos()
    
    c1 = TCanvas()
    c1.cd()
    
    tg1 = TGraph()
    tg2 = TGraph()
    tg3 = TGraph()
    tmg = TMultiGraph()
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
    

    tmg.Draw("A")

    tmg.GetYaxis().CenterTitle()
    tmg.GetXaxis().CenterTitle()
    tmg.GetYaxis().SetTitle("y/r_{earth}")
    tmg.GetXaxis().SetTitle("x/r_{earth}")
    
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

    tl = TLegend(0.5,0.3,0.6,0.8,"Energy","nbNDC")
    tl.AddEntry(tke,"Kinetic","l")
    tl.AddEntry(tpe,"Potential","l")
    tl.AddEntry(te,"Total","l")
    
    for i in xrange(len(p.KEtot)):
        tke.SetPoint(i,i,p.KEtot[i])
        tpe.SetPoint(i,i,p.PEtot[i])
        te.SetPoint(i, i,p.Etot[i] )

    tke.SetLineColor(3)
    tpe.SetLineColor(2)
    te.SetLineColor(1)

    tke.SetLineWidth(2)
    tpe.SetLineWidth(2)
    te.SetLineWidth(3)
    
    tme.Draw("AL")
    tme.GetYaxis().SetTitle("Energy")
    tme.GetYaxis().CenterTitle()
    tme.GetXaxis().SetTitle("Time")
    tme.GetXaxis().CenterTitle()
    tl.Draw()
  
    c2.Update()
    c2.Modified()
    
    
    print len(p.KEtot)
    
    
    c3 = TCanvas()
    c3.cd()

    tesum      = TGraph() # instantaneous sum of KE and PE
    teperabove  = TGraph() # percentage above
    teperbelow  = TGraph() # percentage below
    tmall = TMultiGraph()

    tmall.Add(tesum,"L")
    tmall.Add(teperabove,"L")
    tmall.Add(teperbelow,"L")

    tll = TLegend(0.5,0.3,0.6,0.8,"","nbNDC")
    tll.AddEntry(tesum,"Energy Sum","l")
    tll.AddEntry(teperabove,"0.01 upper","l")
    tll.AddEntry(teperbelow,"0.01 lower","l")

    for i in xrange(len(p.KEtot)) :
        tesum.SetPoint(i,i,p.Etot[i])
        teperabove.SetPoint(i,i,p.Etot[0]*1.01)
        teperbelow.SetPoint(i,i,p.Etot[0]*0.99)

    tesum.SetLineColor(4)
    teperbelow.SetLineColor(1)
    teperabove.SetLineColor(2)
    tesum.SetLineWidth(2)
    teperbelow.SetLineWidth(2)
    teperabove.SetLineWidth(2)

    tmall.Draw("A")
    tmall.GetYaxis().CenterTitle()
    tmall.GetXaxis().CenterTitle()
    tmall.GetYaxis().SetTitle("Energy")
    tmall.GetXaxis().SetTitle("Time")
    tll.Draw()
    c3.Update()
    c3.Modified()
    
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
