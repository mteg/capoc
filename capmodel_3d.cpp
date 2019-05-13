/*
 *
 * Copyright (c) 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under CC BY-SA 4.0 license
 *
 */

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <stdint.h>
#include "capengine.h"
#include "capdebug.h"
#include "img.h"




int capModel::load3d(const char *filename, int append, capColor *rgba) {
    FILE *fh;
    unsigned int voffs = 0, foffs = 0, vsize = 0, fsize = 0, nv = 0, nt = 0;
    img *im;

    append = this->loadStart(filename, append);

    if(!(fh = utf8open(this->fullpath, "rb")))
        return capDebug::error("Cannot open file: %s", this->filename);

    if(!(im = img_read_stream_survey(fh, fclose, this->filename, NULL)))
        return capDebug::error("Cannot open file: %s, error code %d", this->filename, img_error());

    if(append)
    {
        voffs = this->nv;
        foffs = this->nt;
    }
    else
    {
        capDebug::report(1, "Deleting old model data");
        this->unload();
    }
    vsize = this->nv; fsize = this->nt;
    flags &= ~(CAMF_NORMALS | CAMF_POINTS | CAMF_FACES); flags |= CAMF_COLOR;
    flags |= CAMF_EDGES;

    while(1)
    {
        img_point pt;
        int res = img_read_item(im, &pt);
        if(res == img_STOP) break;
        if(res == img_BAD) {
            img_close(im);
            this->reallocBuffers(nv+voffs, nt+foffs);
            capDebug::error("Error while reading file: %s, error code %d", this->filename, img_error());
            return this->loadEnd();
        }
        switch(res) {
            case img_MOVE:
            case img_LINE:
                /* Insert new point */

                /* Grow buffers if neccessary */
                if ((voffs + nv)>= vsize) {
                    vsize += 64;
                    this->reallocBuffers(vsize, fsize);
                }

                v[voffs + nv].x = pt.y;
                v[voffs + nv].y = pt.z;
                v[voffs + nv].z = pt.x;
                v[voffs + nv].r = 255;
                v[voffs + nv].g = 0;
                v[voffs + nv].b = 255;
                vf[voffs+ nv] = 0;
                nv++;

                if (res == img_LINE && nv > 1) {
                    /* Grow buffers if neccessary */
                    if((foffs + nt) >= fsize) {
                        fsize += 64;
                        this->reallocBuffers(vsize, fsize);
                    }
                    t[foffs + nt].a = nv - 1 + voffs;
                    t[foffs + nt].b = nv - 1 + voffs;
                    t[foffs + nt].c = nv - 2 + voffs;
                    nt++;
                }
                break;
            case img_LABEL:
                setPoint(im->label, pt.y, pt.z, pt.x);
                break;

        }
    }

    /* Shrink buffers */
    this->reallocBuffers(nv+voffs, nt+foffs);
    img_close(im);
    return this->loadEnd();
}
