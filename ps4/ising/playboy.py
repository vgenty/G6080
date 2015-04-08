from ROOT import TCanvas, TGraph

tg = TGraph()

file = open('cluster.dat')

count = 0

for line in file:
    tg.SetPoint(count,count,float(line.strip()))
    count += 1;

tg.Draw("AL")
