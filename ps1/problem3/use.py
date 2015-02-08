#!/usr/bin/python

import ROOT
from ROOT import TGraph
import sys,math
ROOT.gSystem.Load('libBessel')

b = ROOT.Bessel()

for i in xrange(2,200):
    r = b.recursion(5.0,i)
    m = b.value(2,5.0)
    print "i = %d : recursion: %0.16f  ~  boost::math: %0.15f" % (i,r,m)
    print "diff = %0.15f" % abs(r-m)
    print ""


    
