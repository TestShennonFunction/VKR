#include <iostream>
#include <fstream>
#include <string.h>
#define __STDC_FORMAT_MACROS 1
#include "cstdint"
#include <inttypes.h>

#include "settings.h"
#include "tcommon.h"
#include "combinations.h"
#include "init.h"
#include "alltest.h"
#include "utils.h"


#ifndef DebugPrint
#define Parallel
#endif

#ifdef Parallel
#include "omp.h"
#define MAX 8
#endif // Parallel

using namespace std;


uint64_t stat[64] = {};
unsigned totalstat = 0;

// проверка набора тестов для всех функций и всех перестановок пар переменных
int stattest(uint64_t testvector)
{
    uint64_t fn2;
    // пробег по всем функциям кроме 0 и 1
//    for (uint64_t fn = 1; fn <= max_function; fn++) {
    for (uint64_t fn = 2; fn <= max_function; fn+=2) {
        // пробег по перестановкам переменных
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение нового вектора значений лля перестановки
            fn2 = GVector(fn, &FTOGmatr[k][0]);
            if (fn != fn2) { // если вектор значений изменился
                // проверяем биты значений, соответствующие набору тестов
                if (((fn2 ^ fn) & testvector) == 0) {
                    return 0; // дальше не проверяем, одного провала достаточно
                }
            }
        }
    }
    totalstat++;
    int k = 0;
    while (testvector != 0) {
        if (testvector & 1) {
            stat[k]++;
        }
        testvector >>= 1;
        k++;
    }
    // набор тестов выявляет различия для любой функции и перестановки любой пары переменных
    return 1;
}

int mainstat()
{
    time_t start_time, fin_time, start_time_it, fin_time_it;
    start_time = time(NULL);
    ShowLocalTime(start_time);
    InitArrays();

    uint64_t testvector;

    //Combs test;        // генератор комбинаций (наборов) тестов
    int oktestlen, oktest;
    uint64_t testi;
    uint64_t okmask = 0; // результат тестирования функций набором тестов
    for (uint64_t testlen = 50; testlen >= 50; testlen--) {
        uint64_t failed1 = 0, failed2 = 0;
        printf("testlen = %" PRIu64 "\n", testlen);
        start_time_it = time(NULL);
        oktestlen = 0;
        // инициалзация генератора наборов С(k,n)
        uint64_t *testp = PrepareCombinations6(testlen, &testcnt, num_tests);
        #ifdef Parallel
        #pragma omp parallel num_threads(MAX) private(testi) shared(okmask,stat,oktestlen)
        #endif // Parallel
        {
            #ifdef Parallel
            #pragma omp for schedule(static)
            #endif // Parallel
            for (testi = 0; testi < testcnt; testi++) {
                //if (!oktestlen) {
                    // обработка очередного набора тестов
                    testvector = testp[testi];
                    #ifdef DebugPrint
                        PrintTestSet(testvector);
                        printf("%12s %8s %10s %10s    %10s\n","F ","F2 ","F ^ F2","tst","(F^F2)&Tst");
                    #endif // DebugPrint
                    // проверка набора тестов для всех функций и перестановок переменных
                    oktest = quickchecktest(testvector);
                    /*if (oktest) {
                        oktest = checktest(testvector);
                        if (!oktest) {
                            failed2 ++;
                        }
                    } else {
                        failed1 ++;
                    }
                    */

                    if (oktest) {
                        // набор тестов подошел ко всем функциям, заканичиаем
cout << "+" << fun2string(testvector) << endl;
                        oktestlen++;
                        okmask = testvector;
    int k = 0;
    totalstat++;
    while (testvector != 0) {
        if (testvector & 1) {
            stat[k]++;
        }
        testvector >>= 1;
        k++;
    }
                    }
                //} // if (!oktestlen)
            } // for testi
        } // Parallel
        fin_time_it = time(NULL);

        printf("Total number of test sets %" PRIu64 ".", testcnt);
        printf (" Fails: %" PRIu64 " %" PRIu64 ".", failed1, failed2);
        ShowDifTime(" Iteration time:", fin_time_it - start_time_it);
        free(testp);
        if (oktestlen) {
            printf("Test set found. Set length = %" PRIu64 "\n", testlen);
            PrintTestSet(okmask);
        } else {
            printf("========== Test set NOT found. Set length = %" PRIu64 "\n", testlen);
            break;
        }
    } // for testlen
    for (unsigned i=1; i<=num_tests; i++) {
        //if (stat[i] != 0) {
            printf("%" PRIu64 " %u\n",stat[i],i);
        //}
    }
    printf("total=%u\n",totalstat);

    fin_time = time(NULL);
    ShowDifTime("Working time:", fin_time - start_time);
    return 0;
}

