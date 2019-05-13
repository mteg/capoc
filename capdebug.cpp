/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under CC BY-SA 4.0 license
 *
 */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "capdebug.h"

int capDebug::report(unsigned int level, const char *fmt, ...) {
    va_list ap;
    char msgBuffer[1024];
    if (level > capDebug::debugLevel) return 0;
    va_start(ap, fmt);
    vsnprintf(msgBuffer, 1023, fmt, ap);
    ringInsert(msgBuffer);
    va_end(ap);
    return 0;
}


int capDebug::error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(capDebug::errorMessage, CA_ERROR_MAX, fmt, ap);
    ringInsert(capDebug::errorMessage);
    va_end(ap);
    return 0;
}

void  capDebug::assert(void *ptr)
{
    if(!ptr)
        abort();
}


unsigned int capDebug::debugLevel;
char capDebug::errorMessage[CA_ERROR_MAX];
unsigned int capDebug::ring_pos,  capDebug::ring_count;
struct capRingEntry capDebug::ring[RING_ENTRY_COUNT];

int capDebug::init() {
    capDebug::debugLevel = 0;
    capDebug::errorMessage[0] = 0;
    capDebug::ring_pos = 0;
    capDebug::ring_count = 0;
    return 0;
}




size_t capDebug::ringSize() {
    return ring_count;
}

const char *capDebug::ringGet(unsigned int n) {
    return ring[(ring_pos - ring_count + n) & (RING_ENTRY_COUNT - 1)].entry;
}


void capDebug::ringInsert(const char *msg) {
    if(ring_count < RING_ENTRY_COUNT) ring_count++;
    strncpy(ring[ring_pos++ % (RING_ENTRY_COUNT - 1)].entry, msg, RING_ENTRY_LENGTH - 1);

}
