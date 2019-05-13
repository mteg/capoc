/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under CC BY-SA 4.0 license
 *
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdarg>
#ifdef WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif
#define _USE_MATH_DEFINES
#include <cmath>
#include "capcli.h"
#include "clidata.h"
#include "capdebug.h"

capCliThread::capCliThread(capCli *cliController, struct tcpConn *conn)  : cli(cliController->capCliData) {
    this->cliController = cliController;
    this->conn = conn;
    this->startChat();
}

capCliThread::capCliThread(capCli *cliController)  : cli(cliController->capCliData) {
    this->cliController = cliController;
    this->conn = NULL;
    this->noTerminal = true;
    this->commandResult.size = 65536;
    this->commandResult.mem = (char*) malloc(this->commandResult.size);
}

void capCliThread::keyAction(unsigned  char key, int type)
{
    capCli *cli = this->cliController;
    if(cli->currentKeymap)
        if(cli->currentKeymap->actions[type][key])
        {
            this->execString(cli->currentKeymap->actions[type][key]);
            return;
        }

    if(cli->rootKeymap)
        if(cli->rootKeymap->actions[type][key])
        {
            this->execString(cli->rootKeymap->actions[type][key]);
            return;
        }
}

void capCliThread::axisAction(int action, float step, float x1, float y1, float x2, float y2)
{
    capCli *cli = this->cliController;
    capRenderer *r = cli->engine->renderer;

    if(step != 0.0f)
        cli->engine->needRefresh = true;

    switch(action)
    {
        case CAD_CAM_PITCH: r->camPitch(-step); break;
        case CAD_CAM_YAW: r->camYaw(step); break;
        case CAD_CAM_PANZ: r->camMove(-step); break;
        case CAD_CAM_PANX: r->camStrafe(step); break;
        case CAD_CAM_PANY: r->camLift(step); break;
        case CAD_RANGE_ADJUST:
        {
            capNvmCam *nc = cli->engine->renderer->texturedCam;
            if (!nc) break;

            capGenericPoint m1 = cli->engine->renderer->screenPosToNvm(x2, y2);
            capGenericPoint m2 = cli->engine->renderer->screenPosToNvm(x1, y1);
            cli->engine->repr.rasterRange = m1.x - m2.x;
            cli->engine->needRefresh = true;
            break;
        }
        case CAD_MARKER_SET: {
            capNvmCam *nc = cli->engine->renderer->texturedCam;
            if (!nc) break;

            capGenericPoint m = cli->engine->renderer->screenPosToNvm(x2, y2);
            nc->marker.x = m.x;
            nc->marker.y = m.y;
            nc->marker.valid = true;

            cli->engine->needRefresh = true;
            break;
            // todo reimplement nvm markers
            /*
            case CAD_MARKER_X:
                if(this->texturedCam)
                {
                    this->texturedCam->markerX += step;
                    if(this->texturedCam->markerX > 1.0)
                        this->texturedCam->markerX = 1.0;
                    else if(this->texturedCam->markerX < -1.0)
                        this->texturedCam->markerX = -1.0;
                }
                break;
            case CAD_MARKER_Y:
                if(this->texturedCam)
                {
                    this->texturedCam->markerY += step;
                    if(this->texturedCam->markerY > 1.0)
                        this->texturedCam->markerY = 1.0;
                    else if(this->texturedCam->markerY < -1.0)
                        this->texturedCam->markerY = -1.0;
                }
                break;
                */
        }
        case CAD_NVM_PANX:
            r->views[CAV_NVMCAM].panX -= step*r->views[CAV_NVMCAM].orthoZoom;
            break;
        case CAD_NVM_PANY:
            r->views[CAV_NVMCAM].panY += step*r->views[CAV_NVMCAM].orthoZoom;
            break;
        case CAD_MARK_POINTS:
        case CAD_MEASURE:
        {
            float z1 = -r->views[CAV_XSECT].zOffset;
            float radius = fabsf(2 * r->views[CAV_XSECT].orthoZoom * (x2-x1));

            x1 *= r->views[CAV_XSECT].orthoZoom;
            y1 *= r->views[CAV_XSECT].orthoZoom;

            if(action == CAD_MEASURE)
            {
                x2 *= r->views[CAV_XSECT].orthoZoom;
                y2 *= r->views[CAV_XSECT].orthoZoom;

                r->setRuler(x1, y1, x2, y2);
                cli->engine->virtualMeasurement(x1, y1, x2, y2);
                // todo reimplement measurements
                /*
                this->lx1 = x1; this->ly1 = y1;
                this->lx2 = x2; this->ly2 = y2;
                this->miscFlags |= CMF_DISPLAY_LINE;
                 */
            }
            else
            {
                if(!cli->engine->models.empty())
                {
                    capModel_ball mb, vb;
                    vb.x0 = x1; vb.y0 = y1; vb.z0 = z1; vb.radius = radius;
                    unsigned int p = cli->engine->models[0]->ballSelect(mb = cli->engine->models[0]->ballViewToModel(vb));
                    cli->engine->setStatus("Selection: (%.2f, %.2f, %.2f) m, radius %.2f m, points %d", mb.x0, mb.y0, mb.z0, mb.radius, p);
                }
            }
            cli->engine->needRefresh = true;
            break;
        }
    }

}

