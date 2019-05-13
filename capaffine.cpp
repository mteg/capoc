/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include "captypes.h"
#include "capaffine.h"
#include "capdebug.h"
#include "svd.h"
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <vector>



/*
 * Constructor - initialize with unit matrix
 */
capAffineMatrix::capAffineMatrix()
{
    setUnit();
}

capAffineMatrix::capAffineMatrix(capAffineMatrix *c)
{
    memcpy(m, c->m,  sizeof(m));
}

/*
 * Reset matrix to zeros
 */
void capAffineMatrix::setZero()
{
    memset(m, 0, sizeof(m));
    m[15] = 1.0;
}

/*
 * Reset matrix to unit matrix
 */
void capAffineMatrix::setUnit()
{
    setScale(1.0);
}

/*
 * Reset matrix to scale matrix
 */
void capAffineMatrix::setScale(float s) {
    setZero();
    m[0] = s;
    m[5] = s;
    m[10] = s;
}

/*
 * Compose with another matrix
 */
void capAffineMatrix::compose(capAffineMatrix *newMatrix)
{
    int x, y, j;
    float result[16];

    memset(result, 0, sizeof(result));
    for(x = 0; x<4; x++)
        for(y = 0; y<4; y++)
            for(j = 0; j<4; j++)
                result[y + x*4] += newMatrix->m[y + j*4] * m[j + x*4];
    memcpy(m, result, sizeof(result));
}

/*
 * Dump  matrix with custom functions
 */
void capAffineMatrix::dumpWith(void *ptr, int (*print)(void *, const char *, ...))
{
    print(ptr, "x' = %f*x %+f*y %+f*z %+f\r\n", m[0], m[4], m[8], m[12]);
    print(ptr, "y' = %f*x %+f*y %+f*z %+f\r\n", m[1], m[5], m[9], m[13]);
    print(ptr, "z' = %f*x %+f*y %+f*z %+f\r\n", m[2], m[6], m[10], m[14]);
    print(ptr, "model 0 transform matrix x %f %f %f %f y %f %f %f %f z %f %f %f %f\r\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14]);
}

/*
 * Dump matrix to stderr
 */
void capAffineMatrix::dump()
{
    dumpWith(stderr, (int (*)(void *, const char *, ...)) fprintf);
}

/*
 * Transform a point using matrix
 */
void capAffineMatrix::transform(float v[3])
{
    float x = v[0], y = v[1], z = v[2];
    float xp, yp, zp;
    xp = m[0] * x + m[4] * y + m[8] * z + m[12];
    yp = m[1] * x + m[5] * y + m[9] * z + m[13];
    zp = m[2] * x + m[6] * y + m[10] * z + m[14];

    v[0] = xp; v[1] = yp; v[2] = zp;
}

/*
 * Transform a vector using matrix (do not add offset from origin)
 */
void capAffineMatrix::transformVector(float v[3])
{
    float x = v[0], y = v[1], z = v[2];
    float xp, yp, zp;
    xp = m[0] * x + m[4] * y + m[8] * z;
    yp = m[1] * x + m[5] * y + m[9] * z;
    zp = m[2] * x + m[6] * y + m[10] * z;

    v[0] = xp; v[1] = yp; v[2] = zp;
}


/*
 * Get scale component from matrix, assuming it is uniformly scaling
 */
float capAffineMatrix::getScale() {
    float v[3] = {1.0, 0, 0};
    transform(v);
    v[0] -= m[12];
    v[1] -= m[13];
    v[2] -= m[14];
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void capAffineMatrix::setTranslation(float x, float y, float z)
{
    setUnit();
    m[3*4 + 0] = x;
    m[3*4 + 1] = y;
    m[3*4 + 2] = z;
}

void capAffineMatrix::setRotation(float degAngle, int axis)
{
    float radAngle = -degAngle * CAP_PI / 180.0f;
    float s = sinf(radAngle), c = cosf(radAngle);

    setUnit();

    /*
     *
     *     xp = m[0] * x + m[4] * y + m[8] * z + m[12];
           yp = m[1] * x + m[5] * y + m[9] * z + m[13];
           zp = m[2] * x + m[6] * y + m[10] * z + m[14];
     */

    if(axis == 0)	/* x-axis */
    {
        m[5] = c; m[9] = -s;
        m[6] = s; m[10] = c;
    }
    else if(axis == 1)	/* y-axis */
    {
        m[0] = c; m[8] = s;
        m[2] = -s; m[10] = c;
    }
    else if(axis == 2)  /* z-axis */
    {
        m[0] = c; m[4] = -s;
        m[1] = s; m[5] = c;
    }
}

float capAffineMatrix::det33(float a[3][3]) {
    return a[0][0] * ((a[1][1] * a[2][2]) - (a[2][1] * a[1][2])) -
           a[0][1] * (a[1][0] * a[2][2] - a[2][0] * a[1][2]) + a[0][2] * (a[1][0] * a[2][1] - a[2][0] * a[1][1]);
}


int capAffineMatrix::invert()
{
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
             m[4]  * m[11] * m[14] +
             m[8]  * m[6]  * m[15] -
             m[8]  * m[7]  * m[14] -
             m[12] * m[6]  * m[11] +
             m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
              m[4]  * m[10] * m[13] +
              m[8]  * m[5] * m[14] -
              m[8]  * m[6] * m[13] -
              m[12] * m[5] * m[10] +
              m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
             m[1]  * m[11] * m[14] +
             m[9]  * m[2] * m[15] -
             m[9]  * m[3] * m[14] -
             m[13] * m[2] * m[11] +
             m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
             m[0]  * m[11] * m[13] +
             m[8]  * m[1] * m[15] -
             m[8]  * m[3] * m[13] -
             m[12] * m[1] * m[11] +
             m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
             m[0]  * m[7] * m[14] +
             m[4]  * m[2] * m[15] -
             m[4]  * m[3] * m[14] -
             m[12] * m[2] * m[7] +
             m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
              m[0]  * m[6] * m[13] +
              m[4]  * m[1] * m[14] -
              m[4]  * m[2] * m[13] -
              m[12] * m[1] * m[6] +
              m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
             m[1] * m[7] * m[10] +
             m[5] * m[2] * m[11] -
             m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7] +
             m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
              m[0] * m[7] * m[9] +
              m[4] * m[1] * m[11] -
              m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] +
              m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return 0;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        m[i] = inv[i] * det;

    return 1;
}

