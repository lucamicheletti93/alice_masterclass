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
    TGTextEntry *entryPtDimuMin, *entryPtDimuMax, *entryChargeDimu, *entryEtaMuMin, *entryEtaMuMax, *entryTrackType1, *entryTrackType2;
    TGTextEntry *entryHistName;
    TGTextEntry *fitMin, *fitMax;
    TGLabel *labelHistStatus;
    TRootEmbeddedCanvas *canvasTop, *canvasBottom;
    TGComboBox  *comboVar, *comboHists;

    TGCheckButton *cbChargePlus, *cbChargeMinus, *cbChargeZero;

    ROOT::RDF::RNode df;
    ROOT::RDF::RResultPtr<TH1D> histVar;
    ROOT::RDF::RResultPtr<TH1D> histDimuMassPM, histDimuMassPP, histDimuMassMM;

    // Istogrammi creati
    std::map<std::string, TH1D*> histograms;

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

        // ---------- SHOW (SOPRA) ----------
        TGGroupFrame *showFrame = new TGGroupFrame(left, "Data selection");
        left->AddFrame(showFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGCompositeFrame *table = new TGCompositeFrame(showFrame);
        table->SetLayoutManager(new TGTableLayout(table, 3, 3));
        showFrame->AddFrame(table, new TGLayoutHints(kLHintsExpandX));
        
        table->AddFrame(new TGLabel(table, ""), new TGTableLayoutHints(0, 1, 0, 1, kLHintsCenterX));
        table->AddFrame(new TGLabel(table, "Min"), new TGTableLayoutHints(1, 2, 0, 1, kLHintsCenterX));
        table->AddFrame(new TGLabel(table, "Max"), new TGTableLayoutHints(2, 3, 0, 1, kLHintsCenterX));


        // Riga pT
        table->AddFrame(new TGLabel(table, "pT dimuone"), new TGTableLayoutHints(0,1,1,2, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryPtDimuMin = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtDimuMin->SetText("0");
        entryPtDimuMin->Resize(80,22);
        entryPtDimuMin->SetAlignment(kTextCenterX);
        table->AddFrame(entryPtDimuMin, new TGTableLayoutHints(1,2,1,2, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryPtDimuMax = new TGTextEntry(table, new TGTextBuffer(10));
        entryPtDimuMax->SetText("100");
        entryPtDimuMax->Resize(80,22);
        entryPtDimuMax->SetAlignment(kTextCenterX);
        table->AddFrame(entryPtDimuMax, new TGTableLayoutHints(2,3,1,2, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        // Riga eta
        table->AddFrame(new TGLabel(table, "eta muone"), new TGTableLayoutHints(0,1,2,3, kLHintsLeft | kLHintsCenterY, 0, 10, 0, 0));

        entryEtaMuMin = new TGTextEntry(table, new TGTextBuffer(10));
        entryEtaMuMin->SetText("2.5");
        entryEtaMuMin->Resize(80,22);
        entryEtaMuMin->SetAlignment(kTextCenterX);
        table->AddFrame(entryEtaMuMin, new TGTableLayoutHints(1,2,2,3, kLHintsCenterX | kLHintsExpandY, 0, 20, 0, 0));

        entryEtaMuMax = new TGTextEntry(table, new TGTextBuffer(10));
        entryEtaMuMax->SetText("4");
        entryEtaMuMax->Resize(80,22);
        entryEtaMuMax->SetAlignment(kTextCenterX);
        table->AddFrame(entryEtaMuMax, new TGTableLayoutHints(2,3,2,3, kLHintsCenterX | kLHintsExpandY, 20, 0, 0, 0));

        table->MapSubwindows();
        table->Resize();

        
        AddInput(showFrame, "Carica dimuone:", entryChargeDimu, "0");
        AddInput(showFrame, "Type muone 1:", entryTrackType1, "3");
        AddInput(showFrame, "Type muone 2:", entryTrackType2, "3");


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

        /*TGTextButton *bAddHist = new TGTextButton(showFrame, "Add histogram");
        showFrame->AddFrame(bAddHist, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));*/

        TGHorizontalFrame *frameAddHist = new TGHorizontalFrame(showFrame);

        TGTextButton *bAddHist = new TGTextButton(frameAddHist, "Aggiungi");
        frameAddHist->AddFrame(bAddHist, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,10,5,5));

        entryHistName = new TGTextEntry(frameAddHist, new TGTextBuffer(30));
        entryHistName->Resize(160, 22);
        entryHistName->SetAlignment(kTextLeft);
        entryHistName->SetToolTipText("Hist. name");
        frameAddHist->AddFrame(entryHistName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        showFrame->AddFrame(frameAddHist, new TGLayoutHints(kLHintsLeft, 5,5,10,5));

        labelHistStatus = new TGLabel(showFrame, " ");
        showFrame->AddFrame(labelHistStatus, new TGLayoutHints(kLHintsLeft, 5,5,10,5));


        /*TGTextButton *bAddHist = new TGTextButton(showFrame, "Aggiungi istogramma");
        showFrame->AddFrame(bAddHist, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,10,5,5));

        TGLabel *labelHistName = new TGLabel(showFrame, ""); // inizialmente vuota
        showFrame->AddFrame(labelHistName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5,5,5,5));

        showFrame->AddFrame(showFrame, new TGLayoutHints(kLHintsLeft));*/

        // ---------- FIT (SOTTO) ----------
        TGGroupFrame *fitFrame = new TGGroupFrame(left, "Analysis");
        left->AddFrame(fitFrame, new TGLayoutHints(kLHintsExpandX, 5,5,5,5));

        TGTextButton *bDraw = new TGTextButton(fitFrame, "Plot selected histograms");
        fitFrame->AddFrame(bDraw, new TGLayoutHints(kLHintsCenterX, 10,10,5,10));

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

    //-----------------------------------//
    // Fill variable menu
    void FillComboFromRDF() {

        comboVar -> RemoveAll();

        auto cols = df.GetColumnNames();
        int id = 0;
        for (auto &c : cols) {
            comboVar -> AddEntry(c.c_str(), id++);
        }

        comboVar -> Select(0);
    }

    //-----------------------------------//
    // Select variable to show
    TString GetSelectedVariable() const {

        TGTextLBEntry *varName = (TGTextLBEntry*) comboVar -> GetSelectedEntry();

        if (!varName) return "";
        return varName -> GetText() -> GetString();
    }

    void DrawTree() {
        TString var = GetSelectedVariable();
        if (var.IsNull()) return;
        
        double ptDimuMin = std::stod(entryPtDimuMin->GetText());
        double ptDimuMax = std::stod(entryPtDimuMax->GetText());
        int chargeDimu = std::stod(entryChargeDimu->GetText());
        double etaMuMin = std::stod(entryEtaMuMin->GetText());
        double etaMuMax = std::stod(entryEtaMuMax->GetText());
        int trackTypeMu1 = std::stod(entryTrackType1->GetText());
        int trackTypeMu2 = std::stod(entryTrackType2->GetText());

        //double min = std::stod(entryMin->GetText());
        //double max = std::stod(entryMax->GetText());
        //std::cout << min << " " << max << std::endl;

        string cut = "massa > 0 && massa < 6.5";
        cut = cut + Form("&& pt > %f && pt < %f", ptDimuMin, ptDimuMax);
        cut = cut + Form("&& typeMu1 == %i && typeMu2 == %i", trackTypeMu1, trackTypeMu2);
        cut = cut + Form("&& carica == %i", chargeDimu);
        cut = cut + Form("&& etaMu1 > %f && etaMu1 < %f && etaMu2 > %f && etaMu2 < %f", etaMuMin, etaMuMax, etaMuMin, etaMuMax);

        histVar = df.Filter(cut).Histo1D(var);
        histVar -> SetTitle("");
        histVar -> GetXaxis() -> SetRangeUser(0, 6);
        histVar -> GetXaxis() -> SetTitle(var);
        histVar -> GetYaxis() -> SetTitle("conteggi");
        histVar -> SetMarkerStyle(20);
        histVar -> SetMarkerColor(kBlack);
        histVar -> SetLineColor(kBlack);

        canvasTop -> GetCanvas() -> cd();
        gPad -> SetLogy(true);
        gStyle -> SetOptStat(false);
        histVar -> Draw("EP");
        canvasTop -> GetCanvas() -> Update();
    }

    void AddHistogram() {
        int chargeDimu = std::stod(entryChargeDimu->GetText());
        string histName = entryHistName->GetText();

        // Update label telling which hisogram is added
        labelHistStatus->SetTextColor(kBlue);
        labelHistStatus->SetText(Form("Istogramma '%s' aggiunto", histName.c_str()));
        labelHistStatus->MapWindow();
        this->Layout();

        //auto tmpHist = new TH1D(histName.c_str(), histName.c_str(), 60, 0, 120);
        //histograms[histName] = tmpHist;

        if (chargeDimu == 0) {
            std::cout << "Added PM histo" << std::endl;
            histDimuMassPM = histVar;
            histDimuMassPM -> SetName("histDimuMassPM");
            histDimuMassPM -> SetMarkerStyle(20);
            histDimuMassPM -> SetMarkerColor(kBlack);
            histDimuMassPM -> SetLineColor(kBlack);
        } else if (chargeDimu == 2) {
            std::cout << "Added PP histo" << std::endl;
            histDimuMassPP = histVar;
            histDimuMassPP -> SetName("histDimuMassPP");
            histDimuMassPP -> SetMarkerStyle(20);
            histDimuMassPP -> SetMarkerColor(kRed+1);
            histDimuMassPP -> SetLineColor(kRed+1);
        } else {
            std::cout << "Added MM histo" << std::endl;
            histDimuMassMM = histVar;
            histDimuMassMM -> SetName("histDimuMassMM");
            histDimuMassMM -> SetMarkerStyle(20);
            histDimuMassMM -> SetMarkerColor(kAzure+2);
            histDimuMassMM -> SetLineColor(kAzure+2);
        }
    }

    void DrawHistogram() {
        canvasBottom -> GetCanvas() -> cd();
        gPad -> SetLogy(true);
        histDimuMassPM -> Draw("EP");
        histDimuMassPP -> Draw("EP SAME");
        histDimuMassMM -> Draw("EP SAME");

        canvasBottom->GetCanvas()->Clear();

        gPad -> BuildLegend();
        canvasBottom -> GetCanvas() -> Update();



        /*canvasBottom->GetCanvas()->cd();
        canvasBottom->GetCanvas()->Clear();

        bool first = true;

        for (auto &[name, h] : histograms) {
            if (histChecks[name]->IsOn()) {
                h->SetLineWidth(2);
                h->Draw(first ? "" : "SAME");
                first = false;
            }
        }

        canvasBottom->GetCanvas()->BuildLegend();
        canvasBottom->GetCanvas()->Update();*/
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