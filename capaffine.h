//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPAFFINE_H
#define CAPOC2_CAPAFFINE_H


#include <vector>
#include "captypes.h"

class capAffineMatrix {
public:
    capAffineMatrix(capAffineMatrix *c);

    float m[16];

    capAffineMatrix();
    void setUnit();
    void setScale(float s);
    void setZero();
    void setTranslation(float, float, float);
    void setRotation(float, int);
    void compose(capAffineMatrix*);

    void dumpWith(void *ptr, int (*print)(void *, const char *, ...));

    void dump();

    void transform(float *v);
    void transformVector(float *v);

    float getScale();
    static float det33(float a[3][3]);

    int invert();
    void transpose();

    void setFrom(const capAffineMatrix *pMatrix);

    static capAffineMatrix *findBetween(std::vector<capGenericPoint> ca, std::vector<capGenericPoint> cb);
};


#endif //CAPOC2_CAPAFFINE_H
