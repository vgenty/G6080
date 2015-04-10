from ROOT import TCanvas, TGraph

import numpy as np

from methods import *
from looks   import *

looks_minos()
tt = TGraph()

c1 = TCanvas()

file = open('cluster.dat')

count = 0

for line in file:
    tt.SetPoint(count,count,float(line.strip()))
    count += 1;

t = tgs([tt],"Magnetization")

t[0].Draw("AL")
t[2].Draw()

c1.Update()
c1.Modified()
