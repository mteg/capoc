/*
 *
 * Copyright (c) 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */
 #include "capengine.h"
#include "caprenderer_ancientgl.h"
#include "capdebug.h"
#include "wxcapoc.h"
#include "wxcapoc_logbrowser.h"
#include "wxcapoc_console.h"
#include <wx/sizer.h>

wxBEGIN_EVENT_TABLE(wxCapoc_Main, wxFrame)
                EVT_MENU(ID_Open_Model,   wxCapoc_Main::OnOpenModel)
                EVT_MENU(ID_Open_NVM,   wxCapoc_Main::OnOpenNVM)
                EVT_MENU(ID_Open_Script,   wxCapoc_Main::OnOpenScript)
                EVT_MENU(ID_Save_PNG,   wxCapoc_Main::OnSavePNG)
                EVT_MENU(ID_Save_Project,   wxCapoc_Main::OnSaveProject)
                EVT_MENU(ID_Log0,   wxCapoc_Main::OnLog0)
                EVT_MENU(ID_Log1,   wxCapoc_Main::OnLog1)
                EVT_MENU(ID_Log2,   wxCapoc_Main::OnLog2)
                EVT_MENU(wxID_EXIT,  wxCapoc_Main::OnExit)
                EVT_MENU(wxID_ABOUT, wxCapoc_Main::OnAbout)
                EVT_MENU(wxID_ANY,   wxCapoc_Main::OnSimpleMenuCommand)
                EVT_TIMER(wxID_ANY, wxCapoc_Main::OnTimer)
                EVT_SHOW(wxCapoc_Main::OnShow)
				EVT_SET_FOCUS(wxCapoc_Main::OnFocus)
				EVT_CLOSE(wxCapoc_Main::OnClose)
wxEND_EVENT_TABLE()


