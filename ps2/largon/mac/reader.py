#!/usr/bin/python

#vgenty@nevis.columbia.edu

#ROOT includes
from ROOT import TGraph, TMultiGraph, TCanvas, TLatex, TLegend, TH1D, TGraph2D, TH2D, gStyle
#User includes
from looks import *
#Python includes
import sys,math

#Load the object
ROOT.gSystem.Load('../lib/libLArgon')
from ROOT import LArgon


def main():
    infile = ROOT.TFile('../output/out.root','read')
    intree = infile.Get('outtree')

    intree.GetEntry(0) #only one entry get first entry
    b = intree.LArgon    
    looks_minos()
    
    
    args = sys.argv
    args.pop(0)
    print "\n\n\n\n"
    if(len(args) == 0):
        print "./reader.py energy/position/velocity"
        sys.exit()
    
    if(args[0] == "energy"):
        c1 = [TCanvas() for x in xrange(4)]
        
        
        #Get energy
        e = E(b)
        
        #Kinetic
        c1[0].cd()
        e[0].SetLineColor(2)
        e[0].GetXaxis().SetTitle("Time")
        e[0].GetXaxis().CenterTitle()
        e[0].GetYaxis().SetTitle("Kinetic Energy")
        e[0].GetYaxis().CenterTitle()
        e[0].Draw("AL")
        
        #Potential
        c1[1].cd()
        e[1].SetLineColor(4)
        e[1].GetXaxis().SetTitle("Time")
        e[1].GetXaxis().CenterTitle()
        e[1].GetYaxis().SetTitle("Potential Energy")
        e[1].GetYaxis().CenterTitle()
        e[1].Draw("AL")

        #Total
        c1[2].cd()
        e[4].SetLineColor(3)
        e[4].GetXaxis().SetTitle("Time")
        e[4].GetXaxis().CenterTitle()
        e[4].GetYaxis().SetTitle("1-Etot/Einit")
        e[4].GetYaxis().CenterTitle()
        e[4].Draw("AL")
        
        #Legend
        tll = TLegend(0.5,0.3,0.6,0.8,"Energy","nbNDC")
        tll.AddEntry(e[0],"Kinetic","l")
        tll.AddEntry(e[1],"Potential","l")
        tll.AddEntry(e[2],"Total","l")
        
        #TMultiGraph
        c1[3].cd()
        e[3].Draw("A")
        
        e[3].GetXaxis().SetTitle("Time")
        e[3].GetYaxis().SetTitle("Energy")
        e[3].GetXaxis().CenterTitle()
        e[3].GetYaxis().CenterTitle()
        tll.Draw()

        c1[3].Update()
        c1[3].Modified()
    
        

    if(args[0] == "position"):
        c2 = [TCanvas() for x in xrange(4)]

        if args[1] is None:
            print "Give me a position step!"
            sys.exit()

        rr = initialR(b,int(args[1]))
        
        #X
        c2[0].cd()
        rr[0][0].Draw()
        c2[0].Update()
        c2[0].Modified()
        
        #Y
        c2[1].cd()
        rr[0][1].Draw()
        c2[1].Update()
        c2[1].Modified()
        
        #Z
        c2[2].cd()
        rr[0][2].Draw()
        c2[2].Update()
        c2[2].Modified()

        #3D
        c2[3].cd()
        rr[1].Draw("AP")
        c2[3].Update()
        c2[3].Modified()

    if(args[0] == "velocity"):
        c2 = [TCanvas() for x in xrange(4)]

        try:
            args[1]
        except NameError:
            print "Give me a velocity step!"
            sys.exit()

        vv = initialV(b,-10,10,int(args[1]))
            
        gStyle.SetPalette(55)
        #X
        c2[0].cd()
        vv[0][0].Draw()
        c2[0].Update()
        c2[0].Modified()
        
        #Y
        c2[1].cd()
        vv[0][1].Draw()
        c2[1].Update()
        c2[1].Modified()
        
        #Z
        c2[2].cd()
        vv[0][2].Draw()
        c2[2].Update()
        c2[2].Modified()

        #3D
        c2[3].cd()
        vv[1].Draw("COLZ")
        c2[3].Update()
        c2[3].Modified()
         
         
    # tt = TT(b)

    # c8 = TCanvas()
    # c8.cd()
    # p = P(b)
    # p.Draw("AL")
    # c8.Update()
    # c8.Modified()

    raw_input('')
    
