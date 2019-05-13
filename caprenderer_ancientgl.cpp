/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <string.h>
#include "capaffine.h"
#include "caprenderer_ancientgl.h"
#include "capdebug.h"
#include "lodepng.h"

capRenderer_AncientGL::capRenderer_AncientGL()
{


    this->reshape(640, 480);
}

void capRenderer_AncientGL::startDisplay()
{
#ifdef WINDOWS
    GLenum err = glewInit();
    capDebug::report(2, "GLEW initialized, error: %d", err);
    if (err != GLEW_OK) {
            const GLubyte* msg = glewGetErrorString(err);
            throw std::exception(reinterpret_cast<const char*>(msg));
    }


#endif
    glGenTextures(1, &this->camTexture);
    capDebug::report(2, "Camera texture ID: %d", this->camTexture);
}

void capRenderer_AncientGL::drawCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat ar, int filled)
{
    int i;
    int lineAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = 2.0f * CAP_PI;

    if(filled)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, 0);
    }
    else
        glBegin(GL_LINE_LOOP);
    for(i = 0; i <= lineAmount;i++)
    {
        glVertex3f(
                (x + (ar * radius * cosf(((float) i) *  twicePi / ((float) lineAmount)))),
                (y + (radius* sinf(((float) i) * twicePi / ((float) lineAmount)))),
                0.0
        );
    }
    glEnd();
}


void capRenderer_AncientGL::uninitRenderData(struct ancientGL_renderData **rd)
{
    if(!*rd) return;
    auto md = *rd;
    if(md->flags & AGLF_LISTED)
        glDeleteLists(md->displayList, 1);
    delete *rd;
    *rd = NULL;
}

void capRenderer_AncientGL::uninitModel(capModel *m)
{
    if(!m->rendererData) return;
    this->uninitRenderData((struct ancientGL_renderData**) &m->rendererData);
}

void capRenderer_AncientGL::reselectModel(capModel *model)
{
    this->reinitModel(model);
}

void capRenderer_AncientGL::replaneModel(capModel *model)
{
    this->reinitModel(model);
}

