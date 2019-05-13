/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#define _USE_MATH_DEFINES
#include <cmath>
#include "capengine.h"
#include "capdebug.h"
#include "nanojpeg.h"
#include "lodepng.h"

#define NVM_LINE_MAX 2048
#define NVM_CAMS_MAX 8192
#define NVM_POINTS_MAX 10000000
#define NVM_ALLOC_STEP 1024
#define NVM_PATH_MAX 2048

capNvm::capNvm(capRenderer *r, capModel *m) {
    this->renderer = r;
    this->model = m;
}

capNvm::~capNvm()
{
    unsigned int i;

    free(this->filename);
    free(this->dirname);
    if(this->cams)
        for(i = 0; i<this->n_cams; i++) {
            delete this->cams[i].rot;
            free(this->cams[i].filename);
        }

    free(this->camProjs);

    free(this->cams);
    free(this->points);
    free(this->projs);
}

int capNvm::error(const char *file, const char *error, FILE *fh)
{
    fclose(fh);
    return capDebug::error("Cannot load %s: %s", file, error);
}


int capNvm::errorStr(const char *file, const char *error, char *str, FILE *fh)
{
    fclose(fh);
    return capDebug::error("Cannot load %s: %s (%s)", file, error, str);
}


int capNvm::errorNum(const char *file, const char *error, int num, FILE *fh)
{
    fclose(fh);
    return capDebug::error("Cannot load %s: %s (%d)", file, error, num);
}


void capNvm::trim(char *line)
{
    char *eol;
    if((eol = strchr(line, '\r'))) *eol = 0;
    if((eol = strchr(line, '\n'))) *eol = 0;
}


char *capNvm::fgets(char *line, FILE *fh)
{
    while(::fgets(line, NVM_LINE_MAX, fh))
    {
        this->trim(line);
        if(strlen(line))
            return(line);
    }

    return NULL;
}

int capNvm::skipToModel(FILE *fh, int modelIndex)
{
    char lineBuf[NVM_LINE_MAX + 10];
    int i;

    for(i = 0; i<modelIndex; i++)
    {
        int sect;

        /* Two sections to skip: cameras & points */
        for(sect = 0; sect<2; sect++)
        {
            char *eptr, *line = this->fgets(lineBuf, fh);
            unsigned int nToSkip, l;

            if(!line) return 0;

            nToSkip = (unsigned int) strtoul(line, &eptr, 0);
            if(*eptr) return 0;

            for(l = 0; l<nToSkip; l++)
                if(!this->fgets(line, fh))
                    return 0;
        }
    }
    return 1;

}


void capNvm::assert(void *m)
{
    if(!m) abort();
}

