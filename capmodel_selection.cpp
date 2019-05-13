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


float capModel::getMarkedArea()
{
    return this->getMarkedArea(this->vf);
}

float capModel::getMarkedArea(unsigned char *mv)
{
    float totalArea = 0;
    float scaleFactor = this->affineMatrix.getScale();
    unsigned int i;

    scaleFactor *= scaleFactor;

    for(i = 0; i<this->nt; i++)
    {
        struct capVertex va, vb, n;
        int a = this->t[i].a, b = this->t[i].b, c = this->t[i].c;
        float area;

        if(!(CA_HASFLAG_A(mv, a, CAV_MARKED) && CA_HASFLAG_A(mv, b, CAV_MARKED) && CA_HASFLAG_A(mv, c, CAV_MARKED))) continue;

        /* Compute normal in model coordinates */
        va.nx = this->v[b].x - this->v[a].x;
        va.ny = this->v[b].y - this->v[a].y;
        va.nz = this->v[b].z - this->v[a].z;

        vb.nx = this->v[c].x - this->v[a].x;
        vb.ny = this->v[c].y - this->v[a].y;
        vb.nz = this->v[c].z - this->v[a].z;

        n.nx = (va.ny * vb.nz) - (va.nz * vb.ny);
        n.ny = (va.nz * vb.nx) - (va.nx * vb.nz);
        n.nz = (va.nx * vb.ny) - (va.ny * vb.nx);

        area = sqrtf(n.nx*n.nx + n.ny*n.ny + n.nz*n.nz);
        totalArea += area;
    }

    totalArea *= 0.5 * scaleFactor;
    return totalArea;
}


void capModel::walkEdge(int steps)
{
    unsigned int i;
    this->ensureEdges();

    for(i = 0; i<this->nv; i++)
    {
        int v,  st;
        if(!CA_HASFLAG(i, CAV_MARKED)) continue;

        v = i;
        for(st = 0; st<steps; st++)
        {
            int es, el, e, flagged = 0;
            es = this->ep[v].edgeStart;
            el = this->ep[v].edgeLen;
            for(e = 0; e<el; e++)
            {
                if(e<(el-1))
                {
                    if(this->e[es+e+1].to == this->e[es+e].to)
                    {
                        while(this->e[es+e+1].to == this->e[es+e].to && e<(el-1)) e++;
//                        fprintf(stderr, "vert %d e %d/%d multiple edges\n", i, e, el);
                        continue;
                    }
                }

//                fprintf(stderr, "vert %d e %d/%d lone edge to %d\n", i, e, el, this->e[es+e].to);
                /* Sentinelle edge */
                if(!CA_HASFLAG(this->e[es+e].to, CAV_MARKED))
                {
                    CA_FLAG(this->e[es+e].to, CAV_MARKED);
                    flagged++;
                }
            }
            if(!flagged) break;
        }
    }
    this->renderer->reselectModel(this);
}

unsigned char *capModel::getField()
{
    unsigned char *pv;
    capDebug::assert((pv = (unsigned char*) malloc(this->nv)));
    return pv;
}

#define FDIST(x0, y0, z0, x1, y1, z1) sqrtf(((x0)-(x1))*((x0)-(x1)) + ((y0)-(y1))*((y0)-(y1)) + ((z0)-(z1))*((z0)-(z1)))

/* leave only vertices adjacent to non-selected vertices */
unsigned char * capModel::getBoundary(const unsigned char *mv)
{
    unsigned char *of = getField();
    unsigned int i;

    ensureEdges();

    for(i = 0; i<this->nv; i++)
    {
        int e, es, el;
        if(!(i & 7))
            if(!mv[i >> 3])
            {
                i += 7;
                continue;
            }

        if(!CA_HASFLAG_A(mv, i, CAV_MARKED)) continue;
        es = this->ep[i].edgeStart;
        el = this->ep[i].edgeLen;
        for(e = 0; e<el; e++)
        {
            int j = this->e[e+es].to;
            if(!CA_HASFLAG_A(mv, j, CAV_MARKED))
                CA_FLAG_A(of, j, CAV_MARKED);
        }
    }

    return of;
}

