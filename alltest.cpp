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
#include "utils.h"
#include "alltest.h"

#ifndef DebugPrint
#define Parallel
#endif

#ifdef Parallel
#include "omp.h"
#define MAX 8
#endif // Parallel

using namespace std;

// проверка набора тестов testvector только для всех функций с одной единицей в векторе значений
int quickchecktest(uint64_t testvector)
{
    uint64_t G;
    // пробег по всем функциям кроме 0 и 1
    for (uint64_t F = 2; F <= max_function; F <<= 1) {
        // пробег по перестановкам переменных
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение нового вектора значений лля перестановки
            G = GVector(F, &FTOGmatr[k][0]);
            if (F != G) { // если вектор значений изменился
                #ifdef DebugPrintFun
                    if ((G ^ F) & testvector) {
//                        printf("  ok!\n");
                    } else {
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    cout << fun2string(testvector) << " ";
                    cout << fun2string((F^G) & testvector) << " ";
                    cout << "  failed!\n";
                    }
                #endif // DebugPrintFun
                // проверяем биты значений, соответствующие набору тестов
                if (((G ^ F) & testvector) == 0) {
                    // биты не изменились, набор тестов не выявляет различий
                    // бракованный набор
                    //#ifdef DebugPrint
                    //    printf("failure: F=%u; exchange count=%u\n", F, k); // отладочная печать
                    //#endif // DebugPrint
                    return 0; // дальше не проверяем, одного провала достаточно
                }
            }
            #ifdef DebugPrintFun1
            else {
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    cout << "  no differences, no test!\n";
            }
            #endif // DebugPrintFun
        }
    }
    // набор тестов выявляет различия для любой функции и перестановки любой пары переменных
    return 1;
}

// проверка набора тестов для всех функций и всех перестановок пар переменных
// полный дамп всех проверенных функций
int quickchecktest2(uint64_t testvector)
{
    uint64_t G;
    // пробег по всем функциям кроме 0 и 1
    for (uint64_t F = 2; F <= max_function; F <<= 1) {
        // пробег по перестановкам переменных
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение нового вектора значений лля перестановки
            G = GVector(F, &FTOGmatr[k][0]);
            if (F != G) { // если вектор значений изменился
                #ifdef DebugPrintFun
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    cout << fun2string(testvector) << " ";
                    cout << fun2string((F^G) & testvector) << " ";
                    if ((G ^ F) & testvector) {
                        printf("  ok!\n");
                    } else {
                        printf("  failed!\n");
                    }
                #endif // DebugPrintFun
                // проверяем биты значений, соответствующие набору тестов
                if (((G ^ F) & testvector) == 0) {
                    // биты не изменились, набор тестов не выявляет различий
                    // бракованный набор
                    //#ifdef DebugPrint
                    //    printf("failure: F=%u; exchange count=%u\n", F, k); // отладочная печать
                    //#endif // DebugPrint
                    return 0; // дальше не проверяем, одного провала достаточно
                }
            }
            #ifdef DebugPrintFun
            else {
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    printf("  no differences, no test!\n");
            }
            #endif // DebugPrintFun
        }
    }
//    }
    // набор тестов выявляет различия для любой функции и перестановки любой пары переменных
    return 1;
}


// проверка набора тестов для всех функций и всех перестановок пар переменных
int checktest(uint64_t testvector)
{
    uint64_t G;
    // пробег по всем функциям кроме 0 и 1
//    for (uint64_t F = 1; F <= max_function; F++) {
    for (uint64_t F = 2; F <= max_function; F+=2) {
        // пробег по перестановкам переменных
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение нового вектора значений лля перестановки
            G = GVector(F, &FTOGmatr[k][0]);
            if (F != G) { // если вектор значений изменился
                #ifdef DebugPrintFun
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    cout << fun2string(testvector) << " ";
                    cout << fun2string((F^G) & testvector) << " ";
                    if ((G ^ F) & testvector) {
                        printf("  ok!\n");
                    } else {
                        printf("  failed!\n");
                    }
                #endif // DebugPrintFun
                // проверяем биты значений, соответствующие набору тестов
                if (((G ^ F) & testvector) == 0) {
                    // биты не изменились, набор тестов не выявляет различий
                    // бракованный набор
                    //#ifdef DebugPrint
                    //    printf("failure: F=%u; exchange count=%u\n", F, k); // отладочная печать
                    //#endif // DebugPrint
                    return 0; // дальше не проверяем, одного провала достаточно
                }
            }
            #ifdef DebugPrintFun
            else {
                    printf("%2" PRIu64 ": ",k);
                    cout << fun2string(F) << " ";
                    cout << fun2string(G) << " ";
                    cout << fun2string(F^G) << " ";
                    printf("  no differences, no test!\n");
            }
            #endif // DebugPrintFun
        }
    }
    // набор тестов выявляет различия для любой функции и перестановки любой пары переменных
    return 1;
}

