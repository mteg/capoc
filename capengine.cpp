/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under CC BY-SA 4.0 license
 *
 */

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstdarg>
#include "capengine.h"
#include "clidata.c"
#include "capdebug.h"

capEngine::capEngine (capRenderer *r)
{
    this->renderer = r;
    this->server = NULL;
    capDebug::report(2, "Constructing cli controller...");
    this->cli = new capCli((const char*) cli_tab, this);

    capDebug::report(2, "Constructing GUI cli thread...");
    this->guiCliThread = new capCliThread(this->cli);

    r->repr = &this->repr;
}

void capEngine::setStatus(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vsnprintf(status, CA_STATUS_MAX-1, fmt, va);
    statusUpdated = true;
    va_end(va);
}

bool capEngine::execFile(const char *name)
{
    FILE *fh;
    if(!(fh = utf8open(name, "r")))
    {
        capDebug::report(0, "Cannot open %s for execution", name);
        return false;
    }
    this->execStream(fh);
    fclose(fh);
    return true;
}

void capEngine::execStream(FILE *fh)
{
    this->guiCliThread->execStream(fh);
}

const char * capEngine::execString(const char *cmd)
{
    this->guiCliThread->commandResult.pos = 0;
    this->guiCliThread->execString(cmd);
    this->guiCliThread->len = 0;
    this->guiCliThread->pos = 0;
    this->guiCliThread->buffer[0] = 0;
    return this->guiCliThread->commandResult.mem;
}

const char * capEngine::execStringf(const char *fmt, ...)
{
    char commandBuffer[1024];
    va_list va;
    va_start(va, fmt);


    vsnprintf(commandBuffer, 1024-1, fmt, va);
    this->guiCliThread->printf("Executing: %s", commandBuffer);
    this->guiCliThread->execString(commandBuffer);
    va_end(va);
    return this->guiCliThread->commandResult.mem;
}

bool capEngine::loadModel(const char *loadFrom, int modelId, bool append)
{
    capModel * model;
    if(modelId == -1)
        model = new capModel(this->renderer);
    else
        model = this->models[modelId];

    if(model->loadFile(loadFrom, append ? 1 : 0, NULL)) {
        this->models.push_back(model);
        return true;
    }
    else {
        delete model;
        return false;
    }
}

void capEngine::render()
{
    this->renderer->renderScene(this->models);
    this->needRefresh = false;
}

void capEngine::processNetwork()
{
    if(!this->server) return;
#ifndef NO_SOCKETS
    while(true)
    {
        fd_set rd;
        fd_set wr;
        struct timeval tv;
        int maxfd = 0, r;

        tv.tv_sec = 0;
        tv.tv_usec = 500;

        FD_ZERO(&rd);
        FD_ZERO(&wr);

        this->server->updateFdSets(&rd, &wr, NULL, &maxfd);
        if((r = select(maxfd + 1, &rd, &wr, NULL, &tv)) < 0)
            perror("select");
        else if(r == 0)
            return;
        this->server->doNetwork();
    }
#endif
}

bool capEngine::startNetwork(const char *ctrlAddr, int port)
{
    this->server = new capServer(10, 65536, this->cli);
    return this->server->bind(port, ctrlAddr) != 0;
}

bool capEngine::mousePixToFloat(int x, int y, float *xs, float *ys)
{
    int split = 0;
    int xc, yc;
    float x1, y1;

    if(renderer->currentView == CAV_XSECT || renderer->currentView == CAV_DIPHIST || renderer->currentView == CAV_NVMCAM) split = 1;
    if(renderer->currentView == CAV_FOURVIEW) split = 2;

    if(!split) return false;

    xc = renderer->displayWidth / 2 / split;
    yc = renderer->displayHeight / 2 / split;


    x1 = x - xc;
    y1 = yc - y;

    if(split == 2)
    {
        if(abs(lrint(x1)) > (renderer->displayWidth / 4)) return false;
        if(abs(lrint(y1)) > (renderer->displayHeight / 4)) return false;
    }

    x1 *= 2.0  / ((float) renderer->displayWidth / split);
    y1 *= 2.0  / ((float) renderer->displayWidth / split);

    *xs = x1;
    *ys = y1;

//    printf("pix to float: %d, %d --> %.2f, %.2f at %d x %d\n", x, y, x1, y1, renderer->displayWidth, renderer->displayHeight);

    return true;
}


void capEngine::mouseAction(int action, float step, float x1, float y1, float x2, float y2) {
    this->guiCliThread->axisAction(action, step, x1, y1, x2, y2);
}

