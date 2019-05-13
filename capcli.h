//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPCLI_H
#define CAPOC2_CAPCLI_H

class capCli;
class capCliThread;

#include "cli.h"
#include "capserver.h"
#include "capengine.h"
#include "capaffine.h"

struct capKeymap
{
    struct capKeymap *next;
    char *name;
    char *description;
    char *actions[3][256];
#define CA_PRESS 0
#define CA_HOLD 1
#define CA_RELEASE 2
    char dragActions[5][2];
    float dragFactors[5][2];

#define CAB_LEFT 1
#define CAB_RIGHT 2
#define CAB_SHIFT 2

#define CAD_AXIS_X 0
#define CAD_AXIS_Y 1

#define CAD_NONE 0
#define CAD_CAM_PITCH 1
#define CAD_CAM_YAW 2
#define CAD_CAM_PANZ 3
#define CAD_CAM_PANX 4
#define CAD_CAM_PANY 5
#define CAD_MARKER_X 6
#define CAD_MARKER_Y 7
#define CAD_MARK_POINTS 8
#define CAD_MEASURE 9
#define CAD_NVM_PANX 10
#define CAD_NVM_PANY 11
#define CAD_MARKER_SET 12
#define CAD_RANGE_ADJUST 13
};


class capCli {

public:
    explicit capCli(const char *clidata, capEngine *engine);
    struct capKeymap *keymaps;
    struct capKeymap *currentKeymap;
    struct capKeymap *rootKeymap;

    capServer *server;
    capEngine *engine;
    const char *capCliData;

    void bindkey(const char *keyMap, const char *keyName, int actionType, const char *action);
    capKeymap *findmap(const char *keyMap);


    void setKeymap(const char *string);

};

class capCliBuffer {
public:
    unsigned int size = 0, pos = 0;
    char *mem = NULL;
};

class capCliThread : public cli {

private:
    void printAffine(capAffineMatrix *af);
public:
    explicit capCliThread(capCli *cliController, struct tcpConn *conn);
    explicit capCliThread(capCli *cliController);
    capCli *cliController;
    struct tcpConn *conn;
    capCliBuffer commandResult;

    void exec() final;
    void write(const char *buf, size_t) final;
    int complete(clitok_id, char*, int) final;

    void axisAction(int button, int axis, float step, float x1, float y1, float x2, float y2);
    void keyAction(unsigned char key, int type);

    bool fetchColor(capColor *c, bool resetAlpha);
    bool fetchColor(capColor *c);
    float floatToken(clitok_id token, float defval);

    void axisAction(int action, float step, float x1, float y1, float x2, float y2);
    void printf(const char *fmt, ...) final;
};


#endif //CAPOC2_CAPCLI_H