void capCliThread::axisAction(int button, int axis, float step, float x1, float y1, float x2, float y2)
{
    capCli *cli = this->cliController;
    capKeymap *k = cli->currentKeymap;
    if(!k)
        k = cli->rootKeymap;
    else if(!k->dragActions[button][axis])
        k = cli->rootKeymap;

    if(!k) return;

    this->axisAction(k->dragActions[button][axis], step * k->dragFactors[button][axis], x1, y1, x2, y2);
}


struct capKeymap *capCli::findmap(const char *keyMap)
{
    struct capKeymap *k;

    if(!keyMap) keyMap = "root";
    /* Search for a keymap */
    for(k = this->keymaps; k; k = k->next)
        if(!strcmp(keyMap, k->name)) break;

    if(!k)
    {
        // todo handle memory shortage
        k = (struct capKeymap*) malloc(sizeof(struct capKeymap));
        memset(k, 0, sizeof(struct capKeymap));
        k->name = strdup(keyMap);
        k->next = this->keymaps;
        this->keymaps = k;
        if(!strcmp(keyMap, "root"))
            this->rootKeymap = k;
    }
    return k;
}

void capCli::bindkey(const char *keyMap, const char *keyName, int actionType, const char *action)
{
    struct capKeymap *k = findmap(keyMap);
    char *finAction;
    unsigned int key, finLen;

    if(strlen(keyName) == 1)
        key = (unsigned int) keyName[0];
    else
        key = (unsigned int) strtol(keyName, NULL, 0);


    // todo handle memory shortage
    finAction = (char*) malloc(finLen = ((int) strlen(action)) + 16);
    snprintf(finAction, finLen, "%s\r", action);
    k->actions[actionType][key & 0xff] = finAction;
}

void capCli::setKeymap(const char *mapName) {
    struct capKeymap *k;
    for(k = this->keymaps; k; k = k->next)
        if(!strcmp(mapName, k->name))
        {
            this->currentKeymap = k;
            break;
        }
}


bool capCliThread::fetchColor(capColor *c, bool resetAlpha)
{
    if(!this->token(T_SET_B)) return false;

    c->r = (unsigned char) this->numToken(T_SET_R, 255);
    c->g = (unsigned char) this->numToken(T_SET_G, 255);
    c->b = (unsigned char) this->numToken(T_SET_B, 255);
    if(resetAlpha || this->token(T_SET_A))
        c->a = (unsigned char) this->numToken(T_SET_A, 255);
    return true;
}
bool capCliThread::fetchColor(capColor *c)
{
    return this->fetchColor(c, true);
}

float capCliThread::floatToken(clitok_id token, float defval)
{
    cliToken *tok = this->token(token);
    if(!tok) return defval;
    return (float) atof(tok->data.sstr);
}