int capNvm::load(const char *filename, int modelIndex)
{
    char *lastSlash;
    FILE *fh;
    char line[NVM_LINE_MAX + 10];
    char *dataLine, *eptr;
    unsigned int i, projsAllocated = 0, projCounter = 0;

    capDebug::report(1, "Opening NVM file %s for loading", filename);
    fh = utf8open(filename, "r");
    if(!fh)
        return capDebug::error("Cannot open %s: %s", filename, strerror(errno));

    this->assert((this->filename = strdup(filename)));
    this->assert((this->dirname = strdup(filename)));

#ifdef WINDOWS
    if ((lastSlash = strrchr(this->dirname, '\\')))
            *lastSlash = 0;
    else
#endif
    if((lastSlash = strrchr(this->dirname, '/')))
        *lastSlash = 0;
    else
    {
        this->dirname[0] = '.';
        this->dirname[1] = 0;
    }

    if(!::fgets(line, NVM_LINE_MAX, fh))
        return this->error(filename, "Cannot read header", fh);

    capDebug::report(2, "Skipping to model %d", modelIndex);
    if(!this->skipToModel(fh, modelIndex))
        return this->error(filename, "Model index past EOF", fh);

    if(!(dataLine = this->fgets(line, fh)))
        return this->error(filename, "Premature end of file while reading camera count", fh);

    this->n_cams = (unsigned int) strtoul(line, &eptr, 0);
    if(*eptr)
        return this->errorStr(filename, "Bogus data past camera count", eptr, fh);
    if(this->n_cams == 0 || this->n_cams > NVM_CAMS_MAX)
        return this->errorStr(filename, "Suspicious camera count", line, fh);

    this->assert((this->cams = (struct capNvmCam*) malloc(sizeof(struct capNvmCam) * this->n_cams)));
    memset((void*) this->cams, 0, sizeof(struct capNvmCam) * this->n_cams);
    capDebug::report(2, "Reading %d cameras", this->n_cams);

    for(i = 0; i<this->n_cams; i++)
    {
        char *fname;
        float qw, qx, qy, qz, dest[3] =  {0, 0, 1.0};

        if(!(dataLine = this->fgets(line, fh)))
            return this->errorNum(filename, "Premature end of file while reading camera information", i, fh);

        dataLine = vPopstring(dataLine, &fname);
        dataLine = vPopfloat(dataLine, &this->cams[i].f);
        dataLine = vPopfloat(dataLine, &qw);
        dataLine = vPopfloat(dataLine, &qx);
        dataLine = vPopfloat(dataLine, &qy);
        dataLine = vPopfloat(dataLine, &qz);
        dataLine = vPopfloat(dataLine, &this->cams[i].x);
        dataLine = vPopfloat(dataLine, &this->cams[i].y);
        dataLine = vPopfloat(dataLine, &this->cams[i].z);
//        dataLine = vPopfloat(dataLine, &this->cams[i].r);

        if(!dataLine)
            return this->errorNum(filename, "Incomprehensible camera data", i, fh);

        auto *m = new capAffineMatrix;

        m->m[0] = 1 - 2*qy*qy - 2*qz*qz;
        m->m[1] = 2*qx*qy - 2*qz*qw;
        m->m[2] = 2*qx*qz + 2*qy*qw;

        m->m[4] = 2*qx*qy + 2*qz*qw;
        m->m[5] = 1 - 2*qx*qx - 2*qz*qz;
        m->m[6] = 2*qy*qz - 2*qx*qw;

        m->m[8] = 2*qx*qz - 2*qy*qw;
        m->m[9] = 2*qy*qz + 2*qx*qw;
        m->m[10] = 1 - 2*qx*qx - 2*qy*qy;
        
        m->transform(dest);
        this->cams[i].rot = m;

        auto *im = new capAffineMatrix(m);
        im->invert();
        this->cams[i].invRot = im;
        
        this->cams[i].dx = dest[0];
        this->cams[i].dy = dest[1];
        this->cams[i].dz = dest[2];

        this->assert((this->cams[i].filename = strdup(fname)));
        this->cams[i].nvm = this;
        this->cams[i].camIndex = i;


        capDebug::report(3, "Got camera %d: %s (%f, %f, %f)", i, fname, this->cams[i].x, this->cams[i].y, this->cams[i].z);
    }

    if(!(dataLine = this->fgets(line, fh)))
        return this->error(filename, "Premature end of file while reading point count", fh);

    this->n_points = (unsigned int) strtoul(line, &eptr, 0);
    if(*eptr)
        return this->errorStr(filename, "Bogus data past point count", eptr, fh);
    if(this->n_points == 0 || this->n_points > NVM_POINTS_MAX)
        return this->errorStr(filename, "Suspicious point count", line, fh);

    this->assert((this->points = (struct capNvmPoint*) malloc(sizeof(struct capNvmPoint) * this->n_points)));
    memset(this->points, 0, sizeof(struct capNvmPoint) * this->n_points);
    capDebug::report(2, "Reading %d points", this->n_points);
    for(i = 0; i<this->n_points; i++)
    {
        int projCount, junk, j;

        if(!(dataLine = this->fgets(line, fh)))
            return this->errorNum(filename, "Premature end of file while reading point information", i, fh);

        /* XYZ */
        dataLine = vPopfloat(dataLine, &this->points[i].x);
        dataLine = vPopfloat(dataLine, &this->points[i].y);
        dataLine = vPopfloat(dataLine, &this->points[i].z);

        /* RGB */
        dataLine = vPopbyte(dataLine, &this->points[i].r);
        dataLine = vPopbyte(dataLine, &this->points[i].g);
        dataLine = vPopbyte(dataLine, &this->points[i].b);

        dataLine = vPopint(dataLine, &projCount);

        if(!dataLine)
            return this->errorNum(filename, "Incomprehensible point data - cannot get measurement count", i, fh);

        if((this->n_projs + projCount) >= projsAllocated)
        {
            /* Need more space for projections */
            while((this->n_projs + projCount) >= projsAllocated)
                projsAllocated += NVM_ALLOC_STEP;

            this->assert((this->projs = (struct capNvmProj*) realloc(this->projs, sizeof(struct capNvmProj) * projsAllocated)));
        }

        this->points[i].projCount = (unsigned int) projCount;
        this->points[i].firstProj = this->n_projs;
//        capDebug::report(4, "Retrieving %d projections for point %d, dataLine = %s\n", projCount, i, dataLine);

        for(j = 0; j<projCount; j++)
        {
            int camIndex;
            float x, y;
            dataLine = vPopint(dataLine, &camIndex);
            dataLine = vPopint(dataLine, &junk);
            dataLine = vPopfloat(dataLine, &x);
            dataLine = vPopfloat(dataLine, &y);

            if(!dataLine)
                return this->errorNum(filename, "Incomprehensible point data - cannot retrieve all measurements", i, fh);

            if(camIndex < 0 || camIndex >= (int) this->n_cams) {
                return this->errorNum(filename, "Incomprehensible point data - camera index out of range", i, fh);
            }

            this->projs[this->n_projs].x = x;
            this->projs[this->n_projs].y = y;
            this->projs[this->n_projs].pointIndex = i;
            this->projs[this->n_projs].camIndex = (unsigned int) camIndex;
            this->n_projs++;
        }
        capDebug::report(3, "Got point %d: (%f, %f, %f) w/%d measurements", i, this->points[i].x, this->points[i].y, this->points[i].z, projCount);
    }

    this->assert((this->camProjs = (unsigned int*) malloc(sizeof(unsigned int) * this->n_projs)));
    this->assert((this->camIndex = (unsigned int*) malloc(sizeof(unsigned int) * this->n_cams)));

    capDebug::report(2, "Generating camProjs by camera");

    /* Count # of projections for each camera */
    for(i = 0; i<this->n_projs; i++)
        this->cams[this->projs[i].camIndex].projCount++;

    /* Determine where buffer space for each camera starts and the clear number of projections */
    for(i = 0; i<this->n_cams; i++)
    {
        this->cams[i].firstProj = projCounter;
        projCounter += this->cams[i].projCount;
        this->cams[i].projCount = 0;
        this->camIndex[i] = i;
    }

    /* Insert projections into camera index */
    for(i = 0; i<this->n_projs; i++)
        this->camProjs[this->cams[this->projs[i].camIndex].firstProj + this->cams[this->projs[i].camIndex].projCount++] = i;

    capDebug::report(1, "NVM loaded");
    return 1;
}

