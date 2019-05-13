
class wxCapoc_EditPoint : public wxDialog {
public:
    explicit wxCapoc_EditPoint(capGenericPoint def, wxString title, std::string pointName = "");

    wxButton *okButton, *cancelButton;
    wxTextCtrl  *nameInput;
    capGenericPoint p;

    void OnTextChange(wxCommandEvent &event);
};
