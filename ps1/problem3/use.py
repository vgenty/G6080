#!/usr/bin/python

import ROOT
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend
from looks import *
import sys,math
ROOT.gSystem.Load('libBessel')

b = ROOT.Bessel()

tgs = [TGraph() for x in xrange(1,11)]

c1 = TCanvas()
for x in xrange(1,11):
    for i in xrange(2,100):
        r = b.recursion(float(x),i)
        m = b.value(2,float(x))
        tgs[x-1].SetPoint(i-2,i,abs(r-m))
        

tmg = TMultiGraph()
co = 1

looks()

#tgs[0].SetTitle(";Recursion Steps;#left|I^{std::boost::math}_{2} - I^{recursion}_{2}#right|")

for t in tgs:
    t.GetYaxis().SetRangeUser(pow(10,-16),pow(10,4))
    t.GetXaxis().SetRangeUser(0,50)
    t.SetLineColor(co)
    tmg.Add(t)
    co+=1
tgs[9].SetLineColor(11)

tl = TLegend(0.5,0.3,0.6,0.8,"x value", "nbNDC")

for k in xrange(1,11):
    tl.AddEntry(tgs[k-1],"x = %d" % k,"l")

c1.SetLogy()
tmg.Draw("AL")
tmg.GetYaxis().SetRangeUser(pow(10,-16),pow(10,4))
tmg.GetXaxis().SetRangeUser(0,50)

tmg.GetXaxis().SetTitle("Recursion Steps")
tmg.GetYaxis().SetTitle("#left|I^{boost::math}_{2}(x) - I^{recursion}_{2}(x)#right|")
tmg.GetXaxis().CenterTitle()
tmg.GetYaxis().CenterTitle()
tl.Draw()
c1.Update()
c1.Modified()

raw_input('')
