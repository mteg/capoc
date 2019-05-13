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
#include "wxcapoc_editmatrix.h"
#include <wx/valnum.h>

wxCapoc_EditMatrix::wxCapoc_EditMatrix(capAffineMatrix *m, wxString title) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230), wxWS_EX_VALIDATE_RECURSIVELY | wxDEFAULT_DIALOG_STYLE) {

    auto *b_box = new wxBoxSizer(wxHORIZONTAL);
    auto *v_box = new wxBoxSizer(wxVERTICAL);
    int row, col;
    const wxString rowLabels[3] = {wxT("x"), wxT("y"), wxT("z")};

    this->m = m;

    auto *coeff_sizer = new wxGridSizer(5, 5, 5);
    for(row = 0; row<3; row++)
    {
        coeff_sizer->Add(new wxStaticText(this, wxID_ANY, rowLabels[row]));
        for(col = 0; col<4; col++)
        {
            wxTextCtrl *cc = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
            cc->SetValidator(wxFloatingPointValidator <float> (&m->m[col*4+row]));
            coeff_sizer->Add(cc);
        }
    }
    okButton = new wxButton(this, wxID_OK, wxT("OK"));
    cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    b_box->Add(okButton, 1); b_box->Add(cancelButton, 1);

    composeCheckbox = new wxCheckBox(this, wxID_ANY, wxT("Compose with current transform"));

    v_box->Add(coeff_sizer, 1, wxEXPAND | wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    v_box->Add(composeCheckbox, 0, wxEXPAND | wxALIGN_CENTER, 10);
    v_box->Add(b_box, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizerAndFit(v_box);
    v_box->SetSizeHints(this);
    SetAutoLayout(true);
}
