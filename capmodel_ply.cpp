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
#include <stdint.h>
#include "ply_io.h"
#include "capengine.h"
#include "capdebug.h"

int capModel::loadPly(const char *filename, int append, capColor *rgba) {
    FILE *fh;
    unsigned int voffs = 0, foffs = 0;

    append = this->loadStart(filename, append);

    struct tempFace {
        unsigned int num;
        unsigned int *verts;
    };

    PlyProperty vert_props[] = { /* list of property information for a vertex */
            {(char*) "x", Float32, Float32, (int) offsetof(capVertex,x), 0, 0, 0, 0},
            {(char*) "y", Float32, Float32, (int) offsetof(capVertex,y), 0, 0, 0, 0},
            {(char*) "z", Float32, Float32, (int) offsetof(capVertex,z), 0, 0, 0, 0},
            {(char*) "red", Uint8, Uint8, (int) offsetof(capVertex,r), 0, 0, 0, 0},
            {(char*) "green", Uint8, Uint8, (int) offsetof(capVertex,g), 0, 0, 0, 0},
            {(char*) "blue", Uint8, Uint8, (int) offsetof(capVertex,b), 0, 0, 0, 0},
            {(char*) "nx", Float32, Float32, (int) offsetof(capVertex,nx), 0, 0, 0, 0},
            {(char*) "ny", Float32, Float32, (int) offsetof(capVertex,ny), 0, 0, 0, 0},
            {(char*) "nz", Float32, Float32, (int) offsetof(capVertex,nz), 0, 0, 0, 0},
    };

    PlyProperty face_props[] = { /* list of property information for a face */
            {(char*) "vertex_indices", Int32, Int32, (int) offsetof(tempFace,verts), 1, Uint32, Uint32, (int) offsetof(tempFace,num)},
    };

    PlyFile *ply;
    if(!(fh = utf8open(this->fullpath, "rb")))
        return capDebug::error("Cannot open file: %s", this->filename);

    if (!(ply = read_ply(fh))) {
        return capDebug::error("Unable to read PLY file: %s", this->filename);
    }

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


    this->flags &= ~(CAMF_COLOR | CAMF_NORMALS);
    for(int e = 0; e<ply->num_elem_types; e++)
    {
        int ecount;
        char *ename = setup_element_read_ply(ply, e, &ecount);
        if(equal_strings((char*) "vertex", ename)) {
            this->reallocBuffers(ecount + voffs, this->nt);

            setup_property_ply(ply, &vert_props[0]);
            setup_property_ply(ply, &vert_props[1]);
            setup_property_ply(ply, &vert_props[2]);
            for (int j = 0; j < ply->elems[e]->nprops; j++) {
                PlyProperty *prop;
                prop = ply->elems[e]->props[j];
                if (equal_strings((char*) "r", prop->name) || equal_strings((char*) "red", prop->name)) {
                    setup_property_ply(ply, &vert_props[3]);
                    this->flags |= CAMF_COLOR;
                } else if (equal_strings((char*) "g", prop->name) || equal_strings((char*) "green", prop->name))
                    setup_property_ply(ply, &vert_props[4]);
                else if (equal_strings((char*) "b", prop->name) || equal_strings((char*) "blue", prop->name))
                    setup_property_ply(ply, &vert_props[5]);
                else if (equal_strings((char*) "nx", prop->name)) {
                    setup_property_ply(ply, &vert_props[6]);
                    this->flags |= CAMF_NORMALS;
                } else if (equal_strings((char*) "ny", prop->name))
                    setup_property_ply(ply, &vert_props[7]);
                else if (equal_strings((char*) "nz", prop->name))
                    setup_property_ply(ply, &vert_props[8]);
            }


            for (int i = 0; i < ecount; i++) {
                get_element_ply(ply, (void *) &v[i + voffs]);
                vf[i + voffs] = 0;
            }
            this->recolor(rgba, voffs);
        }
        else if(equal_strings((char*) "face", ename)) {
            int importedFaces = 0;
            this->reallocBuffers(this->nv, ecount + foffs);

            setup_property_ply(ply, &face_props[0]);

            tempFace f;

            memset(&t[foffs], 0, sizeof(struct capTriangle)*ecount);
            for (int j = 0; j < ecount; j++) {
                get_element_ply(ply, (void *) &f);
                if (f.num == 3) {
                    unsigned int a = f.verts[0] + voffs, b = f.verts[1] + voffs, c = f.verts[2] + voffs;
                    if(a >= voffs && a < nv && b >= voffs && b < nv && c >= voffs && c < nv) {
                        t[j + foffs].a = a;
                        t[j + foffs].b = b;
                        t[j + foffs].c = c;
                        importedFaces++;
                    }
                }
                free(f.verts);
            }
            capDebug::report(0, "%d faces actually imported from PLY", importedFaces);
        }
#if 0
        else
            get_other_element_ply(ply);
#endif
    }

    close_ply(ply);
    free_ply(ply);
    return this->loadEnd();
}
