//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPMODEL_H
#define CAPOC2_CAPMODEL_H

#include <map>

class capModel;

#include "capaffine.h"
#include "capnvm.h"
#include "caprenderer.h"
#include <vector>
#include <cctype>
#include <cmath>

#define CA_HASFLAG(i, flag) (this->vf[i] & (flag))
#define CA_HASFLAG_A(vf, i, flag) ((vf)[i] & (flag))
#define CA_FLAG(i, flag) this->vf[i] |= (flag)
#define CA_FLAG_A(vf, i, flag) vf[i] |= (flag)
#define CA_UNFLAG(i, flag) this->vf[i] &= ~(flag)
#define CA_UNFLAG_A(vf, i, flag) vf[i] &= ~(flag)

struct capVertex
{
    /* Vertex coordinates */
    float x, y, z;

    /* Normal vector */
    float nx, ny, nz;

    /* Vertex color */
    unsigned char r, g, b;

    /* Are there any faces using this vertex */
    unsigned char isUsed;
};


struct capEdge
{
    /* Vertex indices */
    unsigned int from, to;
};
struct capEdgePointer
{
    /* First edge from this vertex */
    unsigned int edgeStart;

    /* Number of edges going from this vertex */
    unsigned int edgeLen;
};

struct capTriangle
{
    /* Vertex indices */
    unsigned int a, b, c;

    /* Normal of this triangle */
    float nx, ny, nz;
};


static inline void vAdd(struct capVertex *v, struct capVertex *n)
{
    v->nx += n->nx;
    v->ny += n->ny;
    v->nz += n->nz;
}

static inline void vNormalize(struct capVertex *n)
{
    float l;
    if((l = sqrtf(n->nx * n->nx + n->ny * n->ny + n->nz * n->nz)) != 0.0f)
    {
        n->nx /= l;
        n->ny /= l;
        n->nz /= l;
    }
    else
    {
        n->nx = n->ny = n->nz = 0;
    }
}


inline static char *vPopstring(char *in, char **str)
{
    if(!in) return NULL;
    while(isblank(*in)) in++;
    if(!*in) return NULL;
    *str = in;
    while((!isblank(*in)) && *in) in++;
    if(*in)
    {
        *in = 0;
        in++;
        return in;
    }
    else
        return in;
}

inline static char *vPopfloat(char *in, float *out)
{
    char *str, *eptr;

    if(!(in = vPopstring(in, &str)))
        return NULL;

    *out = strtof(str, &eptr);
    if(*eptr) return NULL;

    return in;
}

inline static char *vPopint(char *in, int *out)
{
    char *str, *eptr;

    if(!(in = vPopstring(in, &str)))
        return NULL;

    *out = (int) strtol(str, &eptr, 0);
    if(*eptr) return NULL;

    return in;
}

inline static char *vPopbyte(char *in, unsigned char *out)
{
    int s;
    char * res = vPopint(in, &s);
    *out = s;
    return res;
}


struct capModel_ball
{
    float radius;
    float x0, y0, z0;
};

struct capModel_plane
{
    float cx, cy, cz;
    float nx, ny, nz;
    float **m;
    float radius;
    float dip, dipDir;
};

class capModel {
public:

    explicit capModel(capRenderer *renderer);

    /* Name of the file and full path this model was loaded from */
    char *filename = NULL;
    char *fullpath = NULL;

    /* Transform bringing this model to unified world coordinates */
    capAffineMatrix affineMatrix;

    /* Transform bringing this model to camera coordinates */
    capAffineMatrix modelviewMatrix;

    /* Number of vertices, triangles and edges */
    unsigned int nv = 0, nt = 0, ne = 0;

    /* Vertices and triangles of this model */
    struct capVertex *v = NULL;
    struct capTriangle *t = NULL;

    /* Vertex flags */
    unsigned char *vf = NULL;
#define CAV_MARKED 1
#define CAV_DELETED 2

    /* Edges are ordered by 'from'. */
    struct capEdge *e = NULL;

    /* Edge pointers help listing all neighbors of a particular vertex */
    struct capEdgePointer *ep = NULL;

    /* If NVM was loaded for this model, here it is! */
    capNvm *nvm = NULL;

    /* General flags of this model */

#define CAMF_LOADED 1
#define CAMF_POINTS 2
#define CAMF_FACES 4
#define CAMF_ACTIVE 8
#define CAMF_COLOR 16
#define CAMF_EDGES 32
#define CAMF_NORMALS 64

    unsigned int flags = 0;

    /* Named points of this model */
    std::map <std::string, capGenericPoint> savedPoints;

    capRenderer *renderer;
    capActivePoint cursor;
    void *rendererData = NULL;

#define CA_DEC_MAX 16
    unsigned int decRate = 0, decSeed = 0;  /* decimation rate and decimation seed */

    capModel_plane fitPlane;
    bool hasPlane = false;

    static void trim(char *line);
    int saveOff(const char *filename);
    int saveSelectionOff(const char *filename);
    int loadOff(const char *filename, int append, capColor *rgba);
    int loadPly(const char *filename, int append, capColor *rgba);
    int loadNvm(const char *filename, int append, capColor *rgba);
    int loadFile(const char *filename, int append, capColor *rgba);
    int load3d(const char *filename, int append, capColor *rgba);
    int readNvm(const char *filename, unsigned int modelId = 0);

    void unload();

    float getMarkedArea();

    float getMarkedArea(unsigned char *flagField);

    void walkEdge(int steps);

    void ensureEdges();

    unsigned char *getField();

    unsigned char *getBoundary(const unsigned char *mv);

    void selectBoundary();

    void expandSelection(float radius, unsigned char *mv);

    void subtractSelections(unsigned char *fa, const unsigned char *fb);

    unsigned char *popComponent(unsigned char *mv);

    void popComponent();

    void dumpComponents();

    void dumpComponents(const char *filename);

    void growSelection(float radius);

    unsigned int ballSelect(float radius, float x0, float y0, float z0);
    unsigned int ballSelect(capModel_ball b);
    capModel_ball ballViewToModel(capModel_ball vb);

    bool fitPlaneToSelection();
    void dropPlane();

    void resetSelection();

    void computeNormals();

    void reedge();

    void recolor(capColor *rgba);

    void deleteSelected();


#define CAC_GRAY_UNCOLORABLE 1
#define CAC_DELETE_UNCOLORABLE 2
#define CAC_UNDELETE_COLORABLE 4
    int recolorFrom(const char *filename, int flags);

    void findTransformTo(capModel *destModel, bool b);

    void undeleteAll();

    void updateDisplay();

    void scale(float sz);

    void rotate(float degAngle, int axis);

    void translate(float tx, float ty, float tz);

    void setPoint(char* name, capGenericPoint p);
    void setPoint(char* name, float x, float y, float z);
    void deletePoint(char* name);
    void seePoint(char* name, capCamera *view, float step = 5.0f);

    void selectBehind();

private:
    int loadStart(const char *filename, int append);

    void reallocBuffers(unsigned int i, unsigned int i1);

    void recolor(capColor *pColor, unsigned int i);

    int loadEnd();

};



#endif //CAPOC2_CAPMODEL_H
