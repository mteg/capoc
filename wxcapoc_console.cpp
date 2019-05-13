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
#include "wxcapoc_console.h"

wxBEGIN_EVENT_TABLE(wxCapoc_Console, wxDialog)
    EVT_BUTTON(ID_ExecButton, wxCapoc_Console::OnExec)
    EVT_BUTTON(ID_ClearButton, wxCapoc_Console::OnClear)
    EVT_TEXT(ID_CommandEntry, wxCapoc_Console::OnEntry)
    EVT_TEXT_ENTER(ID_CommandEntry, wxCapoc_Console::OnExec)
    EVT_CHAR_HOOK(wxCapoc_Console::OnKeyUp)
wxEND_EVENT_TABLE()

wxCapoc_Console::wxCapoc_Console(capEngine *engine, wxString title, const char *buffer) :
        wxDialog(NULL, -1, title, wxPoint(50, 50), wxSize(600, 400), wxWS_EX_VALIDATE_RECURSIVELY | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {

    auto *b_box = new wxBoxSizer(wxHORIZONTAL);
    auto *v_box = new wxBoxSizer(wxVERTICAL);

    auto *mainPanel = new wxPanel(this, wxID_ANY);

    this->engine = engine;

    if(engine)
    {
        commandEntry = new wxTextCtrl(mainPanel, ID_CommandEntry, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB | wxTE_PROCESS_ENTER);
        execButton = new wxButton(mainPanel, ID_ExecButton, wxT("Exec"));
        okButton = new wxButton(mainPanel, wxID_OK, wxT("Close"));
        b_box->Add(commandEntry, 8);
        b_box->Add(execButton, 1);
        b_box->Add(new wxButton(mainPanel, ID_ClearButton, wxT("Clear")), 1);
        b_box->Add(okButton, 1);

    }
    else {
        okButton = new wxButton(mainPanel, wxID_OK, wxT("OK"));
        b_box->Add(okButton, 1);
    }

    resultBuffer = new wxTextCtrl(mainPanel, wxID_ANY, buffer, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    resultBuffer->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    resultBuffer->SetEditable(false);

    v_box->Add(resultBuffer, 1, wxEXPAND | wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    v_box->Add(b_box, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM, 10);

    mainPanel->SetSizer(v_box);

    auto* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(mainPanel, 1, wxEXPAND);
    SetSizer(topSizer);

    SetAutoLayout(true);
}

void wxCapoc_Console::OnExec(wxCommandEvent &event) {
    wxString text = commandEntry->GetValue();
    text.Append("\n");
    resultBuffer->AppendText(engine->execString(text.ToUTF8().data()));
    commandEntry->SetValue("");
}

void wxCapoc_Console::OnEntry(wxCommandEvent &event) {
    wxString text = commandEntry->GetValue();
    if(!text.Len()) return;
    wxUniChar t = text.GetChar(text.Len()-1);
    if(t == '?')
    {
        resultBuffer->AppendText(engine->execString(text.ToUTF8().data()));
        text.RemoveLast(1);
        commandEntry->SetValue(text);
    }
    else if(t == '\t')
    {
        text.RemoveLast(1);
        commandEntry->SetValue(text);
        const char * cmdResult = engine->execString(text.ToUTF8().data());
        if(strchr(cmdResult, '\n'))
            resultBuffer->AppendText(cmdResult);
        else {
            commandEntry->SetValue(cmdResult);
            commandEntry->SetInsertionPointEnd();
        }
    }
}

void wxCapoc_Console::OnKeyUp(wxKeyEvent &event) {
    int k = event.GetKeyCode();
    if(k == WXK_F1)
    {
        wxString text = commandEntry->GetValue();
        text.Append("?");
        resultBuffer->AppendText(engine->execString(text.ToUTF8().data()));
    }
    else if(k == WXK_F2) {
        wxString text = commandEntry->GetValue();
        text.Append("\t");
        const char *cmdResult = engine->execString(text.ToUTF8().data());
        if (strchr(cmdResult, '\n'))
            resultBuffer->AppendText(cmdResult);
        else {
            commandEntry->SetValue(cmdResult);
            commandEntry->SetInsertionPointEnd();
        }
    }
    else
        event.Skip();
}

void wxCapoc_Console::OnClear(wxCommandEvent &event) {
    commandEntry->SetValue("");
    resultBuffer->SetValue("");
}

