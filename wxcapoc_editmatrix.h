
#include <wx/checkbox.h>

class wxCapoc_EditMatrix : public wxDialog {
public:
    explicit wxCapoc_EditMatrix(capAffineMatrix *m, wxString title);
    wxButton *okButton, *cancelButton;
    wxCheckBox *composeCheckbox;

    capAffineMatrix *m;


};
