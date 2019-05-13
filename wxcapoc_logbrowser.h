#ifndef WXCAPOC_LOGBROWSER_H
#define WXCAPOC_LOGBROWSER_H

#include "wxcapoc_console.h"
class wxCapoc_LogBrowser : public wxCapoc_Console {
public:
    explicit wxCapoc_LogBrowser(const char *buffer, wxString title);
};


#endif