// проверка набора подтестов, получаемых из заданного теста
// цель либо показать, что уменьшение длины тестового набора невозможна
// либо найти набор меньшей длины
int checksubtests(uint64_t testvector1)
{
//    time_t start_time_it, fin_time_it;
    uint64_t testvector, fail = 0;
    unsigned count = 0;
    for (unsigned b = 0; b < num_values; b++) {
        if ((testvector1 & (1 << b)) == 0) {
            continue;
        }
        count++;
        testvector = testvector1 ^ (1 << b);
//        printf("check #%u ",count);
//        PrintTestSet(testvector);
//        printf("\n");
        //start_time_it = time(NULL);
        uint64_t fn2, failcount = 0, failfun;
        // пробег по всем функциям кроме 0 и 1
        //for (uint64_t fn = 2; fn <= max_function; fn+=2) {
        for (uint64_t fn = 2; fn <= max_function; fn<<=1) {
            failfun = 0;
            // пробег по перестановкам переменных
            for (uint64_t k = 0; k < num_pairs; k++) {
                // получение нового вектора значений лля перестановки
                fn2 = GVector(fn, &FTOGmatr[k][0]);
                if (fn != fn2) { // если вектор значений изменился
                    // проверяем биты значений, соответствующие набору тестов
                    if (((fn2 ^ fn) & testvector) == 0) {
                        // биты не изменились, набор тестов не выявляет различий
                        // бракованный набор
                        failcount++; // дальше не проверяем, одного провала достаточно
                        if (failcount == 1) {
                    #ifdef DebugPrint
                            printf("      fn = %s  fail on pair (%u,%u) ->\n",
                                    binfun(fn), pairs[k][0], pairs[k][1]);
                            printf("     fn2 = %s\n",
                                    binfun(fn2));
                    #endif
                        }
                        failfun++;
                        break;
                    }
                }  // if (fn != fn2)
                if (failfun) {
                    break;
                }
            } // for k < num_pairs
            if (failfun) {
                break;
            }
        } // for fn
        //fin_time_it = time(NULL);
        //ShowDifTime("",fin_time_it - start_time_it);
        if (failcount == 0) {
            // набор тестов выявляет различия для любой функции и перестановки любой пары переменных
            printf("test subset looks successfull\n");
            PrintTestSet(testvector);
        } else {
            fail++;
            //printf("test failed on %" PRIu64 " functions\n", failcount);
        }
    } // for b < num_values
    if (count == fail) {
                    #ifdef DebugPrint

        printf("all %u subsets are bad\n", count);
/*
        start_time_it = time(NULL);
        printf("checking original test set\n");
        int orig = checktest(testvector1);
        if (orig) {
            printf("original test set is ok\n");
            PrintTestSet(testvector1);
        } else {
            printf("original subset is bad\n");
        }
        fin_time_it = time(NULL);
        ShowDifTime("",fin_time_it - start_time_it);
*/
#endif
        return 0;
    } else {
        printf("good test subset is discovered\n");
        return 1;
    }
}

// скачивается наборы тестов и проверяются на длине минус 1
int CheckFile()
{
    InitArrays();

    uint64_t testvector;
    string line;
    ifstream myfile ("set50");
    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            //cout << line << '\n';
            testvector = bin2u(line.c_str()); // am5
            if (checksubtests(testvector)) {
                    printf("found!\n");
                    printf("checking subtests of %" PRIu64 "\n", testvector);
                    break;
            }
        }
        myfile.close();
    } else {
        cout << "Unable to open file";
    }
    return 0; 
}

// проверка конкретного тестового набора на всех функциях
int check_test_345()
{
//    time_t start_time, fin_time, start_time_it, fin_time_it;
//    start_time = time(NULL);
//    ShowLocalTime(start_time);
    InitArrays();

    uint64_t testvector = bin2u("0011111000111110");
        if (num_vars == 4) {
            //testvector = bin2u("0011110010111110"); // am4
              testvector = bin2u("0011111000111110"); // am4
        } else if (num_vars == 5) {
            testvector = bin2u("00111101101110101110111111111110"); // am5
            printf("checking subtests of %" PRIu64 "\n", testvector);
        }
        checksubtests(testvector);
        return 0;
}
