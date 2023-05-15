#include <iostream>
#include "combinations.h"
#include "settings.h"
#include "tcommon.h"

using namespace std;

long long C(uint64_t k,uint64_t n)
{
    if (k > n - k) {
        k = n - k;
    }
    long long m = 1;
    for (uint64_t i=0; i < k; i++) {
        m *= n - i;
        if (i>0) {
            m /= i + 1;
        }
    }
    return m;
}

void resetCombs(Combs *p, uint64_t k, uint64_t n) {
    for (uint64_t i = 0; i < k; i++) {
        p->elements[i] = i+1;
    }
    p -> used = k;
    p -> limit = n;
    p -> cnt = 0;
}

void printComb(Combs *p) {
    char tmp[20];
    printf("test (");
    for (int i=0; i < p->used; i++) {
        sprintf(tmp, " %" PRIu64 " ", p->elements[i]);
        printf("%s",tmp);
    }
    printf(")\n");
}

int nextComb(Combs *p)
{
    if (p -> cnt == 0) {
        p -> cnt++;
        return p -> cnt;
    }
    int i = p->used - 1;
    uint64_t limit = p->limit;
    while (i>=0 && p->elements[i] >= limit) {
        i--;
        limit--;
    }
    if (i == -1) {
        return 0;
    }
    p -> cnt++;
    p->elements[i] ++;
    for (i = i + 1; i < p->used ; ++i) {
        p->elements[i] = p->elements[i-1] + 1;
    }
    return p -> cnt;
}

uint64_t * AllocCombinations(uint64_t n)
{
    uint64_t size = n * sizeof(uint64_t);
    uint64_t *p = (uint64_t *) malloc(size);
    return p;
}

uint64_t * PrepareCombinations(uint64_t testlen, uint64_t *cnt, uint64_t numtests)
{
    //printf("prepare %u %u\n",testlen,numtests);
    long long m = C(testlen, numtests);
    if (m > (1 << 30)) {
        printf("too many combinations: %lld",m);
        exit(1);
    }
    uint64_t *p = AllocCombinations(m);
    uint64_t *result = p;
    uint64_t n = (uint64_t) m;
    *cnt = n;

    Combs test;        // генератор комбинаций (наборов) тестов
    resetCombs(&test, testlen, numtests);
    uint64_t testmask;

    while (nextComb(&test)) {
        testmask = 0;
        // формрование маски для набора тестов
        for (uint64_t e = 0; e < testlen; e++) {
            testmask |= (uint64_t)1 << test.elements[e];
         //printf("+%s \n",binstr(testmask,num_values));
        }
        *p = testmask;
        p++;
    }
    //printf("\n");
    if (p - result != m) {
        printf("Combinations generated: %lld; Expected: %" PRIu64 "; ", (long long) (p - result), n);
        exit(2);
    }
    return result;
}

uint64_t * PrepareCombinations6a(uint64_t testlen, uint64_t *cnt, uint64_t numtests)
{
    unsigned cut = 0;
    unsigned leftleft = 1, leftright = 13+1;    // ..left
    unsigned rightleft = 41,rightright = numtests - cut;  // right..
    unsigned changed = (leftright - leftleft + 1) + (rightright - rightleft + 1);
    unsigned midleft = leftright + 1, midright = rightleft - 1;
    unsigned known =  midright - midleft + 1;
    unsigned incr = rightleft - leftright - 1;
    numtests = changed;
    testlen -= known + cut;
    printf("combinations with: %u..%u + %u..%u (total %u); fixed %u..%u (total %u); incr=%u;\n" ,
             leftleft,leftright,rightleft,rightright,changed,midleft, midright, known, incr);

    uint64_t m = C(testlen, numtests);
    printf("prepare %" PRIu64 " of %" PRIu64 "; sets %" PRIu64 "\n",testlen,numtests, m);
    if (m > (1LL << 32)) {
        printf("too many combinations: %" PRIu64 "\n",m);
        exit(1);
    }
    uint64_t *p = AllocCombinations(m);
    uint64_t *result = p;
    uint64_t n = (uint64_t) m;
    *cnt = n;

    Combs test;        // генератор комбинаций (наборов) тестов
    resetCombs(&test, testlen, numtests);
//printComb(&test);
    uint64_t testmask;

    uint64_t ref6[64] = {};
    for (unsigned i = leftleft; i <= leftright; i++) {
        ref6[i] = i;
    }
    for (unsigned i = leftright+1; i <= numtests; i++) {
        ref6[i] = i + incr;
    }

//        7    34      21
//  0 1234567 8..41 42..62 63
//                   8..28
    uint64_t initmask = 0, one = 1LL << midleft;
    for (unsigned i = midleft; i <= midright; i++) {
        initmask |= one;
        one <<= 1;
    }

PrintArr(":",ref6, 64);
printComb(&test);
PrintTestSet(initmask);
    while (nextComb(&test)) {
        testmask = initmask;
        // формрование маски для набора тестов
        for (uint64_t e = 0; e < testlen; e++) {
            testmask |= 1LL << ref6[test.elements[e]];
        }
        //printf("%s ",binary(testmask,num_values));
//PrintTestSet(testmask);
        *p = testmask;
        p++;
    }
    //printf("\n");
    if ((uint64_t)(p - result) != m) {
        printf("Combinations generated: %" PRIu64 "; Expected: %" PRIu64 "; ", (uint64_t) (p - result), n);
        exit(2);
    }
    return result;
}

