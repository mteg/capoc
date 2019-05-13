/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include <cstring>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "capengine.h"
#include "capdebug.h"
#include "svd.h"

bool capModel::fitPlaneToSelection()
{
    unsigned int i, row = 0;
    float **a, **q, w[3];

    if(this->hasPlane)
        free(fitPlane.m);
    this->hasPlane = false;

    /* Find selection centroid and count number of rows in correlation matrix */
    double cx = 0, cy = 0, cz = 0;
    int rows = 0;
    for(i = 0; i<nv; i++)
    {
        if(!(vf[i] & CAV_MARKED)) continue;
        cx += v[i].x;
        cy += v[i].y;
        cz += v[i].z;
        rows++;
    }
    if(!rows) return false;

    /* Get centroid */
    cx /= (double) rows; cy /= (double) rows; cz /= (double) rows;
    fitPlane.cx = cx; fitPlane.cy = cy; fitPlane.cz = cz;

    a = svd::allocmatrix(rows, 3);
    q = svd::allocmatrix(3, 3);

    float maxDist = 0;
    /* Compute selection radius */
    for(i = 0; i<nv; i++)
    {
        float cDist;
        if(!(vf[i] & CAV_MARKED)) continue;
        cDist = (v[i].x - cx)*(v[i].x - cx) +
                (v[i].y - cy)*(v[i].y - cy) +
                (v[i].z - cz)*(v[i].z - cz);
        if(cDist > maxDist) maxDist = cDist;
    }
    fitPlane.radius = sqrtf(maxDist);
    capDebug::report(2, "fitPlane(): Centroid %f %f %f, radius %f", cx, cy, cz, fitPlane.radius);

    /* Insert points into correlation matrix */
    for(i = 0; i<nv; i++)
    {
        if(!(vf[i] & CAV_MARKED)) continue;
        a[row][0] = v[i].x - cx;
        a[row][1] = v[i].y - cy;
        a[row][2] = v[i].z - cz;
        row++;
    }

    svd::dsvd_anysize(rows, 3, a, w, q);
    float n[3];
    fitPlane.m = q;
    hasPlane = true;

    n[0] = q[0][0];
    n[1] = q[1][0];
    n[2] = q[2][0];

    fitPlane.nx = n[0];
    fitPlane.ny = n[1];
    fitPlane.nz = n[2];

    affineMatrix.transformVector(n);

    float len;
    /* Normalize the vector */
    len = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    n[0] /= len; n[1] /= len; n[2] /= len;

    capDebug::report(2, "fitPlane(): normal = %f %f %f", n[0], n[1], n[2]);

    if(n[1] < 0) {
        for (i = 0; i < 3; i++) n[i] *= -1;
        fitPlane.nx *= -1;
        fitPlane.ny *= -1;
        fitPlane.nz *= -1;
    }


    fitPlane.dip = acosf(n[1]) / CAP_PI * 180.0f;
    fitPlane.dipDir = -atan2f(n[0], n[2]) / CAP_PI * 180.0f + 90.0f;

    capDebug::report(2, "Got dip = %.1f, dipDir = %.1f", fitPlane.dip, fitPlane.dipDir);

    free(a);

    hasPlane = true;
    renderer->replaneModel(this);
    return true;
}

void capModel::dropPlane()
{
    if(this->hasPlane) free(fitPlane.m);
    this->hasPlane = false;
    this->renderer->replaneModel(this);
}