wxCapoc_Main::wxCapoc_Main(const wxString& title, const wxPoint& pos, const wxSize& size) :
        wxFrame(NULL, wxID_ANY, title, pos, size, wxWANTS_CHARS | wxDEFAULT_FRAME_STYLE) {

    capDebug::init();
    capDebug::report(2, "Constructing renderer...");
    renderer = new capRenderer_AncientGL();
    capDebug::report(2, "Constructing engine...");

    engine = new capEngine(renderer);
    capDebug::report(2, "Engine initialized.");

    wxSize s = GetSize();
    engine->reshape((unsigned int) s.x, (unsigned int) s.y);
    engine->startNetwork("127.0.0.1", 8023);

    auto *mainPanel = new wxPanel(this, wxID_ANY);
    tree = new wxCapoc_Tree(mainPanel, engine);

    canvas = new wxCapoc_Canvas(mainPanel, this);

    auto* panelSizer = new wxBoxSizer(wxHORIZONTAL);
    panelSizer->Add(tree, 1, wxEXPAND);
    panelSizer->Add(canvas, 4, wxEXPAND);
    mainPanel->SetSizer(panelSizer);

    auto* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(mainPanel, 1, wxEXPAND);
    SetSizer(topSizer);

    initKeymaps();

    auto *loglevelMenu = new wxMenu;
    loglevelMenu->Append(ID_Log0, "Level 0 (errors only)");
    loglevelMenu->Append(ID_Log1, "Level 1 (errors and notices)");
    loglevelMenu->Append(ID_Log2, "Level 2 (errors, notices, debug)");

    fileMenu = new wxMenu;
    fileMenu->Append(ID_Open_Model, "&Open model...", "Load an OFF file with points or mesh");
    fileMenu->Append(ID_Open_NVM, "&Open NVM...", "Load an NVM file for the first loaded model");
    fileMenu->Enable(ID_Open_NVM, false);
    fileMenu->Append(ID_Save_Project, "Save project...", "Save current project as script");
    fileMenu->Append(ID_Open_Script, "&Execute script...", "Execute a script");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Save_PNG, "Save screenshot...", "Save current canvas into PNG file");
    fileMenu->Append(ID_Console, "Console...", "Open scripting console");
    fileMenu->Append(wxID_ANY, "Log level", loglevelMenu, "Logging level");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);


    auto *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);

    auto *viewMenu = new wxMenu;
    viewMenu->Append(ID_View_Plan, "Plan\tF1", "Switch to map view");
    viewMenu->Append(ID_View_Profile, "Profile\tF2", "Switch to elevation view");
    viewMenu->Append(ID_View_Xsect, "Xsect\tF3", "Switch to cross section view");
    viewMenu->Append(ID_View_Fourview, "Split\tF4", "Switch to split view");
    viewMenu->Append(ID_View_Caver, "Caver\tF5", "Switch to perspective view");
    viewMenu->Append(ID_View_NVMImage, "NVM image\tF6", "See the current NVM image");
    viewMenu->AppendSeparator();
    viewMenu->Append(ID_View_Zoom_In, "Zoom in\t=", "Zoom into current view");
    viewMenu->Append(ID_View_Zoom_Out, "Zoom out\t-", "Zoom out of current view");
    viewMenu->Append(ID_View_Clip_In, "Narrow\t+", "Narrow cross-section of current view");
    viewMenu->Append(ID_View_Clip_Out, "Widen\t_", "Widen cross-section of current view");
    viewMenu->AppendSeparator();
    viewMenu->Append(ID_View_Xsect_In, "Push Xsect\t.", "Push cross section plane to the front");
    viewMenu->Append(ID_View_Xsect_Out, "Pull Xsect\t,", "Pull cross section plane back");
    viewMenu->AppendSeparator();
    viewMenu->Append(ID_View_Log, "Message log");

    auto *nvmShowMenu = new wxMenu;
    auto *nvmHideMenu = new wxMenu;
    nvmShowMenu->Append(ID_NVM_Show_Image, "Image", "Display NVM image in perspective view");
    nvmShowMenu->Append(ID_NVM_Show_Planar, "Matches (2D)", "Show sparse points seen in images on the image plane");
    nvmShowMenu->Append(ID_NVM_Show_Spatial, "Matches (3D)", "Show sparse points seen in images in perspective view");
    nvmShowMenu->Append(ID_NVM_Show_Matching, "Pairing", "Display lines between matches on the image plane and on perspective view");
    nvmShowMenu->AppendSeparator();
    nvmShowMenu->Append(ID_NVM_Show_Next, "Next match\t}", "Cycle through matches within selected range");
    nvmShowMenu->Append(ID_NVM_Show_Prev, "Previous match\t{", "Cycle through matches within selected range");

    nvmHideMenu->Append(ID_NVM_Hide_Image, "Image", "Hide NVM image in perspective view");
    nvmHideMenu->Append(ID_NVM_Hide_Planar, "Matches (2D)", "Hide sparse points seen in images on the image plane");
    nvmHideMenu->Append(ID_NVM_Hide_Spatial, "Matches (3D)", "Hide sparse points seen in images in perspective view");
    nvmHideMenu->Append(ID_NVM_Hide_Matching, "Pairing", "Hide lines between matches on the image plane and on perspective view");
    nvmHideMenu->Append(ID_NVM_Hide_All, "All", "Hide all NVM data");

    auto *nvmMenu = new wxMenu;
    nvmMenu->Append(ID_NVM_Prev, "Previous image\t[", "Browse NVM images");
    nvmMenu->Append(ID_NVM_Next, "Next image\t]", "Browse NVM images");
    nvmMenu->AppendSeparator();
    nvmMenu->Append(ID_NVM_Reproject, "Reproject pixel\t\\", "Find mean 3D position of image pixels, as selected on NVM image view");
    nvmMenu->Append(ID_NVM_Search, "Find selection\t?", "Find NVM images seeing the 3D selection");
    nvmMenu->AppendSeparator();
    nvmMenu->Append(wxID_ANY, "Show", nvmShowMenu, "Display various NVM features");
    nvmMenu->Append(wxID_ANY, "Hide", nvmHideMenu, "Hide various NVM features");


    auto *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(viewMenu, "&View");
    menuBar->Append(nvmMenu, "&NVM");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Ready!");
    

    netTimer.SetOwner(this, wxID_ANY);
    this->Maximize(true);

    this->consoleDialog = new wxCapoc_Console(engine, wxT("Capoc engine console"));
}

void wxCapoc_Main::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void wxCapoc_Main::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("CAPOC", "About Capoc", wxOK | wxICON_INFORMATION);
}

void wxCapoc_Main::OnHello(wxCommandEvent &event) {
    wxLogMessage("Hello, world");
}


void wxCapoc_Main::initializeDisplay() {
	if (displayInitialized) return;
	displayInitialized = true;

	netTimer.Start(40); /* 25 / sec */
	canvas->startDisplay();

	capDebug::report(0, "Output initialized");
	engine->execFile("capoc2.txt");
	canvas->Render();
	tree->resync();
	if (!engine->models.empty())
		fileMenu->Enable(ID_Open_NVM, true);
	canvas->SetFocus();
}

void wxCapoc_Main::OnShow(wxShowEvent &event) {
	initializeDisplay();
}

void wxCapoc_Main::OnFocus(wxFocusEvent &event) {
	initializeDisplay();
}

void wxCapoc_Main::OnTimer(wxTimerEvent &event) {
    if(engine->statusUpdated) {
        updateStatus();
    }

    engine->processNetwork();
}