struct capNvmCam *capNvm::findCam(const char *camName)
{
    char *eptr;
    unsigned int camIndex;
    unsigned int i;

    if(!strlen(camName)) return NULL;
    camIndex = (unsigned int) strtoul(camName, &eptr, 0);
    if(!*eptr)
        if(camIndex >= 0 && camIndex < this->n_cams)
            return &this->cams[camIndex];

    for(i = 0; i<this->n_cams; i++)
        if(!strcmp(camName, this->cams[i].filename))
            return &this->cams[i];

    return NULL;
}

static capNvm * _qsort_nvm = NULL;

static int capNvmDepthCmp(const void *va, const void *vb)
{
    capNvm * nvm = _qsort_nvm;
    unsigned int a = *((unsigned int*) va);
    unsigned int b = *((unsigned int*) vb);

/*    if(nvm->cams[a].depth > 0 && nvm->cams[b].depth < 0)
        return -1;
    if(nvm->cams[a].depth < 0 && nvm->cams[b].depth > 0)
        return 1;
    if(nvm->cams[a].depth < 0 && nvm->cams[b].depth < 0)
    {
        float sgn = nvm->cams[b].depth - nvm->cams[a].depth;
        if(sgn < 0) return -1;
        if(sgn > 0) return 1;
        return 0;
    }*/

    float s = nvm->cams[a].depth - nvm->cams[b].depth;
    if(s < 0.0f) return -1;
    if(s > 0.0f) return 1;
    return 0;

}

static int capNvmProjCmp(const void *va, const void *vb)
{
    unsigned int a = *((unsigned int*) va);
    unsigned int b = *((unsigned int*) vb);
    capNvm * nvm = _qsort_nvm;

    return nvm->cams[b].seenFeatures - nvm->cams[a].seenFeatures;
}

static int capNvmNameCmp(const void *va, const void *vb)
{
    unsigned int a = *((unsigned int*) va);
    unsigned int b = *((unsigned int*) vb);
    capNvm * nvm = _qsort_nvm;

    return strcmp(nvm->cams[a].filename, nvm->cams[b].filename);
}

static int capNvmCamindexCmp(const void *va, const void *vb)
{
    unsigned int a = *((unsigned int*) va);
    unsigned int b = *((unsigned int*) vb);
    capNvm * nvm = _qsort_nvm;

    return nvm->cams[b].camIndex -  nvm->cams[a].camIndex;
}

void capNvm::orderByDepth(float offs, bool absDepth)
{
    unsigned int i;
    for(i = 0; i<this->n_cams; i++)
    {
        float vec[3] = {this->cams[i].x, this->cams[i].y, this->cams[i].z};
        this->model->modelviewMatrix.transform(vec);
        this->cams[i].depth =  offs - vec[2];
    }

    if(absDepth)
        for(i = 0; i<this->n_cams; i++)
            this->cams[i].depth = fabsf(this->cams[i].depth);

    _qsort_nvm = this;
    qsort(this->camIndex, this->n_cams, sizeof(unsigned int), capNvmDepthCmp);
}

void capNvm::orderByDepth()
{
    this->orderByDepth(0.0);
}

void capNvm::orderByDistance(float offs)
{
    orderByDepth(offs, true);
}

struct capNvmCam *capNvm::stepCam(int offsetFromCurrent)
{
    unsigned int i, pos = 0;
    if(this->activeCam != -1)
        for(i = 0; i<this->n_cams; i++)
            if(this->activeCam == (int) this->camIndex[i])
            {
                pos = i;
                break;
            }

    capDebug::report(3, "go to cam %d + %d = %d", pos, offsetFromCurrent, (pos + offsetFromCurrent) % this->n_cams);

    while((pos + offsetFromCurrent) < 0)
        pos = this->n_cams + pos;

    return &this->cams[this->camIndex[(pos + offsetFromCurrent) % this->n_cams]];
}