void capEngine::mouseAction(int action, float step, int x1, int y1, int x2, int y2) {
    float fx1, fy1, fx2, fy2;
    if(!mousePixToFloat(x1, y1, &fx1, &fy1)) return;
    if(!mousePixToFloat(x2, y2, &fx2, &fy2)) return;
    this->guiCliThread->axisAction(action, step, fx1, fy1, fx2, fy2);
}

void capEngine::mouseAction(int action, float step) {
    this->guiCliThread->axisAction(action, step, 0.0, 0.0, 0.0, 0.0);
}

void capEngine::keyAction(unsigned char key, int type)
{
    this->guiCliThread->keyAction(key, type);
}

void capEngine::reshape(unsigned int w, unsigned int h)
{
    renderer->reshape((unsigned int) w,(unsigned int) h);
    needRefresh = true;
}

void capEngine::virtualMeasurement(float x1, float y1, float x2, float y2)
{
    float nx, ny, nz;
    float v[3] = {x2 - x1, y2 - y1, 0};
    float dip, dipDir, veclen;

    auto *a = new capAffineMatrix(renderer->views[CAV_XSECT].projMatrix);
    a->invert();

    nx = a->m[0] * v[0] + a->m[4] * v[1] + a->m[8] *  v[2]; /* + a[12] */
    ny = a->m[1] * v[0] + a->m[5] * v[1] + a->m[9] *  v[2]; /* + a[13] */
    nz = a->m[2] * v[0] + a->m[6] * v[1] + a->m[10] * v[2]; /* + a[14] */

    veclen = sqrtf(nx*nx + ny*ny + nz*nz);
    nx /= veclen; ny /= veclen; nz /= veclen;
    if(ny < 0)
    {
        nx *= -1;
        ny *= -1;
        nz *= -1;
    }
    dip = asinf(ny) / CAP_PI * 180;
    dipDir = -atan2f(nx, nz) / CAP_PI * 180 - 90;
    if(dipDir < 0.0) dipDir += 360.0;
    if(dipDir > 360.0) dipDir -= 360.0;

    setStatus("Line: %.2f m, dip: %.1f dip direction: %.1f", sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]), dip, dipDir, nx, ny, nz);
}

void capEngine::saveProjectScript(const char *filename)
{
    FILE *fh = utf8open(filename, "w");
    unsigned int i;
    if(!fh)
        return (void) capDebug::error("Unable to write to file '%s'", filename);

    for(i = 0; i<models.size(); i++)
    {
        /* Load model */
        fprintf(fh, "model %d load from %s\r\n", i, models[i]->fullpath);

        /* Set affine transform */
        float *m = models[i]->affineMatrix.m;
        fprintf(fh, "model %d transform matrix x %.6f %.6f %.6f %.6f y %.6f %.6f %.6f %.6f z %.6f %.6f %.6f %.6f\r\n", i,
                m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14]
        );

        /* Push saved points */
        for(auto const & it: models[i]->savedPoints)
            fprintf(fh, "model %d point %s set-to %.6f %.6f %.6f\r\n", i,it.first.c_str(),
                    it.second.x, it.second.y, it.second.z);

        if(models[i]->flags & CAMF_POINTS) fprintf(fh, "model %d display points\r\n", i);
        if(models[i]->flags & CAMF_EDGES) fprintf(fh, "model %d display edges\r\n", i);
        if(models[i]->flags & CAMF_FACES) fprintf(fh, "model %d display faces\r\n", i);
        fprintf(fh, "\r\n");
    }

    if(!models.empty())
        if(models[0]->nvm)
            fprintf(fh, "model 0 nvm load 0 from %s\r\n", models[0]->nvm->filename);

    capCamera &cam = renderer->cam;
    fprintf(fh, "camera set %.6f %.6f %.6f %.1f %.1f\n", cam.x, cam.y, cam.z, cam.yaw/CAP_PI*180.0, cam.pitch/CAP_PI*180.0);

    for(i = 0; i<5; i++) {
        const char *view_names[] = {"plan", "profile", "xsect", "caver", "nvmcam"};
        fprintf(fh, "view zoom %s set-to %.6f\r\n", view_names[i], renderer->views[i].orthoZoom);
        fprintf(fh, "view clip %s set-to %.6f\r\n", view_names[i], renderer->views[i].orthoClip);
        if(renderer->currentView == i) fprintf(fh, "view switch %s\r\n", view_names[i]);
    }

    if(renderer->currentView == CAV_FOURVIEW) fprintf(fh, "view switch fourview\r\n");

    fclose(fh);

}
