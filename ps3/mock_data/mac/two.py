import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

def main():
    looks_minos();

    a = Analyzer("raw")
    a.a()
    a.b()
    
    samples = [a.content()[i].samples() for i in xrange(len(a.content()))]

    means1k  = []
    fun1k    = []
    
    #means10k = []
    tt = []
    
    for i in xrange(5):
        for j in xrange(len(samples[0][1000])) :
            tt.append(samples[i][1000][j].first) #first is mean, second is stdev in N sample
        
        means1k.append(tt)
        tt = []
        
    for j in xrange(len(samples[0][1000])):
        fun1k.append([f1(means1k[0][j],means1k[1][j]),
                      f2(means1k[2][j],means1k[3][j]),
                      f3(means1k[0][j],means1k[1][j],means1k[2][j],means1k[3][j],means1k[4][j])])

    fun1karray = n.array(fun1k)
        # Calculate the standard deviations of the sample means
    for i in xrange(5):
        print "File v%d: 1k ~ %f 10k ~ %f ratio ~ %f" % (i,n.std(means1k[i],ddof=1),1.0,n.std(means1k[i])/1.0)

    print ""
    # Calculate the standard deviations of the f's
    print "sf1 ~ %f sf2 ~ %f sf3 ~ %f" % (n.std(fun1karray[:,0],ddof=1),n.std(fun1karray[:,1],ddof=1),n.std(fun1karray[:,2],ddof=1))

def f1(v1,v2):
    return v1/v2

def f2(v3,v4):
    return n.exp(v3-v4)

def f3(v1,v2,v3,v4,v5):
    return (v1/v2 + v3/v4)*n.log(v5)
    
if __name__ == '__main__':
    main()