unsigned char * capNvm::loadJpeg(struct capNvmCam *m, struct capNvmPixsize *pixSize, bool pow2Align)
{
	unsigned int width, height, bufwidth, bufheight, x, y, size;
	unsigned char *rgbData, *rgbaData, *pRgb, *pRgba;
	unsigned char *jpegData= NULL;
	char texFile[NVM_PATH_MAX + 16];
	FILE *fh;

#ifdef WINDOWS
	snprintf(texFile, NVM_PATH_MAX, "%s\\%s", this->dirname, m->filename);
#else
    if(m->filename[0] == '/')
        snprintf(texFile, NVM_PATH_MAX, "%s", m->filename);
    else
        snprintf(texFile, NVM_PATH_MAX, "%s/%s", this->dirname, m->filename);
#endif

    capDebug::report(2, "Loading texture %s", texFile);
    fh = utf8open(texFile, "rb");
    if (!fh) {
        capDebug::report(0, "Error accessing %s", texFile);
        return NULL;
    }
    fseek(fh, 0, SEEK_END);
    size = (unsigned int) ftell(fh);

    if(size)
    {
        this->assert((jpegData = (unsigned char*) malloc((size_t) size)));
        fseek(fh, 0, SEEK_SET);
        size = (int) fread(jpegData, 1, (size_t) size, fh);
    }

    fclose(fh);
    if(!size)
    {
        free(jpegData);
        capDebug::report(0, "File size is 0 while opening %s", texFile);
        return NULL;
    }
    Jpeg::Decoder decoder(jpegData, (size_t) size);


    if(decoder.GetResult() != Jpeg::Decoder::OK)
    {
        free(jpegData);
        capDebug::report(0, "Error while decoding JPEG %s", texFile);
        return NULL;
    }

    if(!decoder.IsColor())
    {
        free(jpegData);
        capDebug::report(0, "Error while decoding %s, no color found", texFile);
        return NULL;
    }


    bufwidth = width = decoder.GetWidth();
    bufheight = height = decoder.GetHeight();

    if(pow2Align)
    {
        bufwidth = 1; while(bufwidth < width) bufwidth *= 2;
        bufheight = 1; while(bufheight < height) bufheight *=2;
    }

    this->assert((rgbData = decoder.GetImage()));
    this->assert((rgbaData = (unsigned char*) malloc((size_t) bufwidth*bufheight*4)));
    capDebug::report(1, "Loading texture %dx%d into buffer %dx%d", width, height, bufwidth, bufheight);
    pRgb = rgbData; pRgba = rgbaData;
    for(y = 0; y<bufheight; y++) {
        for (x = 0; x<bufwidth; x++) {
            pRgba[0] = pRgb[0];
            pRgba[1] = pRgb[1];
            pRgba[2] = pRgb[2];
            pRgba[3] = 200; // todo implement adjustment
            if(x < width && y < height)
                pRgb += 3;
            pRgba += 4;
        }
    }

    free(jpegData);

    m->width = width; m->height = height;
    pixSize->width = width;
    pixSize->height = height;
    pixSize->bufwidth = bufwidth;
    pixSize->bufheight = bufheight;
    pixSize->fw = ((float) width)/((float) bufwidth);
    pixSize->fh = ((float) height)/((float) bufheight);

    return rgbaData;
}


static inline int in_range(float x1, float y1, float x2, float y2, float range)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return (dx*dx + dy*dy) < (range*range);
}



void capNvm::stepMatch(struct capNvmCam *m, capGenericPoint &cursor, float range, int offset)
{
    unsigned int i;

    for(i = 0; i<m->projCount; i++)
    {
        int p;
        m->activeProj = (m->activeProj + offset) % m->projCount;
        p = this->camProjs[(m->activeProj % m->projCount) + m->firstProj];

        float ix = projs[p].x / m->width * 2.0f - 1.0f, iy = projs[p].y / m->height * 2.0f - 1.0f;
        if(in_range(ix, iy, cursor.x, cursor.y, range))
        {
            int pi = this->projs[p].pointIndex;
            model->cursor.valid = true;
            model->cursor.x = this->points[pi].x;
            model->cursor.y = this->points[pi].y;
            model->cursor.z = this->points[pi].z;
            capDebug::report(2, "Switching to projection: %f %f %f", this->points[pi].x, this->points[pi].y, this->points[pi].z);
            return;
        }
    }
}

static inline void projv(capModel *m, int i, const struct capNvmCam *cam, float x0, float y0, float z0, float f, float fwidth, float fheight, float *xis, float *yis, float *zs)
{
    float vec[3] = {m->v[i].x - x0, m->v[i].y - y0, m->v[i].z - z0};

    cam->invRot->transform(vec);

    if(vec[2] != 0.0)
    {
        *xis = vec[0] * f / vec[2] + fwidth;
        *yis = vec[1] * f / vec[2] + fheight;
    }
    *zs = vec[2];
}

struct tp
{
    float x;
    float y;
    float z;
};

static inline void fillmap(float *map, int w, int h, int y, float x1, float x2, float z1, float z2)
{
    int x, xa, xb;
//    fprintf(stderr, "fillmap %f ... %f with %f ... %f\n", x1, x2, z1, z2);
    if(x1 < 0 && x2 < 0) return;
    if(x1 >= ((float) w) && x2 >= ((float) w)) return;
    xa = (int) lrintf(x1); xb = (int) lrintf(x2);
    if(xa >= w) xa = w - 1;
    if(xb >= w) xb = w - 1;
    if(xa < 0) xa = 0;
    if(xb < 0) xb = 0;

    map += y*w + xa;

    if(fabsf(x2 - x1) < 1.0)
    {
        float z = (z1+z2)/2.0f;
        if(z < *map) *map = z;
        return;
    }

    for(x = xa; x <= xb; x++, map++)
    {
        float z = z1 + (z2 - z1) * (((float) x) - x1) / (x2 - x1);
        if(z < *map) *map = z;
    }
}

