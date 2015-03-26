import pdb
import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

def rebin(num_bins,data_set) :
    num_each_bin = int(n.floor(float(len(data_set))/float(num_bins)))

    bb   = 0.0
    bins = []
    
    for j in xrange(num_bins):

        for k in xrange(num_each_bin*j,
                        num_each_bin*(j+1)): # i'm always loosing the remainder but does it matter?
            bb += data_set[k]
            #bb.append(data_set[k])

        bins.append(bb/num_each_bin)
        bb = 0.0
        
    #pdb.set_trace()
    return bins
    
def f1(v1,v2):
    return v1/v2

def sf1(v1,v2,s1,s2):
    return n.sqrt(1/v2**2 * s1**2 + v1**2/v2**4 * s2**2)

def sf2(v3,v4,s3,s4):
    return n.exp(v3-v4)*n.sqrt(s3**2 + s4**2)

def f2(v3,v4):
    return n.exp(v3-v4)

def f3(v1,v2,v3,v4,v5):
    return (v1/v2 + v3/v4)*n.log(v5)

def sf3(v1,v2,v3,v4,v5,s1,s2,s3,s4,s5):
    return n.sqrt(n.log(v5)**2 * (v1**2 * s2**2 / v2**4 +
                                  s1**2 / v2**2 +
                                  s4**2 * v3**2 / v4**4 +
                                  s3**2 / v4**2 )
                  +
                  s5**2/(v2**2 * v4**2 * v5**2) * (v1*v4 +
                                                   v2*v3)**2
    )


looks_minos();

a = Analyzer("raw")
a.a()
a.b()
    
samples     = [a.content()[i].samples() for i in xrange(len(a.content()))]
#datasamples = [a.content()[i].datasamples() for i in xrange(len(a.content()))]

datasamples = []
datafile    = []
vals        = []

howmany = 10000

lll = int(len(a.content()[0].data()))
for i in xrange(5):
    datafile = []
    for j in xrange(lll):
        if((j%howmany == 0 and j > 0) or j == (lll-1)):
            datafile.append(vals)
            vals = []
            break
        vals.append(a.content()[i].data()[j])
            
    datasamples.append(datafile)

print len(datasamples)
print len(datasamples[0][0])
print len(datasamples[1][0])
print len(datasamples[2][0])
print len(datasamples[3][0])
print len(datasamples[4][0])

means1k  = []
fun1k    = []

means10k = []
tt = []
uu = []
print "Len samples[0][1000]"

print len(samples[0][1000])
for i in xrange(5):
    for j in xrange(len(samples[0][1000])) :
        tt.append(samples[i][1000][j].first) #first is mean, second is stdev in N sample
                   
    means1k.append(tt)
    tt = []

for i in xrange(5):
    for j in xrange(len(samples[0][10000])) :
        uu.append(samples[i][10000][j].first) #first is mean, second is stdev in N sample
                   
    means10k.append(uu)
    uu = []

    # for j in xrange(len(samples[0][1000])):
    #     print "from C++: %f, from python: %f" % (means1k[0][j],n.mean(datasamples[0][j]))
    
for j in xrange(len(samples[0][1000])):
    fun1k.append([f1(means1k[0][j],means1k[1][j]),
                  f2(means1k[2][j],means1k[3][j]),
                  f3(means1k[0][j],means1k[1][j],means1k[2][j],means1k[3][j],means1k[4][j])])

fun1karray = n.array(fun1k)
    # Calculate the standard deviations of the sample means
for i in xrange(5):
    print "File v%d: 1k ~ %f 10k ~ %f ratio ~ %f" % (i,n.std(means1k[i],ddof=1),n.std(means10k[i],ddof=1),n.std(means1k[i])/n.std(means10k[i],ddof=1))


print "Sigma f1 from n.std"
    # Calculate the standard deviations of the f's
print "sf1 ~ %f" % n.std(fun1karray[:,0],ddof=1)
print "sf2 ~ %f" % n.std(fun1karray[:,1],ddof=1)
print "sf3 ~ %f" % n.std(fun1karray[:,2],ddof=1)
print ""
print "Sigma from native prop no autocorr"
print "sf1 ~ %f " % (sf1(n.mean(means1k[0]),
                         n.mean(means1k[1]),
                         n.std(means1k[0],ddof=1),
                         n.std(means1k[1],ddof=1)))
print "sf2 ~ %f" %  (sf2(n.mean(means1k[2]),
                         n.mean(means1k[3]),
                         n.std(means1k[2],ddof=1),
                         n.std(means1k[3],ddof=1)))
print "sf3 ~ %f" %  (sf3(n.mean(means1k[0]),
                         n.mean(means1k[1]),
                         n.mean(means1k[2]),
                         n.mean(means1k[3]),
                         n.mean(means1k[4]),
                         n.std(means1k[0],ddof=1),
                         n.std(means1k[1],ddof=1),
                         n.std(means1k[2],ddof=1),
                         n.std(means1k[3],ddof=1),
                         n.std(means1k[4],ddof=1)))

print ""
print "Jackknife calculation of mean"
print ""
    
vprime = []
pp     = []
zz     = 0.0
bb = 0.0
ssigma = []
ss = 0.0
before = 0;
gg = []
for f in xrange(3,4):
    #for b in xrange(2,1000) :
    #vprime = []
    #before = 0
    for b in xrange(2,1000) : #b == 10
        
        pp = []
        zz = 0.0
    
        Ndivb = int(n.floor(float(howmany)/b))

        if Ndivb == before:
            continue
    
        before = Ndivb
        bb = 0.0

    
        #Do averaging inside each of the bins
        
        #Bin data
    
        bins = rebin(b,datasamples[f][0])

        #pdb.set_trace()
        for k in xrange(b): #b is the number of jackknife bins
            zz = 0.0
            for i in xrange(b):
                if i is not k:
                    zz += bins[i]

            vkp = zz/(b-1)
            pp.append(vkp)
        
        vprime.append(pp)
    

    gg.append(vprime)
    
# Try and do stdev

# stdd1 = 0.0
# stdd2 = 0.0
# stdd3 = 0.0

# mean0 = n.mean(gg[0][0])
# mean1 = n.mean(gg[1][0])
# mean2 = n.mean(gg[2][0])
# mean3 = n.mean(gg[3][0])
# mean4 = n.mean(gg[4][0])

# for i in xrange(5):
#     stdd1 += (f1(gg[0][0][i],gg[1][0][i]) - f1(mean0,mean1))**2
#     stdd2 += (f2(gg[2][0][i],gg[3][0][i]) - f2(mean2,mean3))**2
#     stdd3 += (f3(gg[0][0][i],gg[1][0][i],
#                  gg[2][0][i],gg[3][0][i],
#                  gg[4][0][i])             - f3(mean0,mean1,
#                                                mean2,mean3,
#                                                mean4))**2

# stdd1  = n.sqrt(4.0/5.0*stdd1)
# stdd2  = n.sqrt(4.0/5.0*stdd2)
# stdd3  = n.sqrt(4.0/5.0*stdd3)

# print stdd1
# print stdd2
# print stdd3

tg = TGraph()
for x in xrange(0,len(vprime)):
    mean   = n.mean(vprime[x])
    noverb = float(howmany)/len(vprime[x])
    #tg.SetPoint(x,noverb,mean)
    tg.SetPoint(x,noverb,n.sqrt(n.var(vprime[x])*(len(vprime[x])-1.0)))
    
tg.SetMarkerStyle(20)
tg.Draw("AP")
#raw_input("")
