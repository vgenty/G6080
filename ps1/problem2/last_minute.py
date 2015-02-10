from looks import *
from ROOT import*


c1 = TCanvas()

o3 = [0.56, 0.69, 1.46, 1.92, 1.91, 20.31, 21.14, 5.39]
o0 = [2.22, 2.57, 3.05, 3.82, 3.85, 24.19, 23.36, 4.70]


labels = ["= a[i] * b[i]","+= a[i] * b[i]","+= a[i]*b[2*i + 20]","= a[i] / b[i]","+= a[i] / b[i]","= std::sin(a[i])","= std::exp(a[i])","= std::sqrt(a[i])",]


looks()

th = TH1D("gcc -O3",";;",8,0,8);
th2 = TH1D("gcc -O0",";;",8,0,8);

for x in xrange(len(o3)):
    th.SetBinContent(x+1,o3[x])
    th.GetXaxis().SetBinLabel(x+1,labels[x])

for x in xrange(len(o0)):
    th2.SetBinContent(x+1,o0[x])
    th2.GetXaxis().SetBinLabel(x+1,labels[x])

c1.cd()
#th.SetTitle(";;Operation time [ns] gcc -O3")
th.GetYaxis().CenterTitle()
th2.Draw()

#c2.cd()
th2.SetTitle(";;Operation time [ns]")
th2.GetYaxis().CenterTitle()
th.Draw("SAMES")

th2.GetYaxis().SetRangeUser(0,30)


th.SetFillColor(4)
th.SetFillStyle(3345)
th2.SetFillColor(2)
th2.SetFillStyle(3354)

tl = TLegend(0.6,0.4,0.9,0.8)
tl.AddEntry(th2,th2.GetName(),"f")
tl.AddEntry(th,th.GetName(),"f")
tl.Draw()


c1.Update()
c1.Modified()

raw_input('')
