import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TLegend, TPaveText
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

looks_minos();

a = Analyzer("raw")

######## Question 1 ########

#Part A
a.a()

#Part B
a.b()
sampz1k    = [a.content()[i].datasamples1k()[2]  for i in xrange(len(a.content()))]
sampz10k   = [a.content()[i].datasamples10k()[2] for i in xrange(len(a.content()))]

#cans_B = [TCanvas() for t in xrange(5)]
#tl     = [TLegend(0.6,0.4,0.9,0.8) for t in xrange(5)]
#title  = [TPaveText(0.549569,0.8241525,0.950431,0.8983051,"nbNDC") for t in xrange(5)]
#b1k    = [TH1D("N1k_set_%d" % (t+1),"",100,0,10) for t in xrange(5)]
#b10k   = [TH1D("N10k_set_%d" % (t+1),"",100,0,10) for t in xrange(5)]

###### Part C (3.) ######
nn = int(300)
#a.c(nn)
    
tcc = [TCanvas() for i in xrange(2)]
tgc = [TGraph() for i in xrange(2)]
    
k = 0.0

CC = []
CCC = []
DDD = []


for d in xrange(5):
    mean0 = n.mean(sampz1k[d])
    mean1 = n.mean(sampz10k[d])
    for j in xrange(2):
        for n_ in xrange(80):
            if ( j == 0 ):
                for i in xrange(len(sampz1k[d])-n_):
                    k += ( sampz1k[d][i + n_] - mean0 ) * ( sampz1k[d][i] - mean0 )
                CC.append(k/float(1000.0-n_))
                k = 0.0
            else:
                for i in xrange(len(sampz10k[0])-n_):
                    k += ( sampz10k[d][i + n_] - mean1 ) * ( sampz10k[d][i] - mean1 )
                CC.append(k/float(10000 - n_))
                k = 0.0
        CCC.append(CC)
        CC = []
    DDD.append(CCC)
    CCC = []
KKK = []
kk = 0.0
g  = 0.0

ppp = []
for v in DDD: #for each file : - )
    for u in v : #1000 or 10000
        for b in u:
            g += b
        ppp.append(g/u[0])
        g = 0.0
    KKK.append(ppp)
    ppp = []


#calculation of tauint and sigma!!


sss  = [n.sqrt(2*KKK[i][0]/1000.0)*n.std(sampz1k[i],ddof=1) for i in xrange(5)]
ssss = [n.sqrt(2*KKK[i][0]/10000.0)*n.std(sampz10k[i],ddof=1) for i in xrange(5)]


# for x in xrange(2):
#     tcc[x].cd()
#     for j in xrange(len(CCC[x])):
#         tgc[x].SetPoint(j,j,CCC[x][j]/CCC[x][0])
    
#     tgc[x].GetXaxis().SetTitle("n")
#     tgc[x].GetYaxis().SetTitle("C_{N=%s}(n)/C_(0)" % str(10**(x+3)))

#     tgc[x].GetXaxis().CenterTitle()
#     tgc[x].GetYaxis().CenterTitle()

#     tgc[x].SetLineWidth(2)
#     tgc[x].Draw("AL")
#     #title[x].Draw()

#     tcc[x].Update()
#     tcc[x].Modified()



    # ###### Part D (4.) ######
    # a.d()

    # for x in xrange(5):
    #     print "File v%d: integrated corr ~ %f " % (x,a.content()[x].integrated_correlation())
    


     ##### Part E (5.) #####
     #a.e()

crosscorrelation= n.zeros((5,5))

tt = 0.0
r  = 10000

for i in xrange(5):
    for j in xrange(5):

        one = n.mean(sampz10k[i])
        two = n.mean(sampz10k[j])
        
        for y in xrange(r):
            tt += (sampz10k[i][y] - one)*(sampz10k[j][y] - two)

        crosscorrelation[i][j] = tt/(1000.0 * n.std(sampz10k[i],ddof=1) * n.std(sampz10k[j],ddof=1))
        tt = 0.0
            
cc = TCanvas()
cc.cd()

cc2 = TCanvas()
cc.cd()
gStyle.SetPalette(55)
th2 = TH2D("basod","",5,1,6,5,1,6)

for x in xrange(5):
    for y in xrange(5):
        v = crosscorrelation[x][y]
        print "file: %d with file: %d is %f" % (x,y,v)
        th2.SetBinContent(x+1,y+1,v)

th2.GetXaxis().SetTitle("Data v_a (N = 10000)")
th2.GetYaxis().SetTitle("Data v_a (N = 10000)")
th2.GetXaxis().CenterTitle()
th2.GetYaxis().CenterTitle()

th2.Draw("COLZ")
cc.Update()
cc.Modified()
cc2.cd()
th2.Draw("SURF1")
cc2.Update()

cc2.Modified()
# raw_input('')
