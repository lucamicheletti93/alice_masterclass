void LoadStyle();
void SetLegend(TLegend * );

void plotsForMasterclass(const int nEntries = 10000000, const double sigToBkg = 0.01) {
    LoadStyle();

    TH1D *histDelta = new TH1D("histDelta", "", 3000, 2, 5);
    histDelta -> SetBinContent(histDelta -> FindBin(3.096), 100);

    TF1 *funcGaus = new TF1("funcGaus", "gaus", 2, 5);
    funcGaus -> SetParameter(0, 100);
    funcGaus -> SetParameter(1, 3.096);
    funcGaus -> SetParameter(2, 0.07);
    funcGaus -> SetNpx(300);
    TH1D *histGaus = (TH1D*) funcGaus -> GetHistogram();

    TH1D *histDimu = new TH1D("histDimu", ";#it{m} (GeV/#it{c}^{2});conteggi", 150, 2, 5);
    histDimu -> SetLineColor(kBlack);
    histDimu -> SetMarkerStyle(20);
    histDimu -> SetMarkerSize(0.8);
    histDimu -> SetMarkerColor(kBlack);

    TH1D *histSigReal = new TH1D("histSigReal", ";#it{m} (GeV/#it{c}^{2});conteggi", 150, 2, 5);
    histSigReal -> SetLineColor(kRed+1);
    histSigReal -> SetFillColorAlpha(kRed+1, 0.6);
    for (int i = 0;i < nEntries*sigToBkg;i++) {
        double entry = funcGaus -> GetRandom();
        histSigReal -> Fill(entry);
        histDimu -> Fill(entry);
    }

    TH1D *histSigIdeal = new TH1D("histSigIdeal", ";#it{m} (GeV/#it{c}^{2});conteggi", 150, 2, 5);
    histSigIdeal -> SetLineColor(kRed+1);
    histSigIdeal -> SetFillColorAlpha(kRed+1, 0.6);
    histSigIdeal -> SetBinContent(histSigIdeal -> FindBin(3.096), histSigReal -> GetBinContent(histSigIdeal -> FindBin(3.096)));

    TF1 *funcExpo = new TF1("funcExpo", "expo", 2, 5);
    funcExpo -> SetParameter(0, 1);
    funcExpo -> SetParameter(1, -1.34);
    funcExpo -> SetNpx(300);
    TH1D *histExpo = (TH1D*) funcExpo -> GetHistogram();

    TH1D *histBkgReal = new TH1D("histBkgReal", ";#it{m} (GeV/#it{c}^{2});conteggi", 150, 2, 5);
    histBkgReal -> SetLineColor(kGray+1);
    histBkgReal -> SetFillColorAlpha(kGray+1, 0.6);

    TH1D *histDimuCombBkg = new TH1D("histDimuCombBkg", ";#it{m} (GeV/#it{c}^{2});conteggi", 150, 2, 5);
    histDimuCombBkg -> SetLineColor(kMagenta);
    histDimuCombBkg -> SetMarkerStyle(24);
    histDimuCombBkg -> SetMarkerSize(0.8);
    histDimuCombBkg -> SetMarkerColor(kMagenta);

    for (int i = 0;i < nEntries*(1-sigToBkg);i++) {
        double entry1 = funcExpo -> GetRandom();
        histBkgReal -> Fill(entry1);
        histDimu -> Fill(entry1);
        double entry2 = funcExpo -> GetRandom();
        histDimuCombBkg -> Fill(entry2);
    }

    TH1D *histDimuSubtr = (TH1D*) histDimu -> Clone("histDimuSubtr");
    histDimuSubtr -> SetLineColor(kBlack);
    histDimuSubtr -> SetMarkerStyle(20);
    histDimuSubtr -> SetMarkerSize(0.5);
    histDimuSubtr -> SetMarkerColor(kBlack);
    histDimuSubtr -> Add(histDimuCombBkg, -1);

    TF1 *funcAllDimu = new TF1("funcAllDimu", "gaus(0) + expo(3)", 2, 5);
    funcAllDimu -> SetNpx(300);
    funcAllDimu -> SetParameters(1, 3.096, 0.07, 5, -1.34);
    funcAllDimu -> SetLineColor(kAzure+2);
    histDimu -> Fit(funcAllDimu, "R0Q");

    TF1 *funcGausDimu = new TF1("funcGausDimu", "gaus", 2.8, 3.5);
    funcGausDimu -> SetNpx(300);
    funcGausDimu -> SetLineColor(kRed+1);
    funcGausDimu -> SetFillStyle(1001);
    funcGausDimu -> SetFillColorAlpha(kRed+1, 0.5);
    funcGausDimu -> SetParameters(funcAllDimu -> GetParameter(0), funcAllDimu -> GetParameter(1), funcAllDimu -> GetParameter(2));

    TF1 *funcExpoDimu = new TF1("funcExpoDimu", "expo", 2, 5);
    funcExpoDimu -> SetNpx(300);
    funcExpoDimu -> SetLineColor(kGray+2);
    funcExpoDimu -> SetLineStyle(kDashed);
    funcExpoDimu -> SetParameters(funcAllDimu -> GetParameter(3), funcAllDimu -> GetParameter(4));

    // Plot results
    double meanJpsi = funcAllDimu -> GetParameter(1);
    double sigmaJpsi = funcAllDimu -> GetParameter(2);
    double intRangeJpsiMin = meanJpsi - (3*sigmaJpsi);
    double intRangeJpsiMax = meanJpsi + (3*sigmaJpsi);
    double nJpsi = (funcGausDimu -> Integral(intRangeJpsiMin, intRangeJpsiMax)) / (histDimu -> GetBinWidth(1));
    double errJpsi = TMath::Sqrt(nJpsi);

    TH2D *histGrid1 = new TH2D("histGrid1", ";#it{m} (GeV/#it{c}^{2});conteggi", 100, 2, 5, 100, 0, 1200);
    TH2D *histGrid2 = new TH2D("histGrid2", ";#it{m} (GeV/#it{c}^{2});conteggi", 100, 2, 5, 100, 0, histBkgReal -> GetBinContent(1));
    TH2D *histGrid3 = new TH2D("histGrid2", ";#it{m} (GeV/#it{c}^{2});conteggi", 100, 2, 5, 100, 1e2, 1e2*histBkgReal -> GetBinContent(1));

    TCanvas *canvas1 = new TCanvas("canvas1", "", 800, 600);
    gStyle -> SetOptStat(false);
    histGrid1 -> Draw();
    histSigIdeal -> Draw("H SAME");

    TCanvas *canvas2 = new TCanvas("canvas2", "", 800, 600);
    gStyle -> SetOptStat(false);
    histGrid1 -> Draw();
    histSigReal -> Draw("H SAME");

    TCanvas *canvas3 = new TCanvas("canvas3", "", 800, 600);
    gStyle -> SetOptStat(false);
    histGrid1 -> Draw();
    histSigReal -> Draw("H SAME");
    histBkgReal -> Draw("H SAME");


    TCanvas *canvas4 = new TCanvas("canvas4", "", 800, 600);
    gStyle -> SetOptStat(false);
    histGrid2 -> Draw();
    histDimu -> Draw("EP SAME");

    TLegend *legend4 = new TLegend(0.65, 0.60, 0.83, 0.85);
    SetLegend(legend4);
    legend4 -> AddEntry(histDimu, "Dati", "PL");
    legend4 -> AddEntry(funcAllDimu, "Fit", "PL");
    legend4 -> AddEntry(funcGausDimu, "Segnale", "F");
    legend4 -> AddEntry(funcExpoDimu, "Fondo", "L");
    legend4 -> Draw();


    TCanvas *canvas5 = new TCanvas("canvas5", "", 800, 600);
    gStyle -> SetOptStat(false);
    histGrid2 -> Draw();
    histDimu -> Draw("EP SAME");
    funcExpoDimu -> Draw("SAME");
    funcGausDimu -> Draw("SAME");
    funcAllDimu -> Draw("SAME");

    TLegend *legend5 = new TLegend(0.65, 0.60, 0.83, 0.85);
    SetLegend(legend5);
    legend5 -> AddEntry(histDimu, "Dati", "PL");
    legend5 -> AddEntry(funcAllDimu, "Fit", "PL");
    legend5 -> AddEntry(funcGausDimu, "Segnale", "F");
    legend5 -> AddEntry(funcExpoDimu, "Fondo", "L");
    legend5 -> Draw();

    TLatex latexTitle6;
    latexTitle6.SetNDC();
    latexTitle6.SetTextSize(0.05);
    latexTitle6.SetTextFont(42);
    latexTitle6.DrawLatex(0.65, 0.55, Form("#it{N}_{J/#psi} = %3.0f #pm %3.0f", nJpsi, errJpsi));
    latexTitle6.DrawLatex(0.65, 0.48, Form("#chi^{2} = %3.2f", funcAllDimu -> GetChisquare() / funcAllDimu -> GetNDF()));

    TCanvas *canvas6 = new TCanvas("canvas6", "", 800, 600);
    gStyle -> SetOptStat(false);
    gPad -> SetLogy(true);
    histGrid3 -> Draw();
    histDimu -> Draw("EP SAME");

    TLegend *legend6 = new TLegend(0.55, 0.65, 0.73, 0.85);
    SetLegend(legend6);
    legend6 -> AddEntry(histDimu, "Dati: #mu^{+}#mu^{#minus}", "PL");
    legend6 -> AddEntry(histDimuCombBkg, "Fondo: 2 #upoint #sqrt{#mu^{+}#mu^{+} #upoint #mu^{#minus}#mu^{#minus}}", "L");
    legend6 -> AddEntry(histDimuSubtr, "Dati #minus Fondo", "PL");
    legend6 -> Draw();


    TCanvas *canvas7 = new TCanvas("canvas7", "", 800, 600);
    gStyle -> SetOptStat(false);
    gPad -> SetLogy(true);
    histGrid3 -> Draw();
    histDimu -> Draw("EP SAME");
    histDimuCombBkg -> Draw("H SAME");
    histDimuSubtr -> Draw("EPH SAME");

    TLegend *legend7 = new TLegend(0.55, 0.65, 0.73, 0.85);
    SetLegend(legend7);
    legend7 -> AddEntry(histDimu, "Dati: #mu^{+}#mu^{#minus}", "PL");
    legend7 -> AddEntry(histDimuCombBkg, "Fondo: 2 #upoint #sqrt{#mu^{+}#mu^{+} #upoint #mu^{#minus}#mu^{#minus}}", "L");
    legend7 -> AddEntry(histDimuSubtr, "Dati #minus Fondo", "PL");
    legend7 -> Draw();

    canvas1 -> SaveAs("figures/plot1.pdf");
    canvas2 -> SaveAs("figures/plot2.pdf");
    canvas3 -> SaveAs("figures/plot3.pdf");
    canvas4 -> SaveAs("figures/plot4.pdf");
    canvas5 -> SaveAs("figures/plot5.pdf");
    canvas6 -> SaveAs("figures/plot6.pdf");
    canvas7 -> SaveAs("figures/plot7.pdf");
}
////////////////////////
void LoadStyle(){
    int font = 42;
    gStyle -> SetFrameBorderMode(0);
    gStyle -> SetFrameFillColor(0);
    gStyle -> SetCanvasBorderMode(0);
    gStyle -> SetPadBorderMode(0);
    gStyle -> SetPadColor(10);
    gStyle -> SetCanvasColor(10);
    gStyle -> SetTitleFillColor(10);
    gStyle -> SetTitleBorderSize(1);
    gStyle -> SetStatColor(10);
    gStyle -> SetStatBorderSize(1);
    gStyle -> SetLegendBorderSize(1);
    gStyle -> SetDrawBorder(0);
    gStyle -> SetTextFont(font);
    gStyle -> SetStatFontSize(0.05);
    gStyle -> SetStatX(0.97);
    gStyle -> SetStatY(0.98);
    gStyle -> SetStatH(0.03);
    gStyle -> SetStatW(0.3);
    gStyle -> SetTickLength(0.02,"y");
    gStyle -> SetEndErrorSize(3);
    gStyle -> SetLabelSize(0.05,"xyz");
    gStyle -> SetLabelFont(font,"xyz");
    gStyle -> SetLabelOffset(0.01,"xyz");
    gStyle -> SetTitleFont(font,"xyz");
    gStyle -> SetTitleOffset(0.9,"x");
    gStyle -> SetTitleOffset(1.02,"y");
    gStyle -> SetTitleSize(0.05,"xyz");
    gStyle -> SetMarkerSize(1.3);
    gStyle -> SetOptStat(0);
    gStyle -> SetEndErrorSize(0);
    gStyle -> SetCanvasPreferGL(kTRUE);
    gStyle -> SetHatchesSpacing(0.5);
    gStyle -> SetPadLeftMargin(0.15);
    gStyle -> SetPadBottomMargin(0.15);
    gStyle -> SetPadTopMargin(0.05);
    gStyle -> SetPadRightMargin(0.05);
    gStyle -> SetEndErrorSize(0.0);
    gStyle -> SetTitleSize(0.05,"X");
    gStyle -> SetTitleSize(0.045,"Y");
    gStyle -> SetLabelSize(0.045,"X");
    gStyle -> SetLabelSize(0.045,"Y");
    gStyle -> SetTitleOffset(1.2,"X");
    gStyle -> SetTitleOffset(1.35,"Y");
}
////////////////////////
void SetLegend(TLegend *legend){
    legend -> SetBorderSize(0);
    legend -> SetFillColor(10);
    legend -> SetFillStyle(1);
    legend -> SetLineStyle(0);
    legend -> SetLineColor(0);
    legend -> SetTextFont(42);
    legend -> SetTextSize(0.05);
}