def initialR(b,d):
    
    tR = [TH1D("X",";x pos;",100,0,10),
          TH1D("Y",";y pos;",100,0,10),
          TH1D("Z",";z pos;",100,0,10)]

    tg2d = TGraph2D()
    c = 0
    o = 0
    for i in b.R():
        if(c == d):
            for j in i:
                #print j
                x = j[0]
                y = j[1]
                z = j[2]
                tR[0].Fill(x)
                tR[1].Fill(y)
                tR[2].Fill(z)
                tg2d.SetPoint(o,x,y,z)
                o +=1
                #print "(%f,%f,%f)" % (x,y,z)
        c +=1
        
    
    tR[0].GetXaxis().SetTitle("X pos @ t = %d" % d) 
    tR[1].GetXaxis().SetTitle("Y pos @ t = %d" % d)
    tR[2].GetXaxis().SetTitle("Z pos @ t = %d" % d)

    for y in xrange(3):
        tR[y].GetYaxis().SetTitle("Count")
        tR[y].GetYaxis().CenterTitle()
        tR[y].GetXaxis().CenterTitle()

        
    tg2d.GetXaxis().SetTitle("X")
    tg2d.GetYaxis().SetTitle("Y")
    tg2d.GetZaxis().SetTitle("Z")
        
    tg2d.GetXaxis().CenterTitle()
    tg2d.GetYaxis().CenterTitle()
    tg2d.GetZaxis().CenterTitle()

    return [tR,tg2d]

def initialV(b,vmin, vmax,d):

    tV2D = TH2D("2d","",150,0,150,100,vmin,vmax)

    tV = [TH1D("Vx","",100,vmin,vmax),
          TH1D("Vy","",100,vmin,vmax),
          TH1D("Vz","",100,vmin,vmax)]

    c = 0
    for i in b.V():
        for j in i:
            tV2D.Fill(c,j[0])

        if(c == d):
            for j in i:
                tV[0].Fill(j[0])
                tV[1].Fill(j[1])
                tV[2].Fill(j[2])
        c +=1
  
    
    tV[0].GetXaxis().SetTitle("Vx @ t = %d" % d)
    tV[1].GetXaxis().SetTitle("Vy @ t = %d" % d)
    tV[2].GetXaxis().SetTitle("Vz @ t = %d" % d)

    for y in xrange(3):
        tV[y].GetYaxis().SetTitle("Count")
        tV[y].GetYaxis().CenterTitle()
        tV[y].GetXaxis().CenterTitle()

        
    tV2D.GetXaxis().SetTitle("Step")
    tV2D.GetYaxis().SetTitle("Vx")
    tV2D.GetXaxis().CenterTitle()
    tV2D.GetYaxis().CenterTitle()

    return [tV,tV2D]

def E(b):
    
    tg1 = TGraph() 
    tg2 = TGraph()
    tg3 = TGraph()
    tg4 = TGraph()
    tm  = TMultiGraph()

    tm.Add(tg1,"L")
    tm.Add(tg2,"L")
    tm.Add(tg3,"L")

    #print len(b.KE())
    
    for i in xrange(len(b.KE())) :
        tg1.SetPoint(i,i,b.KE()[i])
        tg2.SetPoint(i,i,b.PE()[i])
        tg3.SetPoint(i,i,b.PE()[i]+b.KE()[i])
        tg4.SetPoint(i,i,1-(b.PE()[i] + b.KE()[i])/(b.PE()[0] + b.KE()[0]))
        
    tg1.SetLineWidth(2)
    tg2.SetLineWidth(2)
    tg3.SetLineWidth(2)
    tg4.SetLineWidth(2)
        
    return [tg1,tg2,tg3,tm,tg4]

def P(b):
    
    tg1 = TGraph() 
    #tg2 = TGraph()
    #tm  = TMultiGraph()

    #tm.Add(tg1,"L")
    #tm.Add(tg2,"L")

    #print len(b.KE())
    
    for i in xrange(len(b.P())) :
        tg1.SetPoint(i,i,b.P()[i])
        #tg2.SetPoint(i,i,b.PE()[i])
       #print "TOTAL E = %f, energy now = %f, diff = %f" % (b.PE()[0]+b.KE()[0], b.PE()[i]+b.KE()[i], (b.PE()[0]+b.KE()[0]) - (b.PE()[i]+b.KE()[i]))
    
    return tg1

def TT(b):
    tgT = TGraph()
    
    for i in xrange(len(b.T())):
        tgT.SetPoint(i,i,b.T()[i])

    tgT.SetTitle(";step;T")
    return tgT

if __name__ == '__main__' :
    main()
    