void capRenderer_AncientGL::reinitModel(capModel *m)
{
    unsigned int decThreshold, decMask,  doDec = 1;
    struct ancientGL_renderData *md;

    if(m->rendererData)
        md = (struct ancientGL_renderData*) m->rendererData;
    else {
        m->rendererData = md = new ancientGL_renderData();
        md->flags = 0;
        md->displayList = glGenLists(1);
    }

    if(!(m->flags & CAMF_LOADED)) return;

    glNewList(md->displayList, GL_COMPILE);
    md->flags |= AGLF_LISTED;

    glColor3f (1.0, 0.0, 0.0);  /* Default color: red */

    /* Precompute decimation stuff */
    srand(m->decSeed);
    decMask = (1 << CA_DEC_MAX) - 1; /* 65535 */
    decThreshold = 1 << (CA_DEC_MAX - m->decRate);
    if(!m->decRate) doDec = 0;
    capDebug::report(3, "Decimation: decThreshold = %d, decMask = %d, decRate = %d", decThreshold, decMask, m->decRate);


    /* Start drawing, depending on modes */
    if(m->flags & CAMF_POINTS)
    {
        unsigned int i;
        glBegin(GL_POINTS);
        glPointSize(0.2);
        for(i = 0; i<m->nv; i++)
        {
            if(doDec)
            {
                unsigned int decVal = rand() & decMask;
                if(decVal > decThreshold) continue;
            }

            if(CA_HASFLAG_A(m->vf, i, CAV_DELETED)) continue;
            if(CA_HASFLAG_A(m->vf, i, CAV_MARKED))
                glColor3ub(255, 255, 0);
            else
                glColor3ub(m->v[i].r, m->v[i].g, m->v[i].b);
            glVertex3f(m->v[i].x, m->v[i].y, m->v[i].z);
        }
    }
    else if(m->flags & (CAMF_FACES | CAMF_EDGES)) {
        unsigned int i;
        if (m->flags & CAMF_EDGES)
            glLineWidth(2.0);


        glBegin(GL_TRIANGLES);
        for (i = 0; i < m->nt; i++) {
            unsigned int a = m->t[i].a, b = m->t[i].b, c = m->t[i].c;

            if (doDec) {
                unsigned int decVal = rand() & decMask;
                if (decVal > decThreshold) continue;
            }

            if (CA_HASFLAG_A(m->vf, a, CAV_DELETED)) continue;
            if (CA_HASFLAG_A(m->vf, b, CAV_DELETED)) continue;
            if (CA_HASFLAG_A(m->vf, c, CAV_DELETED)) continue;

            glNormal3f(m->v[a].nx, m->v[a].ny, m->v[a].nz);
            if (CA_HASFLAG_A(m->vf, a, CAV_MARKED))
                glColor3ub(255, 255, 0);
            else
                glColor3ub(m->v[a].r, m->v[a].g, m->v[a].b);
            glVertex3f(m->v[a].x, m->v[a].y, m->v[a].z);

            glNormal3f(m->v[b].nx, m->v[b].ny, m->v[b].nz);
            if (CA_HASFLAG_A(m->vf, b, CAV_MARKED))
                glColor3ub(255, 255, 0);
            else
                glColor3ub(m->v[b].r, m->v[b].g, m->v[b].b);
            glVertex3f(m->v[b].x, m->v[b].y, m->v[b].z);

            glNormal3f(m->v[c].nx, m->v[c].ny, m->v[c].nz);
            if (CA_HASFLAG_A(m->vf, c, CAV_MARKED))
                glColor3ub(255, 255, 0);
            else
                glColor3ub(m->v[c].r, m->v[c].g, m->v[c].b);
            glVertex3f(m->v[c].x, m->v[c].y, m->v[c].z);
        }


        glEnd();
        glLineWidth(1.0);
    }


    if(m->hasPlane)
    {
        int n = 0;
        glPointSize(2.0);
        glBegin(GL_POINTS);
        glColor3ub(0, 0, 255);
        for(n = 0; n<1000; n++)
        {
            float u = ((float)rand()/(float)(RAND_MAX)) * 2 - 1.0;
            float v = ((float)rand()/(float)(RAND_MAX)) * 2 - 1.0;
            float x = m->fitPlane.cx, y = m->fitPlane.cy, z = m->fitPlane.cz;

            u *= m->fitPlane.radius * 3;
            v *= m->fitPlane.radius * 3;


            x += u * m->fitPlane.m[0][2] + v * m->fitPlane.m[0][1];
            y += u * m->fitPlane.m[1][2] + v * m->fitPlane.m[1][1];
            z += u * m->fitPlane.m[2][2] + v * m->fitPlane.m[2][1];

            glVertex3f(x, y, z);
        }

        glEnd();
        glPointSize(1.0);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex3f(m->fitPlane.cx, m->fitPlane.cy, m->fitPlane.cz);
        glVertex3f(m->fitPlane.cx + m->fitPlane.nx * m->fitPlane.radius,
                   m->fitPlane.cy + m->fitPlane.ny * m->fitPlane.radius,
                   m->fitPlane.cz + m->fitPlane.nz * m->fitPlane.radius);

        glEnd();
        glLineWidth(1.0);
    }

    // todo implement fitPlane

    glEnd();
    glEndList();
}

inline static void rotVertex(capNvmCam *cam, float ox, float oy, float oz, float x, float y, float z)
{
    float v[3] = {x, y, z};
    cam->rot->transform(v);
    glVertex3f(ox + v[0], oy + v[1], oz + v[2]);
}

static inline int in_range(float x1, float y1, float x2, float y2, float range)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return (dx*dx + dy*dy) < (range*range);
}


int capRenderer_AncientGL::loadTexture(capNvmCam *m) {
    int width, height;

    unsigned char *rgbData = m->nvm->loadJpeg(m, &m->pixSize, true);

    if(!rgbData) return 0;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, this->camTexture);
#ifndef WINDOWS
    glActiveTexture(this->camTexture);
#endif
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLint tsize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tsize);
    if(tsize) {
        if (m->pixSize.bufwidth > (unsigned int) tsize) {
            /* Texture too wide - decimate width */
            int decRate = m->pixSize.bufwidth / (unsigned int) tsize;
            unsigned int x, y, c;
            unsigned char *p = rgbData;
            for (y = 0; y < m->pixSize.height; y++) {
                for (x = 0; x < (unsigned int) tsize; x++) {
                    for(c = 0; c<4; c++)
                        *(p++) = rgbData[(x * decRate + y * m->pixSize.bufwidth)*4+c];
                }
            }
            m->pixSize.bufwidth = (unsigned int) tsize;
        }

        if (m->pixSize.bufheight > (unsigned int) tsize)
        {
            /* Texture too high - decimate height */
            int decRate = m->pixSize.bufheight / tsize, y;
            for(y = 0; y<tsize; y++)
                memmove(rgbData + m->pixSize.bufwidth*y*4, rgbData + m->pixSize.bufwidth*y*4*decRate, m->pixSize.bufwidth*4);
            m->pixSize.bufheight = (unsigned int) tsize;
        }
    }


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width = m->pixSize.bufwidth, height = m->pixSize.bufheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbData);
    capDebug::report(3, "glTexImage2D: %d, tsize = %d", glGetError(), tsize);


    capDebug::report(2, "Loaded texture (%dx%d)",  width, height);
    free(rgbData);

    this->texturedCam = m;
    return 1;

}

