import pdb
import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TMultiGraph
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

looks_minos();

a = Analyzer("raw")

a.a()
a.b()

gg1k  = []
gg10k = []

tg1k  = TGraph()
tg10k = TGraph()

for i in xrange(1,999,50):
    a.three(i,int(1000))
    j = n.array(a.samples_tint()[0])
    s = n.std(j,ddof=1)
    gg1k.append(s)
    
for i in xrange(1,999,50):
    a.three(i,int(10000))
    j = n.array(a.samples_tint()[0])
    s = n.std(j,ddof=1)
    gg10k.append(s)


for i in xrange(len(gg1k)):
    tg1k.SetPoint(i,50*i+1,gg1k[i])
    tg10k.SetPoint(i,50*i+1,gg10k[i])

tmg = TMultiGraph()
tg1k.SetLineWidth(2)
tg10k.SetLineWidth(2)

tmg.Add(tg1k)
tmg.Add(tg10k)

c1 = TCanvas()
c1.cd()

tmg.Draw("AL")

tmg.GetXaxis().SetTitle("n")
tmg.GetYaxis().SetTitle("sigma")
tmg.GetXaxis().CenterTitle()
tmg.GetYaxis().CenterTitle()

c1.Update()
c1.Modified()



