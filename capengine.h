#ifndef CAPOC2_CAPENGINE_H
#define CAPOC2_CAPENGINE_H

class capEngine;

#ifdef WINDOWS
#include <windows.h>
#include <stringapiset.h>
#include <stdio.h>
static FILE * utf8open(const char *file, const char *mode)
{
    int wfile_num = MultiByteToWideChar(CP_UTF8, 0, file, -1, NULL, 0);
    int wmode_num = MultiByteToWideChar(CP_UTF8, 0, mode, -1, NULL, 0);

    wchar_t* wfile = new wchar_t[wfile_num];
    wchar_t* wmode = new wchar_t[wmode_num];
    FILE *ret;

    MultiByteToWideChar(CP_UTF8, 0, file, -1, wfile, wfile_num);
    MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, wmode_num);

    ret = _wfopen(wfile, wmode);

    delete[] wfile;
    delete[] wmode;
	return ret;
}
#else
#define utf8open(file, mode) fopen(file,mode)
#endif

#include "captypes.h"



#include "capmodel.h"
#include "capnvm.h"
#include "capserver.h"
#include "caprenderer.h"
#include "capcli.h"




class capEngine
{
public:
    explicit capEngine(capRenderer *r);

    std::vector <capModel*> models;
    capReprojParams repr;

    int logSeq = 0, stationId = 0;


    float shiftStep = 0.5, rotateStep = CAP_PI/180.0f*2.0f, zoomStep = 1.1f;

    capServer *server;
    capCli *cli;
    capCliThread *guiCliThread;
    capRenderer *renderer;
    bool needRefresh = true;

    void execStream(FILE *fh);

    bool execFile(const char *name);

    const char * execString(const char *cmd);
    const char * execStringf(const char *fmt, ...);

    void saveProjectScript(const char *filename);

    void render();

    void processNetwork();

    bool startNetwork(const char *ctrlAddr, int port);

    void mouseAction(int action, float step, float x1, float y1, float x2, float y2);

    void mouseAction(int action, float step);

    void mouseAction(int action, float step, int x1, int y1, int x2, int y2);

    void reshape(unsigned int w, unsigned int h);

    void keyAction(unsigned char key, int type);

    void setStatus(const char *fmt, ...);

    void virtualMeasurement(float x1, float y1, float x2, float y2);

#define CA_STATUS_MAX 256
    char status[CA_STATUS_MAX];
    bool statusUpdated = false;
    bool loadModel(const char *loadFrom, int modelId = -1, bool append = false);


protected:
    bool mousePixToFloat(int x, int y, float *xs, float *ys);

};




#endif //CAPOC2_CAPENGINE_H
