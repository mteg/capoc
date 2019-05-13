/*
 *
 * Copyright (c) 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "capengine.h"
#include "capdebug.h"

int capModel::loadNvm(const char *filename, int append, capColor *rgba) {
    unsigned int voffs = 0;
    capNvm *nvm;

    nvm = new capNvm(this->renderer, this);
    if(!nvm->load(filename, 0)) {
        /* We should already have an error message */
        delete nvm;
        return 0;
    }

    append = this->loadStart(filename, append);

    if(append)
        voffs = this->nv;
    else
    {
        capDebug::report(1, "Deleting old model data");
        this->unload();
    }

    this->flags &= ~CAMF_NORMALS;
    this->reallocBuffers(this->nv + nvm->n_points, this->nt);

    for(unsigned int i = 0; i<nvm->n_points; i++)
    {
        capVertex &p = v[i+voffs];
        capNvmPoint &np = nvm->points[i];

        p.x = np.x;
        p.y = np.y;
        p.z = np.z;
        p.r = np.r;
        p.g = np.g;
        p.b = np.b;
        vf[i+voffs] = 0;
    }

    if(!this->nvm)
        this->nvm = nvm;
    else
        delete nvm;

    return this->loadEnd();
}
