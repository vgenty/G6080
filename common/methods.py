from ROOT import TGraph, TMultiGraph, TLegend, TPaveText

def tgs(tgraphs,title) :

    tmg = TMultiGraph()
    tle = TLegend(0.6,0.4,0.9,0.8)
    tit = TPaveText(0.30,0.91,0.70,1,"nbNDC")

    # Make Title:
    tit.SetFillColor(0)
    tit.SetFillStyle(0)
    tit.SetLineColor(0)
    tit.AddText(title)
    
    for i in xrange(len(tgraphs)):
        tgraphs[i].SetLineWidth(2)
        tgraphs[i].SetLineColor(i+1)

        tmg.Add(tgraphs[i])
        tle.AddEntry(tgraphs[i],tgraphs[i].GetName(),"l")

    
    tmg.SetDrawStyle("AL")
    return [tmg,tle,tit]
