#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGIcon.h>
#include <TSystem.h>

class SplashScreen : public TGMainFrame {

private:
    const TGPicture *logo;
    TGVerticalFrame* main;

public:
    SplashScreen(const TGWindow *p) : TGMainFrame(p, 900, 500)
    {
        SetWindowName("Masterclass");

        Pixel_t bkgColor, buttonColor;
        gClient -> GetColorByName("white", bkgColor);
        gClient -> GetColorByName("white", buttonColor);

        main = new TGVerticalFrame(gClient -> GetRoot(), 800, 800);
        main -> SetBackgroundColor(bkgColor);

        TGLabel *title = new TGLabel(main, "Welcome to ALICE Masterclass!");
        title -> SetTextFont("-*-helvetica-bold-r-*-*-16-*-*-*-*-*-*-*");
        title -> SetBackgroundColor(bkgColor);
        main -> AddFrame(title, new TGLayoutHints(kLHintsCenterX, 10,10,20,10));

        TGVerticalFrame* vframe = new TGVerticalFrame(main, 800, 800);
        vframe -> SetBackgroundColor(bkgColor);
        main->AddFrame(vframe, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGPictureButton* button = new TGPictureButton(vframe, "ALICE_logo.png");
        button -> SetEnabled(kTRUE);
        vframe -> AddFrame(button, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

        TGTextView* description = new TGTextView(vframe, 400, 300);
        description -> AddLine("Welcome to alice masterclass interface.");
        description -> AddLine("");
        description -> AddLine("This masterclass is meant to provide an example of all the");
        description -> AddLine("steps of an anlysis in high energy physics.");
        description -> AddLine("the main focus in this case is the study of the charmonia");
        description -> AddLine("in Pb-Pb collisions. Your goal is to find the best analysis");
        description -> AddLine("cuts to select the signal candidates and then extract the");
        description -> AddLine("number of J/psi e Psi(2S).");
        description -> AddLine("");
        description -> AddLine("With this interface you can:");
        description -> AddLine(" - inspect the different observables (mass, pT, eta, ecc..)");
        description -> AddLine(" - study the analysis cuts that maximize the signal you are searching");
        description -> AddLine(" - fit the data to extract the number of candidates");
        description -> AddLine(" - Compare the results with other analyzers");
        description -> AddLine("");
        description -> AddLine("Enjoy!");

        const TGFont* font = gClient->GetFont("-*-helvetica-r-*-*-16-*-*-*-*-*-*-*");
        description -> SetFont(font -> GetFontStruct());

        description -> SetReadOnly(kTRUE);

        description -> SetBackgroundColor(bkgColor);
        vframe -> AddFrame(description, new TGLayoutHints(kLHintsLeft, 10, 10, 20, 20));

        //TGTextButton *bStart = new TGTextButton(main, "Start analysis");
        TGPictureButton* bStart = new TGPictureButton(main, "start_analysis.png");
        bStart -> SetBackgroundColor(buttonColor);
        bStart -> SetEnabled(kTRUE);
        bStart -> Connect("Clicked()", "SplashScreen", this, "Start()");
        main -> AddFrame(bStart, new TGLayoutHints(kLHintsCenterX, 10,10,5,5));

        //TGTextButton *bClose = new TGTextButton(main, "Close");
        TGPictureButton* bClose = new TGPictureButton(main, "close.png");
        bClose -> SetBackgroundColor(buttonColor);
        bClose -> Connect("Clicked()", "SplashScreen", this, "Close()");
        main -> AddFrame(bClose, new TGLayoutHints(kLHintsCenterX, 10,10,5,5));

        main -> MapSubwindows();
        main -> Resize();
        main -> MapWindow();
    }

    void Start() {
        main -> DeleteWindow();
        gROOT -> Macro("prototype.C");
    }

    void Close() {
        main -> DeleteWindow();
        gApplication -> Terminate(0); 
    }

    virtual ~SplashScreen() { Cleanup(); }

    ClassDef(SplashScreen, 0);
};

// ===========================
// MAIN
// ===========================
void splash()
{
    new SplashScreen(gClient->GetRoot());
}