void wxCapoc_Main::leftDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift) {
    if(engine->renderer->currentView == CAV_NVMCAM)
    {
        if (dx) engine->mouseAction(CAD_NVM_PANX, dx * 0.01f * (shift ? 0.2f : 1.0f));
        if (dy) engine->mouseAction(CAD_NVM_PANY, dy * 0.01f * (shift ? 0.2f : 1.0f));
    }
    else if(tree->modelEditing != -1)
    {
        int rotAxis = 0;
        capModel &m = *engine->models[tree->modelEditing];
        if(engine->renderer->currentView == CAV_PLAN)
            rotAxis = 1;
        else if(engine->renderer->currentView == CAV_PROFILE)
            rotAxis = 0;
        else if(engine->renderer->currentView == CAV_XSECT)
            rotAxis = 2;

        if(dx) m.rotate((shift ? 0.1f : 0.5f) * dx, rotAxis);
        if(dy) {
            float scale = 1.0f + fabsf(dy) / (shift ? 400.0f : 40.0f);
            if(dy < 0) scale = 1.0f/scale;
            m.scale(scale);
        }

        engine->needRefresh = true;
    }
    else
    {
        if (dx) engine->mouseAction(CAD_CAM_YAW, dx * 0.002f * (shift ? 1.0f : 5.0f));
        if (dy) engine->mouseAction(CAD_CAM_PITCH, dy * 0.002f * (shift ? 1.0f : 5.0f));
    }
    capRenderer *r = engine->renderer;
    engine->setStatus("Camera at %f %f %f, looking towards %.1f° pitch %+.1f°", r->cam.x, r->cam.y, r->cam.z, r->cam.yaw/CAP_PI*180.0, r->cam.pitch/CAP_PI*180.0);

}


void wxCapoc_Main::rightDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift)
{
    if(engine->renderer->currentView == CAV_NVMCAM) {
        if(wxGetKeyState(WXK_SHIFT))
            engine->mouseAction(CAD_RANGE_ADJUST, 0.0, cx, cy, x, y);
        else
            engine->mouseAction(CAD_MARKER_SET, 0.0, cx, cy, x, y);
        return;
    }
    else if(tree->modelEditing != -1)
    {
        capModel &m = *engine->models[tree->modelEditing];
        float tx = 0.0, ty = 0.0, tz = 0.0;

        if(engine->renderer->currentView == CAV_PLAN) {
            tx = -dx; tz = -dy;
        } else if(engine->renderer->currentView == CAV_PROFILE) {
            ty = -dy; tz = dx;
        } else if(engine->renderer->currentView == CAV_XSECT) {
            tx = -dx; ty = -dy;
        }

        tx *= shift ? 0.05f : 0.5f / engine->renderer->views[engine->renderer->currentView].orthoZoom;
        ty *= shift ? 0.05f : 0.5f / engine->renderer->views[engine->renderer->currentView].orthoZoom;
        tz *= shift ? 0.05f : 0.5f / engine->renderer->views[engine->renderer->currentView].orthoZoom;

        m.translate(tx, ty, tz);

        engine->needRefresh = true;
    }
    else if(shift)
        engine->mouseAction(CAD_MARK_POINTS, 0.01f * (x-cx), cx, cy, x, y);
    else
        engine->mouseAction(CAD_MEASURE, 0.0f, cx, cy, x, y);
}

void wxCapoc_Main::middleDrag(int x, int y, int cx, int cy, int dx, int dy, bool shift)
{

}

void wxCapoc_Main::doMap(int c, int action, bool shift)
{
    keyMap &m = maps[WXCMAP_ROOT][action][shift ? WXCA_SHIFT : WXCA_BARE];
    if(!m.count(c)) return;
    engine->execString(m[c]);

    /* Exception for tree search */
    if((c == '/') && shift) tree->resync_nvm();
}

void wxCapoc_Main::keyPress(int c, bool shift) {
    doMap(c, WXCA_PRESS, shift);
}

void wxCapoc_Main::keyHold(int c, bool shift) {
    doMap(c, WXCA_HOLD, shift);
}

void wxCapoc_Main::keyRelease(int c, bool shift)
{
}

void wxCapoc_Main::updateStatus()
{
    SetStatusText(wxString::FromUTF8(engine->status));
    engine->statusUpdated = false;
}

