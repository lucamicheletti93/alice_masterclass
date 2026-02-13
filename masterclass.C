#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGToolTip.h>
#include <TGComboBox.h>
#include <TGTableLayout.h>

#include <TRootEmbeddedCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TH1D.h>
#include <TF1.h>
#include <TRandom.h>
#include <TApplication.h>

#include "ROOT/RDataFrame.hxx"

class MyGUI : public TGMainFrame {

private:
    TGHorizontalFrame *main;

    TGTextEntry *entryHistToFit;
    TGTextEntry *entryPtDimuMin, *entryPtDimuMax, *entryChargeDimu;
    TGTextEntry *entryPtMuMin, *entryPtMuMax, *entryEtaMuMin, *entryEtaMuMax, *entryTrackType1, *entryTrackType2, *entryRebin;
    TGTextEntry *entryHistName;
    TGTextEntry *entryFitMeanMin, *entryFitMeanMax, *entryFitWidthMin, *entryFitWidthMax, *entryFitMin, *entryFitMax;

    TGLabel *labelHistStatus, *labelHistFitStatus;

    TRootEmbeddedCanvas *canvasTopLeft, *canvasTopRight, *canvasBottomLeft, *canvasBottomRight;

    TGComboBox  *comboVar;
    TGComboBox  *comboTrackCut;
    TGComboBox  *comboPlotHist;
    TGComboBox  *comboPlotHistForFit;

    TGCheckButton *cbChargePlus, *cbChargeMinus, *cbChargeZero;

    TGTextButton *bAddHist, *bDrawHistPerVar, *bClear;
    TGTextButton *bBuildBkg;
    TGTextButton *bFitPlot, *bSubtractBkgOn, *bSubtractBkgOff, *bFit, *bResults;
    TGTextButton *bClose;

    //ROOT::RDF::RNode df;
    ROOT::RDataFrame df;

    ROOT::RDF::RResultPtr<TH1D> histVar;
    ROOT::RDF::RResultPtr<TH1D> histDimuMassPM, histDimuMassPP, histDimuMassMM;

    std::unordered_map<std::string, ROOT::RDF::RResultPtr<TH1D>> histograms;

    std::vector<string> histogramNames;

    TH1D *histResults, *histDimuMassToFit, *histDimuMassPPMM;

    TLatex latexResults;

