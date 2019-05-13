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

wxIMPLEMENT_APP(wxCapoc);


bool wxCapoc::OnInit() {
    auto *mainWin = new wxCapoc_Main("wxCapoc", wxPoint(50, 50), wxSize(450, 340));
    capDebug::debugLevel = 2;
    mainWin->Show(true);
    return true;
}

