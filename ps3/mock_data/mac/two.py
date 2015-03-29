import pdb
import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D, TPaveText
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

#bins = rebin(b,datasamples[f][0])
#this is the worst way to do the jackknife never do this again


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

# this is now deprecated  I think :)
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
    
for j in xrange(len(samples[0][10000])):
    fun1k.append([f1(means10k[0][j],means10k[1][j]),
                  f2(means10k[2][j],means10k[3][j]),
                  f3(means10k[0][j],means10k[1][j],means10k[2][j],means10k[3][j],means10k[4][j])])

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
print "sf1 ~ %f " % (sf1(n.mean(means10k[0]),
                         n.mean(means10k[1]),
                         n.std(means10k[0],ddof=1),
                         n.std(means10k[1],ddof=1)))
print "sf2 ~ %f" %  (sf2(n.mean(means10k[2]),
                         n.mean(means10k[3]),
                         n.std(means10k[2],ddof=1),
                         n.std(means10k[3],ddof=1)))
print "sf3 ~ %f" %  (sf3(n.mean(means10k[0]),
                         n.mean(means10k[1]),
                         n.mean(means10k[2]),
                         n.mean(means10k[3]),
                         n.mean(means10k[4]),
                         n.std(means10k[0],ddof=1),
                         n.std(means10k[1],ddof=1),
                         n.std(means10k[2],ddof=1),
                         n.std(means10k[3],ddof=1),
                         n.std(means10k[4],ddof=1)))

print ""
print "Jackknife calculation of mean"
print ""
    
vprime = []
pp     = []
zz     = 0.0
bb     = 0.0
ssigma = []
ss     = 0.0
before = 0;
gg     = []

for f in xrange(0,5):

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
    
# Try and do stdev

stdd1 = 0.0
stdd2 = 0.0
stdd3 = 0.0

mean0 = n.mean(gg[0][0][0])
mean1 = n.mean(gg[1][0][0])
mean2 = n.mean(gg[2][0][0])
mean3 = n.mean(gg[3][0][0])
mean4 = n.mean(gg[4][0][0])

ss0 = n.sqrt(n.var(gg[0][0][0])*(len(gg[0][0][0]) - 1))
ss1 = n.sqrt(n.var(gg[1][0][0])*(len(gg[1][0][0]) - 1))
ss2 = n.sqrt(n.var(gg[2][0][0])*(len(gg[2][0][0]) - 1))
ss3 = n.sqrt(n.var(gg[3][0][0])*(len(gg[3][0][0]) - 1))
ss4 = n.sqrt(n.var(gg[4][0][0])*(len(gg[4][0][0]) - 1)) 

print "Means"
print mean0
print mean1
print mean2
print mean3
print mean4

print "STDs"
print ss0
print ss1
print ss2
print ss3
print ss4
print ""
for i in xrange(len(gg[0][0][0])):
    stdd1 += (f1(gg[0][0][0][i],gg[1][0][0][i]) - f1(mean0,mean1))**2
    stdd2 += (f2(gg[2][0][0][i],gg[3][0][0][i]) - f2(mean2,mean3))**2
    stdd3 += (f3(gg[0][0][0][i],gg[1][0][0][i],
                 gg[2][0][0][i],gg[3][0][0][i],
                 gg[4][0][0][i])             - f3(mean0,mean1,
                                               mean2,mean3,
                                               mean4))**2

stdd1  = n.sqrt(79.0/80.0*stdd1)
stdd2  = n.sqrt(79.0/80.0*stdd2)
stdd3  = n.sqrt(79.0/80.0*stdd3)

print stdd1
print stdd2
print stdd3

# c1 = TCanvas()
# c1.cd()
# tg  = TGraph()
# title = TPaveText(0.549569,0.8241525,0.950431,0.8983051,"nbNDC")


# for x in xrange(0,len(vprime)):
#     mean   = n.mean(vprime[x][0])
#     #noverb = n.floor(float(howmany)/len(vprime[x]))
#     #tg.SetPoint(x,noverb,mean)
#     tg.SetPoint(x,vprime[x][1],n.sqrt(n.var(vprime[x][0])*(len(vprime[x][0]) - 1)))

# tg.SetLineWidth(2)
# tg.GetYaxis().SetTitle("#hat{#sigma}_{jack}")
# tg.GetXaxis().SetTitle("b - Bin size")
# tg.GetYaxis().SetTitleOffset(1.10)
# tg.GetXaxis().CenterTitle()
# tg.GetYaxis().CenterTitle()

# tg.SetMarkerStyle(20)
# tg.Draw("AL")
# title.SetFillColor(0);
# title.SetFillStyle(0);
# title.SetLineColor(0);
# title.AddText("File v5")
# title.Draw()

# c1.Update()
# c1.Modified()
#tg.Draw("AP")
#raw_input("")
