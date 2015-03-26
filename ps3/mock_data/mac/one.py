import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TLegend, TPaveText
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
    tl     = [TLegend(0.6,0.4,0.9,0.8) for t in xrange(5)]
    title  = [TPaveText(0.549569,0.8241525,0.950431,0.8983051,"nbNDC") for t in xrange(5)]
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

        title[i].SetFillColor(0);
        title[i].SetFillStyle(0);
        title[i].SetLineColor(0);
        title[i].AddText("File v%d" % (i+1))
                             
        
        tl[i].AddEntry(b1k[i],"1k","f")
        tl[i].AddEntry(0,"Mean: %0.2f" % b1k[i].GetMean()      ,"")
        tl[i].AddEntry(0,"STD : %0.2f" % b1k[i].GetRMS(),"")
        tl[i].AddEntry(b10k[i],"10k","f")
        tl[i].AddEntry(0,"Mean: %0.2f" % b10k[i].GetMean(),"")
        tl[i].AddEntry(0,"STD : %0.2f" % b10k[i].GetRMS(),"")
        
        b1k[i].Draw()
        b10k[i].Draw("SAMES")
        tl[i].Draw()
        title[i].Draw()
        
        # Calculate the standard deviations of the sample means
    for i in xrange(5):
        print "File v%d: 1k ~ %f 10k ~ %f ratio ~ %f" % (i,n.std(means1k[i],ddof=1),n.std(means10k[i],ddof=1),n.std(means1k[i])/n.std(means10k[i]))




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
    
        tgc[x].GetXaxis().SetTitle("n")
        tgc[x].GetYaxis().SetTitle("C_{v%i}(n)/C_{v%i}(0)" % (x+1,x+1))

        tgc[x].GetXaxis().CenterTitle()
        tgc[x].GetYaxis().CenterTitle()

        tgc[x].SetLineWidth(2)
        tgc[x].Draw("AL")
        title[x].Draw()

        tcc[x].Update()
        tcc[x].Modified()



    ###### Part D (4.) ######
    a.d()

    for x in xrange(5):
        print "File v%d: integrated corr ~ %f " % (x,a.content()[x].integrated_correlation())
    


    ##### Part E (5.) #####
    a.e()
    cc = TCanvas()
    cc.cd()
    
    gStyle.SetPalette(55)
    th2 = TH2D("basedgod","",5,1,6,5,1,6)

    for x in xrange(5):
        for y in xrange(5):
             v = a.crosscorrelation()[x][y]
             print "file: %d with file: %d is %f" % (x,y,v)
             th2.SetBinContent(x+1,y+1,v)

    th2.Draw("COLZ")
    cc.Update()
    cc.Modified()
    
    raw_input('')
    
    
    
    
if __name__ == '__main__':
    main()
