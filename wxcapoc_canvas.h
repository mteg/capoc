
class wxCapoc_Main;

class wxCapoc_Canvas : public wxGLCanvas {

public:
    explicit wxCapoc_Canvas(wxWindow *parent, wxCapoc_Main *main);

    void Render();
    void Resize(wxSizeEvent &event);
    void Paint(wxPaintEvent &event);
    void refreshIfNeeded();
    void startDisplay();

private:
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnMouseMoved(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);
    void OnTimer(wxTimerEvent &event);

    wxTimer keyTimer;
    wxGLContext *glContext;
    wxCapoc_Main *app;

    int currentKey = WXK_NONE;
    int clickX = 0, clickY = 0, queuedDx = 0, queuedDy = 0, prevX = 0, prevY = 0;

    DECLARE_EVENT_TABLE()

};


