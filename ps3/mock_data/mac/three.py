import pdb
import ROOT
from ROOT import TCanvas, TLatex, TH1D, TGraph, TH2D
from looks import *

ROOT.gSystem.Load('./lib/libAnalyzer.so');
from ROOT import Analyzer

import numpy as n

looks_minos();

a = Analyzer("raw")

a.a()
a.b()