static inline void trtb(const struct tp* v1, const struct tp* v2, const struct tp* v3, float *map, int width, int height)
{
    struct tp v4;
    float scanStart, scanEnd;
    int yStart, yEnd, y;

//    fprintf(stderr, "draw triangle: %f %f %f -- %f %f %f -- %f %f %f\n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, v3->x, v3->y, v3->z);

    if(v3->y == v1->y) return;

    v4.x = v1->x + ((v2->y - v1->y) / (v3->y - v1->y)) * (v3->x - v1->x);
    v4.y = v2->y;
    v4.z = v1->z + ((v2->y - v1->y) / (v3->y - v1->y)) * (v3->z - v1->z);

    scanStart = v1->y; if(scanStart < 0.0) scanStart = 0; if(scanStart > ((float) height)) scanStart = height;
    scanEnd = v2->y; if(scanEnd < 0.0) scanEnd = 0; if(scanEnd > ((float) height)) scanEnd = height;
    yStart = (int) lrintf(scanStart); yEnd = (int) lrintf(scanEnd);  if(yEnd >= height) yEnd = height - 1;
//    fprintf(stderr, "v4 = %f %f %f\n", v4.x, v4.y, v4.z);
//    fprintf(stderr, "y = %d ... %d\n", yStart, yEnd);

    if(fabsf(v2->y -  v1->y) >= 0.5 && (v2->y >= 0.0 || v1->y >=  0.0) && (v2->y <= ((float) height - 1) || (v1->y <= ((float) height -1))))
        for(y = yStart; y<=yEnd; y++)
        {
            float t =  (((float) y) - v1->y) / (v2->y - v1->y);
            float xStart = v1->x + (v2->x - v1->x) * t;
            float xEnd  = v1->x + (v4.x - v1->x) * t;
            float zStart = v1->z + (v2->z - v1->z) * t;
            float zEnd = v1->z + (v4.z - v1->z) * t;

#if 0
            if(zStart < 0 || zEnd < 0)
            {
                printf("? y = %d (%d ... %d: %f ... %f), t = %f, x = %f ... %f, z = %f ... %f (\n", y, yStart, yEnd, v1->y, v2->y, t, xStart, xEnd, zStart, zEnd);
            }
#endif

            if(xStart <= xEnd)
                fillmap(map, width, height, y, xStart, xEnd, zStart, zEnd);
            else
                fillmap(map, width, height, y, xEnd, xStart, zEnd, zStart);
        }
    scanStart = v3->y; if(scanStart < 0.0) scanStart = 0; if(scanStart > ((float) height)) scanStart = height;
    yStart = (int) lrintf(scanStart); yEnd = (int) lrintf(scanEnd);  if(yEnd >= height) yEnd = height - 1; if(yStart >= height) yStart = height - 1;
//    fprintf(stderr, "y = %d ... %d\n", yStart, yEnd);
    if(fabsf(v3->y - v2->y) >= 0.5  && (v2->y >= 0.0  || v3->y >= 0.0 ) && (v2->y <= ((float) height -1) || v3->y  <= ((float) height -1)))
        for(y = yStart; y>=yEnd; y--)
        {
            float t =  (((float) y) - v2->y) / (v3->y - v2->y);
            float xStart = v2->x + (v3->x - v2->x) * t;
            float xEnd  = v4.x + (v3->x - v4.x) * t;
            float zStart = v2->z + (v3->z - v2->z) * t;
            float zEnd  = v4.z + (v3->z - v4.z) * t;
            if(xStart <= xEnd)
                fillmap(map, width, height, y, xStart, xEnd, zStart, zEnd);
            else
                fillmap(map, width, height, y, xEnd, xStart, zEnd, zStart);
        }
}

int capNvm::reprojectPoint(struct capNvmCam *cam, float x, float y, float z, int *xis, int *yis, float *fxis, float *fyis)
{
    float xp, yp, zp;

    int xi, yi, width = 0, height = 0;
    float fwidth, fheight, fxi, fyi;

    if(cam->width == 0.0f) {
        struct capNvmPixsize ps;
        free(this->loadJpeg(cam, &ps, false));
        width = ps.width; height = ps.height;
    }
    else
    {
        width = (int) lrintf(cam->width);
        height = (int) lrintf(cam->height);
    }

    if(!width) return 0;

    fwidth = width; fheight = height;
    fwidth /= 2.0; fheight /= 2.0;

    float vec[3] = {x - cam->x, y - cam->y, z - cam->z};

    /* Quickly invert rotation matrix */
    cam->invRot->transform(vec);
    xp = vec[0]; yp = vec[1]; zp = vec[2];

    if(zp <= 0.0) return 0;

    xi = (int) lrintf(xp * cam->f / zp + fwidth);
    yi = (int) lrintf(yp * cam->f / zp + fheight);

    fxi = ((float) (xi - width/2)) / fwidth;
    fyi = ((float) (yi - height/2)) / fheight;

    if(xis) *xis = xi;
    if(yis) *yis = yi;
    if(fxis) *fxis = fxi;
    if(fyis) *fyis = fyi;

    return 1;
}

static inline void markUnprojectable(capModel *m, int i, int toDo, unsigned char gray)
{
    if(toDo & CAR_DEL_UNPROJ)
        CA_FLAG_A(m->vf, i, CAV_DELETED);
    else if(toDo & CAR_GRY_UNPROJ)
    {
        m->v[i].r = gray;
        m->v[i].g = gray;
        m->v[i].b = gray;
    }
}

