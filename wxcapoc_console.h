#ifndef WXCAPOC_CONSOLE_H
#define WXCAPOC_CONSOLE_H

class wxCapoc_Console : public wxDialog {
public:
    explicit wxCapoc_Console(capEngine *cap, wxString title, const char *onlyDisplayThis = NULL);
    wxButton *okButton;
    wxButton *execButton;

    wxTextCtrl *resultBuffer;
    wxTextCtrl *commandEntry;

    capEngine *engine;

private:
    void OnExec(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnEntry(wxCommandEvent& event);
    void OnKeyUp(wxKeyEvent &event);
    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_ExecButton = 1, ID_CommandEntry, ID_ClearButton
};


#endif
