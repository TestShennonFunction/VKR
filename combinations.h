#ifndef combinations_h
#define combinations_h

#define __STDC_FORMAT_MACROS 1
#include "cstdint"
#include <inttypes.h>

struct Combs {
    uint64_t elements[64];
    int used;
    uint64_t limit;
    uint64_t cnt;
};

long long C(uint64_t k,uint64_t n);

void resetCombs(Combs *p, uint64_t k, uint64_t n);
int nextComb(Combs *p);

uint64_t * PrepareCombinations(uint64_t testlen, uint64_t *cnt, uint64_t numtests);
uint64_t * PrepareCombinations6(uint64_t testlen, uint64_t *cnt, uint64_t numtests);

uint64_t * AllocCombinations(uint64_t n);

#endif // combinations_h
