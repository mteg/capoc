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
#include "wxcapoc_editpoint.h"
#include <wx/valnum.h>

wxCapoc_EditPoint::wxCapoc_EditPoint(capGenericPoint def, wxString title, std::string pointName) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230), wxWS_EX_VALIDATE_RECURSIVELY | wxDEFAULT_DIALOG_STYLE) {

    auto *b_box = new wxBoxSizer(wxHORIZONTAL);
    auto *v_box = new wxBoxSizer(wxVERTICAL);

    auto *xyz_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("Position"));
    auto *name_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Point name"));

    this->p = def;

    wxTextCtrl *cx, *cy, *cz;
    cx = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    cy = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    cz = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);

    cx->SetValidator(wxFloatingPointValidator <float> (&p.x));
    cy->SetValidator(wxFloatingPointValidator <float> (&p.y));
    cz->SetValidator(wxFloatingPointValidator <float> (&p.z));

    xyz_sizer->Add(cx, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
    xyz_sizer->Add(cy, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
    xyz_sizer->Add(cz, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    name_sizer->Add(nameInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize), 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
    nameInput->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(wxCapoc_EditPoint::OnTextChange), NULL, this);



    okButton = new wxButton(this, wxID_OK, wxT("OK"));
    cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    b_box->Add(okButton, 1); b_box->Add(cancelButton, 1);

    nameInput->SetValue(pointName);
    okButton->Enable(!pointName.empty());

    v_box->Add(xyz_sizer, 1, wxEXPAND | wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    v_box->Add(name_sizer, 1, wxEXPAND | wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    v_box->Add(b_box, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizerAndFit(v_box);
    v_box->SetSizeHints(this);
    SetAutoLayout(true);

//    TransferDataToWindow();
}

void wxCapoc_EditPoint::OnTextChange(wxCommandEvent& event)
{
     okButton->Enable(!nameInput->GetValue().IsEmpty());
}
