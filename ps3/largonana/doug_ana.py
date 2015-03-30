import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TLegend, TPaveText
from ROOT import TTree, TFile, gStyle
from looks import *
h = 1
ROOT.gSystem.Load('libLArgon')
from ROOT import LArgon

import sys,math
import numpy as n

infile = TFile("infile/out_10000.root","READ")
intree = infile.Get('outtree')

intree.GetEntry(0) #only one entry get first entry
b = intree.LArgon    
looks_minos()

print b

PE = n.array(b.PE())
T  = n.array(b.T())
VI = n.array(b.P())

varrs = [PE,T,VI]
j = 0
#Negative covariance - variable is inversely related!!!!!

means = [n.mean(varrs[i]) for i in xrange(len(varrs))]
names = ["Pot Energy","Temp","Virial"]
corrs = []
corr = 0.0
CORR = []

tg = [TGraph() for i in xrange(len(varrs))]
tc = [TCanvas() for i in xrange(len(varrs))]
title  = [TPaveText(0.549569,0.8241525,0.950431,0.8983051,"nbNDC") for t in xrange(3)]
    
for k in xrange(len(varrs)):
    corrs = []
    for nn in xrange(0,250):
        corr = 0.0
        for i in xrange(len(varrs[k])-nn):
            corr += (varrs[k][i+nn] - means[k])*(varrs[k][i] - means[k])
        corrs.append(corr/(len(varrs[k])-nn))


    tc[k].cd()
    for x in xrange(len(corrs)):
        tg[k].SetPoint(x,x,corrs[x]/corrs[0])

    tg[k].GetXaxis().SetTitle("n")
    tg[k].GetYaxis().SetTitle("C_{%s}(n)/C_{%s}(0)" % (names[k],names[k]))
    tg[k].GetXaxis().CenterTitle()
    tg[k].GetYaxis().CenterTitle()
    title[k].SetFillColor(0);
    title[k].SetFillStyle(0);
    title[k].SetLineColor(0);
    title[k].AddText("%s" % (names[k]))
    tg[k].SetLineWidth(2)
    tg[k].Draw("AL")
    title[k].Draw()
    tc[k].Update()
    tc[k].Modified()
    CORR.append(corrs)
    
##Finished autocorrelation
##Now correlation matrix

# for(int i = 0; i < 5; ++i){
#   for(int j = 0; j < 5; ++j) {
#     for(int k = 0; k < NUMVALS; ++k) {
#       _crosscorr[i][j] +=
#         (_content[i]->data()[k] - _content[i]->mean()) *
#         (_content[j]->data()[k] - _content[j]->mean());
#     }
#     //Normalize...
#     _crosscorr[i][j] /= (NUMVALS*_content[i]->stdev()*_content[j]->stdev());
#   }
# }


crosscorr = n.zeros((3,3))

for i in xrange(3):
    for j in xrange(3):
        for k in xrange(len(varrs[0])):
            crosscorr[i][j] += (varrs[i][k] - means[i])*(varrs[j][k] - means[j])
        crosscorr[i][j] /= (len(varrs[0]) * n.std(varrs[i],ddof=1))* n.std(varrs[j],ddof=1)


cc = TCanvas()

gStyle.SetPalette(55)
th2 = TH2D("basedgod","",3,1,4,3,1,4)

for x in xrange(3):
    for y in xrange(3):
        v = crosscorr[x][y]
        #print "file: %d with file: %d is %f" % (x,y,v)
        th2.SetBinContent(x+1,y+1,v)
        
th2.GetXaxis().SetTitle("")
th2.GetYaxis().SetTitle("")
for i in xrange(3):
    th2.GetXaxis().SetBinLabel(i+1,names[i])
    th2.GetYaxis().SetBinLabel(i+1,names[i])
    
th2.GetZaxis().SetRangeUser(-1.0,1.0)
th2.GetXaxis().CenterTitle()
th2.GetYaxis().CenterTitle()
    
th2.Draw("COLZ")
cc.Update()
cc.Modified()


#Negative covariance - variable is inversely related!!!!!


#Jackknife of Liquid argon virial theorem

if( j == h):
    for f in xrange(0,1):

    #for b in xrange(2,1000) :
        vprime = []
        before = 0

        for b in xrange(80,81): #bin size = 125??
            
            pp = []
            zz = 0.0
            Ndivb = int(n.floor(float(howmany)/b))
        
            if Ndivb == before:
                continue
    
            before = Ndivb
            bb = 0.0
    
            #Do averaging inside each of the bins
        
            #Bin data
        
            binss = rebin(b,datasamples[f][0])
            bins = binss[0]
                    
            for k in xrange(len(bins)): #b is the number of jackknife bins
                zz = 0.0
                for i in xrange(len(bins)):
                    if i != k: #never ever use is not to compare numbers idiot
                        zz += bins[i]

                    vkp = zz/(len(bins)-1.0)
                    pp.append(vkp)
        
                vprime.append([pp,binss[1]])
                
        gg.append(vprime)



    delP = rho * k * T - rho/(6*N)*vir

    print "Jackkknife of pressure"
    print delP

raw_input('')
