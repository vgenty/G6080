import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

def main():
    looks_minos();

    a = Analyzer("raw")

    ######## Question 1 ########
    
    #Part A
    a.a()
    
    #Part B
    a.b()
    
    samples = [a.content()[i].samples() for i in xrange(len(a.content()))]

        
    cans_B = [TCanvas() for t in xrange(5)]
    b1k    = [TH1D("N1k_set_%d" % (t+1),"",100,0,10) for t in xrange(5)]
    b10k   = [TH1D("N10k_set_%d" % (t+1),"",100,0,10) for t in xrange(5)]


    means1k  = []
    means10k = []
    tt = []
    
    for i in xrange(5):
        
        for j in xrange(len(samples[0][1000])) :
            tt.append(samples[i][1000 ][j].first)
            b1k[i ].Fill(samples[i][1000 ][j].first)

        means1k.append(tt)
        tt = []
        
        for j in xrange(len(samples[0][10000])) :
            tt.append(samples[i][10000][j].first)
            b10k[i].Fill(samples[i][10000][j].first)
        
        means10k.append(tt)
        tt = []
        
        cans_B[i].cd()

        b1k[i].GetXaxis().SetTitle("Sample Mean")
        b1k[i].GetYaxis().SetTitle("Count")
        b1k[i].GetXaxis().CenterTitle()
        b1k[i].GetYaxis().CenterTitle()

        b1k[i].SetFillColor(4)
        b1k[i].SetFillStyle(3345)
        b10k[i].SetFillColor(2)
        b10k[i].SetFillStyle(3354)

        b1k[i].Draw()
        b10k[i].Draw("SAMES")
        
        
        # Calculate the standard deviations of the sample means
    for i in xrange(5):
        print "File v%d: 1k ~ %f 10k ~ %f ratio ~ %f" % (i,n.std(means1k[i]),n.std(means10k[i]),n.std(means1k[i])/n.std(means10k[i]))




    ###### Part C (3.) ######
    nn = int(300)
    a.c(nn)

    tcc = [TCanvas() for i in xrange(5)]
    tgc = [TGraph() for i in xrange(5)]

    corrs = [a.content()[i].correlation() for i in xrange(len(a.content()))]

    for x in xrange(5):
        tcc[x].cd()
        for j in xrange(len(corrs[x])):
           tgc[x].SetPoint(j,j,corrs[x][j]/corrs[x][0])
    
        tgc[x].Draw("AL")
        tcc[x].Update()
        tcc[x].Modified()



    ###### Part D (4.) ######
    a.d()

    for x in xrange(5):
        print "File v%d: integrated corr ~ %f " % (x,a.content()[x].integrated_correlation())
    
    
    raw_input('')

    


if __name__ == '__main__':
    main()