void capCliThread::exec() {
    const char *s;
    int modelId, delay, p;
    capRenderer *r = this->cliController->engine->renderer;
    capEngine *engine = this->cliController->engine;

    if((delay = this->numToken(T_DELAY, 0)))
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int) delay));

    if(this->token(T_EXIT))
    {
        if(!exitContext())
            this->cliController->server->drop(this->conn);
        return;
    }

    if((s = this->strToken(T_BINDCODE, NULL)))
    {
        int action = CA_HOLD;
        if(this->token(T_PRESS))
            action = CA_PRESS;
        else if(this->token(T_RELEASE))
            action = CA_RELEASE;


        this->cliController->bindkey(this->strToken(T_KEYMAP, NULL), s, action, this->strToken(T_COMMAND, "camera advance"));
        return;
    }

    if(this->token(T_DRAG))
    {
        struct capKeymap *k = this->cliController->findmap(this->strToken(T_KEYMAP, NULL));
        int btn = this->numToken(T_BUTTON, 0) + (this->token(T_SHIFT) ? CAB_SHIFT : 0);
        int ax  = this->numToken(T_AXIS, 0);

        k->dragActions[btn][ax] = (char) this->numToken(T_DRAG, 0);
        k->dragFactors[btn][ax] = strtof(this->strToken(T_FACTOR, "0.1"), NULL);
        return;
    }

    if(this->token(T_CAMERA))
    {
        float shiftStep = engine->shiftStep, rotateStep = engine->rotateStep;
        const char *captureFile, *by;


        if((by = this->strToken(T_BY, NULL)))
            shiftStep = (float) atof(by);

        if(this->token(T_SLIGHTLY))
        {
            shiftStep *= 0.1;
            rotateStep *= 0.1;
        }

        if(this->token(T_LOG))
        {
            capDebug::report(0, "camera set %.6f %.6f %.6f %.1f %.1f\n", r->cam.x, r->cam.y, r->cam.z, r->cam.yaw/CAP_PI*180.0, r->cam.pitch/CAP_PI*180.0);
            return;
        }
        else if((captureFile = this->strToken(T_CAPTURE, NULL)))
        {
            r->screenshot(captureFile);
            return;
        }

        if(this->token(T_ADVANCE))
            r->camMove(shiftStep);
        else if(this->token(T_RETREAT))
            r->camMove(-shiftStep);
        else if(this->token(T_RIGHT))
            r->camStrafe(-shiftStep);
        else if(this->token(T_LEFT))
            r->camStrafe(shiftStep);
        else if(this->token(T_LOOKRIGHT))
            r->camYaw(rotateStep);
        else if(this->token(T_LOOKLEFT))
            r->camYaw(-rotateStep);
        else if(this->token(T_LOOKUP))
            r->camPitch(rotateStep);
        else if(this->token(T_LOOKDOWN))
            r->camPitch(-rotateStep);
        else if(this->token(T_UP))
            r->camLift(shiftStep);
        else if(this->token(T_DOWN))
            r->camLift(-shiftStep);
        else if(this->token(T_SET))
        {
            const char *str;
            if((str = this->strToken(T_X, NULL))) r->cam.x = (float) atof(str);
            if((str = this->strToken(T_Y, NULL))) r->cam.y = (float)  atof(str);
            if((str = this->strToken(T_Z, NULL))) r->cam.z = (float)  atof(str);
            if((str = this->strToken(T_YAW, NULL))) r->cam.yaw = (float) (atof(str)/180.0 * CAP_PI);
            if((str = this->strToken(T_PITCH, NULL))) r->cam.pitch = (float) (atof(str)/180.0 * CAP_PI);
        }
        else if(this->token(T_XPLANE))
        {
            this->fetchColor(&r->sectColor, false);
            if(this->token(T_ALPHA))
            {
                int step = this->numToken(T_STEP, 0);
                r->sectColor.a += step;
            }
        }

        engine->setStatus("Camera at %f %f %f, looking towards %.1f° pitch %+.1f°", r->cam.x, r->cam.y, r->cam.z, r->cam.yaw/CAP_PI*180.0, r->cam.pitch/CAP_PI*180.0);
    }

    if((p = this->numToken(T_SETVIEW, -1)) != -1)
        r->switchView(p);

    if(this->token(T_ACTIVATE))
        this->cliController->setKeymap(this->strToken(T_KEYMAP, "root"));

    if(this->token(T_VIEW))
        if(this->token(T_ZOOM) || this->token(T_CLIP) || this->token(T_OFFSET))
        {
            int viewId = this->numToken(T_MODVIEW, 255);
            float factor = engine->zoomStep, offset = engine->shiftStep;
            const char *by;

            if(viewId == 255) viewId = r->currentView;
            if(!(viewId == CAV_PLAN || viewId == CAV_XSECT || viewId == CAV_PROFILE || viewId == CAV_NVMCAM || viewId == CAV_CAVER)) viewId = CAV_XSECT;
            if((by = this->strToken(T_BY, NULL)))
                factor = offset = (float) atof(by);

            if(viewId == CAV_CAVER)
            {
                if(this->token(T_ZOOM))
                {
                    float fov = r->views[CAV_CAVER].fov;
                    fov += (this->token(T_OUT) ? -1 : 1) * engine->shiftStep;
                    if (fov <= 1.0) fov = 1.0;
                    if (fov >= 360.0) fov = 360.0;
                    r->views[viewId].fov = fov;
                }
            }

            if(this->token(T_OUT)) { factor = 1.0f / factor; offset = -offset; }
            if(this->token(T_SET))
            {
                if(this->token(T_ZOOM))
                    r->views[viewId].orthoZoom = factor;
                else if(this->token(T_CLIP))
                    r->views[viewId].orthoClip = factor;
                else
                    r->views[viewId].zOffset = factor;
            }
            else if(this->token(T_ZOOM))
                r->views[viewId].orthoZoom /= factor;
            else if(this->token(T_CLIP))
                r->views[viewId].orthoClip /= factor;
            else
            {
                r->views[viewId].zOffset += offset;
                capDebug::report(2, "zOffset is now %f", r->views[viewId].zOffset);
            }
        }

    if(this->token(T_FOG))
        if(r->currentView != CAV_FOURVIEW)
        {
            float shiftStep = engine->shiftStep;
            if(this->token(T_SLIGHTLY))
                shiftStep *= 0.1;

            if(this->token(T_ON))
                r->views[r->currentView].fogStart = 10.0;
            else if(this->token(T_OFF))
                r->views[r->currentView].fogStart = 0.0;
            else if(this->token(T_IN))
                r->views[r->currentView].fogStart -= shiftStep;
            else if(this->token(T_OUT))
                r->views[r->currentView].fogStart += shiftStep;
            else if(this->token(T_SET))
                r->views[r->currentView].fogStart = this->floatToken(T_LEVEL, 0.0);

        }


    if(this->token(T_VIEW))
    {
        if(this->token(T_RSIZE))
        {
            int v = r->currentView;

            if(v == CAV_FOURVIEW) v = CAV_CAVER;
            if(this->token(T_ENLARGE))
                r->views[v].rasterSize *= 1.41;
            else if(this->token(T_SHRINK))
                r->views[v].rasterSize /= 1.41;
            else if(this->token(T_VALUE))
                r->views[v].rasterSize = this->floatToken(T_VALUE, 0.0);
        }

        
        // todo orientation histogram
        /*
        if(this->token(T_DRADIUS))
        {
            if(this->token(T_ENLARGE))
                m->dipRadius *= 1.05;
            else if(this->token(T_SHRINK))
                m->dipRadius /= 1.05;
            else if(this->token(T_VALUE))
                m->dipRadius = atof(this->strToken(T_VALUE, "0.0"));
        }

        if(this->token(T_DSCALE))
        {
            if(this->token(T_ENLARGE))
                m->dipScale *= 1.05;
            else if(this->token(T_SHRINK))
                m->dipScale /= 1.05;
            else if(this->token(T_VALUE))
                m->dipScale = atof(this->strToken(T_VALUE, "0.0"));
        }
         */

        if(this->token(T_RRANGE))
        {
            if(this->token(T_ENLARGE))
                engine->repr.rasterRange *= 1.2;
            else if(this->token(T_SHRINK))
                engine->repr.rasterRange /= 1.2;
        }
    }

    if(this->token(T_LIGHT))
    {
        const char *bystr;
        if(this->token(T_ENABLE))
            r->enableLight();
        else if(this->token(T_DISABLE))
            r->disableLight();
        else if((bystr = this->strToken(T_BY, NULL)))
        {
            auto by = (float) atof(bystr);
            if(this->token(T_RAISE))
                r->light.lightHeight += by;
            else if(this->token(T_OFFSET))
                r->light.lightOffset += by;
            else if(this->token(T_QUADRATIC) && this->token(T_BRIGHTEN))
                r->light.lightQuadAtt *= by;
            else if(this->token(T_QUADRATIC) && this->token(T_SET))
                r->light.lightQuadAtt = by;
            else if(this->token(T_LINEAR) && this->token(T_BRIGHTEN))
                r->light.lightLinAtt *= by;
            else if(this->token(T_LINEAR) && this->token(T_SET))
                r->light.lightLinAtt = by;
            else if(this->token(T_CONSTANT) && this->token(T_BRIGHTEN))
                r->light.lightConstAtt *= by;
            else if(this->token(T_CONSTANT) && this->token(T_SET))
                r->light.lightConstAtt = by;
            capDebug::report(2, "Set light to: q=%.6f l=%.6f c=%.6f", r->light.lightQuadAtt, r->light.lightLinAtt, r->light.lightConstAtt);
        }
        else if(this->token(T_LDRAG))
        {
            r->light.lightFlags |= CLF_DRAGGED;
        }
        else if(this->token(T_LDROP))
        {
            r->light.lightFlags |= CLF_DROPPED;
            r->light.x = r->cam.x;
            r->light.y = r->cam.y;
            r->light.z = r->cam.z;
        }
    }

    if(this->token(T_SET_REPROJECTION))
    {
        if((s = this->strToken(T_ZTOL, NULL))) engine->repr.zTol = (float) atof(s);
        if((s = this->strToken(T_ZMAX, NULL))) engine->repr.zMax = (float) atof(s);
        if((s = this->strToken(T_ZMIN, NULL))) engine->repr.zMin = (float) atof(s);
        if((s = this->strToken(T_DOTMAX, NULL))) engine->repr.dotMax = (float) atof(s);
        if((s = this->strToken(T_DOTMIN, NULL))) engine->repr.dotMin = (float) atof(s);
        if(this->token(T_ENABLE_CACHE)) engine->repr.flags |= CARF_CACHE_MAPS;
        if(this->token(T_DISABLE_CACHE)) engine->repr.flags &= ~CARF_CACHE_MAPS;
    }

    // todo nvm texture transparency adjustment
    /*
    if(this->token(T_SET_NVM))
    {
        if(this->token(T_NVM_ALPHA))
            m->nvmAlpha = this->numToken(T_NVM_ALPHA, 0);
    }
     */


    if((modelId = this->numToken(T_MODEL, -1)) != -1)
    {
        int destModel, modelCount = (int) engine->models.size();
        const char *loadFrom;

        if(modelId == 255) modelId = modelCount;

        if((loadFrom = this->strToken(T_LOAD_MODEL, NULL)))
            if(modelId <= modelCount || ((modelId<modelCount) && this->token(T_APPEND)))
            {
                capColor rgba;
                capModel *model;
                bool got_rgba = this->fetchColor(&rgba);

                if(modelId == modelCount)
                    engine->models.push_back(model = new capModel(r));
                else
                    model = engine->models[modelId];

                if(!model->loadFile(loadFrom, this->token(T_APPEND) ? 1 : 0, got_rgba ? &rgba : NULL))
                {
                    this->printf("Cannot load model '%s': '%s'\r\n", loadFrom, capDebug::errorMessage);
                    return;
                }
                engine->needRefresh = true;
                return;
            }

        if(modelId < modelCount)
        {
            const char *saveAs;
            capModel *model = engine->models[modelId];

            if(this->token(T_POINT))
            {
                const char *zC;
                if((zC = this->strToken(T_PZ, NULL))) {
                    const char *pointName = this->strToken(T_POINT_ID, NULL);
                    model->setPoint((char *const) pointName, this->floatToken(T_PX, 0.0), this->floatToken(T_PY, 0.0),
                                    (float) atof(zC));
                }
                else if(this->strToken(T_DELETE, NULL))
                    model->deletePoint((char *const) this->strToken(T_POINT_ID, NULL));
                else if(this->token(T_SEE))
                    model->seePoint((char *const) this->strToken(T_POINT_ID, NULL), &engine->renderer->cam, -3.0f);
            }

            if((destModel = this->numToken(T_TRANSFORM_TO, -1)) != -1)
                    if(destModel < (int) engine->models.size() && destModel >= 0)
                        model->findTransformTo(engine->models[destModel], this->boolToken(T_COMPOSE));


            if(this->token(T_SELECTION))
            {
                if(this->token(T_DELETE))
                    model->deleteSelected();
                else if(this->token(T_UNDELETE))
                    model->undeleteAll();
                else if(this->token(T_RESET))
                    model->resetSelection();
                else if(this->token(T_FIT)) {
                    if(model->fitPlaneToSelection())
                        engine->setStatus("Fit plane: dip %.1f, dip direction %.1f", model->fitPlane.dip, model->fitPlane.dipDir);
                }
                else if(this->token(T_GROW))
                    model->growSelection(this->floatToken(T_BY, 0.1));
                else if(this->token(T_WALKEDGE))
                    model->walkEdge(this->numToken(T_STEPS, 1));
                else if(this->token(T_BOUNDARY))
                    model->selectBoundary();
                else if(this->token(T_POPCOMPONENT))
                    model->popComponent();
                else if(this->token(T_PATCHES))
                    model->dumpComponents();
                else if(this->token(T_ADD_BEHIND))
                    model->selectBehind();

                r->reinitModel(model);
            }


            // todo decide upon colorQuality
            /*

            if(this->token(T_RESET_COLORS))
            {
                free(model->colorQuality);
                model->colorQuality = NULL;
            }

             */

            if(this->token(T_RECOLOR))
            {
                const char *recolorFile;
                capColor rgba;
                if(this->fetchColor(&rgba))
                    model->recolor(&rgba);

                if((recolorFile = this->strToken(T_RECOLOR_FROM, NULL)))
                {
                    int toDo = 0;
                    if(this->token(T_GRAY_UNCOLORABLE)) toDo |= CAC_GRAY_UNCOLORABLE;
                    if(this->token(T_DELETE_UNCOLORABLE)) toDo |= CAC_DELETE_UNCOLORABLE;
                    if(this->token(T_UNDELETE_COLORABLE)) toDo |= CAC_UNDELETE_COLORABLE;

                    if(!model->recolorFrom(recolorFile, toDo))
                        this->printf("Error while recoloring: %s\r\n", capDebug::errorMessage);
                }
            }

            if((saveAs = this->strToken(T_SAVE_AS, NULL)))
            {
                if(this->token(T_SELECTION))
                    model->saveSelectionOff(saveAs);
                else
                    model->saveOff(saveAs);
                return;
            }

            if(this->token(T_INFO))
            {
                this->printf("Vert count %d, face count %d\r\n", model->nv, model->nt);
                this->printf("Affine matrix:\r\n");
                this->printAffine(&model->affineMatrix);
                this->printf("\r\nModelview matrix:\r\n");
                this->printAffine(&model->modelviewMatrix);
                this->printf("\r\nScales: affine %g modelview %g\r\n", model->affineMatrix.getScale(), model->modelviewMatrix.getScale());

            }

            if(this->token(T_DISPLAY))
            {

                if(model->decRate > 0)
                    if(this->token(T_REFINE))
                        model->decRate--;

                if(model->decRate < CA_DEC_MAX)
                    if(this->token(T_GENERALIZE))
                        model->decRate++;

                if(this->token(T_POINTS))
                {
                    model->flags &= ~(CAMF_POINTS | CAMF_FACES | CAMF_EDGES);
                    model->flags |= CAMF_POINTS;
                }
                else if(this->token(T_FACES))
                {
                    model->flags &= ~(CAMF_POINTS | CAMF_FACES | CAMF_EDGES);
                    model->flags |= CAMF_FACES;
                }
                else if(this->token(T_EDGES))
                {
                    model->flags &= ~(CAMF_POINTS | CAMF_FACES | CAMF_EDGES);
                    model->flags |= CAMF_EDGES;
                }
                else if(this->token(T_NONE))
                    model->flags &= ~(CAMF_POINTS | CAMF_FACES | CAMF_EDGES);

                model->updateDisplay();
            }

            if(this->token(T_NVM))
            {
                const char *fname;
                if((fname = this->strToken(T_LOAD_NVM, NULL)))
                {
                    int n = this->numToken(T_NVM_ID, 0);
                    
                    delete model->nvm;
                    model->nvm = new capNvm(r, model);
                    if(!model->nvm->load(fname, n))
                        this->printf("%s\r\n", capDebug::errorMessage);
                }
                else if(this->token(T_SEARCH))
                {
                    double cx = 0, cy = 0, cz = 0, r = 0.0, pcnt = 0.0;
                    for(p = 0; p<(int) model->nv; p++)
                    {
                        if(!CA_HASFLAG_A(model->vf, p, CAV_MARKED)) continue;
                        cx += model->v[p].x; cy += model->v[p].y; cz += model->v[p].z;
                        pcnt = pcnt + 1.0;
                    }
                    if(pcnt == 0.0)
                    {
                        this->printf("Selection is empty\r\n");
                        return;
                    }

                    cx /= pcnt; cy /= pcnt; cz /= pcnt;
                    for(p = 0; p<(int) model->nv; p++)
                    {
                        double dist;
                        if(!CA_HASFLAG_A(model->vf, p,  CAV_MARKED)) continue;
                        dist =  (model->v[p].x-cx)*(model->v[p].x-cx) +
                                (model->v[p].y-cy)*(model->v[p].y-cy) +
                                (model->v[p].z-cz)*(model->v[p].z-cz);
                        if(dist > r) r = dist;
                    }
                    this->printf("Selection center: %f %f %f, radius %f\r\n", cx, cy, cz, sqrt(r));

                    if(this->token(T_DENSE) && model->nvm)
                    {
                        float affineScale = model->affineMatrix.getScale();
                        unsigned int i, skipped = 0;
                        double maxDist;
                        int toDo = CAR_ONLY_SELECTED;


                        if(this->token(T_NOMAP)) toDo |= CAR_NO_DEPTHMAP;

                        maxDist = sqrt(r) + engine->repr.zMax/affineScale + engine->repr.zTol/affineScale;
                        maxDist *= maxDist;

                        for(i = 0; i<model->nvm->n_cams; i++)
                        {
                            double distSquare;
                            if(!(i & 63))
                                capDebug::report(1, "Reprojected %d cams + %d skipped, %d remaining...", i-skipped, skipped, model->nvm->n_cams-i);
                            if(engine->repr.zMax > 0.0)
                            {
                                distSquare = (model->nvm->cams[i].x - cx) * (model->nvm->cams[i].x - cx) +
                                             (model->nvm->cams[i].y - cy) * (model->nvm->cams[i].y - cy) +
                                             (model->nvm->cams[i].z - cz) * (model->nvm->cams[i].z - cz);
                                if(distSquare > maxDist)
                                {
                                    model->nvm->cams[i].seenFeatures = 0;
                                    skipped++;
                                    continue;
                                }

                            }
                            model->nvm->reproject(model, &engine->repr, &model->nvm->cams[i], toDo, NULL);

                        }
                    }
                    else if(model->nvm)
                        model->nvm->findCamsLookingIntoBall((float) cx, (float) cy, (float) cz, (float) sqrt(r), NULL);

                    if(model->nvm)
                    {
                        unsigned int i;
                        if(this->token(T_REORDER))
                        {
                            model->nvm->orderBySeen();
                            model->nvm->stepCam(1);
                        }
                        for(i = 0; i<model->nvm->n_cams; i++)
                        {
                            int camId = model->nvm->camIndex[i];
                            if(model->nvm->cams[camId].seenFeatures > 0)
                                this->printf("%s %4d points\r\n", model->nvm->cams[camId].filename, model->nvm->cams[camId].seenFeatures);
                        }
                    }
                }
            }

            if(this->token(T_NORMALS))
                model->computeNormals();

            if(this->token(T_TRANSFORM))
            {
                const char *transformParam;
                char *eptr;
                if((transformParam = this->strToken(T_TRANSFORM_SCALE, NULL)))
                {
                    float scale = strtof(transformParam, &eptr);
                    if(scale > 0.0)
                        model->scale(scale);
                }

                if((transformParam = this->strToken(T_TRANSFORM_ANGLE, NULL)))
                {
                    float angle = strtof(transformParam, &eptr);
                    int axis = 0;
                    if(this->token(T_TRANSFORM_YAW)) axis = 1;
                    if(this->token(T_TRANSFORM_ROLL)) axis = 2;
                    model->rotate(angle, axis);
                }

                if(this->token(T_TZ))
                    model->translate(this->floatToken(T_TX, 0.0), this->floatToken(T_TY, 0.0), this->floatToken(T_TZ, 0.0));

                if(this->token(T_MATRIX))
                {
                    const clitok_id matrixTokens[] = {
                            T_M11, T_M21, T_M31, 0,
                            T_M12, T_M22, T_M32, 0,
                            T_M13, T_M23, T_M33, 0,
                            T_M14, T_M24, T_M34, 0,
                    };
                    capAffineMatrix *a = &model->affineMatrix;
                    int i;
                    a->setUnit();
                    for(i = 0; i<16; i++)
                        if(matrixTokens[i])
                            a->m[i] = this->floatToken(matrixTokens[i], 0.0);
                }


            }

            if(model->nvm)
            {
                if((this->token(T_SHOW) || this->token(T_HIDE)) && this->token(T_NVM))
                {
                    unsigned int flagMask = 0;
                    if(this->token(T_2DPOINTS))
                      flagMask |= CACF_DISPLAY_PLANAR;
                    if(this->token(T_3DPOINTS))
                      flagMask |= CACF_DISPLAY_SPATIAL;
                    if(this->token(T_LINES))
                      flagMask |= CACF_DISPLAY_MATCHES;
                    if(this->token(T_IMAGE))
                      flagMask |= CACF_DISPLAY_IMAGE;
                    if(this->token(T_HIDEALL))
                        flagMask = CACF_DISPLAY_PLANAR | CACF_DISPLAY_SPATIAL | CACF_DISPLAY_MATCHES | CACF_DISPLAY_IMAGE;

                    if(this->token(T_HIDE))
                        r->nvmDisplay &= ~flagMask;
                    else
                        r->nvmDisplay |= flagMask;



                    if(r->texturedCam) {
                        capNvmCam *cam = r->texturedCam;
                        if(flagMask)
                            r->reinitNvmCam(r->texturedCam);
                        if (this->token(T_NEXT_MATCH))
                            model->nvm->stepMatch(cam, cam->marker, engine->repr.rasterRange, 1);
                        else if (this->token(T_PREV_MATCH))
                            model->nvm->stepMatch(cam, cam->marker, engine->repr.rasterRange, -1);
                    }


                }
                if(this->token(T_ALTER_CAMERA))
                {
                    int loopAll = 0;
                    capNvmCam *cam;
                    const char *camName = this->strToken(T_CAMERA_NAME, NULL);

                    if(this->token(T_ORDER))
                    {
                        if(this->token(T_DEPTH) || this->token(T_DISTANCE))
                        {
                            float offs = this->floatToken(T_OFFSET, 0.0) / model->affineMatrix.getScale();
                            if(this->token(T_DEPTH))
                                model->nvm->orderByDepth(offs);
                            else
                                model->nvm->orderByDistance(offs);
                        }
                        else if(this->token(T_NAME))
                            model->nvm->orderByName();
                        else if(this->token(T_INDEX))
                            model->nvm->orderByCamindex();
                        else if(this->token(T_SEEN))
                            model->nvm->orderBySeen();
                        model->nvm->stepCam(1);
                    }


                    if(camName)
                        cam = model->nvm->findCam(camName);
                    else
                    {
                        int offset = 0;
                        if(this->token(T_NEXT)) offset = 1;
                        if(this->token(T_PREV)) offset = -1;
                        cam = model->nvm->stepCam(offset);
                    }

                    if(this->token(T_ALL)) loopAll = 1;

                    if(!cam)
                    {
                        this->printf("Cannot find camera '%s'\r\n", camName ? camName : "<unknown>");
                        return;
                    }

                    if(this->token(T_REPROJECT))
                    {
                        unsigned int toDo = 0, i;

                        if(this->token(T_RECOLOR)) toDo |= CAR_COLOR;
                        if(this->token(T_THROW)) toDo |= CAR_THROW;
                        if(this->token(T_DEL_UNPROJ)) toDo |= CAR_DEL_UNPROJ;
                        if(this->token(T_GRAY_UNPROJ)) toDo |= CAR_GRY_UNPROJ;
                        if(this->token(T_UNDEL_COL)) toDo |= CAR_UND_COLOR;
                        if(this->token(T_DUMP)) toDo |= CAR_DUMP;
                        if(this->token(T_MARK)) toDo |= CAR_MARK;
                        if(this->token(T_ONLY_SELECTION)) toDo |= CAR_ONLY_SELECTED;
                        if(this->token(T_NOMAP)) toDo |= CAR_NO_DEPTHMAP;
                        if(this->token(T_SUMMARY)) toDo |= CAR_DUMP_SUMMARY;

                        /*
                        if(this->token(T_IMPROVE_ONLY)) toDo |= CAR_ONLY_IMPROVE;
                        if(this->token(T_RESET_QUALITY)) toDo |= CAR_RESET_QUALITY;
                         */

                        if(loopAll)
                        {
                            for(i = 0; i<model->nvm->n_cams; i++)
                                model->nvm->reproject(model, &engine->repr, &model->nvm->cams[model->nvm->camIndex[i]], toDo, this->strToken(T_DEPTH_MAP, NULL));

                            model->nvm->orderBySeen();
                            model->nvm->stepCam(1);
                        }
                        else
                            model->nvm->reproject(model, &engine->repr, cam, toDo, this->strToken(T_DEPTH_MAP, NULL));

                        if(toDo&CAR_THROW)
                        {
                            if(!model->cursor.valid)
                                engine->setStatus("Reprojection failed");
                            else
                            {
                                float v[3] = {model->cursor.x, model->cursor.y, model->cursor.z};
                                model->affineMatrix.transform(v);
                                engine->setStatus("Reprojected to: model (%.4f, %.4f, %.4f), real (%.4f, %.4f, %.4f)", model->cursor.x, model->cursor.y, model->cursor.z, v[0], v[1], v[2]);
                            }
                        }

                        if(toDo&(CAR_MARK | CAR_GRY_UNPROJ | CAR_DEL_UNPROJ | CAR_COLOR))
                            r->reinitModel(model);
                    }

                    if(this->token(T_DIAG))
                    {
                        if(this->token(T_NVMINFO))
                        {
                            this->printf("#%d: %s (%.0fx%.0f)\r\n", cam->camIndex, cam->filename, cam->width, cam->height);
                            this->printf("f = %.1f r = %.8f\r\n", cam->f, cam->r);
                            this->printf("[x, y, z] = X - [%.6f %.6f %.6f]\r\n", cam->x, cam->y, cam->z);
                            this->printf("xp = %+.6f*x %+.6f*y %+.6f*z\r\n", cam->rot->m[0], cam->rot->m[1], cam->rot->m[2]);
                            this->printf("yp = %+.6f*x %+.6f*y %+.6f*z\r\n", cam->rot->m[4], cam->rot->m[5], cam->rot->m[6]);
                            this->printf("zp = %+.6f*x %+.6f*y %+.6f*z\r\n", cam->rot->m[8], cam->rot->m[9], cam->rot->m[10]);
                            this->printf("Seen projections: %d, model features: %d\r\n", cam->projCount, cam->seenFeatures);
                        }
                        else if(this->token(T_MEAS))
                        {
                            unsigned int i;
                            this->printf("# imX imY seenBy X Y Z\r\n");
                            for(i = 0; i<cam->projCount; i++)
                            {
                                auto *s = &cam->nvm->projs[cam->nvm->camProjs[cam->firstProj + i]];
                                auto *p = &cam->nvm->points[s->pointIndex];
                                this->printf("%3d %4.0f %4.0f %3d %f %f %f\r\n", i, s->x, s->y, p->projCount, p->x, p->y, p->z);
                            }
                        }
                        else if(this->token(T_REPROJECT_POINT) && this->token(T_Z))
                        {
                            float x = this->floatToken(T_X, 0.0) - cam->x,
                                    y = this->floatToken(T_Y, 0.0) - cam->y,
                                    z = this->floatToken(T_Z, 0.0) - cam->z;

                            float xp, yp, zp;
                            xp = cam->rot->m[0]*x + cam->rot->m[1]*y + cam->rot->m[2]*z;
                            yp = cam->rot->m[4]*x + cam->rot->m[5]*y + cam->rot->m[6]*z;
                            zp = cam->rot->m[8]*x + cam->rot->m[9]*y + cam->rot->m[10]*z;

                            xp = xp * cam->f / zp;
                            yp = yp * cam->f / zp;

                            this->printf("%f %f\r\n", xp, yp);
                        }
                        return;
                    }


                    if(this->token(T_SET_ACTIVE))
                        model->nvm->setActive(cam);

                    if(this->token(T_SEE))
                        model->nvm->seeCam(cam, &engine->renderer->cam);
                }
            }

        }
        else
            this->printf("Model %d not loaded\r\n", modelId);
    }

    if((s = this->strToken(T_EXEC, NULL)))
    {
        FILE *fh = utf8open(s, "r");
        if(!fh)
        {
            this->printf("Cannot open '%s'\r\n", s);
            return;
        }
        this->sErase();
        this->execStream(fh);
        fclose(fh);
        return;
    }

    /*
    if((t = this->token(T_MISCFLAG)))
    {
        n = t->data.num;
        while((t = cli_token_next(cli, T_MISCFLAG, t)))
            n |= t->data.num;
        if(this->token(T_SET))
            m->miscFlags |= n;
        else if(this->token(T_CLEAR))
            m->miscFlags &= ~n;
        else
            m->miscFlags ^= n;
    }
     */

    /*
    if(this->token(T_LOG))
    {
        char  *captureFile;
        if(this->token(T_NEXT))
            this->engine->stationId++;
        else if(this->token(T_PREV) && this->engine->stationId)
            this->engine->stationId--;
        else if(this->token(T_COMMIT))
        {
            if(r->logLine[0])
            {
                fprintf(state.execLog, "station %d sequence %d %s\r\n", state.stationId, m->logSeq, state.logLine);
                fflush(state.execLog);
                m->logSeq++;
            }
        }
        else if((captureFile = this->strToken(T_CHANGE_LOG, NULL)))
        {
            if(state.execLog != stdout) fclose(state.execLog);
            state.execLog = fopen(captureFile, "a");
            if(!state.execLog)
                state.execLog = stdout;
            else
            {
                fprintf(state.execLog, "\r\nstarting new session\r\n");
                fflush(state.execLog);
            }
        }
        else if(this->token(T_DIPHIST))
        {
            int d, dd;
            struct ca_model *m = &state.models[0];
            diphist(&state, &state.models[0]);
            for(d = 0; d <= 90; d++)
                for(dd = 0; dd<360; dd++)
                    if(m->markHist[d][dd] > 0.0)
                        fprintf(state.execLog, "station %d sequence %d area %f dip %d dir %d\r\n", state.stationId, state.logSeq, m->markHist[d][dd], d, dd);
            state.logSeq++;
            fflush(state.execLog);
        }
    }
     */
    // todo logging does not work
    engine->needRefresh = true;
}

