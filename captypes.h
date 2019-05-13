#ifndef CAPOC2_CAPTYPES_H
#define CAPOC2_CAPTYPES_H


class capGenericPoint {
public:
    float x = 0.0, y = 0.0, z = 0.0;
    capGenericPoint(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }
    capGenericPoint() {

    }
};

class capActivePoint : public capGenericPoint {
public:
    capActivePoint() {

    }
    bool valid = false;
};

struct capReprojParams
{
    float zMin = 0.0f, zMax = 0.0f, zTol = 0.01f;
    float dotMin = -1.0f, dotMax = 1.0f, minImprovement = 0.0f;
    float rasterRange = 1.0f;
#define CARF_CACHE_MAPS 1
    unsigned int flags;

};

#define CAP_PI 3.1415926535f
#define CAP_PI_2 (CAP_PI/2.0f)

#ifdef _MSC_VER 
#define strncasecmp _strnicmp
#define strcasecmp _stricmp

#include <string.h>

inline char *basename(char *f)
{
    char *lastDirSep = strrchr(f, '\\');
    return lastDirSep + 1;
}

#include <stdint.h>
#endif


#endif

