#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGToolTip.h>
#include <TRootEmbeddedCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TRandom.h>

class MyGUI : public TGMainFrame {

private:
    TGTextEntry *entryHistToFit;
    TGTextEntry *entryPtDimuMin, *entryPtDimuMax, *entryChargeDimu;
    TGTextEntry *entryPtMuMin, *entryPtMuMax, *entryEtaMuMin, *entryEtaMuMax, *entryTrackType;
    TGTextEntry *entryHistName;
    TGTextEntry *entryFitMeanMin, *entryFitMeanMax, *entryFitWidthMin, *entryFitWidthMax, *entryFitMin, *entryFitMax;

    TGLabel *labelHistStatus, *labelHistFitStatus;

    TRootEmbeddedCanvas *canvasTopLeft, *canvasTopRight, *canvasBottomLeft, *canvasBottomRight;

    TGComboBox  *comboVar;

    TGCheckButton *cbChargePlus, *cbChargeMinus, *cbChargeZero;

    TGTextButton *bAddHist, *bDraw, *bClear;
    TGTextButton *bBuildBkg;
    TGTextButton *bFitPlot, *bSubtractBkg, *bFit, *bResults;

    ROOT::RDF::RNode df;

    ROOT::RDF::RResultPtr<TH1D> histVar;
    ROOT::RDF::RResultPtr<TH1D> histDimuMassPM, histDimuMassPP, histDimuMassMM;

    std::map<std::string, ROOT::RDF::RResultPtr<TH1D>> histograms;

    std::vector<string> histogramNames;

    TLatex latexTitleCanvas;

    TH1D *histResults, *histDimuMassToFit, *histDimuMassPPMM;

