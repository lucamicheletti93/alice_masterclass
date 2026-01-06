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

        main = new TGVerticalFrame(gClient->GetRoot(), 800, 800);

        TGLabel *title = new TGLabel(main, "Welcome to ALICE Masterclass");
        main->AddFrame(title, new TGLayoutHints(kLHintsCenterX, 10,10,20,10));

        TGVerticalFrame* vframe = new TGVerticalFrame(main, 800, 800);
        main->AddFrame(vframe, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        TGPictureButton* button = new TGPictureButton(vframe, "ALICE_logo.png");
        button->SetEnabled(kTRUE); // se vuoi solo mostrare l'immagine senza interazione

        vframe->AddFrame(button, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

        TGTextButton *bStart = new TGTextButton(main, "Start analysis");
        bStart->Connect("Clicked()", "SplashScreen", this, "Start()");
        main->AddFrame(bStart, new TGLayoutHints(kLHintsCenterX, 10,10,20,20));

        TGTextButton *bClose = new TGTextButton(main, "Close");
        bClose->Connect("Clicked()", "SplashScreen", this, "Close()");
        main->AddFrame(bClose, new TGLayoutHints(kLHintsCenterX, 10,10,20,20));

        main->MapSubwindows();
        main->Resize();
        main->MapWindow();
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
