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

wxCapoc_LogBrowser::wxCapoc_LogBrowser(const char *buffer, wxString title) :
        wxCapoc_Console(NULL, title, buffer)
{

}
