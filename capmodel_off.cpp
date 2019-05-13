/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
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


void capModel::trim(char *line)
{
    char *eol;
    if((eol = strchr(line, '\r'))) *eol = 0;
    if((eol = strchr(line, '\n'))) *eol = 0;
}

int capModel::saveOff(const char *filename)
{
    FILE *fh = fopen(filename, "w");
    const char *hdr = "OFF";
    int *newIndexes, index = 0;
    unsigned int i, nnv = 0, nnt = 0;

    if(!fh)
        return capDebug::error("Cannot open file: %s", filename);

    capDebug::report(1, "Saving model as %s...", filename);

    newIndexes = (int*) malloc(sizeof(int) * this->nv);

    if((this->flags & CAMF_NORMALS) && (this->flags & CAMF_COLOR))
        hdr = "NCOFF";
    else if(this->flags & CAMF_NORMALS)
        hdr = "NOFF";
    else if(this->flags & CAMF_COLOR)
        hdr = "COFF";

    for(i = 0; i<this->nv; i++)
        if(!CA_HASFLAG(i, CAV_DELETED)) nnv++;

    for(i = 0; i<this->nt; i++)
        if(!(
                CA_HASFLAG(this->t[i].a, CAV_DELETED) ||
                CA_HASFLAG(this->t[i].b, CAV_DELETED) ||
                CA_HASFLAG(this->t[i].c, CAV_DELETED))) nnt++;

    fprintf(fh, "%s\n", hdr);
    fprintf(fh, "%d %d 0\n", nnv, nnt);

    for(i = 0; i<this->nv; i++)
    {
        if(CA_HASFLAG(i, CAV_DELETED)) continue;
        fprintf(fh, "%.6f %.6f %.6f", this->v[i].x, this->v[i].y, this->v[i].z);
        if(this->flags & CAMF_COLOR)
            fprintf(fh, " %d %d %d", this->v[i].r, this->v[i].g, this->v[i].b);
        if(this->flags & CAMF_NORMALS)
            fprintf(fh, " %.6f %.6f %.6f", this->v[i].nx, this->v[i].ny, this->v[i].nz);
        fputs("\n", fh);
        newIndexes[i] = index++;
    }

    for(i = 0; i<this->nt; i++)
    {
        unsigned int a = this->t[i].a, b = this->t[i].b, c = this->t[i].c;
        if(CA_HASFLAG(a, CAV_DELETED)) continue;
        if(CA_HASFLAG(b, CAV_DELETED)) continue;
        if(CA_HASFLAG(c, CAV_DELETED)) continue;
        fprintf(fh, "3 %d %d %d\n", newIndexes[a], newIndexes[b], newIndexes[c]);
    }

    fclose(fh);
    free(newIndexes);
    capDebug::report(1, "Saved %d vertices & %d triangles...", nnv, nnt);
    return 1;
}

int capModel::saveSelectionOff(const char *filename)
{
    unsigned char *oldFlags;
    unsigned int i;
    int q;

    oldFlags = (unsigned char*) malloc(this->nv);
    for(i = 0; i<this->nv; i++)
    {
        oldFlags[i] = this->vf[i];
        this->vf[i] = (unsigned char) (CA_HASFLAG(i, CAV_MARKED) ? 0 : CAV_DELETED);
    }
    
    q = saveOff(filename);
    for(i = 0; i<this->nv; i++)
        this->vf[i] = oldFlags[i];

    free(oldFlags);
    return q;
}