/* wrapper for CLI */
void capModel::selectBoundary()
{
    unsigned char *nv = getBoundary(this->vf);
    free(this->vf);
    this->vf = nv;
    this->renderer->reselectModel(this);
}


/* expand selection on manifold by radius */
void capModel::expandSelection(float radius, unsigned char *mv)
{
    float *energyBuffer;
    unsigned char *pv = getField();
    unsigned int i, vertMarked = 1, iter;
    unsigned int lsz;

    radius /= this->affineMatrix.getScale();

    capDebug::assert((energyBuffer = (float*) malloc(lsz = sizeof(float)*this->nv)));
    memset(energyBuffer, 0, lsz);
    memcpy(pv, mv, this->nv);

    ensureEdges();
    for(i = 0; i<this->nv; i++)
    {
        if(!(i & 7))
            if(!mv[i >> 3])
            {
                i += 7;
                continue;
            }
        if(CA_HASFLAG_A(mv, i, CAV_MARKED))
            energyBuffer[i] = radius;
    }

    iter = 0;
    while(vertMarked)
    {
        vertMarked = 0; iter++;
        for(i = 0; i<this->nv; i++)
        {
            int e, es, el;
            if(!(i & 7))
                if(!pv[i >> 3])
                {
                    i += 7;
                    continue;
                }

            if(!CA_HASFLAG_A(pv, i, CAV_MARKED)) continue;
            CA_UNFLAG_A(pv, i, CAV_MARKED);

            es = this->ep[i].edgeStart;
            el = this->ep[i].edgeLen;

            for(e = 0; e<el; e++)
            {
                int j = this->e[e+es].to;
                float dist = FDIST(this->v[i].x, this->v[i].y, this->v[i].z, this->v[j].x, this->v[j].y, this->v[j].z);
                if(dist <= energyBuffer[i])
                {
                    float newEnergy = energyBuffer[i] - dist;
                    if(newEnergy > energyBuffer[j])
                    {
                        energyBuffer[j] = newEnergy;
                        CA_FLAG_A(pv, j, CAV_MARKED);
                        CA_FLAG_A(mv, j, CAV_MARKED);
                        vertMarked++;
                    }
                }
            }
        }
        capDebug::report(2, "Marking radius %g, iter %d changed %d", radius, iter, vertMarked);
    }

    free(pv);
    free(energyBuffer);
}

void capModel::subtractSelections(unsigned char *fa, const unsigned char *fb)
{
    int i, bsz = this->nv;
    for(i = 0;i<bsz; i++)
        fa[i] = fa[i] & ~fb[i];
}

unsigned char *capModel::popComponent(unsigned char *mv)
{
    unsigned int v;
    unsigned int *bfsQueue;
    unsigned char *of;
    int qsize, qlen =  0, qpos =  0;

    for(v = 0; v<this->nv;v++)
        if(CA_HASFLAG_A(mv, v, CAV_MARKED)) break;

    if(v >= this->nv)  return NULL;

    qsize = 1024;
    capDebug::assert((bfsQueue = (unsigned int*) malloc(sizeof(unsigned int) * qsize)));
    bfsQueue[qpos] = v;
    qlen++;

    of = getField();
    memset(of, 0, this->nv);
    CA_FLAG_A(of, v, CAV_MARKED);

    ensureEdges();

    while(qlen > 0)
    {
        int e, es, el;
        /* pop one vertex from queue */
        v = bfsQueue[qpos & (qsize - 1)];
        qpos++; qlen--;

        /* mark it */
//        printf("popping %d len = %d\n", v, qlen);

        /* get unmarked neighbors and queue them */
        es = this->ep[v].edgeStart;
        el = this->ep[v].edgeLen;

        for(e = 0; e<el; e++)
        {
            unsigned int j = this->e[e+es].to;
            if((!CA_HASFLAG_A(of, j,  CAV_MARKED)) && (CA_HASFLAG_A(mv, j, CAV_MARKED)))
            {
                if(qlen >= qsize)
                {
                    unsigned int *newQueue;
                    int k;
                    capDebug::assert((newQueue = (unsigned int*) malloc(sizeof(unsigned int) * qsize * 2)));
                    for(k = 0; k<qlen; k++)
                        newQueue[k] = bfsQueue[(qpos + k) & (qsize - 1)];
                    qpos = 0;
                    qsize *= 2;
                    free(bfsQueue);
                    bfsQueue = newQueue;

                }
                bfsQueue[(qpos + qlen) & (qsize - 1)]  = j;
                qlen++;
                //               printf("pushing %d (len = %d)\n", j, qlen);
                CA_FLAG_A(of, j, CAV_MARKED);
            }
        }
        //      printf("done pushing (len = %d)\n", qlen);
    }

    free(bfsQueue);
    return of;
}

