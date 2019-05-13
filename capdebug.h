//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPDEBUG_H
#define CAPOC2_CAPDEBUG_H


#define RING_ENTRY_LENGTH 256
#define RING_ENTRY_COUNT 1024

struct capRingEntry
{
    char entry[RING_ENTRY_LENGTH];
};

#define CA_ERROR_MAX 256
class capDebug {

public:
    static unsigned int debugLevel;
    static unsigned int ring_pos, ring_count;
    static struct capRingEntry ring[RING_ENTRY_COUNT];

    static char errorMessage[CA_ERROR_MAX];

    static int report(unsigned int severity, const char *msg, ...);
    static int error(const char *fmt, ...);
    static void assert(void *ptr);

    static int init();

    static size_t ringSize();
    static const char *ringGet(unsigned int n);
    static void ringInsert(const char *msg);
};


#endif //CAPOC2_CAPDEBUG_H