int capModel::loadOff(const char *filename, int append, capColor *rgba)
{
    FILE *fh;
#define OFFLINE_MAX 512
    char line[OFFLINE_MAX + 1];
    unsigned int nf, nv, voffs = 0, foffs = 0;
    int vmax = 3, coffs = 255, noffs = 255;
    unsigned int i;

    append = this->loadStart(filename, append);

    if(!(fh = utf8open(this->fullpath, "r")))
        return capDebug::error("Cannot open file: %s", this->filename);

    /* Read header */
    if(!fgets(line, OFFLINE_MAX, fh))
    {
        fclose(fh);
        return capDebug::error("No header in file: %s", this->filename);
    }

    /* Reset normals/color flags */

    this->flags &= ~(CAMF_COLOR | CAMF_NORMALS);

    /* Test header */
    this->trim(line);
    if((!strcmp(line, "COFF")) || (!strcmp(line, "NCOFF")))
        this->flags |= CAMF_COLOR;
    else if((!strcmp(line, "OFF")) || (!strcmp(line, "NOFF")))
        this->flags &= ~CAMF_COLOR;
    else
    {
        fclose(fh);
        return capDebug::error("Wrong (C)OFF header in file: %s", this->filename);
    }

    if((!strcmp(line, "NCOFF")) || (!strcmp(line, "NOFF")))
        this->flags |= CAMF_NORMALS;


    if(this->flags & CAMF_NORMALS)
    {
        capDebug::report(1, "Will read vertex normals from this file");
        vmax += 3;
        noffs = 3;
    }
    if(this->flags & CAMF_COLOR)
    {
        capDebug::report(1, "Will read vertex colors from this file");
        vmax += 3;
        coffs = 3;
        noffs += 3;
    }

    /* Read the number of points & triangles */
    if(!fgets(line, OFFLINE_MAX, fh))
    {
        fclose(fh);
        return capDebug::error("No information on number of points/faces in file: %s", this->filename);
    }
    trim(line);
    if(sscanf(line, "%u %u ", &nv, &nf) != 2)
    {
        fclose(fh);
        return capDebug::error("No information on number of points/faces in file: %s", this->filename);
    }

    if(append)
    {
        voffs = this->nv;
        foffs = this->nt;
        free(this->ep); free(this->e);
        this->e = NULL; this->ep = NULL; 
    }
    else
    {
        capDebug::report(1, "Deleting old model data");
        this->unload();
    }

    /* Save info into the model struct & allocate data  */
    this->reallocBuffers(nv + voffs, nf + foffs);
    capDebug::report(1, "%s: Reading %d vertexes", this->filename, nv);

    for(i = 0; i<nv; i++)
    {
        float pos[3], normal[3];
        unsigned int color[3];
        char *eptr, *buf;
        int v;


        if(!fgets(line, OFFLINE_MAX, fh))
        {
            fclose(fh);
            this->unload();
            return capDebug::error("Premature end of file '%s', while reading vertex %d of %d", this->filename, i, nv);
        }

        buf = line;
        for(v = 0; v<vmax; v++)
        {
            if(v >= noffs)
                normal[v - noffs] = strtof(buf, &eptr);
            else if(v >= coffs)
                color[v - coffs] = (unsigned int) strtoul(buf, &eptr, 0);
            else
                pos[v] = strtof(buf, &eptr);

            if(eptr == buf)
            {
                fclose(fh);
                this->unload();
                return capDebug::error("Premature end of line in '%s', while reading vertex %d of %d", this->filename, i, nv);
            }

            buf = eptr;
        }

        this->vf[i + voffs] = 0;
        this->v[i + voffs].x = pos[0]; this->v[i + voffs].y = pos[1]; this->v[i + voffs].z = pos[2];
        this->v[i + voffs].isUsed = 0;
        if(this->flags & CAMF_COLOR)
        {
            this->v[i + voffs].r = (unsigned char) color[0];
            this->v[i + voffs].g = (unsigned char) color[1];
            this->v[i + voffs].b = (unsigned char) color[2];
        }

        if(this->flags & CAMF_NORMALS)
        {
            this->v[i + voffs].nx = normal[0];
            this->v[i + voffs].ny = normal[1];
            this->v[i + voffs].nz = normal[2];
        }
    }

    this->recolor(rgba, voffs);

    capDebug::report(2, "Loading %d faces...", nf);
    for(i = 0; i<nf; i++)
    {
        char *eptr, *buf;
        int v;
        unsigned int triangle[4];

        if(!fgets(line, OFFLINE_MAX, fh))
        {
            fclose(fh);
            this->unload();
            return capDebug::error( "Premature end of file in '%s', while reading face %d of %d", this->filename, i, nf);
        }


        buf = line;
        for(v = 0; v<4; v++)
        {
            triangle[v] = (unsigned int) strtoul(buf, &eptr, 0);

            if(eptr == buf)
            {
                fclose(fh);
                this->unload();
                return capDebug::error( "Premature end of line in '%s', while reading face %d of %d", this->filename, i, nf);
            }
            else if((v > 0 && (triangle[v] < 0 || triangle[v] >= nv)) || (v == 0 && triangle[v] != 3))
            {
                fclose(fh);
                this->unload();
                return capDebug::error( "Premature end of line in '%s', while reading face %d of %d", this->filename, i, nf);
            }
            buf = eptr;
        }

        this->t[i + foffs].a = triangle[1] + voffs;
        this->t[i + foffs].b = triangle[2] + voffs;
        this->t[i + foffs].c = triangle[3] + voffs;
    }

    fclose(fh);
    capDebug::report(1, "All data loaded.");

    return loadEnd();
}

void capModel::reallocBuffers(unsigned int nnv, unsigned int nnt) {
    size_t lsz;
    this->nv = nnv; this->nt =  nnt;


    free(this->ep); free(this->e);
    this->e = NULL; this->ep = NULL;

    capDebug::assert(this->v = (struct capVertex*) realloc(this->v, lsz = sizeof(struct capVertex)*this->nv));
    capDebug::report(1, "Allocated vertex buffer: %d MB", lsz / 1024 / 1024);

    if(this->nt)
    {
        capDebug::assert((this->t = (struct capTriangle*) realloc(this->t, lsz = sizeof(struct capTriangle)*this->nt)));
        capDebug::report(1, "Allocated triangle buffer: %d MB", lsz / 1024 / 1024);
    }
    else
        this->t = NULL;

    capDebug::assert((this->vf = (unsigned char*) realloc(this->vf, this->nv)));

}

int capModel::loadStart(const char* filename, int append)
{
    if(!append) {
        free(this->fullpath);
        this->fullpath = strdup(filename);
        this->filename = basename(this->fullpath);
    }

    if(append)
        if(!(this->flags & CAMF_LOADED))
            append = 0;

    return append;
}
int capModel::loadEnd() {
    if((nt == 0) && !(this->flags & CAMF_POINTS))
        this->flags |= CAMF_POINTS;

    if(nt && !(this->flags & (CAMF_EDGES | CAMF_FACES)))
        this->flags |= CAMF_FACES;
    
    this->flags |= CAMF_LOADED;
    this->renderer->reinitModel(this);
    return 1;
}

int capModel::loadFile(const char *filename, int append, capColor *rgba) {
    if(strlen(filename) >= 4) {
        if (!strcasecmp(filename + strlen(filename) - 4, ".ply"))
            return loadPly(filename, append, rgba);
        if (!strcasecmp(filename + strlen(filename) - 4, ".nvm"))
            return loadNvm(filename, append, rgba);
        if (!strcasecmp(filename + strlen(filename) - 3, ".3d"))
            return load3d(filename, append, rgba);
    }
    return loadOff(filename, append, rgba);
}
