//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPNVM_H
#define CAPOC2_CAPNVM_H

#include "capaffine.h"
#include <cstdio>


class capNvm;
class capNvmCam;

#include "capmodel.h"

struct capNvmPixsize
{
    /* Width and height of image */
    unsigned int width, height;

    /* Width and height of image buffer */
    unsigned int bufwidth, bufheight;

    /* Proportions of width/height */
    float fw, fh;
};

/* Describes a particular camera pose (image) used in photogrammetry */
struct capNvmCam
{
    /* NVM object responsible for this camera */
    capNvm *nvm;

    /* Camera index */
    unsigned int camIndex;

    /* Image file name */
    char *filename;

    /* Width, height as floats */
    float width, height;

    /* Pixel width/height */
    struct capNvmPixsize pixSize;

    /* Position, focal length, radial distortion coefficient */
    float x, y, z, f, r;

    /* Orientation of this camera - forward and inverse rotation */
    capAffineMatrix *rot;
    capAffineMatrix *invRot;
    float dx, dy, dz;

    /* Depth relative to current viewing position */
    float depth;

    /* Current marker position on camera */
    capActivePoint marker;

    /* Vertex ID of model that best corresponds to markerX/markerY */
    int bestV;

    /* Index of first measurement seen by this camera in the per-camera measurement index  */
    unsigned int firstProj;

    /* Number of measurements seen by this camera */
    unsigned int projCount;

    /* Number of 3d points seen by this camera - used as per-camera storage by various functions */
    unsigned int seenFeatures;

    /* Active measurement number */
    unsigned int activeProj;
};

/* Describes a sparse (SfM) 3d reconstruction point */
struct capNvmPoint
{
    /* 3d position of this point */
    float x, y, z;

    /* Index of first measurement of this point in the measurement array */
    unsigned int firstProj;

    /* Number of measurements of this point */
    unsigned int projCount;

    /* Color of this point */
    unsigned char r, g, b, a;
};


/* Describes a single measurement (observation) of a sparse (SfM) reconstruction 3d point */
struct capNvmProj
{
    /* Index of the 3d point in question */
    unsigned int pointIndex;

    /* Index of the camera this point was observed on */
    unsigned int camIndex;

    /* Pixel coordinates of this particular observation */
    float x, y;
};



#include "capaffine.h"
#include "caprenderer.h"



/* Describes a NVM file */
class capNvm
{
public:

    explicit capNvm(capRenderer *r, capModel *m);

    /* File and directory this NVM was sourced from */
    char *filename;
    char *dirname;

    /* Number of cameras, sparse 3d points and observations */
    unsigned int n_cams = 0, n_points = 0, n_projs = 0;

    /* Model this NVM refers to */
    capModel *model = NULL;

    /* Images, 3d points and observations of this NVM */
    struct capNvmCam *cams = NULL;
    struct capNvmPoint *points = NULL;
    struct capNvmProj *projs = NULL;

    /* Index of measurements by camera ID */
    unsigned int *camProjs = NULL;

    /* Camera iteration order (starts from 0, 1, 2, ..., n_cams-1, but might be changed after camera reordering) */
    unsigned int *camIndex = NULL;

    /* Currently selected camera */
    int activeCam;
    struct capNvmCam *activePtr = NULL;

    capRenderer *renderer;
    void *rendererData = NULL;

    ~capNvm();

    int error(const char *file, const char *error, FILE *fh);

    int errorStr(const char *file, const char *error, char *str, FILE *fh);

    int errorNum(const char *file, const char *error, int num, FILE *fh);

    static void trim(char *line);

    char *fgets(char *line, FILE *fh);

    int skipToModel(FILE *fh, int modelIndex);

    int load(const char *filename, int modelIndex);

    void assert(void *m);

    capNvmCam *findCam(const char *camName);

    void orderByDepth(float offs, bool absDepth = false);
    void orderByDepth();
    void orderBySeen();
    void orderByName();
    void orderByCamindex();
    void orderByDistance(float offs);
    void orderByDistance();
    void resetSeen();

    capNvmCam *stepCam(int offsetFromCurrent);

    void seeCam(capNvmCam *cam, capCamera *view);

    unsigned char *loadJpeg(capNvmCam *m, struct capNvmPixsize *ps, bool power2Align);

    int reprojectPoint(capNvmCam *cam, float x, float y, float z, int *xis, int *yis, float *fxis, float *fyis);

    float *obtainDepthMap(capNvmCam *cam, int pixcnt, int *toDo);

    void releaseDepthMap(capNvmCam *cam, int pixcnt, float *depthMap);

    void reproject(capModel *m, capReprojParams *cap, capNvmCam *cam, int toDo, const char *savemap);

    void setActive(capNvmCam *cam);


    int findCamsLookingIntoBall(float cx, float cy, float cz, float r, unsigned int *out);

    void stepMatch(capNvmCam *m, capGenericPoint &cursor, float range, int offset);

};




#define CAR_COLOR 1
#define CAR_DEL_UNPROJ 2
#define CAR_GRY_UNPROJ 4
#define CAR_UND_COLOR 8
#define CAR_DUMP 16
#define CAR_THROW 32
#define CAR_ONLY_SELECTED 64
#define CAR_NO_DEPTHMAP 128
#define CAR_DUMP_SUMMARY 512
#define CAR_MARK 1024



#endif //CAPOC2_CAPNVM_H