void capRenderer_AncientGL::reinitNvmCam(capNvmCam *m) {
    capRenderer * c = m->nvm->renderer;
    capNvm * n = m->nvm;
    float rsW = c->views[CAV_CAVER].rasterSize, rsH;
    struct ancientGL_renderData *md;

    if(n->rendererData)
        md = (struct ancientGL_renderData*) n->rendererData;
    else {
        n->rendererData = md = new ancientGL_renderData();
        md->flags = 0;
        md->displayList = glGenLists(1);
    }

    if(this->texturedCam != m)
/*        if(c->nvmDisplay & CACF_DISPLAY_IMAGE)*/
            loadTexture(m);

    glNewList(md->displayList, GL_COMPILE);
    md->flags |= AGLF_LISTED;

    if(!c->nvmDisplay) {
        glEndList();
        return;
    }

    rsH  = rsW * m->height / m->width;

    if(c->nvmDisplay & CACF_DISPLAY_SPATIAL)
    {
        unsigned int i, pi = m->firstProj;
        glBegin(GL_POINTS);
        glColor3f(0, 0, 1.0);
        for(i = 0; i<m->projCount; i++, pi++)
        {
            int p = n->camProjs[pi];
            int v = n->projs[p].pointIndex;
            float ix = n->projs[p].x / m->width * 2.0f - 1.0f, iy = n->projs[p].y / m->height * 2.0f - 1.0f;

            if(in_range(ix, iy, m->marker.x, m->marker.y, this->repr->rasterRange))
                glVertex3f(n->points[v].x, n->points[v].y, n->points[v].z);
        }
        glEnd();
    }


    if(c->nvmDisplay & CACF_DISPLAY_PLANAR)
    {
        unsigned int i, pi = m->firstProj;
        glBegin(GL_LINES);
        glColor3f(0, 0, 1.0);
        for(i = 0; i<m->projCount; i++, pi++)
        {
            int p = n->camProjs[pi];
            float ix = n->projs[p].x / m->width * 2.0f - 1.0f, iy = n->projs[p].y / m->height * 2.0f - 1.0f;
            if(in_range(ix, iy, m->marker.x, m->marker.y, this->repr->rasterRange))
            {
                if(i == m->activeProj)
                    glColor3f(1.0, 0, 1.0);
                rotVertex(m, m->x, m->y, m->z, (ix-0.02f) * rsW, (iy-0.02f) * rsH, -rsW*0.01f);
                rotVertex(m, m->x, m->y, m->z, (ix+0.02f) * rsW, (iy+0.02f) * rsH, -rsW*0.01f);
                rotVertex(m, m->x, m->y, m->z, (ix-0.02f) * rsW, (iy+0.02f) * rsH, -rsW*0.01f);
                rotVertex(m, m->x, m->y, m->z, (ix+0.02f) * rsW, (iy-0.02f) * rsH, -rsW*0.01f);
                if(i == m->activeProj)
                    glColor3f(0.0, 0, 1.0);
            }
        }
        glEnd();
    }

    /*
    if(m->bestV != -1 && !(c->miscFlags & CMF_NVM_IMAGEONLY))
    {
        float ix = m->bestX, iy = m->bestY;
        int v = m->bestV;

        glBegin(GL_LINES);
        glColor3f(0, 1.0, 0);
        rotVertex(m, m->x, m->y, m->z, (ix-0.02) * rsW, (iy-0.02) * rsH, -rsW*0.01);
        rotVertex(m, m->x, m->y, m->z, (ix+0.02) * rsW, (iy+0.02) * rsH, -rsW*0.01);
        rotVertex(m, m->x, m->y, m->z, (ix-0.02) * rsW, (iy+0.02) * rsH, -rsW*0.01);
        rotVertex(m, m->x, m->y, m->z, (ix+0.02) * rsW, (iy-0.02) * rsH, -rsW*0.01);

        glVertex3f(n->model->v[v].x, n->model->v[v].y, n->model->v[v].z);
        rotVertex(m, m->x, m->y, m->z, ix * rsW, iy * rsH, 0);

        glEnd();

    }*/

    if(c->nvmDisplay & CACF_DISPLAY_MATCHES)
    {
        unsigned int i, pi = m->firstProj;
        glBegin(GL_LINES);
        glColor3f(0, 0, 1.0);
        for(i = 0; i<m->projCount; i++, pi++)
        {
            int p = n->camProjs[pi];
            int v = n->projs[p].pointIndex;
            float ix = n->projs[p].x / m->width * 2.0f - 1.0f, iy = n->projs[p].y / m->height * 2.0f - 1.0f;

            if(in_range(ix, iy, m->marker.x, m->marker.y, this->repr->rasterRange))
            {
                if(i == m->activeProj)
                    glColor3f(0, 1.0, 0);
                glVertex3f(n->points[v].x, n->points[v].y, n->points[v].z);
                rotVertex(m, m->x, m->y, m->z, ix * rsW, iy * rsH, 0);
                if(i == m->activeProj)
                    glColor3f(0, 0, 1.0);
            }
        }
        glEnd();
    }

    if(c->nvmDisplay & CACF_DISPLAY_IMAGE)
    {
        glEnable(GL_TEXTURE_2D);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glTexCoord2f (0.0, 0.0f);
        rotVertex(m, m->x, m->y, m->z, -rsW, -rsH, 0);
        glTexCoord2f(m->pixSize.fw, 0.0f);
        rotVertex(m, m->x, m->y, m->z, +rsW, -rsH, 0);
        glTexCoord2f(m->pixSize.fw, m->pixSize.fh);
        rotVertex(m, m->x, m->y, m->z, +rsW, +rsH, 0);
        glTexCoord2f (0.0, m->pixSize.fh);
        rotVertex(m, m->x, m->y, m->z, -rsW, +rsH, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    if(c->nvmDisplay & CACF_DISPLAY_IMAGE)
    {
        float qx, qy, qz, qlen;
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        rotVertex(m, m->x, m->y, m->z, -rsW, -rsH, 0);
        rotVertex(m, m->x, m->y, m->z, +rsW, -rsH, 0);
        rotVertex(m, m->x, m->y, m->z, +rsW, +rsH, 0);
        rotVertex(m, m->x, m->y, m->z, -rsW, +rsH, 0);
        rotVertex(m, m->x, m->y, m->z, -rsW, -rsH, 0);
        glEnd();
        glBegin(GL_LINES);
        rotVertex(m, m->x, m->y, m->z, 0, 0, -rsW/2.0f);
        rotVertex(m, m->x, m->y, m->z, -rsW, -rsH, 0);
        rotVertex(m, m->x, m->y, m->z, 0, 0, -rsW/2.0f);
        rotVertex(m, m->x, m->y, m->z, +rsW, -rsH, 0);
        rotVertex(m, m->x, m->y, m->z, 0, 0, -rsW/2.0f);
        rotVertex(m, m->x, m->y, m->z, +rsW, +rsH, 0);
        rotVertex(m, m->x, m->y, m->z, 0, 0, -rsW/2.0f);
        rotVertex(m, m->x, m->y, m->z, -rsW, +rsH, 0);

        glEnd();
        glBegin(GL_LINES);

        qx = m->dx; qy = m->dy; qz = m->dz;
        qlen = sqrtf(qx*qx + qy*qy + qz*qz);
        qx /= qlen; qy /= qlen; qz /= qlen;

        glVertex3f(m->x, m->y, m->z);
        glVertex3f(m->x+qx, m->y+qy, m->z+qz);
        glEnd();
    }

    glEndList();

}

void capRenderer_AncientGL::reshape(unsigned int w, unsigned int h)
{
    this->displayWidth = w;
    this->displayHeight = h;
    this->aspectRatio = ((float) h) / ((float) w);
}

void capRenderer_AncientGL::drawNvmCam()
{
    float zoom = this->views[CAV_NVMCAM].orthoZoom;
    float ar =  1.0;
    struct capNvmCam * cam = this->texturedCam;

    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glOrtho(-1.0/this->aspectRatio*zoom, 1.0/this->aspectRatio*zoom, -1.0*zoom, 1.0*zoom, -1000.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);

    capAffineMatrix af;
    af.setTranslation(this->views[CAV_NVMCAM].panX, this->views[CAV_NVMCAM].panY, 0.0);
    glMultMatrixf(af.m);

    if(cam)
        if(cam->height > 0.0)
            ar = ((float) cam->width) / ((float) cam->height);


    if(cam)
    {
        float fracX = cam->marker.x, fracY = cam->marker.y;

        glEnable(GL_TEXTURE_2D);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glTexCoord2f (0.0, 0.0f);
        glVertex3f (-ar, 1.0f, 0.0);
        glTexCoord2f(cam->pixSize.fw, 0.0f);
        glVertex3f (ar, 1.0f, 0.0);
        glTexCoord2f(cam->pixSize.fw, cam->pixSize.fh);
        glVertex3f (ar, -1.0f, 0.0);
        glTexCoord2f (0.0, cam->pixSize.fh);
        glVertex3f (-ar, -1.0f, 0.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glDisable(GL_DEPTH_TEST);
        glBegin(GL_LINES);
        glColor3f(1.0, 0, 0);
        glVertex3f(fracX*ar-0.05f, -fracY, 0);
        glVertex3f(fracX*ar+0.05f, -fracY, 0);
        glVertex3f(fracX*ar, -fracY-0.05f, 0);
        glVertex3f(fracX*ar, -fracY+0.05f, 0);
        glEnd();

        glColor3f(1.0, 0, 0);
        drawCircle(cam->marker.x*ar, -cam->marker.y, this->repr->rasterRange, 1.0, 0);

        if(nvmDisplay & CACF_DISPLAY_PLANAR)
        {
            unsigned int i, pi = cam->firstProj;
            glBegin(GL_LINES);
            glColor3f(0, 0, 1.0);
            for(i = 0; i<cam->projCount; i++, pi++)
            {
                int p = cam->nvm->camProjs[pi];
                float ix = cam->nvm->projs[p].x / cam->width * 2.0f - 1.0f, iy = cam->nvm->projs[p].y / cam->height * 2.0f - 1.0f;
//                if(in_range(ix, iy, cam->marker.x, cam->marker.y, this->repr->rasterRange))
                {
                    if(i == cam->activeProj)
                        glColor3f(1.0, 0, 1.0);

                    glVertex3f(ix*ar-0.05f, -iy, 0);
                    glVertex3f(ix*ar+0.05f, -iy, 0);
                    glVertex3f(ix*ar, -iy-0.05f, 0);
                    glVertex3f(ix*ar, -iy+0.05f, 0);

                    if(i == cam->activeProj)
                        glColor3f(0.0, 0, 1.0);
                }
            }
            glEnd();
        }



        if(cam->nvm->model->cursor.valid)
        {
            auto &c = cam->nvm->model->cursor;

            if(cam->nvm->reprojectPoint(cam, c.x, c.y, c.z, NULL, NULL, &fracX, &fracY))
            {
                glBegin(GL_LINES);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(fracX*ar-0.05f, -fracY, 0);
                glVertex3f(fracX*ar+0.05f, -fracY, 0);
                glVertex3f(fracX*ar, -fracY-0.05f, 0);
                glVertex3f(fracX*ar, -fracY+0.05f, 0);
                glEnd();
            }
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glRasterPos2f(-0.99f, 0.8);
        glColor3f(1.0, 0.0, 0.0);

        glEnable(GL_DEPTH_TEST);
    }
}

void capRenderer_AncientGL::uninitNvmCam(capNvmCam *n) {
    this->uninitRenderData((struct ancientGL_renderData**) &n->nvm->rendererData);

}

void capRenderer::setupPerspective(float fovY, float ar, float zNear, float zFar)
{
    GLdouble fW, fH;
    fH = tan( fovY / 360.0 * CAP_PI ) * zNear;
    fW = fH * ar;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


void capRenderer_AncientGL::setupProjection(int p)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    if(p == CAV_CAVER)
        setupPerspective(this->views[CAV_CAVER].fov, (GLfloat) this->displayWidth / (GLfloat) this->displayHeight, 0.1 , 10000.0);
    else if((p != CAV_NVMCAM) && (p != CAV_DIPHIST))
    {
        GLdouble heightZoom = this->views[p].orthoZoom * ((GLfloat) this->displayHeight) / ((GLfloat) this->displayWidth);
        glOrtho(-this->views[p].orthoZoom, this->views[p].orthoZoom, -heightZoom, heightZoom, -this->views[p].orthoClip + this->views[p].zOffset, this->views[p].orthoClip + this->views[p].zOffset);
    }

    glMatrixMode(GL_MODELVIEW);

}

void capRenderer_AncientGL::applyAffine(capAffineMatrix *af)
{
    glMultMatrixf(af->m);
}

void capRenderer_AncientGL::setupCamera(int proj)
{
    struct capCamera * cam = &this->cam;
    float yaw = cam->yaw, pitch = cam->pitch;

    switch(proj)
    {
        case CAV_CAVER: break; /* Defaults */
        case CAV_PLAN: pitch = -CAP_PI/2.0f; break;
        case CAV_PROFILE: pitch = 0; yaw -= CAP_PI/2.0f; break;
        case CAV_XSECT: pitch = 0; break;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(proj == CAV_NVMCAM) return;
    if(proj == CAV_DIPHIST) return;

    capAffineMatrix res;
    capAffineMatrix m;

    m.setRotation(-yaw / CAP_PI * 180.0f - 90.0f, 1); res.compose(&m);
    m.setRotation(pitch / CAP_PI * 180.0f, 0); res.compose(&m);

    this->applyAffine(&res);
    glTranslatef(-cam->x, -cam->y, -cam->z);

}

void capRenderer_AncientGL::setupLight()
{
    // todo support light
}

void capRenderer_AncientGL::drawNvm(capNvm *nvm)
{
    struct capNvmCam *cam = nvm->activePtr;
    if(!cam) return;

    auto *rd = (ancientGL_renderData *)  nvm->rendererData;
    if(!rd) return;

    glPointSize(2.0);


    glCallList(rd->displayList);
    float rsW = this->views[CAV_CAVER].rasterSize, rsH = rsW * cam->height / cam->width;

    if(((int) cam->camIndex == nvm->activeCam) && (this->nvmDisplay & CACF_DISPLAY_IMAGE))
    {
        float fracX = cam->marker.x, fracY = cam->marker.y;

//            printf("display pointer at %.3f %.3f (cX = %d, cY = %d)\n", fracX, fracY, c->currentX, c->currentY);

        glColor3f(1.0, 0, 0);
        glDisable(GL_DEPTH_TEST);
        glBegin(GL_LINES);
        rotVertex(cam, cam->x, cam->y, cam->z, (fracX-0.05f)*rsW, fracY*rsH, 0);
        rotVertex(cam, cam->x, cam->y, cam->z, (fracX+0.05f)*rsW, fracY*rsH, 0);
        rotVertex(cam, cam->x, cam->y, cam->z, fracX*rsW, (fracY-0.05f)*rsH, 0);
        rotVertex(cam, cam->x, cam->y, cam->z, fracX*rsW, (fracY+0.05f)*rsH, 0);
        glEnd();

        glEnable(GL_DEPTH_TEST);


    }

    /* Draw active match */
    if(nvmDisplay & CACF_DISPLAY_MATCHES)
    {
        int p = nvm->camProjs[(cam->activeProj % cam->projCount) + cam->firstProj];
        int v = nvm->projs[p].pointIndex;
        float ix = nvm->projs[p].x / cam->width * 2.0f - 1.0f, iy = nvm->projs[p].y / cam->height * 2.0f - 1.0f;

        glDisable(GL_DEPTH_TEST);
        glBegin(GL_LINES);
        glColor3f(1.0, 0, 1.0);
        glVertex3f(nvm->points[v].x, nvm->points[v].y, nvm->points[v].z);
        rotVertex(cam, cam->x, cam->y, cam->z, ix * rsW, iy * rsH, 0);
        glEnd();
        glEnable(GL_DEPTH_TEST);
    }

}

void capRenderer_AncientGL::drawModel(capModel *m)
{
    auto *rd = (ancientGL_renderData *)  m->rendererData;
    if(!rd) return;

    glPushMatrix();
    if(rd->flags & AGLF_LISTED)
    {
        glMultMatrixf(m->affineMatrix.m);
        glGetFloatv(GL_MODELVIEW_MATRIX, m->modelviewMatrix.m);
        if(m->flags & CAMF_EDGES)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);


        glCallList(rd->displayList);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    }

    if(m->cursor.valid)
    {
        float e = 0.05f / m->affineMatrix.getScale();
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(m->cursor.x + e, m->cursor.y, m->cursor.z);
        glVertex3f(m->cursor.x - e, m->cursor.y, m->cursor.z);
        glVertex3f(m->cursor.x, m->cursor.y + e, m->cursor.z);
        glVertex3f(m->cursor.x, m->cursor.y - e, m->cursor.z);
        glVertex3f(m->cursor.x, m->cursor.y, m->cursor.z - e);
        glVertex3f(m->cursor.x, m->cursor.y, m->cursor.z + e);
        glEnd();
    }

    if(m->nvm)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawNvm(m->nvm);
        glDisable(GL_BLEND);
    }

    glPopMatrix();

}


void capRenderer_AncientGL::drawView(std::vector <capModel*> &models, int proj)
{
    unsigned int i;
    GLfloat lightpos[4], lx, ly, lz;

    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    this->setupProjection(proj);
    this->setupCamera(proj);

    if(this->views[proj].fogStart != 0.0)
    {
        GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
        GLint fogMode = GL_LINEAR;

        glEnable(GL_FOG);

        glFogi(GL_FOG_MODE, fogMode);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.35);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 0);
        glFogf(GL_FOG_END, this->views[proj].fogStart);
    }
    else
        glDisable(GL_FOG);


    glColor3f(0, 1, 0);
    glPointSize(1);

	const GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    if(this->light.lightFlags & CLF_DRAGGED)
    {
        ly = this->cam.y + this->light.lightHeight;
        lx = cos(this->cam.yaw - CAP_PI_2) * this->light.lightOffset + this->cam.x;
        lz = sin(this->cam.yaw - CAP_PI_2) * this->light.lightOffset + this->cam.z;
    }
    else
    {
        lx = this->light.x;
        ly = this->light.y;
        lz = this->light.z;
    }


    lightpos[0] = lx;
    lightpos[1] = ly;
    lightpos[2] = lz;
    lightpos[3] = 1.0;
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, this->light.lightConstAtt);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, this->light.lightLinAtt);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, this->light.lightQuadAtt);

    glGetFloatv(GL_MODELVIEW_MATRIX, this->views[proj].projMatrix.m);

    for(i = 0; i < models.size(); i++)
        drawModel(models[i]);

    glDisable(GL_DEPTH_TEST);
    this->drawCamera();

    if(proj == CAV_PLAN || proj == CAV_PROFILE || proj == CAV_CAVER)
    {
        float vx1, vz1, vx2, vz2, yaw = this->cam.yaw, d = this->views[CAV_XSECT].orthoZoom;
        float tx = this->cam.x, ty = this->cam.y, tz = this->cam.z;
        vx1 = cosf(yaw - CAP_PI_2) * d;
        vz1 = sinf(yaw - CAP_PI_2) * d;
        vx2 = cosf(yaw + CAP_PI_2) * d;
        vz2 = sinf(yaw + CAP_PI_2) * d;

        tx += cosf(yaw) * this->views[CAV_XSECT].zOffset;
        tz += sinf(yaw) * this->views[CAV_XSECT].zOffset;

        if((proj == CAV_CAVER) && (this->views[CAV_XSECT].zOffset != 0.0))
        {
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glColor4ub(this->xRgba[0], this->xRgba[1], this->xRgba[2], this->xRgba[3]);
//            glColor4f(0.0, 0.0, 0.0, 0.5);

            glBegin(GL_QUADS);
            glVertex3f(tx + vx1*2.0f, ty - d*2.0f, tz + vz1*2.0f);
            glVertex3f(tx + vx1*2.0f, ty + d*2.0f, tz + vz1*2.0f);
            glVertex3f(tx + vx2*2.0f, ty + d*2.0f, tz + vz2*2.0f);
            glVertex3f(tx + vx2*2.0f, ty - d*2.0f, tz + vz2*2.0f);
            glEnd();

            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
        }
        else
        {
            glColor3f(0.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(tx + vx1, ty, tz + vz1);
            glVertex3f(tx + vx2, ty, tz + vz2);
            glEnd();
        }

    }

    if(proj == CAV_XSECT )
        if(wantRuler)
        {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glColor3f(0.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(ruler.x1, ruler.y1, -views[proj].zOffset);
            glVertex3f(ruler.x2, ruler.y2, -views[proj].zOffset);

            glEnd();
            glPopMatrix();
        }


    if(proj == CAV_XSECT || (this->currentView != CAV_FOURVIEW && this->currentView != CAV_COMPARE &&  this->currentView != CAV_GEOLOGIST))
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();


        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        if(proj != CAV_CAVER)
        {
            /* this->views[p].orthoZoom in model units == 1 in view
               1 in model units 		       == x width in view

               x = 1/orthoZoom
            */
            float unitWidth = 1.0f / this->views[proj].orthoZoom;
            float unit = 1;

            while(unitWidth > 1.0)
            {
                unitWidth /= 10;
                unit *= 0.1;
            }

            while(unitWidth < 0.05)
            {
                unitWidth *= 10;
                unit *= 10;
            }


            glDisable(GL_LIGHTING);
            glColor3f(1, 0, 0);
            glBegin(GL_LINES);
            glVertex3f(-0.90f, 0.92, 0);
            glVertex3f(-0.90f + unitWidth, 0.92, 0);

            glEnd();

            glRasterPos2f(0.5, 0.9);

        }

        glDisable(GL_LIGHTING);
        glRasterPos2f(-0.99f, -0.99f);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    glEnable(GL_DEPTH_TEST);

}

void capRenderer_AncientGL::drawCamera() {
    struct capCamera *cam = &this->cam;
    float vx, vy, vz, yaw = cam->yaw, pitch = cam->pitch;
    vx = cosf(yaw) * cosf(pitch);
    vy = sinf(pitch);
    vz = sinf(yaw) * cosf(pitch);

    glColor3f(0, 1, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(cam->x, cam->y, cam->z);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(cam->x, cam->y, cam->z);
    glVertex3f(cam->x + vx * 10.0f, cam->y + vy * 10.0f, cam->z + vz * 10.0f);
    glEnd();
}

void capRenderer_AncientGL::renderScene(std::vector <capModel*> &models)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, (GLsizei) this->displayWidth, (GLsizei) this->displayHeight);
    if(this->currentView == CAV_FOURVIEW || this->currentView == CAV_COMPARE || this->currentView == CAV_GEOLOGIST)
    {
        GLsizei w = this->displayWidth, h = this->displayHeight;
        glViewport(0, 0, w/2, h/2); drawView(models, CAV_PLAN);
        glViewport(w/2, 0, w/2, h/2);
/*        if(this->currentView == CAV_GEOLOGIST)
            capoc_draw_diphist(&state);
        else*/ 
            
        if(this->currentView == CAV_COMPARE)
            drawNvmCam();
        else
            drawView(models, CAV_PROFILE);
        glViewport(w/2, h/2, w/2, h/2); drawView(models, CAV_CAVER);
        glViewport(0, h/2, w/2, h/2);
        drawView(models, CAV_XSECT);
    }
    else if(this->currentView == CAV_NVMCAM)
        drawNvmCam();
/*    else if(this->currentView == CAV_DIPHIST)
        capoc_draw_diphist(&state);*/
// todo dipHist
    else
        drawView(models, this->currentView);


    glFlush();
}

