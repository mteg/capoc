//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPRENDERER_H
#define CAPOC2_CAPRENDERER_H

class capRenderer;

struct capViewinfo
{
    float orthoZoom;
    float orthoClip;
    float fogStart;

    float panX;
    float panY;

    float rasterSize;
    float zOffset;
    float fov;

    capAffineMatrix projMatrix;
};


struct capLightParams
{
    unsigned char lightFlags;
#define CLF_ENABLED 1
#define CLF_DRAGGED 2
#define CLF_DROPPED 4
    float lightHeight, lightOffset, lightQuadAtt, lightLinAtt, lightConstAtt;
    float x, y, z;
};



struct capCamera
{
public:
    float x, y, z;
    float yaw, pitch, roll;

};

struct capColor
{
    unsigned char r, g, b, a;
};


#include "capengine.h"
#include "capnvm.h"
#include <vector>


class capRenderer {
public:
    capRenderer();
    virtual void renderScene(std::vector <capModel*> &models) = 0;
    virtual void reshape(unsigned int width, unsigned int height)  = 0;

    virtual void reinitModel(capModel *model) = 0;
    virtual void reselectModel(capModel *model) = 0;
    virtual void replaneModel(capModel *model) = 0;
    virtual void reinitNvmCam(capNvmCam *nvmCam) = 0;

    virtual void uninitModel(capModel *model)  = 0;
    virtual void uninitNvmCam(capNvmCam *nvmCam) = 0;

    virtual void switchView(unsigned int view) = 0;
    virtual void updateView(unsigned int view) = 0;

    virtual void setRuler(float x1, float y1, float x2, float y2) = 0;

    virtual void startDisplay() = 0;

    capReprojParams *repr = NULL;


#define CAV_PLAN 0
#define CAV_PROFILE 1
#define CAV_XSECT 2
#define CAV_CAVER 3
#define CAV_NVMCAM 4
#define CAV_DIPHIST 5
#define CAV_SINGLE_MAX 5
#define CAV_ISSINGLE(v) ((v) <= CAV_SINGLE_MAX)

#define CAV_FOURVIEW 6
#define CAV_COMPARE 7
#define CAV_GEOLOGIST 8

    struct capViewinfo views[CAV_SINGLE_MAX + 1] = {0};
    struct capCamera cam = {0};
	capColor sectColor; 

    unsigned int currentView = CAV_CAVER;
    unsigned int displayWidth, displayHeight;

#define CACF_DISPLAY_PLANAR 8
#define CACF_DISPLAY_SPATIAL 16
#define CACF_DISPLAY_MATCHES 32
#define CACF_DISPLAY_BBOX 64
#define CACF_DISPLAY_IMAGE 128
    unsigned int nvmDisplay = CACF_DISPLAY_IMAGE | CACF_DISPLAY_BBOX;

    struct capLightParams light = {0};
    struct capNvmCam *texturedCam = NULL;


    void camMove(float step);

    void camStrafe(float step);

    void camYaw(float angle);

    void camPitch(float angle);

    void camLift(float step);

    void camLog();

    virtual void enableLight();

    virtual void disableLight();

    virtual capGenericPoint screenPosToNvm(capGenericPoint &p) = 0;
    capGenericPoint screenPosToNvm(float x, float y);

    void setupPerspective(float fovY, float ar, float zNear, float zFar);

    virtual bool screenshot(const char *saveas);
};


#endif //CAPOC2_CAPRENDERER_H
