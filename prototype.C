#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TRootEmbeddedCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TRandom.h>

class MyGUI : public TGMainFrame {

private:
    TGTextEntry *entryPtDimuMin, *entryPtDimuMax, *entryChargeDimu;
    TGTextEntry *entryPtMuMin, *entryPtMuMax, *entryEtaMuMin, *entryEtaMuMax, *entryTrackType;
    TGTextEntry *entryHistName;
    TGTextEntry *fitMin, *fitMax;
    TGLabel *labelHistStatus;
    TRootEmbeddedCanvas *canvasTop, *canvasBottom;
    TGComboBox  *comboVar, *comboHists;

    TGCheckButton *cbChargePlus, *cbChargeMinus, *cbChargeZero;

    TGTextButton *bAddHist, *bDraw, *bClear;

    ROOT::RDF::RNode df;
    ROOT::RDF::RResultPtr<TH1D> histVar;
    ROOT::RDF::RResultPtr<TH1D> histDimuMassPM, histDimuMassPP, histDimuMassMM;

    // Selected histograms
    std::map<std::string, ROOT::RDF::RResultPtr<TH1D>> histograms;
    std::vector<string> histogramNames;

    // Checkbox associati
    std::map<std::string, TGCheckButton*> histChecks;

public:
    MyGUI(const TGWindow *p, ROOT::RDF::RNode rdf) : TGMainFrame(p, 900, 500), df(rdf) {

        // ===========================
        // FRAME PRINCIPALE
        // ===========================
        TGHorizontalFrame *main = new TGHorizontalFrame(this); 
        
        AddFrame(main, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        // =====================================================
        //  FRAME SINISTRO (SHOW + FIT)
        // =====================================================
        TGVerticalFrame *left = new TGVerticalFrame(main, 300);
        main->AddFrame(left, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,5));

        // ---------- SHOW (TOP) ----------
        TGGroupFrame *showFrame = new TGGroupFrame(left, "Data selection");
        left -> AddFrame(showFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGCompositeFrame *table = new TGCompositeFrame(showFrame);
        table -> SetLayoutManager(new TGTableLayout(table, 4, 4));
        showFrame -> AddFrame(table, new TGLayoutHints(kLHintsExpandX));
        
        table -> AddFrame(new TGLabel(table, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        table -> AddFrame(new TGLabel(table, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        table -> AddFrame(new TGLabel(table, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));

        // Muon pT
        table -> AddFrame(new TGLabel(table, "pT muone"), new TGTableLayoutHints(0,1,1,2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtMuMin = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtMuMin -> SetText("0");
        entryPtMuMin -> Resize(80,22);
        entryPtMuMin -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryPtMuMin, new TGTableLayoutHints(1,2,1,2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtMuMax = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtMuMax -> SetText("100");
        entryPtMuMax -> Resize(80,22);
        entryPtMuMax -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryPtMuMax, new TGTableLayoutHints(2,3,1,2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Muon eta
        table -> AddFrame(new TGLabel(table, "eta muone"), new TGTableLayoutHints(0,1,2,3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryEtaMuMin = new TGTextEntry(table, new TGTextBuffer(10));
        entryEtaMuMin -> SetText("2.5");
        entryEtaMuMin -> Resize(80,22);
        entryEtaMuMin -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryEtaMuMin, new TGTableLayoutHints(1,2,2,3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryEtaMuMax = new TGTextEntry(table, new TGTextBuffer(10));
        entryEtaMuMax -> SetText("4");
        entryEtaMuMax -> Resize(80,22);
        entryEtaMuMax -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryEtaMuMax, new TGTableLayoutHints(2,3,2,3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Dimuon pt
        table -> AddFrame(new TGLabel(table, "pT dimuone"), new TGTableLayoutHints(0,1,3,4, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtDimuMin = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtDimuMin -> SetText("0");
        entryPtDimuMin -> Resize(80,22);
        entryPtDimuMin -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryPtDimuMin, new TGTableLayoutHints(1,2,3,4, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtDimuMax = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtDimuMax -> SetText("100");
        entryPtDimuMax -> Resize(80,22);
        entryPtDimuMax -> SetAlignment(kTextCenterX);
        table -> AddFrame(entryPtDimuMax, new TGTableLayoutHints(2,3,3,4, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        table -> MapSubwindows();
        table -> Resize();

        
        AddInput(showFrame, "Carica dimuone:", entryChargeDimu, "0");
        AddInput(showFrame, "Type muone:", entryTrackType, "3");


        //AddInput(showFrame, "Pt min dimuone:", entryPtDimuMin, "0");
        //AddInput(showFrame, "Pt max dimuone:", entryPtDimuMax, "100");
        //AddInput(showFrame, "Eta  min muone:", entryEtaMuMin, "2.5");
        //AddInput(showFrame, "Eta  max muone:", entryEtaMuMax, "4");


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
        TGGroupFrame *analysisFrame = new TGGroupFrame(left, "Analysis");
        left->AddFrame(analysisFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGHorizontalFrame *frameAddHist = new TGHorizontalFrame(analysisFrame);

        bAddHist = new TGTextButton(frameAddHist, "Add");
        bAddHist -> SetEnabled(false);
        frameAddHist->AddFrame(bAddHist, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,10,5,5));

        entryHistName = new TGTextEntry(frameAddHist, new TGTextBuffer(30));
        entryHistName->Resize(160, 22);
        entryHistName->SetAlignment(kTextLeft);
        entryHistName->SetToolTipText("Hist. name");
        frameAddHist->AddFrame(entryHistName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        analysisFrame->AddFrame(frameAddHist, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        labelHistStatus = new TGLabel(analysisFrame, "No histogram added");
        analysisFrame->AddFrame(labelHistStatus, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        bDraw = new TGTextButton(analysisFrame, "Plot selected histograms");
        bDraw -> SetEnabled(false);
        analysisFrame->AddFrame(bDraw, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

        bClear = new TGTextButton(analysisFrame, "Clear all histograms");
        bClear -> SetEnabled(false);
        analysisFrame->AddFrame(bClear, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

        // ---------- FIT (BOTTOM) ----------
        TGGroupFrame *fitFrame = new TGGroupFrame(left, "Fit");
        left->AddFrame(fitFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        AddInput(fitFrame, "Fit Min:", fitMin, "-2");
        AddInput(fitFrame, "Fit Max:", fitMax, "2");

        TGTextButton *bFit = new TGTextButton(fitFrame, "Fit data");
        fitFrame->AddFrame(bFit, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

        /*TGHorizontalFrame *fCombo = new TGHorizontalFrame(fitFrame);
        fCombo->AddFrame(new TGLabel(fCombo, "Istogrammi:"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        comboHists = new TGComboBox(fCombo);
        comboHists->Resize(180, 22);

        fCombo->AddFrame(comboHists, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));
        fitFrame->AddFrame(fCombo);

        TGVerticalFrame *checkFrame = new TGVerticalFrame(fitFrame);
        fitFrame->AddFrame(checkFrame, TGLayoutHints(kLHintsExpandX, 10,5,5,5));*/

        // =====================================================
        //  CANVAS (DESTRA)
        // =====================================================
        TGVerticalFrame *right = new TGVerticalFrame(main, 600);
        main->AddFrame(right, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,5));

        canvasTop = new TRootEmbeddedCanvas("canvasTop", right, 600, 500);
        right->AddFrame(canvasTop, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));

        canvasBottom = new TRootEmbeddedCanvas("canvasBottom", right, 600, 500);
        right->AddFrame(canvasBottom, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5,5,5,5));

        bShow->Connect("Clicked()", "MyGUI", this, "DrawTree()");
        bAddHist->Connect("Clicked()", "MyGUI", this, "AddHistogram()");
        bFit->Connect("Clicked()", "MyGUI", this, "DoFit()");
        bDraw->Connect("Clicked()", "MyGUI", this, "DrawHistogram()");
        bClear->Connect("Clicked()", "MyGUI", this, "ClearHistogram()");

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
    void DrawTree() {
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

        string cut = "massa > 0 && massa < 6.5";
        cut = cut + Form("&& pt > %f && pt < %f", ptDimuMin, ptDimuMax);
        cut = cut + Form("&& typeMu1 == %i && typeMu2 == %i", trackTypeMu, trackTypeMu);
        cut = cut + Form("&& carica == %i", chargeDimu);
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

        canvasTop -> GetCanvas() -> cd();
        gPad -> SetLogy(true);
        gStyle -> SetOptStat(false);
        histVar -> Draw("EP");
        canvasTop -> GetCanvas() -> Update();

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
            std::cout << histVar -> GetName() << std::endl;
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
            histograms[histName] -> SetMarkerStyle(20);
            histograms[histName] -> SetMarkerColor(int(histogramNames.size())+1);
        }
    }

    //--------------------------------------------//
    // Function to Draw selected histograms
    void DrawHistogram() {

        std::vector<string> xAxisTitle;
        for (auto &[name, hist] : histograms) {
            //std::cout << hist -> GetXaxis() -> GetTitle() << std::endl;
            xAxisTitle.push_back(hist -> GetXaxis() -> GetTitle());
        }

        auto vars = df.GetColumnNames();
        std::vector<string> chosenVars;
        for (auto &var : vars) {
            if (count(xAxisTitle.begin(), xAxisTitle.end(), var) > 0) {
                chosenVars.push_back(var);
            }
        }
        
        canvasBottom -> GetCanvas() -> cd();
        canvasBottom -> GetCanvas() -> Clear();

        std::cout << chosenVars.size() << std::endl;
        if (int(chosenVars.size()) < 4) {
            canvasBottom -> GetCanvas() -> Divide(int(chosenVars.size()), 1);
        } else if (int(chosenVars.size()) % 2 == 0) {
            canvasBottom -> GetCanvas() -> Divide(int(chosenVars.size()) / 2, int(chosenVars.size()) / 2);
        } else {
            canvasBottom -> GetCanvas() -> Divide(3, 2);
        }
        

        int iColumn = 1;
        for (auto &chosenVar : chosenVars) {

            canvasBottom -> GetCanvas() -> cd(iColumn);
            gPad -> SetLogy(true);

            bool first = true;
            for (auto &[name, hist] : histograms) {
                if (hist -> GetXaxis() -> GetTitle() == chosenVar) {
                    hist -> Draw(first ? "EP" : "EP SAME");
                    first = false;
                }
            }

            gPad -> BuildLegend();
            
            iColumn++;
        }

        canvasBottom -> GetCanvas() -> Update();
    }

    //--------------------------------------------//
    // Clear the list of the histograms
    void ClearHistogram() {
        bDraw -> SetEnabled(false);
        bClear -> SetEnabled(false);
        histograms.clear();
        histogramNames.clear();
        DrawHistogram();
    }

    void DoFit() {
        double xmin = atof(fitMin->GetText());
        double xmax = atof(fitMax->GetText());

        TF1 *gaus = new TF1("gausFit", "gaus", xmin, xmax);
        histVar -> Fit(gaus, "R");

        canvasTop->GetCanvas()->Update();
    }

    virtual ~MyGUI() { Cleanup(); }

    ClassDef(MyGUI, 0);
};

#ifdef __MAKECINT__
#pragma link C++ class MyGUI+;
#endif

void prototype() {

    TFile *fIn = TFile::Open("data.root");
    TTree *tree = (TTree*)fIn -> Get("data");
    ROOT::RDataFrame df(*tree);

    new MyGUI(gClient -> GetRoot(), df);
}