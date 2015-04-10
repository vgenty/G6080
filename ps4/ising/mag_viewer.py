#/usr/bin/python -i

from ROOT import TCanvas, TGraph

import numpy as np

import sys

from methods import *
from looks   import *

looks_minos()
tt = TGraph()

c1 = TCanvas()
try:
    sys.argv[1]
except IndexError:
    print "./playboy.py [filename]"
    sys.exit()
else:
    datfile = sys.argv[1]
    

file = open(datfile)

count = 0

for line in file:
    tt.SetPoint(count,count,float(line.strip()))
    count += 1;

t = tgs([tt],"Magnetization")

t[0].Draw("AL")
t[2].Draw()

c1.Update()
c1.Modified()
