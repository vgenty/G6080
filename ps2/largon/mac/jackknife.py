import pdb
import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TPaveText
from looks import *
ROOT.gSystem.Load('../lib/libLArgon')
from ROOT import LArgon

import numpy as n

def rebin(num_bins,data_set) :
    num_each_bin = int(n.floor(float(len(data_set))/float(num_bins)))
    remainder = len(data_set) - num_each_bin*num_bins
    
    bb   = 0.0
    bins = []
    for j in xrange(num_bins):

        for k in xrange(num_each_bin*j,
                        num_each_bin*(j+1)): # i'm always losing the remainder but does it matter?
            bb += data_set[k]
            #bb.append(data_set[k])

        bins.append(bb/num_each_bin)
        bb = 0.0


    #bin the remainder??
    if remainder > 5:
        newbins = int(n.floor(remainder/num_each_bin))
        offset = (len(data_set)-remainder-1)
        for j in xrange(newbins):
            for i in xrange(offset + num_each_bin*j,
                            offset + num_each_bin*(j+1)):
                bb += data_set[i]
            
            bins.append(bb/num_each_bin)
            bb = 0.0

        remainder = len(data_set) - num_each_bin*num_bins - newbins*num_each_bin
        
        print "nbins %f neach %f remainder %f " % (num_bins,num_each_bin,remainder)
    
    #pdb.set_trace()
    return [bins,num_each_bin]


print ""
print "Jackknife calculation of mean"
print ""


infile = ROOT.TFile('../output/out.root','read')
intree = infile.Get('outtree')

intree.GetEntry(0) #only one entry get first entry
bb = intree.LArgon    
looks_minos()


potentialE = []

for i in xrange(len(bb.PE())):
    if i >= 2000:
        #potentialE.append(bb.PE()[i]/864.0)
        potentialE.append(1-1/(6*864*1.069)*bb.P()[i])

print len(potentialE)

vprime = []
pp     = []
zz     = 0.0
bb     = 0.0
ssigma = []
ss     = 0.0
before = 0;
gg     = []

#for b in xrange(2,1000) :
vprime = []
before = 0
howmany = 8000

for b in xrange(2,8000): #bin size = 125??
        
    pp = []
    zz = 0.0
    Ndivb = int(n.floor(float(howmany)/b))
        
    if Ndivb == before:
        continue
    
    before = Ndivb
    bb = 0.0
    
    #Do averaging inside each of the bins
    
    #Bin data

    binss = rebin(b,potentialE)
    #pdb.set_trace()
    bins = binss[0]
                    
    for k in xrange(len(bins)): #b is the number of jackknife bins
        zz = 0.0
        for i in xrange(len(bins)):
            if i != k: #never ever use is not to compare numbers idiot
                zz += bins[i]

        vkp = zz/(len(bins)-1.0)
        pp.append(vkp)
        
    vprime.append([pp,binss[1]])






c1 = TCanvas()
c1.cd()
tg  = TGraph()


for x in xrange(0,len(vprime)):
    mean   = n.mean(vprime[x][0])
    tg.SetPoint(x,vprime[x][1],n.sqrt(n.var(vprime[x][0])*(len(vprime[x][0]) - 1)))
    print str(x) + " " + str(vprime[x][1]) + " " + str(n.sqrt(n.var(vprime[x][0])*(len(vprime[x][0]) - 1)))
    #tg.SetPoint(x,vprime[x][1],mean)

tg.SetLineWidth(2)
title= TPaveText(0.549569,0.8241525,0.950431,0.8983051,"nbNDC")
title.SetFillColor(0);
title.SetFillStyle(0);
title.SetLineColor(0);
title.AddText("Pot. E.")

tg.GetYaxis().SetTitle("#hat{#sigma}_{jack}")
tg.GetXaxis().SetTitle("b - Bin size")
tg.GetYaxis().SetTitleOffset(1.10)
tg.GetXaxis().CenterTitle()
tg.GetYaxis().CenterTitle()

tg.Draw("AL")
