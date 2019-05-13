//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPRENDERER_ANCIENTGL_H
#define CAPOC2_CAPRENDERER_ANCIENTGL_H


#include "caprenderer.h"
#ifdef WINDOWS
#include <GL/glew.h>
#include <GL/freeglut.h>
#else
#include <GL/gl.h>
#endif

#define AGLF_LISTED 1

struct ancientGL_renderData {
    GLuint displayList;
    int flags;
};


class capRenderer_AncientGL : public capRenderer {
public:
    capRenderer_AncientGL();

    unsigned char xRgba[4] = {255, 255, 255, 64};
    float aspectRatio;

    void uninitModel(capModel *model) final;
    void reinitModel(capModel *model) final;
    void reselectModel(capModel *model) final;
    void replaneModel(capModel *model) final;
    void uninitNvmCam(capNvmCam *nvmCam) final;
    void reinitNvmCam(capNvmCam *nvmCam) final;
    void renderScene(std::vector <capModel*> &models) final;
    void reshape(unsigned int width, unsigned int height)  final;
    void switchView(unsigned int view) final;
    void updateView(unsigned int view) final;
    void setRuler(float x1, float y1, float x2, float y2) final;

private:
    void uninitRenderData(ancientGL_renderData **rd);
    int loadTexture(capNvmCam *m);
    GLuint camTexture;

    void setupProjection(int p);

    void drawView(std::vector <capModel*> &models, int proj);

    void setupCamera(int proj);

    void drawModel(capModel *m);

    void setupLight();

    void drawNvm(capNvm *n);

    void drawCamera();

    void drawNvmCam();

    void drawCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat ar, int filled);

    capGenericPoint screenPosToNvm(capGenericPoint &p) final;

    bool wantRuler = false;
    struct { float x1, y1, x2, y2; } ruler;

    void startDisplay() final;

    void applyAffine(capAffineMatrix *m);

    bool screenshot(const char *toFile) final;
};


#endif //CAPOC2_CAPRENDERER_ANCIENTGL_H
