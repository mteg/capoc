
/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "capengine.h"
#include "capdebug.h"
#include "caprenderer.h"

capModel::capModel(capRenderer *renderer)
{
    this->modelviewMatrix.setUnit();
    this->affineMatrix.setUnit();
    this->renderer = renderer;
}


void capModel::ensureEdges()
{
    if(!this->e)
        this->reedge();
}



void capModel::computeNormals()
{
    unsigned int i;
    if(!(this->flags & CAMF_LOADED)) return;
    for(i = 0; i<this->nv; i++)
    {
        this->v[i].nx = 0;
        this->v[i].ny = 0;
        this->v[i].nz = 0;
    }

    /* Compute face normals */
    for(i = 0; i<this->nt; i++)
    {
        struct capVertex va, vb, n;
        int a = this->t[i].a, b = this->t[i].b, c = this->t[i].c;

        va.nx = this->v[b].x - this->v[a].x;
        va.ny = this->v[b].y - this->v[a].y;
        va.nz = this->v[b].z - this->v[a].z;

        vb.nx = this->v[c].x - this->v[a].x;
        vb.ny = this->v[c].y - this->v[a].y;
        vb.nz = this->v[c].z - this->v[a].z;

        n.nx = (va.ny * vb.nz) - (va.nz * vb.ny);
        n.ny = (va.nz * vb.nx) - (va.nx * vb.nz);
        n.nz = (va.nx * vb.ny) - (va.ny * vb.nx);

//        off_normalize(&n);
        vAdd(&this->v[a], &n);
        vAdd(&this->v[b], &n);
        vAdd(&this->v[c], &n);
    }

    /* Normalize vertex normals */
    for(i = 0; i<this->nv; i++)
        vNormalize(&this->v[i]);

    /* Mark that we now have these normals */
    this->flags |= CAMF_NORMALS;
    this->renderer->reinitModel(this);
}

static int vEdgeCmp(const void *va, const void *vb)
{
    auto *a = (struct capEdge*) va;
    auto *b = (struct capEdge*) vb;

    return (a->from == b->from) ? (a->to - b->to) : (a->from - b->from);
}

void capModel::reedge()
{
    unsigned int i, ei = 0, listLen = 0,  lsz;

    if(!this->e)
    {
        capDebug::assert((this->e = (struct capEdge*) realloc(this->e, lsz = sizeof(struct capEdge)*this->nt*6)));
        capDebug::report(1, "Allocated edge buffer: %d MB", lsz / 1024 / 1024);
    }

    if(!this->ep)
    {
        capDebug::assert((this->ep = (struct capEdgePointer*) realloc(this->ep, lsz = sizeof(struct capEdgePointer)*this->nv)));
        capDebug::report(1, "Allocated edge pointer buffer: %d MB", lsz / 1024 / 1024);
    }

    capDebug::report(1, "Building adjacency list");
    for(i = 0; i<this->nt; i++)
    {
        this->e[ei].from = this->t[i].a; this->e[ei++].to = this->t[i].b;
        this->e[ei].from = this->t[i].a; this->e[ei++].to = this->t[i].c;
        this->e[ei].from = this->t[i].b; this->e[ei++].to = this->t[i].a;
        this->e[ei].from = this->t[i].b; this->e[ei++].to = this->t[i].c;
        this->e[ei].from = this->t[i].c; this->e[ei++].to = this->t[i].a;
        this->e[ei].from = this->t[i].c; this->e[ei++].to  = this->t[i].b;
    }

    this->ne = ei;
    capDebug::report(1, "Sorting adjacency list (%d items).", ei);
    qsort(this->e, this->ne, sizeof(struct capEdge), vEdgeCmp);

    capDebug::report(1, "Assigning adjacency lists");
    for(i = 0; i<this->ne; i+= listLen)
    {
        unsigned int thisVert = this->e[i].from,  maxLen = this->ne - i;
        this->ep[thisVert].edgeStart = i;
        for(listLen = 1; listLen<maxLen; listLen++)
            if(this->e[i + listLen].from != thisVert) break;
        this->ep[thisVert].edgeLen = listLen;
    }
}


void capModel::unload()
{
    this->renderer->uninitModel(this);
    free(this->vf); free(this->t); free(this->v); free(this->e);  free(this->ep);
    this->t = NULL; this->v = NULL; this->e = NULL; this->ep =  NULL; this->vf = NULL;
    this->flags &= ~CAMF_LOADED;
    this->nt = 0; this->nv = 0; this->ne = 0;
}



void capModel::recolor(capColor *rgba)
{
    recolor(rgba, 0);
    this->renderer->reinitModel(this);
}