    double nJpsi, errJpsi, nPsi2s, errPsi2s, nBkgRangeJpsi, errBkgRangeJpsi, nBkgRangePsi2s, errBkgRangePsi2s, signfJpsi, signfPsi2s;
    bool removeBkg, subtractBkg;
    Color_t colorPalette[6] = {kBlack, kRed+1, kAzure+2, kGreen+1, kOrange+7, kViolet};



public:
    MyGUI(const TGWindow *p, ROOT::RDF::RNode rdf) : TGMainFrame(p, 900, 500), df(rdf) {

        // ===========================
        // MAIN FRAME
        // ===========================
        TGHorizontalFrame *main = new TGHorizontalFrame(this); 
        
        AddFrame(main, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        // =====================================================
        //  LEFT FRAME
        // =====================================================
        TGVerticalFrame *left = new TGVerticalFrame(main, 300);
        main->AddFrame(left, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,5));

        // ---------- SHOW (TOP) ----------
        TGGroupFrame *showFrame = new TGGroupFrame(left, "Data selection");
        left -> AddFrame(showFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGCompositeFrame *tableShow = new TGCompositeFrame(showFrame);
        tableShow -> SetLayoutManager(new TGTableLayout(tableShow, 4, 4));
        showFrame -> AddFrame(tableShow, new TGLayoutHints(kLHintsExpandX));
        
        tableShow -> AddFrame(new TGLabel(tableShow, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        tableShow -> AddFrame(new TGLabel(tableShow, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        tableShow -> AddFrame(new TGLabel(tableShow, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));

        // Muon pT
        tableShow -> AddFrame(new TGLabel(tableShow, "pT muone"), new TGTableLayoutHints(0,1,1,2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtMuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtMuMin -> SetText("0");
        entryPtMuMin -> Resize(80,22);
        entryPtMuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtMuMin, new TGTableLayoutHints(1,2,1,2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtMuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtMuMax -> SetText("100");
        entryPtMuMax -> Resize(80,22);
        entryPtMuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtMuMax, new TGTableLayoutHints(2,3,1,2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Muon eta
        tableShow -> AddFrame(new TGLabel(tableShow, "eta muone"), new TGTableLayoutHints(0,1,2,3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryEtaMuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryEtaMuMin -> SetText("2.5");
        entryEtaMuMin -> Resize(80,22);
        entryEtaMuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryEtaMuMin, new TGTableLayoutHints(1,2,2,3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryEtaMuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryEtaMuMax -> SetText("4");
        entryEtaMuMax -> Resize(80,22);
        entryEtaMuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryEtaMuMax, new TGTableLayoutHints(2,3,2,3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Dimuon pt
        tableShow -> AddFrame(new TGLabel(tableShow, "pT dimuone"), new TGTableLayoutHints(0,1,3,4, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtDimuMin = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtDimuMin -> SetText("0");
        entryPtDimuMin -> Resize(80,22);
        entryPtDimuMin -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtDimuMin, new TGTableLayoutHints(1,2,3,4, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtDimuMax = new TGTextEntry(tableShow, new TGTextBuffer(10));
        entryPtDimuMax -> SetText("100");
        entryPtDimuMax -> Resize(80,22);
        entryPtDimuMax -> SetAlignment(kTextCenterX);
        tableShow -> AddFrame(entryPtDimuMax, new TGTableLayoutHints(2,3,3,4, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        tableShow -> MapSubwindows();
        tableShow -> Resize();

        AddInput(showFrame, "Carica dimuone:", entryChargeDimu, "0");
        AddInput(showFrame, "Type muone:", entryTrackType, "3");

        TGHorizontalFrame *fVar = new TGHorizontalFrame(showFrame);
        fVar->AddFrame(new TGLabel(fVar, "Variabile:"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        comboVar = new TGComboBox(fVar);
        comboVar->Resize(180, 22);
        fVar->AddFrame(comboVar, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));
        showFrame->AddFrame(fVar, new TGLayoutHints(kLHintsExpandX));

        FillComboFromRDF();

        TGTextButton *bShow = new TGTextButton(showFrame, "Show data");
        showFrame->AddFrame(bShow, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));


        // ---------- ANALYSIS (MID) ----------
        TGGroupFrame *analysisFrame = new TGGroupFrame(left, "Data analysis");
        left->AddFrame(analysisFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGHorizontalFrame *frameAddHist = new TGHorizontalFrame(analysisFrame);

        bAddHist = new TGTextButton(frameAddHist, "Add");
        bAddHist -> SetEnabled(false);
        frameAddHist->AddFrame(bAddHist, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,10,5,5));

        entryHistName = new TGTextEntry(frameAddHist, new TGTextBuffer(30));
        entryHistName -> Resize(160, 22);
        entryHistName -> SetAlignment(kTextLeft);
        entryHistName -> SetToolTipText("Hist. name");
        frameAddHist -> AddFrame(entryHistName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        analysisFrame -> AddFrame(frameAddHist, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        labelHistStatus = new TGLabel(analysisFrame, "No histogram added");
        analysisFrame -> AddFrame(labelHistStatus, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        TGHorizontalFrame *frameDrawHists = new TGHorizontalFrame(analysisFrame);

        bDraw = new TGTextButton(frameDrawHists, "Plot selected histograms");
        bDraw -> SetEnabled(false);
        frameDrawHists -> AddFrame(bDraw, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

        bClear = new TGTextButton(frameDrawHists, "Clear all histograms");
        bClear -> SetEnabled(false);
        frameDrawHists -> AddFrame(bClear, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

        analysisFrame -> AddFrame(frameDrawHists, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        bBuildBkg = new TGTextButton(analysisFrame, "Build background");
        bBuildBkg -> SetEnabled(false);
        bBuildBkg -> SetToolTipText("Se disabilitato, per abilitare selezionare mass\ne 3 cariche per il dimuone = 0, +2, -2");
        analysisFrame -> AddFrame(bBuildBkg, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));
        removeBkg = false;

        // ---------- FIT (BOTTOM) ----------
        TGGroupFrame *fitFrame = new TGGroupFrame(left, "Fit data");
        left->AddFrame(fitFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        AddInput(fitFrame, "Histogram to fit", entryHistToFit, "");

        labelHistFitStatus = new TGLabel(fitFrame, "No histogram selected");
        fitFrame -> AddFrame(labelHistFitStatus, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        bFitPlot = new TGTextButton(fitFrame, "Plot histogram to fit");
        fitFrame -> AddFrame(bFitPlot, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));
        bFitPlot -> SetEnabled(false);

        bSubtractBkg = new TGTextButton(fitFrame, "Subtract background");
        bSubtractBkg -> SetToolTipText("Se disabilitato, e' necessario Build background");
        fitFrame -> AddFrame(bSubtractBkg, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));
        bSubtractBkg -> SetEnabled(false);
        subtractBkg = false;

        TGCompositeFrame *tableFit = new TGCompositeFrame(fitFrame);
        tableFit -> SetLayoutManager(new TGTableLayout(tableFit, 4, 4));
        fitFrame -> AddFrame(tableFit, new TGLayoutHints(kLHintsExpandX));
        
        tableFit -> AddFrame(new TGLabel(tableFit, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        tableFit -> AddFrame(new TGLabel(tableFit, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        tableFit -> AddFrame(new TGLabel(tableFit, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));

        // Min fit range
        tableFit -> AddFrame(new TGLabel(tableFit, "Range"), new TGTableLayoutHints(0,1,1,2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMin -> SetText("2");
        entryFitMin -> Resize(80,22);
        entryFitMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMin, new TGTableLayoutHints(1,2,1,2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMax -> SetText("5");
        entryFitMax -> Resize(80,22);
        entryFitMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMax, new TGTableLayoutHints(2,3,1,2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Mean fit
        tableFit -> AddFrame(new TGLabel(tableFit, "Signal mean"), new TGTableLayoutHints(0,1,2,3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitMeanMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMeanMin -> SetText("2.95");
        entryFitMeanMin -> Resize(80,22);
        entryFitMeanMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMeanMin, new TGTableLayoutHints(1,2,2,3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitMeanMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitMeanMax -> SetText("3.3");
        entryFitMeanMax -> Resize(80,22);
        entryFitMeanMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitMeanMax, new TGTableLayoutHints(2,3,2,3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Width fit
        tableFit -> AddFrame(new TGLabel(tableFit, "Signal width"), new TGTableLayoutHints(0,1,3,4, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryFitWidthMin = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitWidthMin -> SetText("0");
        entryFitWidthMin -> Resize(80,22);
        entryFitWidthMin -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitWidthMin, new TGTableLayoutHints(1,2,3,4, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryFitWidthMax = new TGTextEntry(tableFit, new TGTextBuffer(10));
        entryFitWidthMax -> SetText("0.1");
        entryFitWidthMax -> Resize(80,22);
        entryFitWidthMax -> SetAlignment(kTextCenterX);
        tableFit -> AddFrame(entryFitWidthMax, new TGTableLayoutHints(2,3,3,4, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        tableFit -> MapSubwindows();
        tableFit -> Resize();


        bFit = new TGTextButton(fitFrame, "Fit data");
        fitFrame -> AddFrame(bFit, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));
        bFit -> SetEnabled(false);

        bResults = new TGTextButton(fitFrame, "Add to results");
        fitFrame -> AddFrame(bResults, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));
        bResults -> SetEnabled(false);

        // =====================================================
        // RIGHT FRAME
        // =====================================================
        latexTitleCanvas.SetNDC();
        latexTitleCanvas.SetTextSize(0.05);
        latexTitleCanvas.SetTextFont(42);

        TGVerticalFrame *right = new TGVerticalFrame(main, 600);
        main -> AddFrame(right, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,5));

        TGHorizontalFrame *topRow = new TGHorizontalFrame(right);
        right->AddFrame(topRow, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        canvasTopLeft = new TRootEmbeddedCanvas("canvasTopLeft", topRow, 500, 400);
        topRow->AddFrame(canvasTopLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        latexTitleCanvas.DrawLatex(0.40, 0.95, "Data selection");

        canvasTopRight = new TRootEmbeddedCanvas("canvasTopRight", topRow, 500, 400);
        topRow->AddFrame(canvasTopRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        latexTitleCanvas.DrawLatex(0.40, 0.95, "Data analysis");


        TGHorizontalFrame *bottomRow = new TGHorizontalFrame(right);
        right->AddFrame(bottomRow, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        canvasBottomLeft = new TRootEmbeddedCanvas("canvasBottomLeft", bottomRow, 500, 400);
        bottomRow->AddFrame(canvasBottomLeft, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        latexTitleCanvas.DrawLatex(0.40, 0.95, "Fit data");

        canvasBottomRight = new TRootEmbeddedCanvas("canvasBottomRight", bottomRow, 500, 400);
        bottomRow->AddFrame(canvasBottomRight, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        canvasBottomRight -> GetCanvas() -> cd();
        canvasBottomRight -> GetCanvas() -> Clear();

        gStyle -> SetOptStat(false);
        gPad -> SetLogy(true);

        histResults = new TH1D("histResults", "Analysis results", 4, 0, 4);
        histResults -> GetXaxis() -> SetLabelSize(0.07);
        histResults -> GetXaxis() -> SetBinLabel(1, "N. J/#psi");
        histResults -> GetXaxis() -> SetBinLabel(2, "N. #psi(2S)");
        histResults -> GetXaxis() -> SetBinLabel(3, "Signf. J/#psi");
        histResults -> GetXaxis() -> SetBinLabel(4, "Signf. #psi(2S)");
        histResults -> GetYaxis() -> SetRangeUser(0.1, 1e3);
        histResults -> Draw("H");

        TLine *line3sigma = new TLine(2, 3, 4, 3);
        line3sigma -> SetLineColor(kRed+1);
        line3sigma -> Draw("SAME");

        canvasBottomRight -> GetCanvas() -> Update();

        bShow -> Connect("Clicked()", "MyGUI", this, "DrawDataset()");
        bAddHist -> Connect("Clicked()", "MyGUI", this, "AddHistogram()");
        bBuildBkg -> Connect("Clicked()", "MyGUI", this, "BuildBackground()");
        bSubtractBkg -> Connect("Clicked()", "MyGUI", this, "EnableSubtractBkg()");
        bFitPlot -> Connect("Clicked()", "MyGUI", this, "FitDataFalse()");
        bFit -> Connect("Clicked()", "MyGUI", this, "FitDataTrue()");
        bResults -> Connect("Clicked()", "MyGUI", this, "UpdateResults()");
        bDraw -> Connect("Clicked()", "MyGUI", this, "DrawHistograms()");
        bClear -> Connect("Clicked()", "MyGUI", this, "ClearHistogram()");

        SetWindowName("Analysis prototype");
        MapSubwindows();
        Resize();
        MapWindow();
    }

    //-----------------------------------//
    // Add input to the frame
    void AddInput(TGCompositeFrame *parent, const char *label, TGTextEntry *&entry, const char *def) {

        TGHorizontalFrame *frame = new TGHorizontalFrame(parent);
        frame->AddFrame(new TGLabel(frame, label), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10,5,5,5));

        entry = new TGTextEntry(frame, new TGTextBuffer(10));
        entry->SetText(def);
        frame->AddFrame(entry, new TGLayoutHints(kLHintsExpandX, 5,10,5,5));

        parent->AddFrame(frame, new TGLayoutHints(kLHintsExpandX));
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

        comboVar -> Select(0);
    }

    //--------------------------------------------//
    // Function to select the variables to show
    TString GetSelectedVariable() const {

        TGTextLBEntry *varName = (TGTextLBEntry*) comboVar -> GetSelectedEntry();

        if (!varName) return "";
        return varName -> GetText() -> GetString();
    }

    //--------------------------------------------//
    // Function to draw the data in the canvas
    void DrawDataset() {
        TString var = GetSelectedVariable();
        if (var.IsNull()) return;
        
        double ptDimuMin = std::stod(entryPtDimuMin->GetText());
        double ptDimuMax = std::stod(entryPtDimuMax->GetText());
        int chargeDimu = std::stod(entryChargeDimu->GetText());
        double ptMuMin = std::stod(entryPtMuMin->GetText());
        double ptMuMax = std::stod(entryPtMuMax->GetText());
        double etaMuMin = std::stod(entryEtaMuMin->GetText());
        double etaMuMax = std::stod(entryEtaMuMax->GetText());
        int trackTypeMu = std::stod(entryTrackType->GetText());

        string cut = "mass > 0 && mass < 6.5";
        cut = cut + Form("&& pt > %f && pt < %f", ptDimuMin, ptDimuMax);
        cut = cut + Form("&& typeMu1 == %i && typeMu2 == %i", trackTypeMu, trackTypeMu);
        cut = cut + Form("&& charge == %i", chargeDimu);
        cut = cut + Form("&& ptMu1 > %f && ptMu1 < %f && ptMu2 > %f && ptMu2 < %f", ptMuMin, ptMuMax, ptMuMin, ptMuMax);
        cut = cut + Form("&& etaMu1 > %f && etaMu1 < %f && etaMu2 > %f && etaMu2 < %f", etaMuMin, etaMuMax, etaMuMin, etaMuMax);

        histVar = df.Filter(cut).Histo1D(var);
        histVar -> SetTitle("");
        histVar -> GetXaxis() -> SetRangeUser(0, 6);
        histVar -> GetXaxis() -> SetTitle(var);
        histVar -> GetYaxis() -> SetTitle("counts");
        histVar -> SetMarkerStyle(20);
        histVar -> SetMarkerColor(kBlack);
        histVar -> SetLineColor(kBlack);

        string xAxisName = histVar -> GetXaxis() -> GetTitle();
        if (xAxisName == "mass") {histVar -> Rebin(2);}

        canvasTopLeft -> GetCanvas() -> cd();
        gPad -> SetLogy(true);
        gStyle -> SetOptStat(false);
        histVar -> Draw("EP");
        latexTitleCanvas.DrawLatex(0.40, 0.95, "Data selection");
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
            bDraw -> SetEnabled(true);
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
            int colorCounter = 0;
            for (auto &[name, hist] : histograms) {
                if (hist -> GetXaxis() -> GetTitle() == chosenVar) {
                    hist -> SetMarkerStyle(20);
                    hist -> SetMarkerColor(colorPalette[colorCounter]);
                    hist -> SetLineColor(colorPalette[colorCounter]);
                    hist -> Draw(first ? "EP" : "EP SAME");
                    first = false;
                    colorCounter++;
                }
            }
            if (removeBkg && chosenVar == "mass") {
                histDimuMassPPMM -> SetName("2 * #sqrt{PP * MM}");
                histDimuMassPPMM -> SetMarkerColor(kMagenta);
                histDimuMassPPMM -> SetLineColor(kMagenta);
                histDimuMassPPMM -> SetLineWidth(2);
                histDimuMassPPMM -> Draw("H SAME");
            }

            gPad -> BuildLegend();
            iColumn++;
        }
        latexTitleCanvas.DrawLatex(0.40, 0.95, "Data analysis");
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

        histDimuMassPPMM = new TH1D("histDimuMassPPMM", "", nBins, minBin, maxBin);

        int PP, MM;
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

        bSubtractBkg -> SetEnabled(true);
        removeBkg = true;
        DrawHistograms();
    }

    //--------------------------------------------//
    // Function to fit the data
    void FitData(bool doFit = false) {
        string nameHistToFit = entryHistToFit -> GetText();
        double fitMin = atof(entryFitMin -> GetText());
        double fitMax = atof(entryFitMax -> GetText());
        double fitMeanMin = atof(entryFitMeanMin -> GetText());
        double fitMeanMax = atof(entryFitMeanMax -> GetText());
        double fitWidthMin = atof(entryFitWidthMin -> GetText());
        double fitWidthMax = atof(entryFitWidthMax -> GetText());

        if (nameHistToFit == "") {
            labelHistFitStatus -> SetTextColor(kBlue);
            labelHistFitStatus -> SetText("No histogram selected, select one!!!");
            labelHistFitStatus -> MapWindow();
            this -> Layout();

        } else {
            labelHistFitStatus -> SetTextColor(kBlue);
            labelHistFitStatus -> SetText(Form("'%s' added!", nameHistToFit.c_str()));
            labelHistFitStatus -> MapWindow();
            this -> Layout();

            canvasBottomLeft -> GetCanvas() -> cd();
            canvasBottomLeft -> GetCanvas() -> Clear();

            histDimuMassToFit = histograms[nameHistToFit].GetPtr();

            if (subtractBkg) {
                histDimuMassToFit -> Add(histDimuMassPPMM, -1);
                subtractBkg = false;
            }

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
            double maxYval = 1.5 * (histDimuMassToFit -> GetBinContent(histDimuMassToFit -> FindBin(3.1)));

            std::cout << minYval << " " << maxYval << std::endl;

            histDimuMassToFit -> SetMarkerColor(kBlack);
            histDimuMassToFit -> GetXaxis() -> SetRangeUser(2, 5);
            histDimuMassToFit -> GetYaxis() -> SetRangeUser(minYval, maxYval);
            histDimuMassToFit -> Draw("EP");

            if (doFit) {
                TF1 *funcFit = new TF1("funcFit", "gaus(0)+gaus(3)+pol2(6)", fitMin, fitMax);
                funcFit -> SetNpx(1000);
                funcFit -> SetParameter(0, 50);
                funcFit -> SetParLimits(1, fitMeanMin, fitMeanMax);
                funcFit -> SetParLimits(2, fitWidthMin, fitWidthMax);
                funcFit -> SetParameter(3, 20);
                funcFit -> FixParameter(4, 3.686);
                funcFit -> FixParameter(5, 0.07);
                histDimuMassToFit -> Fit(funcFit, "R0");

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
                latexTitle.DrawLatex(0.55, 0.60, Form("N. J/#psi^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangeJpsiMin, intRangeJpsiMax, nJpsi, errJpsi));
                latexTitle.DrawLatex(0.55, 0.55, Form("N. Bkg.^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangeJpsiMin, intRangeJpsiMax, nBkgRangeJpsi, errBkgRangeJpsi));
                latexTitle.DrawLatex(0.55, 0.50, Form("N. #psi(2S)^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangePsi2sMin, intRangePsi2sMax, nPsi2s, errPsi2s));
                latexTitle.DrawLatex(0.55, 0.45, Form("N. Bkg.^{%3.2f - %3.2f} = %1.0f #pm %1.0f", intRangePsi2sMin, intRangePsi2sMax, nBkgRangePsi2s, errBkgRangePsi2s));

                bResults -> SetEnabled(true);
            }
        }

        latexTitleCanvas.DrawLatex(0.40, 0.95, "Fit data");
        canvasBottomLeft -> GetCanvas() -> Update();
    }

    //--------------------------------------------//
    // Function to enable the background subtraction
    void EnableSubtractBkg () {
        subtractBkg = true;
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

        canvasBottomLeft -> GetCanvas() -> cd();
        canvasBottomLeft -> GetCanvas() -> Clear();

        histResults -> GetXaxis() -> SetLabelSize(0.07);
        histResults -> SetBinContent(1, nJpsi);
        histResults -> SetBinError(1, errJpsi);
        histResults -> SetBinContent(2, nPsi2s);
        histResults -> SetBinError(2, errPsi2s);
        histResults -> SetBinContent(3, signfJpsi);
        histResults -> SetBinError(3, 0);
        histResults -> SetBinContent(4, signfPsi2s);
        histResults -> SetBinError(4, 0);
        histResults -> Draw("H");

        TLine *line3sigma = new TLine(2, 3, 4, 3);
        line3sigma -> SetLineColor(kRed+1);
        line3sigma -> Draw("SAME");

        canvasBottomRight -> GetCanvas() -> Update();
    }

    //--------------------------------------------//
    // Function to clear the list of the histograms
    void ClearHistogram() {
        bDraw -> SetEnabled(false);
        bClear -> SetEnabled(false);
        bSubtractBkg -> SetEnabled(false);
        bFit -> SetEnabled(false);
        bFitPlot -> SetEnabled(false);
        bResults -> SetEnabled(false);

        removeBkg = false;
        subtractBkg = false;

        histograms.clear();
        histogramNames.clear();
        /*if (histDimuMassPM && histDimuMassPP && histDimuMassMM && histDimuMassPM -> GetEntries() > 0 && histDimuMassPP -> GetEntries() > 0 && histDimuMassMM -> GetEntries() > 0) {
            histDimuMassPM -> Reset();
            histDimuMassPP -> Reset();
            histDimuMassMM -> Reset();
        }
        if (histDimuMassToFit && histDimuMassPPMM && histDimuMassToFit -> GetEntries() > 0 && histDimuMassPPMM -> GetEntries() > 0) {
            histDimuMassToFit -> Reset();
            histDimuMassPPMM -> Reset();
        }*/
        DrawHistograms();
    }

    virtual ~MyGUI() { Cleanup(); }

    ClassDef(MyGUI, 0);
};

#ifdef __MAKECINT__
#pragma link C++ class MyGUI+;
#endif

void prototype() {

    TFile *fIn = TFile::Open("dataMasterclass.root");
    TTree *tree = (TTree*)fIn -> Get("data");
    ROOT::RDataFrame df(*tree);

    new MyGUI(gClient -> GetRoot(), df);
}