void capCliThread::write(const char *buf, size_t len) {
    if(this->conn)
        this->cliController->server->writeTo(this->conn, buf, len);
    else
    {
        if((this->commandResult.pos + len + 1) < this->commandResult.size)
            memcpy(this->commandResult.mem + this->commandResult.pos, buf, len);
        this->commandResult.pos += len;
        this->commandResult.mem[this->commandResult.pos] = 0;
/*        ::write(0, buf, len);*/

    }
}

int capCliThread::complete(clitok_id id, char *str, int len) {
    return 0;
}

capCli::capCli(const char *clidata, capEngine *engine)  {
    this->currentKeymap = NULL;
    this->rootKeymap = NULL;
    this->keymaps = NULL;
    this->capCliData = clidata;
    this->server = NULL;
    this->engine  = engine;
}

void capCliThread::printAffine(capAffineMatrix *am)
{
    float *m = am->m;
    this->printf("x' = %+f*x %+f*y %+f*z %+f\r\n", m[0], m[4], m[8], m[12]);
    this->printf("y' = %+f*x %+f*y %+f*z %+f\r\n", m[1], m[5], m[9], m[13]);
    this->printf("z' = %+f*x %+f*y %+f*z %+f\r\n", m[2], m[6], m[10], m[14]);
    this->printf("model 0 transform matrix x %f %f %f %f y %f %f %f %f z %f %f %f %f\r\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14]);
}

void capCliThread::printf(const char *fmt, ...)
{
    va_list ap;
#define CLI_MSG_BUFFER_SIZE	2048
    static char buffer[CLI_MSG_BUFFER_SIZE];
    char *nlbr1, *nlbr2;

    va_start(ap, fmt);
    vsnprintf(buffer, CLI_MSG_BUFFER_SIZE - 1, fmt, ap);
    this->write(buffer, strlen(buffer));
    
    if((nlbr1 = strrchr(buffer, '\r'))) *nlbr1 = 0;
    if((nlbr2 = strrchr(buffer, '\n'))) *nlbr2 = 0;
    capDebug::report(1, "%s", buffer);
    if(nlbr1) *nlbr1 = '\r';
    if(nlbr2) *nlbr2 = '\n';
    
    va_end(ap);
}