void capModel::recolor(capColor *rgba, unsigned int voffs) {
    unsigned int i;
    if(rgba)
    {
        if((this->flags & CAMF_COLOR) && rgba->a != 255)
        {
            for(i = 0; i<nv; i++)
            {
                this->v[i + voffs].r = (unsigned char) ((this->v[i + voffs].r * (255 - rgba->a) + rgba->r * rgba->a) / 255);
                this->v[i + voffs].g = (unsigned char) ((this->v[i + voffs].g * (255 - rgba->a) + rgba->g * rgba->a) / 255);
                this->v[i + voffs].b = (unsigned char) ((this->v[i + voffs].b * (255 - rgba->a) + rgba->b * rgba->a) / 255);
            }
        }
        else
        {
            for(i = 0; i<nv; i++)
            {
                this->v[i + voffs].r = rgba->r;
                this->v[i + voffs].g = rgba->g;
                this->v[i + voffs].b = rgba->b;
            }
        }
        this->flags |= CAMF_COLOR;
    }

}



void capModel::deleteSelected()
{
    int i, n = this->nv;

    for(i = 0; i<n; i++)
        if(CA_HASFLAG(i, CAV_MARKED))
            CA_FLAG(i, CAV_DELETED);
    this->renderer->reinitModel(this);
}

void capModel::undeleteAll()
{
    int i, n = this->nv;

    for(i = 0; i<n; i++)
        if(CA_HASFLAG(i, CAV_MARKED))
            CA_UNFLAG(i, CAV_DELETED);
    this->renderer->reinitModel(this);
}


int capModel::recolorFrom(const char *filename, int flags)
{
    char colorLine[128 + 4];
    unsigned int nv = this->nv, done = 0, lsz, i;
    unsigned char *cf = getField();
    FILE *fh;

    if(!(fh = utf8open(filename, "r")))
        return capDebug::error("Cannot open file: %s", filename);

    memset(cf, 0, lsz = this->nv);

    while(fgets(colorLine, 128, fh))
    {
        char *dataLine = colorLine;
        int r, g, b = -1, v;
        dataLine = vPopint(dataLine, &v);
        dataLine = vPopint(dataLine, &r);
        dataLine = vPopint(dataLine, &g);
        dataLine = vPopint(dataLine, &b);
        if(b != -1)
        {
            if(v >= 0 && v < ((int) nv))
            {
                this->v[v].r = (unsigned char) r;
                this->v[v].g = (unsigned char) g;
                this->v[v].b = (unsigned char) b;
                if(flags & CAC_UNDELETE_COLORABLE)
                    CA_UNFLAG(v, CAV_DELETED);
                CA_FLAG_A(cf, v, CAV_MARKED);
                done++;
            }
        }
    }

    if(flags & CAC_UNDELETE_COLORABLE)
        for(i = 0; i<lsz; i++)
            if(CA_HASFLAG_A(cf, i, CAV_MARKED))
                CA_UNFLAG(i, CAV_DELETED);

    if(flags & CAC_DELETE_UNCOLORABLE)
        for(i = 0; i<lsz; i++)
            if(!CA_HASFLAG_A(cf, i, CAV_MARKED))
                CA_FLAG(i,  CAV_DELETED);

    if(flags & CAC_GRAY_UNCOLORABLE)
        for(i = 0; i<nv; i++)
            if(!CA_HASFLAG_A(cf, i, CAV_MARKED))
            {
                this->v[i].r = 192;
                this->v[i].g = 192;
                this->v[i].b = 192;
            }

    free(cf);

    capDebug::report(1, "Colored %d verts", done);
    fclose(fh);
    this->renderer->reinitModel(this);
    return 1;
}

void  capModel::updateDisplay()
{
    this->renderer->reinitModel(this);
}

void capModel::setPoint(char* const name, float tx, float ty, float tz)
{
    capGenericPoint p;
    p.x = tx; p.y = ty; p.z = tz;
    setPoint(name, p);
}

void capModel::setPoint(char* const name, const capGenericPoint p)
{
    savedPoints[name] = p;
}

void capModel::deletePoint(char* const name)
{
    if(!savedPoints.count(name)) return;
    savedPoints.erase(name);
}

void capModel::seePoint(char *name, capCamera *view, float step) {
    if (!savedPoints.count(name)) return;
    capGenericPoint &p = savedPoints[name];

    /* Get point */
    float v[3] = {p.x, p.y, p.z};

    /* Transform to real-world coordinates */
    this->affineMatrix.transform(v);

    /* Place camera at the point */
    view->x = v[0];
    view->y = v[1];
    view->z = v[2];

    /* Go back */
    float lx = cosf(view->yaw) * cosf(view->pitch);
    float ly = sinf(view->pitch);
    float lz = sinf(view->yaw) * cosf(view->pitch);

    view->x += lx * step;
    view->y += ly * step;
    view->z += lz * step;
}

int capModel::readNvm(const char *fname, unsigned int n) {
    if(nvm)
    {
        delete nvm;
        nvm = NULL;
    }
    nvm = new capNvm(this->renderer, this);
    if(!nvm->load(fname, n))
        return 0;

    return 1;
}
