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

wxBEGIN_EVENT_TABLE(wxCapoc_Canvas, wxGLCanvas)
EVT_PAINT(wxCapoc_Canvas::Paint)
EVT_KEY_UP(wxCapoc_Canvas::OnKeyUp)
EVT_KEY_DOWN(wxCapoc_Canvas::OnKeyDown)
EVT_SIZE(wxCapoc_Canvas::Resize)
EVT_TIMER(wxID_ANY, wxCapoc_Canvas::OnTimer)
EVT_MOTION(wxCapoc_Canvas::OnMouseMoved)
EVT_LEFT_DOWN(wxCapoc_Canvas::OnMouseDown)
EVT_RIGHT_DOWN(wxCapoc_Canvas::OnMouseDown)
wxEND_EVENT_TABLE()

const int canvasAttrs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0 };

wxCapoc_Canvas::wxCapoc_Canvas(wxWindow *parent, wxCapoc_Main *main)
        : wxGLCanvas(parent, wxID_ANY, canvasAttrs , wxDefaultPosition, wxDefaultSize, 0, wxT("GL Canvas"))
{
    glContext = new wxGLContext(this);

    keyTimer.SetOwner(this, wxID_ANY);

    app = main;

}

void wxCapoc_Canvas::startDisplay()
{
    app->renderer->startDisplay();
    keyTimer.Start(40); /* 25 fps should be enough */
}

void wxCapoc_Canvas::OnTimer(wxTimerEvent &event)
{
    if(currentKey != WXK_NONE)
        app->keyHold(currentKey, wxGetKeyState(WXK_SHIFT));

    wxMouseState ms = wxGetMouseState();

    if((ms.LeftIsDown() && ms.RightIsDown()) || ms.MiddleIsDown())
        app->middleDrag(prevX, prevY, clickX, clickY, queuedDx, queuedDy, wxGetKeyState(WXK_SHIFT));
    else if(ms.LeftIsDown())
        app->leftDrag(prevX, prevY, clickX, clickY, queuedDx, queuedDy, wxGetKeyState(WXK_SHIFT));
    else if(ms.RightIsDown())
        app->rightDrag(prevX, prevY, clickX, clickY, queuedDx, queuedDy, wxGetKeyState(WXK_SHIFT));

    queuedDx = 0;
    queuedDy = 0;

    refreshIfNeeded();
}

void wxCapoc_Canvas::refreshIfNeeded()
{
    if(app->engine->needRefresh)
        Render();
}

void wxCapoc_Canvas::Resize(wxSizeEvent &rsz) {
    wxSize s = GetSize();
    app->engine->reshape((unsigned int) s.x, (unsigned int) s.y);
}

void wxCapoc_Canvas::Paint(wxPaintEvent & WXUNUSED(event)) {
    /* Normally timer should take care of this */
    Render();
}

void wxCapoc_Canvas::Render() {
    SetCurrent(*glContext);
    wxPaintDC dc(this);
    app->engine->render();
    SwapBuffers();
}

void wxCapoc_Canvas::OnKeyDown(wxKeyEvent &event) {
    app->keyPress(currentKey = event.GetKeyCode(), wxGetKeyState(WXK_SHIFT));
}

void wxCapoc_Canvas::OnKeyUp(wxKeyEvent &event) {
    app->keyRelease(currentKey, wxGetKeyState(WXK_SHIFT));
    currentKey = WXK_NONE;
}

void wxCapoc_Canvas::OnMouseMoved(wxMouseEvent &event) {
    wxMouseState ms = wxGetMouseState();
    if(ms.LeftIsDown() || ms.RightIsDown()) {
        queuedDx += event.GetX() - prevX;
        queuedDy += event.GetY() - prevY;
        prevX = event.GetX(); prevY = event.GetY();
    }
}

void wxCapoc_Canvas::OnMouseDown(wxMouseEvent &event) {
    prevX = clickX = event.GetX();
    prevY = clickY = event.GetY();
    SetFocus();
}