float *capNvm::obtainDepthMap(struct capNvmCam *cam, int pixcnt, int *toDo)
{
    float *depthMap;
    int i;
#if 0
    char depthFilename[NVM_PATH_MAX+16];
    int fd;

    if(capr->flags &  CARF_CACHE_MAPS)
    {
        if(*toDo & CAR_NO_DEPTHMAP)
      	    snprintf(depthFilename, NVM_PATH_MAX, "%s/generic.map", cam->this->dirname);
        else
      	    snprintf(depthFilename, NVM_PATH_MAX, "%s/%s.map", cam->this->dirname,  cam->filename);

        fd  = open(depthFilename, O_RDONLY);
        if(fd != -1)
        {
            float *ret;
            *toDo |= CAR_NO_DEPTHMAP;
            capDebug::report(1, "Reading map %s from cache", depthFilename);
            ret =  mmap(NULL, pixcnt*sizeof(float), PROT_READ, MAP_SHARED, fd, 0);
            close(fd);
            return ret;
        }
    }
#endif
    this->assert((depthMap = (float*) malloc(sizeof(float) * pixcnt)));

    for(i = 0; i<pixcnt; i++)
        depthMap[i] = 9999999.0f;

#if 0
    if(capr->flags &  CARF_CACHE_MAPS)
    {
        float *ret;
        fd  = open(depthFilename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        if(fd == -1)
        {
            fprintf(stderr, "Cannot open(%s) for cache, %s. Disabling cache.\n",  depthFilename, strerror(errno));
            capr->flags &= ~CARF_CACHE_MAPS;
            return depthMap;
        }

        if(write(fd, depthMap, sizeof(float)*pixcnt) != (sizeof(float)*pixcnt))
        {
            fprintf(stderr, "Cannot write(%s, %d) for cache, %s. Disabling cache.\n",  depthFilename, sizeof(float)*pixcnt, strerror(errno));
            capr->flags &= ~CARF_CACHE_MAPS;
            close(fd);
            unlink(depthFilename);
            return depthMap;
        }
        free(depthMap);
        capDebug::report(1, "Saving map %s to cache", depthFilename);
        ret =  mmap(NULL, pixcnt*sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        return ret;
    }
#endif
    return depthMap;
}

void capNvm::releaseDepthMap(struct capNvmCam *cam,  int pixcnt, float *depthMap)
{
#if 0
    if(capr->flags &  CARF_CACHE_MAPS)
    {
        munmap(depthMap, pixcnt);
        return;
    }
#endif
    free(depthMap);
}

void capNvm::reproject(capModel *m, capReprojParams *capr, struct capNvmCam *cam, int toDo, const char *savemap)
{
    int width = 0, height = 0, bestV = -1, trianglesDone = 0;
    unsigned int i, pixcnt;
    unsigned char *rgbData = NULL;
    float *depthMap;
    float fwidth, fheight;
    float xp, yp, zp;
    float x0 = cam->x, y0 = cam->y, z0 = cam->z, f = cam->f, qx, qy, qz, qlen;
    float bestIX, bestIY, bestDist = 144.0, bestZ = 9999999.0f, dzmax;
    float affineScale, decim;

    float ztol = capr->zTol, zmin = capr->zMin, zmax = capr->zMax;
    float dotmin = capr->dotMin, dotmax = capr->dotMax;

    std::vector <capGenericPoint> ipo, mpo;
    ipo.clear(); mpo.clear();

    
    affineScale = m->affineMatrix.getScale();

    zmax /= affineScale;
    zmin /= affineScale;
    ztol /= affineScale;
    decim = 0.1f / affineScale;
    capDebug::report(2, "Rescaled parameters for reprojection: z = %g ... %g +/- %g", zmin, zmax, ztol);
    dzmax = zmax+ztol;

    if(toDo & CAR_THROW) m->cursor.valid = false;

    if((toDo & (CAR_COLOR | CAR_DUMP)) || (cam->width == 0.0f))
    {
        struct capNvmPixsize ps;
        rgbData = this->loadJpeg(cam, &ps, false);
        width = ps.width; height = ps.height;
        if(!rgbData) return;
    }
    else
    {
        width = (int) lrintf(cam->width);
        height = (int) lrintf(cam->height);
    }

    depthMap = this->obtainDepthMap(cam, pixcnt = width*height, &toDo);

    fwidth = width; fheight = height;
    fwidth /= 2.0f; fheight /= 2.0f;

    cam->seenFeatures = 0;

    qx = cam->dx; qy = cam->dy; qz = cam->dz;
    qlen = sqrtf(qx*qx + qy*qy + qz*qz);
    qx /= qlen; qy /= qlen; qz /= qlen;

    if(!(toDo & CAR_NO_DEPTHMAP))
    {
        for(i = 0; i<m->nt; i++)
        {
            struct tp v[3];

            projv(m, m->t[i].a, cam, x0, y0, z0, f, fwidth, fheight, &v[0].x, &v[0].y, &v[0].z);
            projv(m, m->t[i].b, cam, x0, y0, z0, f, fwidth, fheight, &v[1].x, &v[1].y, &v[1].z);
            projv(m, m->t[i].c, cam, x0, y0, z0, f, fwidth, fheight, &v[2].x, &v[2].y, &v[2].z);

            if(zmax > 0.0)
                if((v[0].z > dzmax) && (v[1].z > dzmax) && (v[2].z > dzmax)) continue;

            if(v[0].z <= decim || v[1].z <= decim || v[2].z <= decim) continue;
            if(v[0].y < v[1].y && v[0].y < v[2].y)
                trtb(&v[0], v[1].y < v[2].y ? &v[1] : &v[2], v[1].y < v[2].y ? &v[2] : &v[1], depthMap, width, height);
            else if(v[1].y < v[2].y)
                trtb(&v[1], v[0].y < v[2].y ? &v[0] : &v[2], v[0].y < v[2].y ? &v[2] : &v[0], depthMap, width, height);
            else
                trtb(&v[2], v[0].y < v[1].y ? &v[0] : &v[1], v[0].y < v[1].y ? &v[1] : &v[0], depthMap, width, height);
            trianglesDone++;
        }
        capDebug::report(1, "Constructed depth map using %d triangles", trianglesDone);
    }


    for(i = 0; i<m->nv; i++)
    {
        int xi, yi, pp;
        float normalDot;

        if(toDo & CAR_ONLY_SELECTED)
            if(!CA_HASFLAG_A(m->vf, i, CAV_MARKED)) continue;

        float vec[3] = {m->v[i].x - x0, m->v[i].y - y0, m->v[i].z - z0};

        /*
        xp = cam->rot[0][0]*x + cam->rot[0][1]*y + cam->rot[0][2]*z;
        yp = cam->rot[1][0]*x + cam->rot[1][1]*y + cam->rot[1][2]*z;
        zp = cam->rot[2][0]*x + cam->rot[2][1]*y + cam->rot[2][2]*z;

        n->cams[i].rot[0][0] = 1 - 2*qy*qy - 2*qz*qz;   0
        n->cams[i].rot[0][1] = 2*qx*qy - 2*qz*qw;       1
        n->cams[i].rot[0][2] = 2*qx*qz + 2*qy*qw;       2

        n->cams[i].rot[1][0] = 2*qx*qy + 2*qz*qw;       4
        n->cams[i].rot[1][1] = 1 - 2*qx*qx - 2*qz*qz;   5
        n->cams[i].rot[1][2] = 2*qy*qz - 2*qx*qw;       6

        n->cams[i].rot[2][0] = 2*qx*qz - 2*qy*qw;       8
        n->cams[i].rot[2][1] = 2*qy*qz + 2*qx*qw;       9
        n->cams[i].rot[2][2] = 1 - 2*qx*qx - 2*qy*qy;   10

        m->m[0] = 1 - 2*qy*qy - 2*qz*qz;
        m->m[1] = 2*qx*qy - 2*qz*qw;
        m->m[2] = 2*qx*qz + 2*qy*qw;

        m->m[4] = 2*qx*qy + 2*qz*qw;
        m->m[5] = 1 - 2*qx*qx - 2*qz*qz;
        m->m[6] = 2*qy*qz - 2*qx*qw;

        m->m[8] = 2*qx*qz - 2*qy*qw;
        m->m[9] = 2*qy*qz + 2*qx*qw;
        m->m[10] = 1 - 2*qx*qx - 2*qy*qy;

        xp = m[0] * x + m[4] * y + m[8] * z + m[12];
        yp = m[1] * x + m[5] * y + m[9] * z + m[13];
        zp = m[2] * x + m[6] * y + m[10] * z + m[14];






        */


        cam->invRot->transform(vec);
        xp = vec[0]; yp = vec[1]; zp = vec[2];

        if(zp <= zmin)
        {
            markUnprojectable(m, i, toDo, 64);
            continue;
        }

        xi = (int) lrintf(xp * cam->f / zp + fwidth);
        yi = (int) lrintf(yp * cam->f / zp + fheight);

        if(xi < 0 || yi < 0 || xi >= width || yi >= height)
        {
            markUnprojectable(m, i, toDo, 96);
            continue;
        }


        if((zp > (depthMap[yi*width+xi]+ztol)) || (zp > zmax && zmax > 0.0f))
        {
            markUnprojectable(m, i, toDo, 128);
            continue;
        }

        if(toDo & CAR_THROW)
        {
            float fxi = ((float) (xi - width/2)) / fwidth;
            float fyi = ((float) (yi - height/2)) / fheight;
            float fdist = (cam->marker.x - fxi)*(cam->marker.x - fxi) + (cam->marker.y - fyi)*(cam->marker.y - fyi);
//            if(fdist<bestDist)
            if(in_range(fxi, fyi, cam->marker.x, cam->marker.y, capr->rasterRange)) {
                if (zp < bestZ) {
                    bestIX = fxi;
                    bestIY = fyi;
                    bestV = i;
                    bestDist = fdist;
                    bestZ = zp;
                }
                capGenericPoint ip(fxi, fyi, 0), mp(m->v[i].x, m->v[i].y, m->v[i].z);
                ipo.push_back(ip);
                mpo.push_back(mp);
                CA_FLAG_A(m->vf, i, CAV_MARKED);
            }

        }


        pp = (yi*width + xi)*4;
        normalDot = m->v[i].nx*qx + m->v[i].ny*qy + m->v[i].nz*qz;

        if(normalDot < dotmin || normalDot > dotmax)
        {
            markUnprojectable(m, i, toDo, 192);
            continue;
        }
        cam->seenFeatures ++;

        if(toDo & CAR_DUMP)
        {
            int r = rgbData[pp], g = rgbData[pp+1], b = rgbData[pp+2];
            capDebug::report(0, "%d %d %d %d %d %d %.3f %.3f", i, cam->camIndex, r, g, b, (r+r+b+g+g+g)/6, zp*affineScale, normalDot);
        }

        if(toDo & CAR_UND_COLOR)
            CA_UNFLAG_A(m->vf, i, CAV_DELETED);

        if(toDo & CAR_MARK)
            CA_FLAG_A(m->vf, i, CAV_MARKED);

        if(toDo & CAR_COLOR)
        {

            m->v[i].r = rgbData[pp];
            m->v[i].g = rgbData[pp+1];
            m->v[i].b = rgbData[pp+2];
        }

    }

    if(toDo & CAR_DUMP_SUMMARY)
        capDebug::report(0, "Cam %d file %s sees %d features", cam->camIndex, cam->filename, cam->seenFeatures);

    if(savemap)
    {
        int r = 0, y, x;
        if(zmax == 0.0)
            for(i = 0; i<pixcnt; i++)
                if(depthMap[i] > zmax && depthMap[i] < 9999999.0f)
                    zmax = depthMap[i];


        if(width > 0 && height > 0)
        {
            if(!rgbData)
                this->assert((rgbData = (unsigned char*) malloc(width * height * 4)));

            for(y = height - 1; y>=0; y--)
            {
                int yptr = y * width;
                for(x = 0; x<width; x++)
                {
                    int b = 0;
                    if(zmax > 0.0)
                    {
                        b = lrintf(depthMap[yptr + x] * 255.0 / zmax);
                        if(b < 0) b = 0;
                        if(b > 255) b = 255;
                    }
                    b = 255 - b;

                    rgbData[r++] = b;
                    rgbData[r++] = b;
                    rgbData[r++] = b;
                    rgbData[r++] = 255;
                }
            }
            lodepng::encode(savemap, rgbData, width, height);
        }
    }
    if(bestV != -1)
    {
        float v[3];
        capAffineMatrix *a = NULL;

        capDebug::report(2, "Best match is at pixel pos %.3f %.3f, dist %.2f, vert #%d at %f %f %f", bestIX, bestIY, sqrtf(bestDist), bestV, m->v[bestV].x, m->v[bestV].y, m->v[bestV].z);
        cam->bestV = bestV;
        if(ipo.size() >= 5)
            a = capAffineMatrix::findBetween(ipo, mpo);

        if(a)
        {
            float iv[3] = {cam->marker.x, cam->marker.y, 0};
            a->transform(iv);
            m->cursor.x = v[0] = iv[0];
            m->cursor.y = v[1] = iv[1];
            m->cursor.z = v[2] = iv[2];
            capDebug::report(2, "Reprojection obtained through SVD, interpolated to: %.2f %.2f %.2f", v[0], v[1], v[2]);
        }
        else {
            cam->marker.x = bestIX;
            cam->marker.y = bestIY;
            m->cursor.x = v[0] = m->v[bestV].x;
            m->cursor.y = v[1] = m->v[bestV].y;
            m->cursor.z = v[2] = m->v[bestV].z;
        }
        m->cursor.valid = true;

        m->affineMatrix.transform(v);
    }


    free(rgbData); this->releaseDepthMap(cam, pixcnt, depthMap);

    if(toDo & (CAR_COLOR | CAR_GRY_UNPROJ))
        m->flags |= CAMF_COLOR;

}

void capNvm::setActive(struct capNvmCam *cam)
{
    this->activeCam = cam->camIndex;
    this->activePtr = cam;
    this->renderer->reinitNvmCam(cam);
}


int capNvm::findCamsLookingIntoBall(float cx, float cy, float cz, float r, unsigned int *out)
{
    unsigned int i, p;
    if(out)
        memset(out, 0, sizeof(int) * this->n_cams);
    r = r*r;

    for(i = 0; i<this->n_cams; i++)
        this->cams[i].seenFeatures = 0;

    for(p = 0; p<this->n_points; p++)
    {
        float dist = (this->points[p].x - cx)*(this->points[p].x - cx) + (this->points[p].y - cy)*(this->points[p].y - cy) + (this->points[p].z - cz)*(this->points[p].z - cz);
        int f;
        if(dist > r) continue;

        f = this->points[p].firstProj;
        for(i = 0; i<this->points[p].projCount; i++)
        {
            int ci = this->projs[f + i].camIndex;
            if(out) out[ci]++;
            this->cams[ci].seenFeatures++;
        }
    }

    return 1;
}

void capNvm::orderBySeen()
{
    _qsort_nvm = this;
    qsort(this->camIndex, this->n_cams, sizeof(unsigned int), capNvmProjCmp);
    this->activeCam = -1;
    this->activePtr = NULL;
}

void capNvm::seeCam(capNvmCam *cam, capCamera *view) {
    float dir[3] = {0.0f, 0.0f, 2.0f};
    capAffineMatrix &am = cam->nvm->model->affineMatrix;
    cam->rot->transformVector(dir);

    float v[3] = {cam->x, cam->y, cam->z};
    am.transform(v);
    view->x = v[0]; view->y = v[1]; view->z = v[2];

    dir[0] += cam->x; dir[1] += cam->y; dir[2] += cam->z;

    cam->nvm->model->cursor.valid = true;
    cam->nvm->model->cursor.x = dir[0];
    cam->nvm->model->cursor.y = dir[1];
    cam->nvm->model->cursor.z = dir[2];

    am.transform(dir);

    /* yaw = xz plane */
    /* pitch = yz plane */
    view->yaw = atan2f(dir[2] - v[2], dir[0] - v[0]);
    if(view->yaw < 0.0f) view->yaw += CAP_PI*2;

    float totalHoriz = sqrtf(powf(dir[0]-v[0], 2.0f) + powf(dir[2]-v[2], 2.0f));

    view->pitch = atan2f(dir[1] - v[1], totalHoriz);
/*    c->cam.yaw = atan2(cz - dz, cx - dx);
    c->cam.pitch = atan2(cy - dy, cz - dz);*/

}

void capNvm::orderByName() {
    _qsort_nvm = this;
    qsort(this->camIndex, this->n_cams, sizeof(unsigned int), capNvmNameCmp);
    this->activeCam = -1;
    this->activePtr = NULL;
}

void capNvm::orderByCamindex() {
    _qsort_nvm = this;
    qsort(this->camIndex, this->n_cams, sizeof(unsigned int), capNvmCamindexCmp);
    this->activeCam = -1;
    this->activePtr = NULL;
}

void capNvm::resetSeen() {
    unsigned int i;
    for(i = 0; i<n_cams; i++)
        cams[i].seenFeatures = 0;
}

void capNvm::orderByDistance() {
    orderByDistance(0.0f);
}
