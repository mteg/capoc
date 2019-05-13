/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include <cstring>
#include <cmath>
#include <cstdio>
#include "capengine.h"
#include "capdebug.h"
#include "svd.h"

void capModel::findTransformTo(capModel *destModel, bool doCompose) {
    std::vector <capGenericPoint> pa, pb;

    pa.clear(); pb.clear();
    for(auto const & it: this->savedPoints)
    {
        if(destModel->savedPoints.count(it.first))
        {
            pa.push_back(it.second);
            pb.push_back(destModel->savedPoints[it.first]);
        }
    }

    unsigned long n = pa.size();
    if(n < 3) return;

    capAffineMatrix *m = capAffineMatrix::findBetween(pa, pb);
    if(!m)
        return;

    if(doCompose)
        this->affineMatrix.compose(m);
    else
        this->affineMatrix.setFrom(m);

    delete m;
}


void capModel::scale(float sz)
{
    capAffineMatrix stepTransform, destTransform;

    destTransform.setUnit();
    /* scale x0.5 around (2,4,6) */

    stepTransform.setTranslation(-this->renderer->cam.x, -this->renderer->cam.y, -this->renderer->cam.z);
    destTransform.compose(&stepTransform);
    stepTransform.setScale(sz);
    destTransform.compose(&stepTransform);
    stepTransform.setTranslation(this->renderer->cam.x, this->renderer->cam.y, this->renderer->cam.z);
    destTransform.compose(&stepTransform);
    this->affineMatrix.compose(&destTransform);
}


void capModel::rotate(float degAngle, int axis)
{
    capAffineMatrix stepTransform, destTransform;

    stepTransform.setTranslation(-this->renderer->cam.x, -this->renderer->cam.y, -this->renderer->cam.z);
    destTransform.compose(&stepTransform);
    stepTransform.setRotation(degAngle,  axis);
    destTransform.compose(&stepTransform);
    stepTransform.setTranslation(this->renderer->cam.x, this->renderer->cam.y, this->renderer->cam.z);
    destTransform.compose(&stepTransform);
    this->affineMatrix.compose(&destTransform);
}

void capModel::translate(float tx, float ty, float tz)
{
    capAffineMatrix destTransform;
    destTransform.setUnit();
    destTransform.setTranslation(tx, ty, tz);
    this->affineMatrix.compose(&destTransform);
}