void capModel::popComponent()
{
    unsigned char *nf =  popComponent(this->vf);
    if(nf)
    {
        free(this->vf);
        this->vf = nf;
    }
}

void capModel::dumpComponents()
{
    dumpComponents("");
}

void capModel::dumpComponents(const char *filename)
{
    unsigned char *nf;
    unsigned char *of;

    of = getField();
    memcpy(of, this->vf, this->nv);

    while((nf = popComponent(of)))
    {
        float area =  getMarkedArea(nf);
        unsigned int i;

        if(area > 0.00000001)
        {

            capDebug::report(0, "patch %.8f %s", area, filename);
            for(i = 0; i<this->nv; i++)
            {
                if(CA_HASFLAG_A(nf, i, CAV_MARKED))
                    capDebug::report(0, "%d %d %d %d", i, this->v[i].r, this->v[i].g, this->v[i].b);
            }
        }

        subtractSelections(of, nf);
        free(nf);
    }
    free(of);
}


void capModel::growSelection(float radius)
{
    unsigned char *bf;
    if(radius == 0.0) return;
    if(radius > 0)
    {
        expandSelection(radius, this->vf);
        return;
    }

    /* and now ... */
    bf = getBoundary(this->vf);
    expandSelection(-radius, bf);
    subtractSelections(this->vf, bf);
    free(bf);
    this->renderer->reselectModel(this);
}

void capModel::resetSelection()
{
    unsigned int i;
    for(i = 0; i<nv; i++)
        CA_UNFLAG(i, CAV_MARKED);
    this->renderer->reselectModel(this);
}

unsigned int capModel::ballSelect(float radius, float x0, float y0, float z0)
{
    unsigned int i, q = 0;

    resetSelection();
    radius *= radius;

    for(i = 0; i<this->nv; i++)
    {
        float x, y, z, dist;

        x = this->v[i].x;
        y = this->v[i].y;
        z = this->v[i].z;
        dist = (x0 - x) * (x0 - x) + (y0 - y) * (y0 - y) + (z0 - z) * (z0 - z); /* (z0 - z) * (z0 - z) ; */

        if(dist <= radius)
        {
            CA_FLAG(i, CAV_MARKED);
            q++;
        }
    }
    this->renderer->reselectModel(this);

    this->cursor.valid = true;
    this->cursor.x = x0;
    this->cursor.y = y0;
    this->cursor.z = z0;

    return q;
}

unsigned int capModel::ballSelect(capModel_ball b)
{
    return ballSelect(b.radius, b.x0, b.y0, b.z0);
}

capModel_ball capModel::ballViewToModel(capModel_ball vb)
{
    auto *ai = new capAffineMatrix(this->modelviewMatrix);
    capModel_ball mb;
    float cv[3];

    cv[0] = vb.x0; cv[1] = vb.y0; cv[2] = vb.z0;
    ai->invert();
    ai->transform(cv);
    mb.x0 = cv[0]; mb.y0 = cv[1]; mb.z0 = cv[2];
    mb.radius = vb.radius/this->modelviewMatrix.getScale();
    return mb;
}

void capModel::selectBehind()
{
    unsigned int i;
    for(i = 0; i<this->nv; i++)
    {
        float v[3] = {this->v[i].x, this->v[i].y, this->v[i].z};
        this->modelviewMatrix.transform(v);
        if(v[2] >= 0.0f)
            CA_FLAG(i, CAV_MARKED);
    }
}


