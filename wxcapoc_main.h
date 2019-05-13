
#ifndef WXCAPOC_MAIN_H
#define WXCAPOC_MAIN_H
#include <unordered_map>
#include "wxcapoc_console.h"

typedef std::unordered_map<int, const char *> keyMap;

class wxCapoc_Main : public wxFrame {

public:
    wxCapoc_Main(const wxString& title, const wxPoint& pos, const wxSize& size);
    wxCapoc_Canvas *canvas;
    wxMenu *fileMenu;
	bool displayInitialized = false;

    capEngine *engine;
    capRenderer *renderer;

    void keyPress(int c, bool shift);
    void keyRelease(int c, bool shift);
    void keyHold(int c, bool shift);

    void leftDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift);
    void rightDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift);
    void middleDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift);

    void updateStatus();
    wxCapoc_Tree *tree;
    wxCapoc_Console *consoleDialog = NULL;

#define WXCMAP_ROOT 0
#define WXCMAP_COUNT 1
#define WXCA_PRESS 0
#define WXCA_HOLD 1
#define WXCA_BARE 0
#define WXCA_SHIFT 1

    keyMap maps[WXCMAP_COUNT][2][2];

private:
	void initializeDisplay();
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShow(wxShowEvent& event);
	void OnFocus(wxFocusEvent& event);
	void OnClose(wxCloseEvent& event);
    void OnTimer(wxTimerEvent &event);
    void OnSimpleMenuCommand(wxCommandEvent &event);

    void OnOpenModel(wxCommandEvent &event);
    void OnOpenNVM(wxCommandEvent &event);
    void OnOpenScript(wxCommandEvent &event);
    void OnLog0(wxCommandEvent &event);
    void OnLog1(wxCommandEvent &event);
    void OnLog2(wxCommandEvent &event);


    void initKeymaps();
    void doMap(int c, int action, bool shift);

    wxTimer netTimer;

    wxDECLARE_EVENT_TABLE();

    void OnSavePNG(wxCommandEvent &event);

    void OnSaveProject(wxCommandEvent &event);
};



enum {
    ID_Open_Model = 1,
    ID_Open_NVM, ID_Save_PNG, ID_Save_Project, ID_Log0, ID_Log1, ID_Log2,
    ID_Open_Script,
    ID_View_Plan,
    ID_View_Profile,
    ID_View_Xsect,
    ID_View_Fourview,
    ID_View_Caver,
    ID_View_NVMImage,
    ID_View_Zoom_In, ID_View_Zoom_Out, ID_View_Clip_In, ID_View_Clip_Out,
    ID_NVM_Prev,
    ID_NVM_Next,
    ID_NVM_Reproject,
    ID_NVM_Search,
    ID_NVM_Hide_Image, ID_NVM_Hide_Planar, ID_NVM_Hide_Spatial, ID_NVM_Hide_Matching, ID_NVM_Hide_All,
    ID_NVM_Show_Image, ID_NVM_Show_Planar, ID_NVM_Show_Spatial, ID_NVM_Show_Matching, ID_NVM_Show_Next, ID_NVM_Show_Prev,
    ID_View_Xsect_In, ID_View_Xsect_Out, ID_View_Log, ID_Reinit_Renderer, ID_Console
};


#endif