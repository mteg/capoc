//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_SVD_H
#define CAPOC2_SVD_H


class svd {
public:
    static int dsvd(float a[3][3], float w[3], float v[3][3]);
    static int dsvd_anysize(int m, int n, float **a, float *w, float **v);
    static float ** allocmatrix(int rows, int cols);
};


#endif //CAPOC2_SVD_H