void capRenderer_AncientGL::switchView(unsigned int view) {
    if(this->currentView == CAV_NVMCAM && view != currentView)
        if(this->texturedCam)
            reinitNvmCam(this->texturedCam);
    this->currentView = (unsigned int) view;
}

void capRenderer_AncientGL::updateView(unsigned int view) {

}

void capRenderer_AncientGL::setRuler(float x1, float y1, float x2, float y2)
{
    wantRuler = true;
    ruler.x1 = x1; ruler.y1 = y1;
    ruler.x2 = x2; ruler.y2 = y2;
}

capGenericPoint capRenderer_AncientGL::screenPosToNvm(capGenericPoint &p) {
    capGenericPoint r;
    float ar = 1.0;
    capNvmCam *cam = texturedCam;

    if(cam)
        if(cam->height > 0.0)
            ar = ((float) cam->width) / ((float) cam->height);

    r.x = p.x*ar;
    r.y = -p.y/aspectRatio;

    r.x *= views[CAV_NVMCAM].orthoZoom; /* Camera aspect ratio */
    r.y *= views[CAV_NVMCAM].orthoZoom; /* Screen aspect ratio */

    r.x -= views[CAV_NVMCAM].panX/ar;
    r.y += views[CAV_NVMCAM].panY;


    return r;
}

bool capRenderer_AncientGL::screenshot(const char *toFile)
{
    unsigned char *pixBuffer;
    unsigned int bufSize, l;

    glFlush();
    glFinish();
    if(!(pixBuffer = (unsigned char*) malloc(bufSize = displayWidth * displayHeight * 4))) return false;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    for(l = 0; l<displayHeight; l++)
        glReadPixels(0, displayHeight - l - 1, displayWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixBuffer + displayWidth*l*4);

    unsigned int error = lodepng::encode(toFile, pixBuffer, displayWidth, displayHeight);

    //if there's an error, display it
    if(error) capDebug::report(0, "Unable to save screenshot: %s", lodepng_error_text(error));

    free(pixBuffer);
    return true;
}