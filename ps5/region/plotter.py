from ROOT import * #import it all who gives a hoot


tfile = TFile("outfile.root","READ")
ttree = tfile.Get("data")

k = []

for e in ttree:
    for o in xrange(len(e.y)):
        k.append(e.y[o])

c = 0

c1 = TCanvas()
tg2d = TGraph2D()


for i in xrange(48):
    for j in xrange(48):
        tg2d.SetPoint(c,i,j,k[c])
        c += 1


tg2d.Draw("SURF1")
c1.Update()
c1.Modified()
raw_input('')
