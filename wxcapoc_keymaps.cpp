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

void wxCapoc_Main::initKeymaps()
{
    keyMap &m = maps[WXCMAP_ROOT][WXCA_HOLD][WXCA_BARE];
    m[WXK_UP] = "camera advance\n";
    m[WXK_DOWN] = "camera retreat\n";
    m[WXK_LEFT] = "camera left\n";
    m[WXK_RIGHT] = "camera right\n";
    m[WXK_PAGEUP] = "camera up\n";
    m[WXK_PAGEDOWN] = "camera down\n";

    m['w'] = "camera advance\n";
    m['s'] = "camera retreat\n";
    m['a'] = "camera left\n";
    m['d'] = "camera right\n";
    m['W'] = "camera advance\n";
    m['S'] = "camera retreat\n";
    m['A'] = "camera left\n";
    m['D'] = "camera right\n";

    m['-'] = "view zoom current out\n";
    m['='] = "view zoom current in\n";
    m[','] = "view offset xsect out\n";
    m['.'] = "view offset xsect in\n";

    keyMap &s = maps[WXCMAP_ROOT][WXCA_HOLD][WXCA_SHIFT];
    s[WXK_UP] = "camera slightly advance\n";
    s[WXK_DOWN] = "camera slightly retreat\n";
    s[WXK_LEFT] = "camera slightly left\n";
    s[WXK_RIGHT] = "camera slightly right\n";
    s[WXK_PAGEUP] = "camera slightly up\n";
    s[WXK_PAGEDOWN] = "camera slightly down\n";

    s['w'] = "camera slightly advance\n";
    s['s'] = "camera slightly retreat\n";
    s['a'] = "camera slightly left\n";
    s['d'] = "camera slightly right\n";
    s['W'] = "camera slightly advance\n";
    s['S'] = "camera slightly retreat\n";
    s['A'] = "camera slightly left\n";
    s['D'] = "camera slightly right\n";

    s['-'] = "view clip current in\n";
    s['='] = "view clip current out\n";
    s[','] = "view offset xsect out by 0.1\n";
    s['.'] = "view offset xsect in by 0.1\n";

    keyMap &p = maps[WXCMAP_ROOT][WXCA_PRESS][WXCA_BARE];
    p[WXK_F1] = "view switch plan\n";
    p[WXK_F2] = "view switch profile\n";
    p[WXK_F3] = "view switch xsect\n";
    p[WXK_F4] = "view switch fourview\n";
    p[WXK_F5] = "view switch caver\n";
    p[WXK_F6] = "view switch nvmcam\n";
    p[WXK_ESCAPE] = "model 0 nvm hide all\n";

    p['1'] = "view switch plan\n";
    p['2'] = "view switch profile\n";
    p['3'] = "view switch xsect\n";
    p['4'] = "view switch fourview\n";
    p['5'] = "view switch caver\n";
    p['6'] = "view switch nvmcam\n";
    p['['] = "model 0 nvm camera next set-active\n";
    p[']'] = "model 0 nvm camera prev set-active\n";
    p['\\'] = "model 0 nvm camera current reproject throw-cursor\n";
    p['/'] = "model 0 selection fit-plane\n";

    keyMap &x = maps[WXCMAP_ROOT][WXCA_PRESS][WXCA_SHIFT];
    x['['] = "model 0 nvm show prev-match\n";
    x[']'] = "model 0 nvm show next-match\n";
    x['/'] = "model 0 nvm search sparse reorder\n";

    /*
    "model 0 transform pitch 2\n"
    "model 0 transform pitch -2\n"
    "model 0 transform yaw 2\n"
    "model 0 transform yaw -2\n"
    "model 0 transform roll 2\n"
    "model 0 transform roll -2\n"
    "model 0 transform scale 1.2\n"
    "model 0 transform scale 0.833333333333\n"

    "model 0 transform translate 0 1 0\n"
    "model 0 transform translate 0 -1 0\n"
    "model 0 transform translate 0 0 1\n"
    "model 0 transform translate 0 0 -1\n"
    "model 0 transform translate 1 0 0\n"
    "model 0 transform translate -1 0 0\n"

    "model 0 transform pitch 0.5\n"
    "model 0 transform pitch -0.5\n"
    "model 0 transform yaw 0.5\n"
    "model 0 transform yaw -0.5\n"
    "model 0 transform roll 0.5\n"
    "model 0 transform roll -0.5\n"
    "model 0 transform scale 1.05\n"
    "model 0 transform scale 0.952380952380\n"

    "model 0 transform translate 0 0.1 0"
    "model 0 transform translate 0 -0.1 0"
    "model 0 transform translate 0 0 0.1"
    "model 0 transform translate 0 0 -0.1"
    "model 0 transform translate 0.1 0 0"
    "model 0 transform translate -0.1 0 0"
*/
}