// функция поиска тестовых наборов для 3-4-5 переменных
//int allfuns_test_345()
int all_test(uint64_t StartLen, uint64_t FinLen, const char * FunctionsFile, const char * DumpFailures)
{
    cout << "Универсальный тест" << endl;
    time_t start_time, fin_time, start_time_it, fin_time_it;
    start_time = time(NULL);
    ShowLocalTime(start_time);
    InitArrays();

    uint64_t testvector;

    //Combs test;        // генератор комбинаций (наборов) тестов
    int oktestlen, oktest;
    uint64_t testi;
    uint64_t okmask; // результат тестирования функций набором тестов
    for (uint64_t testlen = StartLen; testlen <= FinLen; testlen++) {
        //uint64_t failed1 = 0, failed2 = 0;
        cout << SearchTestLenRu << testlen << endl;
        start_time_it = time(NULL);
        oktestlen = 0;
        // инициалзация генератора наборов С(k,n)
        uint64_t *testp = PrepareCombinations(testlen, &testcnt, num_tests);
        #ifdef Parallel
        #pragma omp parallel num_threads(MAX) private(testi) shared(okmask)
        #endif // Parallel
        {
            #ifdef Parallel
            #pragma omp for schedule(static)
            #endif // Parallel
            for (testi = 0; testi < testcnt; testi++) {
                if (!oktestlen) {
                    // обработка очередного набора тестов
                    testvector = testp[testi];
                    #ifdef DebugPrint
                        PrintTestSet(testvector);
                        printf("%12s %8s %11s %10s    %10s\n","F ","DMG"," F ^ DMG","tst","(F^DMG)&Tst");
                    #endif // DebugPrint
                    // проверка набора тестов для всех функций и перестановок переменных
                    //oktest = quickchecktest(testvector);
                    oktest = checktest(testvector);
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
                        oktestlen++;
                        okmask = testvector;
                    }
                } // if (!oktestlen)
            } // for testi
        } // Parallel
        fin_time_it = time(NULL);

        cout << NumberOfTestsRu << testcnt << ".";
        //printf (" Fails: %" PRIu64 " %" PRIu64 ".", failed1, failed2);
        ShowDifTime(IterationTimeRu, fin_time_it - start_time_it);
        free(testp);
        if (oktestlen) {
            cout << TestFoundRu << " " << TestLenRu << " = "<< testlen << endl;
            PrintTestSet(okmask);
            break;
        }
    } // for testlen
    fin_time = time(NULL);
    ShowDifTime(WorkingTimeRu, fin_time - start_time);
    return 0;
}

// функция сокращенного поиска тестовых наборов для 6 переменных
int allfuns_test_6()
{
    cout << "Универсальный тест" << endl;
    time_t start_time, fin_time, start_time_it, fin_time_it;
    start_time = time(NULL);
    ShowLocalTime(start_time);
    InitArrays();

    uint64_t testvector, onlycheck = 0;
    if (onlycheck) {
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

    //Combs test;        // генератор комбинаций (наборов) тестов
    uint64_t oktestlen, oktest;
    uint64_t testi;
    uint64_t okmask; // результат тестирования функций набором тестов
    for (uint64_t testlen = 52; testlen > 45; testlen--) {
        uint64_t failed1 = 0;
        printf("testlen = %" PRIu64 "\n", testlen);
        start_time_it = time(NULL);
        oktestlen = 0;
        // инициалзация генератора наборов С(k,n)
        uint64_t *testp = PrepareCombinations6(testlen, &testcnt, num_tests);
        printf("Total number of test sets %" PRIu64 ".", testcnt);
        #ifdef Parallel
        #pragma omp parallel num_threads(MAX) private(testi) shared(okmask)
        #endif // Parallel
        {
            #ifdef Parallel
            #pragma omp for schedule(static)
            #endif // Parallel
            for (testi = 0; testi < testcnt; testi++) {
                if (!oktestlen) {
                    // обработка очередного набора тестов
                    testvector = testp[testi];
                    #ifdef DebugPrint
                        PrintTestSet(testvector);
                        PrintTestSet(testvector);
                        printf("%12s %8s %10s %10s    %10s\n","F ","F2 ","F ^ F2","tst","(F^F2)&Tst");
                    #endif // DebugPrint
                    // проверка набора тестов для всех функций и перестановок переменных
                    oktest = quickchecktest(testvector);
                    if (!oktest) {
                        failed1 ++;
                    }

                    if (oktest) {
                        // набор тестов подошел ко всем функциям, заканичиаем
                        oktestlen++;
                        okmask = testvector;
                    }
                } // if (!oktestlen)
            } // for testi
        } // Parallel
        fin_time_it = time(NULL);

        printf (" Fails: %" PRIu64 " %" PRIu64 ".", failed1, oktestlen);
        ShowDifTime(" Iteration time:", fin_time_it - start_time_it);
        free(testp);
        if (oktestlen) {
            printf("Test set found. Set length = %" PRIu64 "\n", testlen);
            PrintTestSet(okmask);
        } else {
            printf("Test set not found. Set length = %" PRIu64 "\n", testlen);
            break;
        }
    } // for testlen
    fin_time = time(NULL);
    ShowDifTime("Working time:", fin_time - start_time);
    return 0;
}