void wxCapoc_Main::OnSimpleMenuCommand(wxCommandEvent &event) {
    switch(event.GetId())
    {
        case ID_View_Plan: engine->execString("view switch plan\n"); break;
        case ID_View_Profile: engine->execString("view switch profile\n"); break;
        case ID_View_Xsect: engine->execString("view switch xsect\n"); break;
        case ID_View_Fourview: engine->execString("view switch fourview\n"); break;
        case ID_View_Caver: engine->execString("view switch caver\n"); break;
        case ID_View_NVMImage: engine->execString("view switch nvmcam\n"); break;

        case ID_View_Zoom_In: engine->execString("view zoom current in by 1.4\n"); break;
        case ID_View_Zoom_Out: engine->execString("view zoom current out by 1.4\n"); break;
        case ID_View_Clip_In: engine->execString("view clip current in by 1.4\n"); break;
        case ID_View_Clip_Out: engine->execString("view clip current out by 1.4\n"); break;
        case ID_View_Xsect_In: engine->execString("view offset xsect in\n"); break;
        case ID_View_Xsect_Out: engine->execString("view offset xsect out\n"); break;

        case ID_NVM_Next: engine->execString("model 0 nvm camera next set-active\n"); break;
        case ID_NVM_Prev: engine->execString("model 0 nvm camera prev set-active\n"); break;
        case ID_NVM_Search: engine->execString("model 0 nvm search sparse reorder\n"); tree->resync_nvm(); break;
        case ID_NVM_Reproject: engine->execString("model 0 nvm camera current reproject throw-cursor\n"); tree->resync_nvm(); break;

        case ID_NVM_Show_Image: engine->execString("model 0 nvm show image\n"); break;
        case ID_NVM_Show_Planar: engine->execString("model 0 nvm show planar-points\n"); break;
        case ID_NVM_Show_Spatial: engine->execString("model 0 nvm show spatial-points\n"); break;
        case ID_NVM_Show_Matching: engine->execString("model 0 nvm show matching\n"); break;

        case ID_NVM_Hide_Image: engine->execString("model 0 nvm hide image\n"); break;
        case ID_NVM_Hide_Planar: engine->execString("model 0 nvm hide planar-points\n"); break;
        case ID_NVM_Hide_Spatial: engine->execString("model 0 nvm hide spatial-points\n"); break;
        case ID_NVM_Hide_Matching: engine->execString("model 0 nvm hide matching\n"); break;

        case ID_NVM_Hide_All:         engine->execString("model 0 nvm hide all\n"); break;

        case ID_NVM_Show_Next: engine->execString("model 0 nvm show next-match\n"); break;
        case ID_NVM_Show_Prev: engine->execString("model 0 nvm show prev-match\n"); break;

        case ID_View_Log: {
            wxString s = "";
            unsigned int i;

            for(i = 0;i<capDebug::ringSize(); i++) {
                s.append(capDebug::ringGet(i));
                s.append("\n");
            }
            wxCapoc_LogBrowser logDialog(s.ToUTF8().data(), "Browse logs");
            logDialog.ShowModal();


            break;
        }

        case ID_Console: {
            consoleDialog->Show();
            break;
        };
        default: return;
    }
}

void wxCapoc_Main::OnOpenModel(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open model file"), "", "",
                "All supported types (OFF, PLY, NVM, 3D)|*.off;*.ply;*.nvm;*.3d|OFF files (*.off)|*.off|PLY files (*.ply)|*.ply|NVM files (*.nvm)|*.nvm|3d files (*.3d)|*.3d", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;
	initializeDisplay();
	engine->loadModel(openFileDialog.GetPath().ToUTF8().data());
    tree->resync();
    if(!engine->models.empty())
        fileMenu->Enable(ID_Open_NVM, true);
}

void wxCapoc_Main::OnOpenNVM(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open NVM file"), "", "", "NVM files (*.nvm)|*.nvm", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;
    if(!engine->models.empty())
        engine->models[0]->readNvm(openFileDialog.GetPath().ToUTF8().data());
    tree->resync_nvm();
}

void wxCapoc_Main::OnSavePNG(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Save PNG file"), "", "", "PNG files (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;
    engine->renderer->screenshot(openFileDialog.GetPath().ToUTF8().data());
}

void wxCapoc_Main::OnSaveProject(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Save TXT project"), "", "", "TXT files (*.txt)|*.txt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;
    engine->saveProjectScript(openFileDialog.GetPath().ToUTF8().data());
}

void wxCapoc_Main::OnOpenScript(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open TXT file"), "", "", "TXT files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;
    engine->execFile(openFileDialog.GetPath().ToUTF8().data());
    tree->resync();
}

void wxCapoc_Main::OnClose(wxCloseEvent &event) {
    consoleDialog->Destroy();
    Destroy();
}

void wxCapoc_Main::OnLog0(wxCommandEvent &event) {
    capDebug::debugLevel = 0;
    capDebug::report(0, "Logging level set to 0");
}

void wxCapoc_Main::OnLog1(wxCommandEvent &event) {
    capDebug::debugLevel = 1;
    capDebug::report(0, "Logging level set to 1");
}

void wxCapoc_Main::OnLog2(wxCommandEvent &event) {
    capDebug::debugLevel = 2;
    capDebug::report(0, "Logging level set to 2");
}

