#include "TTreeReader.h"
#include "TTreeReaderValue.h"

void rewriteTree() {

    TFile *fOut = new TFile("data.root", "RECREATE");
    double mass, pt, eta, ptMu1, ptMu2, etaMu1, etaMu2;
    int charge, chargeMu1, chargeMu2, typeMu1, typeMu2;
    TTree *treeOut = new TTree("data", "data");

    treeOut -> Branch("mass", &mass);
    treeOut -> Branch("pt", &pt);
    treeOut -> Branch("eta", &eta);
    treeOut -> Branch("charge", &charge);
    treeOut -> Branch("ptMu1", &ptMu1);
    treeOut -> Branch("ptMu2", &ptMu2);
    treeOut -> Branch("etaMu1", &etaMu1);
    treeOut -> Branch("etaMu2", &etaMu2);
    treeOut -> Branch("chargeMu1", &chargeMu1);
    treeOut -> Branch("chargeMu2", &chargeMu2);
    treeOut -> Branch("typeMu1", &typeMu1);
    treeOut -> Branch("typeMu2", &typeMu2);

    TFile *fIn = TFile::Open("treePbPb.root", "READ");
    TTreeReader myReader("DF_2338498054383952/O2rtdilmtreerec", fIn);

    TTreeReaderValue<Float_t> fMass(myReader, "fMass");
    TTreeReaderValue<Float_t> fPt(myReader, "fPt");
    TTreeReaderValue<Float_t> fEta(myReader, "fEta");
    TTreeReaderValue<Float_t> fPt1(myReader, "fPt1");
    TTreeReaderValue<Float_t> fPt2(myReader, "fPt2");
    TTreeReaderValue<Float_t> fEta1(myReader, "fEta1");
    TTreeReaderValue<Float_t> fEta2(myReader, "fEta2");
    TTreeReaderValue<Int_t> fSign1(myReader, "fSign1");
    TTreeReaderValue<Int_t> fSign2(myReader, "fSign2");
    TTreeReaderValue<Int_t> fTrackType1(myReader, "fTrackType1");
    TTreeReaderValue<Int_t> fTrackType2(myReader, "fTrackType2");

    while (myReader.Next()) {
        mass = *fMass;
        pt = *fPt;
        eta = *fEta;
        ptMu1 = *fPt1;
        ptMu2 = *fPt2;
        etaMu1 = -(*fEta1);
        etaMu2 = -(*fEta2);
        chargeMu1 = *fSign1;
        chargeMu2 = *fSign2;
        typeMu1 = *fTrackType1;
        typeMu2 = *fTrackType2;

        if (chargeMu1 != chargeMu2) {
            charge = 0;
        } else if (chargeMu1 > 0 && chargeMu2 > 0) {
            charge = 2;
        } else {
            charge = -2;
        }
        treeOut -> Fill();
    }
    fOut -> cd();
    treeOut -> Write();
    fOut -> Close();
    fIn -> Close();
}