void capAffineMatrix::setFrom(const capAffineMatrix *pMatrix) {
    memcpy(this->m, pMatrix->m, sizeof(float)*16);
}

capAffineMatrix * capAffineMatrix::findBetween(std::vector<capGenericPoint> pa, std::vector<capGenericPoint> pb)
{
        int j, k;
        float h[3][3];
        float s[3], v[3][3], r[3][3], t[3];
        float distA = 0.0, distB = 0.0, scale;
        capGenericPoint ca, cb;

        unsigned long n = pa.size(), i;
        if(n < 3) return NULL;

        for(i = 0; i<n; i++)
        {
            capDebug::report(2, "findBetween(): Pa[%d] = (%f, %f, %f)", i, pa[i].x, pa[i].y, pa[i].z);
            ca.x += pa[i].x;
            ca.y += pa[i].y;
            ca.z += pa[i].z;
        }
        for(i = 0; i<n; i++)
        {
            capDebug::report(2, "findBetween(): Pb[%d] = (%f, %f, %f)", i, pb[i].x, pb[i].y, pb[i].z);
            cb.x += pb[i].x;
            cb.y += pb[i].y;
            cb.z += pb[i].z;
        }

        ca.x /= (float) n; ca.y /= (float) n; ca.z /= (float) n;
        cb.x /= (float) n; cb.y /= (float) n; cb.z /= (float) n;

        capDebug::report(2, "findBetween(): Centroid ca = (%f, %f, %f)", ca.x, ca.y, ca.z);
        capDebug::report(2, "findBetween(): Centroid cb = (%f, %f, %f)", cb.x, cb.y, cb.z);

        for(i = 0; i<n; i++)
        {
            distA += sqrt(pow(pa[i].x - ca.x, 2) + pow(pa[i].y - ca.y, 2) + pow(pa[i].z - ca.z, 2));
            distB += sqrt(pow(pb[i].x - cb.x, 2) + pow(pb[i].y - cb.y, 2) + pow(pb[i].z - cb.z, 2));
        }

        scale = distB / distA;
        capDebug::report(2, "findBetween(): Scale between models = %f", scale);

        for(j = 0; j<3; j++)
            for(i = 0; i<3; i++)
                h[i][j] = 0;

        for(i = 0; i<n; i++)
        {
            float ax = pa[i].x - ca.x, ay = pa[i].y - ca.y, az = pa[i].z - ca.z;
            float bx = pb[i].x - cb.x, by = pb[i].y - cb.y, bz = pb[i].z - cb.z;
            h[0][0] += ax*bx*scale; h[0][1] += ax*by*scale; h[0][2] += ax*bz*scale;
            h[1][0] += ay*bx*scale; h[1][1] += ay*by*scale; h[1][2] += ay*bz*scale;
            h[2][0] += az*bx*scale; h[2][1] += az*by*scale; h[2][2] += az*bz*scale;
        }

        capDebug::report(2, "findBetween(): Covariance matrix computed:");
        for(i = 0; i<3; i++)
            capDebug::report(2, "\t%f\t%f\t%f", h[i][0], h[i][1], h[i][2]);

        svd::dsvd(h, s, v);

        capDebug::report(2, "findBetwen(): SVD decomposition: U = ");
        for(i = 0; i<3; i++)
            capDebug::report(2, "\t%f\t%f\t%f", h[i][0], h[i][1], h[i][2]);

        capDebug::report(2, "findBetween(): SVD decomposition: V = ");
        for(i = 0; i<3; i++)
            capDebug::report(2, "\t%f\t%f\t%f\n", v[i][0], v[i][1], v[i][2]);

        capDebug::report(2, "findBetween(): S = %f\t%f\t%f", s[0], s[1], s[2]);

        for(i = 0; i<3; i++)
            for(j = 0; j<3; j++)
            {
                float sum = 0.0;
                for(k = 0; k<3; k++)
                    sum += v[i][k]*h[j][k];

                r[i][j] = sum;
            }



        if(capAffineMatrix::det33(r) < 0)
        {
            capDebug::report(2, "findBetween(): Adjusting R...");
            for(i = 0; i<3; i++)
                r[i][2] *= -1;
        }

        capDebug::report(2, "findBetween(): Transform: R = ");
        for(i = 0; i<3; i++)
            capDebug::report(2, "\t%f\t%f\t%f", r[i][0], r[i][1], r[i][2]);


        t[0] = scale*(-r[0][0]*ca.x-r[0][1]*ca.y-r[0][2]*ca.z) + cb.x;
        t[1] = scale*(-r[1][0]*ca.x-r[1][1]*ca.y-r[1][2]*ca.z) + cb.y;
        t[2] = scale*(-r[2][0]*ca.x-r[2][1]*ca.y-r[2][2]*ca.z) + cb.z;

        capDebug::report(2, "findBetween(): Transform T = %f\t%f\t%f", t[0], t[1], t[2]);

        for(i = 0; i<n; i++)
        {
            float ax = pa[i].x, ay = pa[i].y, az = pa[i].z;
            float bx = pb[i].x, by = pb[i].y, bz = pb[i].z;
            float tx, ty, tz;

            tx = ax*r[0][0]*scale + ay*r[0][1]*scale + az*r[0][2]*scale + t[0];
            ty = ax*r[1][0]*scale + ay*r[1][1]*scale + az*r[1][2]*scale + t[1];
            tz = ax*r[2][0]*scale + ay*r[2][1]*scale + az*r[2][2]*scale + t[2];

            float dist = sqrtf(powf(tx-bx, 2.0) + powf(ty-by, 2.0) + powf(tz-bz, 2.0));
            capDebug::report(1, "findBetween(): Point (%f, %f, %f) translates to (%f, %f, %f), should be (%f, %f, %f). Distance = %f", ax, ay, az, tx, ty, tz, bx, by, bz, dist);
        }

        auto m = new capAffineMatrix;

        m->setUnit();
        m->m[0] = r[0][0]*scale; m->m[4] = r[0][1]*scale; m->m[8] = r[0][2]*scale; m->m[12] = t[0];
        m->m[1] = r[1][0]*scale; m->m[5] = r[1][1]*scale; m->m[9] = r[1][2]*scale; m->m[13] = t[1];
        m->m[2] = r[2][0]*scale; m->m[6] = r[2][1]*scale; m->m[10] = r[2][2]*scale; m->m[14] = t[2];
        m->dump();

        return m;
}

void capAffineMatrix::transpose() {
    float nm[16];
    memset(nm, 0, sizeof(float) * 16);
    nm[0] = m[0]; nm[4] = m[1]; nm[8] = m[2];
    nm[1] = m[4]; nm[5] = m[5]; nm[9] = m[6];
    nm[2] = m[8]; nm[6] = m[9]; nm[10] = m[10];
    memcpy(this->m, nm, sizeof(float) * 16);
}