    double nJpsi, errJpsi, nPsi2s, errPsi2s, nBkgRangeJpsi, errBkgRangeJpsi, nBkgRangePsi2s, errBkgRangePsi2s, signfJpsi, signfPsi2s;
    bool removeBkg, subtractBkg;
    Color_t colorPalette[6] = {kBlack, kRed+1, kAzure+2, kGreen+1, kOrange+7, kViolet};



public:
    MyGUI(const TGWindow *p, ROOT::RDataFrame rdf) : TGMainFrame(p, 900, 500), df(rdf) {
    //MyGUI(const TGWindow *p, ROOT::RDF::RNode rdf) : TGMainFrame(p, 900, 500), df(rdf) {

        // ===========================
        // MAIN FRAME
        // ===========================
        main = new TGHorizontalFrame(this); 
        
        AddFrame(main, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        // =====================================================
        //  LEFT FRAME
        // =====================================================
        TGVerticalFrame *left = new TGVerticalFrame(main, 300);
        main->AddFrame(left, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 5, 5, 5));

        // ---------- SHOW (TOP) ----------
        TGGroupFrame *showFrame = new TGGroupFrame(left, "Data selection");
        left -> AddFrame(showFrame, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

        TGCompositeFrame *tableShow = new TGCompositeFrame(showFrame);
        tableShow -> SetLayoutManager(new TGTableLayout(tableShow, 4, 4));
        showFrame -> AddFrame(tableShow, new TGLayoutHints(kLHintsExpandX));
        
        tableShow -> AddFrame(new TGLabel(tableShow, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        tableShow -> AddFrame(new TGLabel(tableShow, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        tableShow -> AddFrame(new TGLabel(tableShow, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));

        // Muon pT
        tableShow -> AddFrame(new TGLabel(tableShow, "pT mu"), new TGTableLayoutHints(0, 1, 1, 2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtMuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtMuMin -> SetText("0");
        entryPtMuMin -> Resize(80,22);
        entryPtMuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtMuMin, new TGTableLayoutHints(1, 2, 1, 2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtMuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtMuMax -> SetText("100");
        entryPtMuMax -> Resize(80,22);
        entryPtMuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtMuMax, new TGTableLayoutHints(2, 3, 1, 2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Muon eta
        tableShow -> AddFrame(new TGLabel(tableShow, "eta mu"), new TGTableLayoutHints(0, 1, 2, 3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryEtaMuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryEtaMuMin -> SetText("2.5");
        entryEtaMuMin -> Resize(80,22);
        entryEtaMuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryEtaMuMin, new TGTableLayoutHints(1, 2, 2, 3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryEtaMuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryEtaMuMax -> SetText("4");
        entryEtaMuMax -> Resize(80,22);
        entryEtaMuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryEtaMuMax, new TGTableLayoutHints(2, 3, 2, 3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Dimuon pt
        tableShow -> AddFrame(new TGLabel(tableShow, "pT dimu"), new TGTableLayoutHints(0,1,3,4, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtDimuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtDimuMin -> SetText("0");
        entryPtDimuMin -> Resize(80,22);
        entryPtDimuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtDimuMin, new TGTableLayoutHints(1, 2, 3, 4, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtDimuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtDimuMax -> SetText("100");
        entryPtDimuMax -> Resize(80,22);
        entryPtDimuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtDimuMax, new TGTableLayoutHints(2, 3, 3, 4, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        tableShow -> MapSubwindows();
        tableShow -> Resize();

        AddInput(showFrame, "Charge dimu", entryChargeDimu, "0");

        TGHorizontalFrame *fTrackCut = new TGHorizontalFrame(showFrame);
        fTrackCut->AddFrame(new TGLabel(fTrackCut, "Track type:"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 5, 5));

        comboTrackCut = new TGComboBox(fTrackCut);
        comboTrackCut->Resize(180, 22);
        fTrackCut->AddFrame(comboTrackCut, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
        showFrame->AddFrame(fTrackCut, new TGLayoutHints(kLHintsExpandX));

        comboTrackCut -> RemoveAll();
        comboTrackCut -> AddEntry("MCH", 1);
        comboTrackCut -> AddEntry("MCH-MID", 0);
        comboTrackCut -> AddEntry("MFT-MCH-MID", 2);
        comboTrackCut -> Select(2);

        AddInput(showFrame, "Data rebin", entryRebin, "1");

        TGHorizontalFrame *fVar = new TGHorizontalFrame(showFrame);
        fVar->AddFrame(new TGLabel(fVar, "Variable:"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 5, 5));

        comboVar = new TGComboBox(fVar);
        comboVar->Resize(180, 22);
        fVar->AddFrame(comboVar, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
        showFrame->AddFrame(fVar, new TGLayoutHints(kLHintsExpandX));

        FillComboFromRDF();

        TGTextButton *bShow = new TGTextButton(showFrame, "Plot selected variable");
        showFrame->AddFrame(bShow, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));


        // ---------- ANALYSIS (MID) ----------
        TGGroupFrame *analysisFrame = new TGGroupFrame(left, "Data analysis");
        left->AddFrame(analysisFrame, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

        // Frame for adding histograms to the list
        TGHorizontalFrame *frameAddHist = new TGHorizontalFrame(analysisFrame);

        bAddHist = new TGTextButton(frameAddHist, "Add");
        bAddHist -> SetEnabled(false);
        frameAddHist->AddFrame(bAddHist, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 10, 5, 5));

        entryHistName = new TGTextEntry(frameAddHist, new TGTextBuffer(30));
        entryHistName -> Resize(160, 22);
        entryHistName -> SetAlignment(kTextLeft);
        entryHistName -> SetToolTipText("Nome dell'istogramma \nse non selezionato usa il nome della variabile");
        frameAddHist -> AddFrame(entryHistName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 5, 5));

        analysisFrame -> AddFrame(frameAddHist, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        labelHistStatus = new TGLabel(analysisFrame, "No histogram added");
        analysisFrame -> AddFrame(labelHistStatus, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        TGHorizontalFrame *frameDrawHists = new TGHorizontalFrame(analysisFrame);

        // Frame for plotting histograms added to the list
        TGHorizontalFrame *framePlotHist = new TGHorizontalFrame(analysisFrame);

        bDrawHistPerVar = new TGTextButton(framePlotHist, "Plot");
        bDrawHistPerVar -> SetEnabled(false);
        framePlotHist->AddFrame(bDrawHistPerVar, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 10, 5, 5));

        comboPlotHist = new TGComboBox(framePlotHist);
        comboPlotHist->Resize(180, 22);
        framePlotHist->AddFrame(comboPlotHist, new TGLayoutHints(kLHintsExpandX));
        comboPlotHist -> RemoveAll();

        analysisFrame -> AddFrame(framePlotHist, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        bBuildBkg = new TGTextButton(frameDrawHists, "Build background");
        bBuildBkg -> SetEnabled(false);
        bBuildBkg -> SetToolTipText("Se disabilitato, per abilitare selezionare mass\ne 3 cariche per il dimuone = 0, +2, -2");
        frameDrawHists -> AddFrame(bBuildBkg, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));
        removeBkg = false;

        bClear = new TGTextButton(frameDrawHists, "Clear all histograms");
        bClear -> SetEnabled(false);
        frameDrawHists -> AddFrame(bClear, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));

        analysisFrame -> AddFrame(frameDrawHists, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        // ---------- FIT (BOTTOM) ----------
        TGGroupFrame *fitFrame = new TGGroupFrame(left, "Fit data");
        left->AddFrame(fitFrame, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

        //AddInput(fitFrame, "Histogram to fit", entryHistToFit, "none");

        //labelHistFitStatus = new TGLabel(fitFrame, "No histogram selected");
        //fitFrame -> AddFrame(labelHistFitStatus, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        // Frame for plotting histograms added to the list and ready for fitting
        TGHorizontalFrame *frameHistToFit = new TGHorizontalFrame(fitFrame);

        bFitPlot = new TGTextButton(frameHistToFit, "Plot");
        bFitPlot -> SetEnabled(false);
        frameHistToFit -> AddFrame(bFitPlot, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 10, 5, 5));

        comboPlotHistForFit = new TGComboBox(frameHistToFit);
        comboPlotHistForFit -> Resize(180, 22);
        frameHistToFit -> AddFrame(comboPlotHistForFit, new TGLayoutHints(kLHintsExpandX));
        comboPlotHistForFit -> RemoveAll();

        fitFrame -> AddFrame(frameHistToFit, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        TGHorizontalFrame *frameFitHists = new TGHorizontalFrame(fitFrame);

        /*bDrawHistPerVar = new TGTextButton(fitFrame, "Plot");
        bDrawHistPerVar -> SetEnabled(false);
        frameFitHists->AddFrame(bDrawHistPerVar, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 10, 5, 5));

        comboPlotHistForFit = new TGComboBox(frameFitHists);
        comboPlotHistForFit->Resize(180, 22);
        frameFitHists->AddFrame(comboPlotHistForFit, new TGLayoutHints(kLHintsExpandX));
        comboPlotHistForFit -> RemoveAll();*/

        /*bFitPlot = new TGTextButton(frameFitHists, "Plot histogram to fit");
        frameFitHists -> AddFrame(bFitPlot, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));
        bFitPlot -> SetEnabled(false);*/

        bSubtractBkgOn = new TGTextButton(frameFitHists, "Bkg. subtraction on");
        bSubtractBkgOn -> SetToolTipText("Se disabilitato, e' necessario Build background");
        frameFitHists -> AddFrame(bSubtractBkgOn, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));
        bSubtractBkgOn -> SetEnabled(false);

        bSubtractBkgOff = new TGTextButton(frameFitHists, "Bkg. subtraction off");
        bSubtractBkgOff -> SetToolTipText("Se disabilitato, e' necessario Build background");
        frameFitHists -> AddFrame(bSubtractBkgOff, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));
        bSubtractBkgOff -> SetEnabled(false);

        subtractBkg = false;

        fitFrame -> AddFrame(frameFitHists, new TGLayoutHints(kLHintsLeft, 5, 5, 10, 5));

        TGCompositeFrame *tableFit = new TGCompositeFrame(fitFrame);
        tableFit -> SetLayoutManager(new TGTableLayout(tableFit, 4, 4));
        fitFrame -> AddFrame(tableFit, new TGLayoutHints(kLHintsExpandX));
        
        tableFit -> AddFrame(new TGLabel(tableFit, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        tableFit -> AddFrame(new TGLabel(tableFit, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        tableFit -> AddFrame(new TGLabel(tableFit, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));

        // Min fit range
        tableFit -> AddFrame(new TGLabel(tableFit, "Fit Range"), new TGTableLayoutHints(0, 1, 1, 2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMin -> SetText("2");
        entryFitMin -> Resize(80,22);
        entryFitMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMin, new TGTableLayoutHints(1, 2, 1, 2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMax -> SetText("5");
        entryFitMax -> Resize(80,22);
        entryFitMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMax, new TGTableLayoutHints(2, 3, 1, 2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Mean fit
        tableFit -> AddFrame(new TGLabel(tableFit, "Signal mean"), new TGTableLayoutHints(0, 1, 2, 3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitMeanMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMeanMin -> SetText("2.95");
        entryFitMeanMin -> Resize(80,22);
        entryFitMeanMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMeanMin, new TGTableLayoutHints(1, 2, 2, 3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitMeanMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMeanMax -> SetText("3.3");
        entryFitMeanMax -> Resize(80,22);
        entryFitMeanMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMeanMax, new TGTableLayoutHints(2, 3, 2, 3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Width fit
        tableFit -> AddFrame(new TGLabel(tableFit, "Signal width"), new TGTableLayoutHints(0,1,3,4, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitWidthMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitWidthMin -> SetText("0");
        entryFitWidthMin -> Resize(80,22);
        entryFitWidthMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitWidthMin, new TGTableLayoutHints(1, 2, 3, 4, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitWidthMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitWidthMax -> SetText("0.1");
        entryFitWidthMax -> Resize(80,22);
        entryFitWidthMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitWidthMax, new TGTableLayoutHints(2, 3, 3, 4, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        tableFit -> MapSubwindows();
        tableFit -> Resize();

        TGHorizontalFrame *frameFitResults = new TGHorizontalFrame(fitFrame);

        bFit = new TGTextButton(frameFitResults, "Fit data");
        frameFitResults -> AddFrame(bFit, new TGLayoutHints(kLHintsLeft, 10, 10, 5, 10));
        bFit -> SetEnabled(false);

        bResults = new TGTextButton(frameFitResults, "Add to results");
        frameFitResults -> AddFrame(bResults, new TGLayoutHints(kLHintsLeft, 10, 10, 5, 10));
        bResults -> SetEnabled(false);

        fitFrame -> AddFrame(frameFitResults, new TGLayoutHints(kLHintsCenterX, 5, 5, 10, 5));

        bClose = new TGTextButton(fitFrame, "Close");
        fitFrame -> AddFrame(bClose, new TGLayoutHints(kLHintsCenterX, 10, 10, 5, 10));
        Pixel_t red;
        gClient -> GetColorByName("red", red);
        bClose -> SetTextColor(red);
        bClose -> SetEnabled(true);

        // =====================================================
        // RIGHT FRAME
        // =====================================================
        TGVerticalFrame *right = new TGVerticalFrame(main, 600);
        main -> AddFrame(right, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 5, 5, 5));

        // Top-row frame
        TGHorizontalFrame *topRow = new TGHorizontalFrame(right);
        right->AddFrame(topRow, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

        // Data selection
        TGVerticalFrame* frameTopLeft = new TGVerticalFrame(topRow, 500, 400, kFixedWidth);
        topRow -> AddFrame(frameTopLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGLabel* titleTopLeft = new TGLabel(frameTopLeft, "Data selection");
        titleTopLeft -> SetTextFont("-*-helvetica-bold-r-*-*-16-*-*-*-*-*-*-*");
        frameTopLeft -> AddFrame(titleTopLeft, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 2, 2, 4, 2));

        canvasTopLeft = new TRootEmbeddedCanvas("canvasTopLeft", frameTopLeft, 500, 400);
        frameTopLeft -> AddFrame(canvasTopLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

        // Data analysis
        TGVerticalFrame* frameTopRight = new TGVerticalFrame(topRow, 500, 400, kFixedWidth);
        topRow -> AddFrame(frameTopRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGLabel* titleTopRight = new TGLabel(frameTopRight, "Data analysis");
        titleTopRight -> SetTextFont("-*-helvetica-bold-r-*-*-16-*-*-*-*-*-*-*");
        frameTopRight -> AddFrame(titleTopRight, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 2, 2, 4, 2));

        canvasTopRight = new TRootEmbeddedCanvas("canvasTopRight", frameTopRight, 500, 400);
        frameTopRight->AddFrame(canvasTopRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

        // Bottom-row frame
        TGHorizontalFrame *bottomRow = new TGHorizontalFrame(right);
        right->AddFrame(bottomRow, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
        
        // Fit data
        TGVerticalFrame* frameBottomLeft = new TGVerticalFrame(bottomRow, 500, 400, kFixedWidth);
        bottomRow -> AddFrame(frameBottomLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGLabel* titleBottomLeft = new TGLabel(frameBottomLeft, "Fit data");
        titleBottomLeft -> SetTextFont("-*-helvetica-bold-r-*-*-16-*-*-*-*-*-*-*");
        frameBottomLeft -> AddFrame(titleBottomLeft, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 2, 2, 4, 2));

        canvasBottomLeft = new TRootEmbeddedCanvas("canvasBottomLeft", frameBottomLeft, 500, 400);
        frameBottomLeft -> AddFrame(canvasBottomLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

        //histGridFit = new TH2D("histGridFit", "", 100, 2, 5, 100, 0, 5000);
        //histGridFit -> GetXaxis() -> SetTitle("mass (GeV/#it{c}^{2})");
        //histGridFit -> Draw();

        // Analysis results
        TGVerticalFrame* frameBottomRight = new TGVerticalFrame(bottomRow, 500, 400, kFixedWidth);
        bottomRow -> AddFrame(frameBottomRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGLabel* titleBottomRight = new TGLabel(frameBottomRight, "Analysis results");
        titleBottomRight -> SetTextFont("-*-helvetica-bold-r-*-*-16-*-*-*-*-*-*-*");
        frameBottomRight -> AddFrame(titleBottomRight, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 2, 2, 4, 2));

        canvasBottomRight = new TRootEmbeddedCanvas("canvasBottomRight", frameBottomRight, 500, 400);
        frameBottomRight->AddFrame(canvasBottomRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

        canvasBottomRight -> GetCanvas() -> cd();
        canvasBottomRight -> GetCanvas() -> Clear();
        canvasBottomRight -> GetCanvas() -> SetGrid();
        canvasBottomRight -> GetCanvas() -> SetBit(TObject::kCannotPick);
        gStyle -> SetOptStat(false);
        gPad -> SetLogy(true);

        histResults = new TH1D("", "", 5, 0, 5);
        histResults -> GetXaxis() -> SetLabelSize(0.07);
        histResults -> GetXaxis() -> SetBinLabel(1, "#it{N}_{J/#psi}");
        histResults -> GetXaxis() -> SetBinLabel(2, "#it{N}_{#psi(2S)}");
        histResults -> GetXaxis() -> SetBinLabel(3, "#sigma_{J/#psi}");
        histResults -> GetXaxis() -> SetBinLabel(4, "#sigma_{#psi(2S)}");
        histResults -> GetXaxis() -> SetBinLabel(5, "#psi(2S) / J/#psi");
        histResults -> GetYaxis() -> SetRangeUser(0.1, histResults -> GetBinContent(1) * 1.5);
        histResults -> SetFillStyle(1001);
        histResults -> SetFillColorAlpha(kAzure+2, 0.7);
        histResults -> SetMarkerStyle(20);
        histResults -> SetMarkerColor(kAzure+2);
        histResults -> SetLineColor(kAzure+2);

        histResults -> SetBarWidth(0.5);
        histResults -> SetBarOffset(0.25);

        histResults -> Draw("BAR");

        latexResults.SetNDC();
        latexResults.SetTextSize(0.04);
        latexResults.SetTextFont(42);
        latexResults.DrawLatex(0.65, 0.85, "#it{N}_{J/#psi} = 0");
        latexResults.DrawLatex(0.65, 0.80, "#it{N}_{#psi(2S)} = 0");
        latexResults.DrawLatex(0.65, 0.75, "#sigma_{J/#psi} = 0");
        latexResults.DrawLatex(0.65, 0.70, "#sigma_{#psi(2S)} = 0");
        latexResults.DrawLatex(0.65, 0.63, "#psi(2S) / J/#psi = 0");

        //TLine *line3sigma = new TLine(2, 3, 4, 3);
        //line3sigma -> SetLineColor(kRed+1);
        //line3sigma -> Draw("SAME");

        canvasBottomRight -> GetCanvas() -> Update();

        bShow -> Connect("Clicked()", "MyGUI", this, "DrawDataset()");
        bAddHist -> Connect("Clicked()", "MyGUI", this, "AddHistogram()");
        bBuildBkg -> Connect("Clicked()", "MyGUI", this, "BuildBackground()");
        bSubtractBkgOn -> Connect("Clicked()", "MyGUI", this, "EnableSubtractBkg()");
        bSubtractBkgOff -> Connect("Clicked()", "MyGUI", this, "DisableSubtractBkg()");
        bFitPlot -> Connect("Clicked()", "MyGUI", this, "FitDataFalse()");
        bFit -> Connect("Clicked()", "MyGUI", this, "FitDataTrue()");
        bResults -> Connect("Clicked()", "MyGUI", this, "UpdateResults()");
        bDrawHistPerVar  -> Connect("Clicked()", "MyGUI", this, "DrawHistogramsPerVar()");
        bClear -> Connect("Clicked()", "MyGUI", this, "ClearHistogram()");
        bClose -> Connect("Clicked()", "MyGUI", this, "Close()");

        SetWindowName("ALICE masterclass");
        MapSubwindows();
        Resize();
        MapWindow();
    }

    //-----------------------------------//
    // Add input to the frame
    void AddInput(TGCompositeFrame *parent, const char *label, TGTextEntry *&entry, const char *def) {
        const int labelWidth = 90;
        const int entryWidth = 120;
        const int entryHeight = 22;

        TGHorizontalFrame *frame = new TGHorizontalFrame(parent);

        TGHorizontalFrame* labelFrame = new TGHorizontalFrame(frame, labelWidth, entryHeight, kFixedWidth);
        frame -> AddFrame(labelFrame, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 8, 5));

        TGLabel* entryLabel = new TGLabel(labelFrame, label);
        labelFrame -> AddFrame(entryLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

        entry = new TGTextEntry(frame, "");
        entry -> Resize(entryWidth, entryHeight);
        entry -> SetAlignment(kTextCenterX);
        entry -> SetText(def);
        frame -> AddFrame(entry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

        parent -> AddFrame(frame, new TGLayoutHints(kLHintsExpandX));
    }

    //--------------------------------------------//
    // Function to fill the menu
    void FillComboFromRDF() {

        comboVar -> RemoveAll();

        auto columns = df.GetColumnNames();
        int id = 0;
        for (auto &column : columns) {
            comboVar -> AddEntry(column.c_str(), id++);
        }

        comboVar -> Select(6); // Select invarian mass WARNING: make it more efficient
    }

    //--------------------------------------------//
    // Function to select the variables to show
    TString GetSelectedVariable(TGComboBox *combo) const {

        TGTextLBEntry *varName = (TGTextLBEntry*) combo -> GetSelectedEntry();

        if (!varName) return "";
        return varName -> GetText() -> GetString();
    }

    //--------------------------------------------//
    // Function to draw the data in the canvas
    void DrawDataset() {
        TString var = GetSelectedVariable(comboVar);
        TString trackCut = GetSelectedVariable(comboTrackCut);
        if (var.IsNull()) return;
        
        double ptDimuMin = std::stod(entryPtDimuMin -> GetText());
        double ptDimuMax = std::stod(entryPtDimuMax -> GetText());
        int chargeDimu = std::stod(entryChargeDimu -> GetText());
        double ptMuMin = std::stod(entryPtMuMin -> GetText());
        double ptMuMax = std::stod(entryPtMuMax -> GetText());
        double etaMuMin = std::stod(entryEtaMuMin -> GetText());
        double etaMuMax = std::stod(entryEtaMuMax -> GetText());
        int rebin = std::stod(entryRebin -> GetText());

        //string strTrackType1 = entryTrackType1 -> GetText();
        //string strTrackType2 = entryTrackType2 -> GetText();

        string strTrackType1 = trackCut.Data();
        string strTrackType2 = trackCut.Data();

        std::map<std::string, int> mapTrackType = {{"MFT-MCH-MID", 0}, {"MCH-MID", 3}, {"MCH", 4}};

        string cut = "mass > 0 && mass < 6";
        cut = cut + Form("&& pt > %f && pt < %f", ptDimuMin, ptDimuMax);

        if (strTrackType1 != "none" && strTrackType2 != "none") {
            cut = cut + Form("&& typeMu1 == %i && typeMu2 == %i", mapTrackType[strTrackType1], mapTrackType[strTrackType2]);
        } else if (strTrackType1 != "none" && strTrackType2 == "none") {
            cut = cut + Form("&& typeMu1 == %i", mapTrackType[strTrackType1]);
        } else {
            cut = cut + Form("&& typeMu2 == %i", mapTrackType[strTrackType2]);
        }

        cut = cut + Form("&& charge == %i", chargeDimu);
        cut = cut + Form("&& ptMu1 > %f && ptMu1 < %f && ptMu2 > %f && ptMu2 < %f", ptMuMin, ptMuMax, ptMuMin, ptMuMax);
        cut = cut + Form("&& etaMu1 > %f && etaMu1 < %f && etaMu2 > %f && etaMu2 < %f", etaMuMin, etaMuMax, etaMuMin, etaMuMax);

        if (var == "mass") {
            histVar = df.Filter(cut).Histo1D({"mass", Form(";%s;counts", var.Data()), 600, 0.0, 6.0}, var);
        } else {
            histVar = df.Filter(cut).Histo1D(var);
        }
        histVar -> Rebin(rebin);
        histVar -> SetTitle("");
        histVar -> GetXaxis() -> SetTitle(var);
        histVar -> GetYaxis() -> SetTitle("counts");
        histVar -> SetMarkerStyle(20);
        histVar -> SetMarkerSize(0.5);
        histVar -> SetMarkerColor(kBlack);
        histVar -> SetLineColor(kBlack);

        string xAxisName = histVar -> GetXaxis() -> GetTitle();

        canvasTopLeft -> GetCanvas() -> cd();
        gPad -> SetLogy(true);
        gStyle -> SetOptStat(false);
        histVar -> Draw("HEP");
        canvasTopLeft -> GetCanvas() -> Update();

        // Enable the button to add histograms
        bAddHist -> SetEnabled(true);
    }

    //--------------------------------------------//
    // Function to add the histogram to the list 
    // of the selected histograms
    void AddHistogram() {
        int chargeDimu = std::stod(entryChargeDimu->GetText());
        string histName = entryHistName -> GetText();

        // If the user does not select the name, the default one is used
        if (histName == "") {
            histName = histVar -> GetName();
        }

        // Check if the histogram name is already used
        if (count(histogramNames.begin(), histogramNames.end(), histName) > 0) {
            labelHistStatus -> SetTextColor(kBlue);
            labelHistStatus -> SetText(Form("'%s' already used, choose another name!", histName.c_str()));
            labelHistStatus -> MapWindow();
            this -> Layout();
        } else {
            histogramNames.push_back(histName);

            // Update label telling which hisogram is added
            labelHistStatus -> SetTextColor(kBlue);
            labelHistStatus -> SetText(Form("Histogram '%s' is added", histName.c_str()));
            labelHistStatus -> MapWindow();
            this -> Layout();

            // Adding histograms enalble the possibility to draw them,
            // before that the drawing is disables
            bDrawHistPerVar -> SetEnabled(true);
            bClear -> SetEnabled(true);

            histograms[histName] = histVar;
            histograms[histName] -> SetName(histName.c_str());

            string xAxisName = histograms[histName] -> GetXaxis() -> GetTitle();
            if (xAxisName == "mass") {
                (chargeDimu > 0) ? histDimuMassPP = histograms[histName] : (chargeDimu < 0) ? histDimuMassMM = histograms[histName] : histDimuMassPM = histograms[histName];
            }
            if (histDimuMassPM && histDimuMassPP && histDimuMassMM && histDimuMassPM -> GetEntries() > 0 && histDimuMassPP -> GetEntries() > 0 && histDimuMassMM -> GetEntries() > 0) {
                bBuildBkg -> SetEnabled(true);
            }
        }

        string xAxisName = histograms[histName] -> GetXaxis() -> GetTitle();
        // Add the mass histograms to the list of histograms that can be fitted
        if (xAxisName == "mass" && chargeDimu == 0) {
            comboPlotHistForFit -> AddEntry(histName.c_str(), 1);
            bFitPlot -> SetEnabled(true);
            bFit -> SetEnabled(true);
        }

        // Add the observable to the list if it does not exists
        if (!(comboPlotHist -> FindEntry(xAxisName.c_str()))) {
            comboPlotHist -> AddEntry(xAxisName.c_str(), histograms.size());
            if (xAxisName == "mass") {
                bFitPlot -> SetEnabled(true);
                bFit -> SetEnabled(true);
            }
        }
    }

    //--------------------------------------------//
    // Function to Draw selected histograms
    void DrawHistograms() {

        std::vector<string> xAxisTitle;
        for (auto &[name, hist] : histograms) {
            xAxisTitle.push_back(hist -> GetXaxis() -> GetTitle());
        }

        auto vars = df.GetColumnNames();
        std::vector<string> chosenVars;
        for (auto &var : vars) {
            if (count(xAxisTitle.begin(), xAxisTitle.end(), var) > 0) {
                chosenVars.push_back(var);
                if (var == "mass") {
                    bFit -> SetEnabled(true);
                    bFitPlot -> SetEnabled(true);
                }
            }
        }
        
        canvasTopRight -> GetCanvas() -> cd();
        canvasTopRight -> GetCanvas() -> Clear();

        if (int(chosenVars.size()) < 4) {
            canvasTopRight -> GetCanvas() -> Divide(int(chosenVars.size()), 1);
        } else if (int(chosenVars.size()) % 2 == 0) {
            canvasTopRight -> GetCanvas() -> Divide(int(chosenVars.size()) / 2, int(chosenVars.size()) / 2);
        } else {
            canvasTopRight -> GetCanvas() -> Divide(3, 2);
        }
        
        int iColumn = 1;
        for (auto &chosenVar : chosenVars) {
            canvasTopRight -> GetCanvas() -> cd(iColumn);
            gPad -> SetLogy(true);
            bool first = true;
            int colorCounter = histograms.size()-1;
            std::cout << colorCounter << std::endl;
            for (auto &[name, hist] : histograms) {
                std::cout << name << " ; " << colorCounter << std::endl;
                if (hist -> GetXaxis() -> GetTitle() == chosenVar) {
                    hist -> SetMarkerStyle(20);
                    hist -> SetMarkerColor(colorPalette[colorCounter]);
                    hist -> SetLineColor(colorPalette[colorCounter]);
                    hist -> Draw(first ? "HEP" : "HEP SAME");
                    first = false;
                    colorCounter--;
                }
            }
            if (removeBkg && chosenVar == "mass") {
                histDimuMassPPMM -> SetName("2 * #sqrt{PP * MM}");
                histDimuMassPPMM -> SetMarkerColor(kMagenta);
                histDimuMassPPMM -> SetLineColor(kMagenta);
                histDimuMassPPMM -> SetLineWidth(2);
                histDimuMassPPMM -> SetFillStyle(3001);
                histDimuMassPPMM -> SetFillColorAlpha(kMagenta, 0.5);
                histDimuMassPPMM -> Draw("H SAME");
            }

            gPad -> BuildLegend();
            iColumn++;
        }
        canvasTopRight -> GetCanvas() -> Update();
    }
    //--------------------------------------------//
    // Function to Draw selected histograms per variable
    void DrawHistogramsPerVar() {
        TString chosenVar = GetSelectedVariable(comboPlotHist);

        canvasTopRight -> GetCanvas() -> cd();
        canvasTopRight -> GetCanvas() -> Clear();

        // Check which histograms contains the chosen variable
        bool first = true;
        //int colorCounter = 0;
        int colorCounter = histograms.size()-1;
        gPad -> SetLogy(true);
        for (auto &[name, hist] : histograms) {
            string tmpHistName = hist -> GetXaxis() -> GetTitle();
            if (tmpHistName == chosenVar) {
                hist -> SetMarkerStyle(20);
                hist -> SetMarkerColor(colorPalette[colorCounter]);
                hist -> SetLineColor(colorPalette[colorCounter]);
                hist -> Draw(first ? "HEP" : "HEP SAME");
                first = false;
                colorCounter--;
            }
        }
        if (removeBkg && chosenVar == "mass") {
            histDimuMassPPMM -> SetName("2 * #sqrt{PP * MM}");
            histDimuMassPPMM -> SetMarkerColor(kMagenta);
            histDimuMassPPMM -> SetLineColor(kMagenta);
            histDimuMassPPMM -> SetLineWidth(2);
            histDimuMassPPMM -> SetFillStyle(3001);
            histDimuMassPPMM -> SetFillColorAlpha(kMagenta, 0.5);
            histDimuMassPPMM -> Draw("H SAME");
        }
        gPad -> BuildLegend();
        canvasTopRight -> GetCanvas() -> Update();
    }

    //--------------------------------------------//
    // Function to build the combinatorial background
    void BuildBackground() {
        int minBinLeftSb = histDimuMassPM -> FindBin(1);
        int maxBinLeftSb = histDimuMassPM -> FindBin(2);
        int minBinRightSb = histDimuMassPM -> FindBin(4);
        int maxBinRightSb = histDimuMassPM -> FindBin(5);

        int nBins = histDimuMassPM -> GetNbinsX();
        double minBin = histDimuMassPM -> GetBinLowEdge(1);
        double maxBin = histDimuMassPM -> GetBinLowEdge(nBins) + histDimuMassPM -> GetBinWidth(nBins);

        std::cout << histDimuMassPM -> GetNbinsX() << " [ " << histDimuMassPM -> GetBinCenter(1) << " , " << histDimuMassPM -> GetBinCenter(5) << " ]" << std::endl;
        std::cout << histDimuMassPP -> GetNbinsX() << " [ " << histDimuMassPP -> GetBinCenter(1) << " , " << histDimuMassPP -> GetBinCenter(5) << " ]" << std::endl;
        std::cout << histDimuMassMM -> GetNbinsX() << " [ " << histDimuMassMM -> GetBinCenter(1) << " , " << histDimuMassMM -> GetBinCenter(5) << " ]" << std::endl;

        histDimuMassPPMM = new TH1D("histDimuMassPPMM", "", nBins, minBin, maxBin);

        double PP, MM;
        for (int iBin = 0;iBin < nBins;iBin++) {
            PP = histDimuMassPP -> GetBinContent(iBin+1);
            MM = histDimuMassMM -> GetBinContent(iBin+1);
            histDimuMassPPMM -> SetBinContent(iBin+1, 2 * TMath::Sqrt(PP * MM));
            histDimuMassPPMM -> SetBinError(iBin+1, TMath::Sqrt(2 * TMath::Sqrt(PP * MM)));
        }

        double leftSbPM = histDimuMassPM -> Integral(minBinLeftSb, maxBinLeftSb);
        double rightSbPM = histDimuMassPM -> Integral(minBinRightSb, maxBinRightSb);
        double leftSbPP = histDimuMassPP -> Integral(minBinLeftSb, maxBinLeftSb);
        double rightSbPP = histDimuMassPP -> Integral(minBinRightSb, maxBinRightSb);
        double leftSbMM = histDimuMassMM -> Integral(minBinLeftSb, maxBinLeftSb);
        double rightSbMM = histDimuMassMM -> Integral(minBinRightSb, maxBinRightSb);
        double leftSbPPMM = histDimuMassPPMM -> Integral(minBinLeftSb, maxBinLeftSb);
        double rightSbPPMM = histDimuMassPPMM -> Integral(minBinRightSb, maxBinRightSb);
        histDimuMassPPMM -> Scale((leftSbPM + rightSbPM) / (leftSbPPMM + rightSbPPMM));

        bSubtractBkgOn -> SetEnabled(true);
        removeBkg = true;
        DrawHistogramsPerVar();
    }

    //--------------------------------------------//
    // Function to fit the data
    void FitData(bool doFit = false) {
        string nameHistToFit = (GetSelectedVariable(comboPlotHistForFit)).Data();
        double fitMin = atof(entryFitMin -> GetText());
        double fitMax = atof(entryFitMax -> GetText());
        double fitMeanMin = atof(entryFitMeanMin -> GetText());
        double fitMeanMax = atof(entryFitMeanMax -> GetText());
        double fitWidthMin = atof(entryFitWidthMin -> GetText());
        double fitWidthMax = atof(entryFitWidthMax -> GetText());

        if (nameHistToFit == "") {
            return;
        }

        canvasBottomLeft -> GetCanvas() -> cd();
        canvasBottomLeft -> GetCanvas() -> Clear();

        //histDimuMassToFit = histograms[nameHistToFit].GetPtr();
        histDimuMassToFit = (TH1D*) histograms[nameHistToFit].GetPtr() -> Clone("histDimuMassToFit");


        if (subtractBkg) {
            histDimuMassToFit -> Add(histDimuMassPPMM, -1);
            //subtractBkg = false;
        }

        // Cosmetics for the dimuon mass histogram
        histDimuMassToFit -> SetMarkerStyle(20);
        histDimuMassToFit -> SetMarkerColor(kBlack);
        histDimuMassToFit -> SetLineColor(kBlack);

        int nBins = histDimuMassToFit -> GetNbinsX();
        int minXbin = histDimuMassToFit -> FindBin(2);
        std::vector<double> binContents;
        bool hasNegative = false;
        for (int iBin = minXbin;iBin < nBins;iBin++) {
            binContents.push_back(histDimuMassToFit -> GetBinContent(iBin));
            if (histDimuMassToFit -> GetBinContent(iBin) < 0) {hasNegative = true;}
        }

        double tmpMinYval = 0;
        for (double binContent : binContents) {
            if (hasNegative) {
                if (binContent < 0) {
                    if (TMath::Abs(binContent) > TMath::Abs(tmpMinYval)) {
                            tmpMinYval = binContent;
                            std::cout << tmpMinYval << std::endl;
                    }
                }
            } else {
                tmpMinYval = 0;
            }
        }

        double minYval = 1.5 * tmpMinYval;
        double maxYval = 1.5 * (histDimuMassToFit -> GetBinContent(histDimuMassToFit -> GetXaxis() -> FindBin(3.1)));

        TH2D *histGridFit = new TH2D("histGridFit", "", 100, 2, 5, 100, minYval, maxYval);
        histGridFit -> GetXaxis() -> SetTitle("mass (GeV/#it{c}^{2})");
        histGridFit -> GetYaxis() -> SetRangeUser(minYval, maxYval);
        histGridFit -> Draw();

        gPad -> SetLogy(false);
        histDimuMassToFit -> SetMarkerColor(kBlack);
        histDimuMassToFit -> SetLineColor(kBlack);
        histDimuMassToFit -> GetXaxis() -> SetRangeUser(2, 5);
        histDimuMassToFit -> GetYaxis() -> SetRangeUser(minYval, maxYval);
        histDimuMassToFit -> Draw("EP SAME");

        if (doFit) {
            TF1 *funcFit = new TF1("funcFit", "gaus(0)+gaus(3)+pol2(6)", fitMin, fitMax);
            funcFit -> SetNpx(1000);
            funcFit -> SetParameter(0, 50);
            funcFit -> SetParLimits(1, fitMeanMin, fitMeanMax);
            funcFit -> SetParLimits(2, fitWidthMin, fitWidthMax);
            funcFit -> SetParameter(3, 20);
            funcFit -> FixParameter(4, 3.675);
            funcFit -> FixParameter(5, 0.099);
            histDimuMassToFit -> Fit(funcFit, "R0Q");

            TF1 *funcBkg = new TF1("funcBkg", "pol2", fitMin, fitMax);
            funcBkg -> SetNpx(1000);
            funcBkg -> SetParameter(0, funcFit -> GetParameter(6));
            funcBkg -> SetParameter(1, funcFit -> GetParameter(7));
            funcBkg -> SetParameter(2, funcFit -> GetParameter(8));
            funcBkg -> SetLineStyle(kDashed);
            funcBkg -> SetLineColor(kGray+2);

            TF1 *funcJpsi = new TF1("funcJpsi", "gaus", fitMin, fitMax);
            funcJpsi -> SetNpx(1000);
            funcJpsi -> SetParameter(0, funcFit -> GetParameter(0));
            funcJpsi -> SetParameter(1, funcFit -> GetParameter(1));
            funcJpsi -> SetParameter(2, funcFit -> GetParameter(2));
            funcJpsi -> SetLineStyle(kSolid);
            funcJpsi -> SetLineColor(kAzure+4);

            TF1 *funcPsi2s = new TF1("funcPsi2s", "gaus", fitMin, fitMax);
            funcPsi2s -> SetNpx(1000);
            funcPsi2s -> SetParameter(0, funcFit -> GetParameter(3));
            funcPsi2s -> SetParameter(1, funcFit -> GetParameter(4));
            funcPsi2s -> SetParameter(2, funcFit -> GetParameter(5));
            funcPsi2s -> SetLineStyle(kSolid);
            funcPsi2s -> SetLineColor(kGreen+2);

            funcBkg -> Draw("SAME");
            funcJpsi -> Draw("SAME");
            funcPsi2s -> Draw("SAME");
            funcFit -> Draw("SAME");

            TLegend *legendFit = new TLegend(0.65, 0.65, 0.80, 0.87);
            legendFit -> SetBorderSize(0);
            legendFit -> SetFillColor(10);
            legendFit -> SetFillStyle(1);
            legendFit -> SetLineStyle(0);
            legendFit -> SetLineColor(0);
            legendFit -> SetTextFont(42);
            legendFit -> SetTextSize(0.045);
            legendFit -> AddEntry(funcFit, "Fit", "L");
            legendFit -> AddEntry(funcJpsi, "J/#psi", "L");
            legendFit -> AddEntry(funcPsi2s, "#psi(2S)", "L");
            legendFit -> AddEntry(funcBkg, "Bkg.", "L");
            legendFit -> Draw();

            double chiSquare = funcFit -> GetChisquare();
            double ndf = funcFit -> GetNDF();

            double meanJpsi = funcFit -> GetParameter(1);
            double sigmaJpsi = funcFit -> GetParameter(2);
            double intRangeJpsiMin = meanJpsi - (3*sigmaJpsi);
            double intRangeJpsiMax = meanJpsi + (3*sigmaJpsi);

            nJpsi = (funcJpsi -> Integral(intRangeJpsiMin, intRangeJpsiMax)) / (histDimuMassToFit -> GetBinWidth(1));
            errJpsi = TMath::Sqrt(nJpsi);

            nBkgRangeJpsi = (funcBkg -> Integral(intRangeJpsiMin, intRangeJpsiMax)) / (histDimuMassToFit -> GetBinWidth(1));
            errBkgRangeJpsi = TMath::Sqrt(nBkgRangeJpsi);

            signfJpsi = nJpsi / TMath::Sqrt(nJpsi + nBkgRangeJpsi);

            double meanPsi2s = funcFit -> GetParameter(4);
            double sigmaPsi2s = funcFit -> GetParameter(5);
            double intRangePsi2sMin = meanPsi2s - (3*sigmaPsi2s);
            double intRangePsi2sMax = meanPsi2s + (3*sigmaPsi2s);

            nPsi2s = (funcPsi2s -> Integral(intRangePsi2sMin, intRangePsi2sMax)) / (histDimuMassToFit -> GetBinWidth(1));
            errPsi2s = TMath::Sqrt(nPsi2s);

            nBkgRangePsi2s = (funcBkg -> Integral(intRangePsi2sMin, intRangePsi2sMax)) / (histDimuMassToFit -> GetBinWidth(1));
            errBkgRangePsi2s = TMath::Sqrt(nBkgRangePsi2s);

            signfPsi2s = nPsi2s / TMath::Sqrt(nPsi2s + nBkgRangePsi2s);

            TLatex latexTitle;
            latexTitle.SetNDC();
            latexTitle.SetTextSize(0.04);
            latexTitle.SetTextFont(42);
            //latexTitle.DrawLatex(0.55, 0.60, Form("N. J/#psi^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangeJpsiMin, intRangeJpsiMax, nJpsi, errJpsi));
            //latexTitle.DrawLatex(0.55, 0.55, Form("N. Bkg.^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangeJpsiMin, intRangeJpsiMax, nBkgRangeJpsi, errBkgRangeJpsi));
            //latexTitle.DrawLatex(0.55, 0.50, Form("N. #psi(2S)^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangePsi2sMin, intRangePsi2sMax, nPsi2s, errPsi2s));
            //latexTitle.DrawLatex(0.55, 0.45, Form("N. Bkg.^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangePsi2sMin, intRangePsi2sMax, nBkgRangePsi2s, errBkgRangePsi2s));
            latexTitle.DrawLatex(0.62, 0.60, Form("#chi^{2} = %3.2f", chiSquare / ndf));
            latexTitle.DrawLatex(0.62, 0.55, Form("#it{N}_{J/#psi} = %1.0f #pm %1.0f", nJpsi, errJpsi));
            latexTitle.DrawLatex(0.62, 0.50, Form("#it{N}_{#psi(2S)} = %1.0f #pm %1.0f", nPsi2s, errPsi2s));
            if (!subtractBkg) { // do not show in case the backgorund is subtracted
                latexTitle.DrawLatex(0.62, 0.45, Form("#it{N}_{Bkg J/#psi} = %1.0f #pm %1.0f", nBkgRangeJpsi, errBkgRangeJpsi));
                latexTitle.DrawLatex(0.62, 0.40, Form("#it{N}_{Bkg #psi(2S)} = %1.0f #pm %1.0f", nBkgRangePsi2s, errBkgRangePsi2s));
            }


            bResults -> SetEnabled(true);
        }
        canvasBottomLeft -> GetCanvas() -> Update();
    }

    //--------------------------------------------//
    // Function to enable the background subtraction
    void EnableSubtractBkg () {
        bSubtractBkgOn -> SetEnabled(false);
        bSubtractBkgOff -> SetEnabled(true);
        subtractBkg = true;
    }

    //--------------------------------------------//
    // Function to disable the background subtraction
    void DisableSubtractBkg () {
        bSubtractBkgOn -> SetEnabled(true);
        bSubtractBkgOff -> SetEnabled(false);
        subtractBkg = false;
    }

    //--------------------------------------------//
    // Function to be called by the fit button
    void FitDataTrue() {
        FitData(true);
    }

    //--------------------------------------------//
    // Function to be called by the plot button
    void FitDataFalse() {
        FitData(false);
    }

    //--------------------------------------------//
    // Function to fit the data
    void UpdateResults() {

        canvasBottomRight -> GetCanvas() -> cd();
        canvasBottomRight -> GetCanvas() -> Clear();

        histResults -> GetYaxis() -> SetRangeUser(0.01, nJpsi * 10);
        histResults -> GetXaxis() -> SetLabelSize(0.07);
        histResults -> SetBinContent(1, nJpsi);
        histResults -> SetBinError(1, errJpsi);
        histResults -> SetBinContent(2, nPsi2s);
        histResults -> SetBinError(2, errPsi2s);
        histResults -> SetBinContent(3, signfJpsi);
        histResults -> SetBinError(3, 0);
        histResults -> SetBinContent(4, signfPsi2s);
        histResults -> SetBinError(4, 0);
        histResults -> SetBinContent(5, nPsi2s / nJpsi);
        histResults -> SetBinError(5, 0);
        histResults -> Draw("BAR");

        latexResults.DrawLatex(0.65, 0.85, Form("#it{N}_{J/#psi} = %3.0f", nJpsi));
        latexResults.DrawLatex(0.65, 0.80, Form("#it{N}_{#psi(2S)} = %3.0f", nPsi2s));
        latexResults.DrawLatex(0.65, 0.75, Form("#sigma_{J/#psi} = %3.2f", signfJpsi));
        latexResults.DrawLatex(0.65, 0.70, Form("#sigma_{#psi(2S)} = %3.2f", signfPsi2s));
        latexResults.DrawLatex(0.65, 0.63, Form("#psi(2S) / J/#psi = %3.2f", nPsi2s / nJpsi));
    
        canvasBottomRight -> GetCanvas() -> Modified();
        canvasBottomRight -> GetCanvas() -> Update();
        gSystem -> ProcessEvents();
    }

    //--------------------------------------------//
    // Function to clear the list of the histograms
    void ClearHistogram() {
        bDrawHistPerVar -> SetEnabled(false);
        bClear -> SetEnabled(false);
        bSubtractBkgOn -> SetEnabled(false);
        bSubtractBkgOff -> SetEnabled(false);
        bFit -> SetEnabled(false);
        bFitPlot -> SetEnabled(false);
        bResults -> SetEnabled(false);

        removeBkg = false;
        subtractBkg = false;

        histograms.clear();
        histogramNames.clear();
        comboPlotHist -> RemoveAll();
        comboPlotHistForFit -> RemoveAll();
        DrawHistograms();
    }

    void Close() {
        main -> DeleteWindow();
        gApplication -> Terminate(0);
    }

    virtual ~MyGUI() { Cleanup(); }

    ClassDef(MyGUI, 0);
};

#ifdef __MAKECINT__
#pragma link C++ class MyGUI+;
#endif

void masterclass() {
    TFile *fIn = TFile::Open("data.root");
    TTree *tree = (TTree*)fIn -> Get("data");
    ROOT::RDataFrame df(*tree);

    new MyGUI(gClient -> GetRoot(), df);
}