uint64_t * PrepareCombinations6(uint64_t testlen, uint64_t *cnt, uint64_t numtests)
{
//                      6         5         4         3         2         1
//                   3210987654321098765432109876543210987654321098765432109876543210
  //char mask[65] = "00.....................111111111111111111111111111.............0"; //52
//                   0011110110101110111110111111111111111111111111111101110010111110
//  char mask[65] = "00....................11111111111111111111111111...............0"; // 51


//        as vector: 0011110110101110111110111111111111111111111111110101110010111110
//    char mask[65] = "001.10.11.0....01.....1111111111111111111111111101.............0"; // 
//  char mask[65] = "00....................11111111111111111111111111...............0"; // 51
//  char mask[65] = "00.............................................................0"; // 56 start
//  char mask[65] = "001.1...1.......1...............1..............................0"; // 56 after
//  char mask[65] = "001.1...1....1..1...........1...1....11.....1..0...............0"; // 52 start
//  char mask[65] = "001.1...1....1..1...........1..11....11.....1..0...........1.110"; // 52 start2
    char mask[65] = "001.1...1....10.1...........1..11....11.....1..0....0..1...1.110"; // 52 after
//    char mask[65] = "001.1...1....10.1...........1..11....11...1.1..0....0..1...1.110"; // 50 after
//                   3210987654321098765432109876543210987654321098765432109876543210
    uint64_t testmask;
    uint64_t ref6[64] = {};
    uint64_t initmask = 0;
    unsigned known = 0;
    unsigned k = 0, changed = 0;


    for (unsigned i = 62; i > 0; i--) {
        k++;
        if (mask[i] == '.') {
            changed++;
            ref6[changed] = k;
        } else {
            if (mask[i] == '1') {
            known++;
                initmask |= (uint64_t) 1 << k;
            }
        }
    }

    printf("testlen %" PRIu64 " \n",testlen);
    numtests = changed;
    testlen -= known;
    printf("combinations with: %u %u\n" ,
             changed,known);

    uint64_t m = C(testlen, numtests);
    printf("prepare %" PRIu64 " of %" PRIu64 "; sets %" PRIu64 "\n",testlen,numtests, m);
    if (m > (1LL << 32)) {
        printf("too many combinations: %" PRIu64 "\n",m);
        exit(1);
    }
    uint64_t *p = AllocCombinations(m);
    uint64_t *result = p;
    uint64_t n = (uint64_t) m;
    *cnt = n;

    Combs test;        // генератор комбинаций (наборов) тестов
    resetCombs(&test, testlen, numtests);
//printComb(&test);

PrintTestSet(initmask);
PrintArr(":",ref6, 64);
printComb(&test);
    while (nextComb(&test)) {
        testmask = initmask;
        // формрование маски для набора тестов
        for (uint64_t e = 0; e < testlen; e++) {
            testmask |= 1LL << ref6[test.elements[e]];
        }
        //printf("%s ",binary(testmask,num_values));
//PrintTestSet(testmask);
        *p = testmask;
        p++;
    }
    //printf("\n");
    if ((uint64_t)(p - result) != m) {
        printf("Combinations generated: %" PRIu64 "; Expected: %" PRIu64 "; ", (uint64_t) (p - result), n);
        exit(2);
    }
    return result;
}
