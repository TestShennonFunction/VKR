#include <iostream>
#include <fstream>
#include "settings.h"
#include "tcommon.h"
#include "combinations.h"
#include "init.h"
#include "anytest.h"
#include <string.h>

#define __STDC_FORMAT_MACROS 1
#include "cstdint"
#include <inttypes.h>

#ifdef Parallel
#include "omp.h"
#define MAX 8
#endif // Parallel

using namespace std;

// Поиск провераяющего теста для функции F
int SearchFunCheck(uint64_t F)
{
    uint64_t G; // Функция неисправноси
    uint64_t ok;
    uint64_t testi;
    uint64_t testvector;
    for (testi = 0; testi < testcnt; testi++) {
        testvector = testp[testi];
        ok = 1;
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение нового вектора значений лля перестановки
            G = GVector(F, &FTOGmatr[k][0]);
            if (F != G) { // если вектор значений изменился
                if (((G ^ F) & testvector) == 0) {
                    ok = 0;
                    break; // дальше не проверяем, одного провала достаточно
                }
            }
        }
        if (ok) {
            return 1;
        }
    }
    return 0;
}

// Нахождение минимального набора тестов
// Для каждой функции ищется свой минимальный набор
// Если задан FunctionsFile, то рассматриваются только функции из файла или их комбинации
// Иначе рассматриваются все функции
// Если задан DumpFailures, то вывод в файл функций,
// для которых не удалось найти тестового набора заданной длины
// Этот файл можно использовать для составления функций, содержащих на одну пееменную больше
// StartLen - стартовая длина тестового набора
// Поиск распараллевается на потоки (количество потоков задается в settings.h)
int any_test(uint64_t StartLen, uint64_t FinLen, const char * FunctionsFile, const char * DumpFailures)
{
    ofstream myfile;
    if (*DumpFailures) {
        myfile.open(DumpFailures);
        if (!myfile.is_open()) {
            cout << "error open file to write: " << DumpFailures << endl;
            return 0;
        }
    }

    time_t start_time, fin_time, start_time_it, fin_time_it;
    cout << "Проверяющий тест" << endl;
    start_time = time(NULL);
    ShowLocalTime(start_time);
    InitArrays();

    uint64_t num_functions = max_function / 2;
    uint64_t *sel_functions = NULL;
    if (*FunctionsFile) {
        sel_functions = LoadSubFunctions(FunctionsFile, &num_functions);
        if (!sel_functions) {
            cout << "Error load functions from " << FunctionsFile << endl;
            return 1;
        }       
    }

    uint64_t stop, F;
    for (uint64_t testlen = StartLen; testlen <= FinLen; testlen++) {
        unsigned failed = 0;
        printf("testlen = %" PRIu64 "\n", testlen);
        start_time_it = time(NULL);
        stop = 0;
        // инициалзация генератора наборов С(k,n)
        testp = PrepareCombinations(testlen, &testcnt, num_tests);
        printf("  Total number of test sets %" PRIu64 ".\n", testcnt);
        #ifdef Parallel
        #pragma omp parallel num_threads(MAX) private(F)
        #endif // Parallel
        {
                #ifdef Parallel
                #pragma omp for schedule(static)
                #endif // Parallel
                for (uint64_t i = 0; i < num_functions; i++) {
                //for (uint64_t i = num_functions; i > 1; i--) {
                    if (!stop) {
                        F = (i + 1) * 2;
                        if (sel_functions) {
                            F = sel_functions[i];
                        }
                        if (!SearchFunCheck(F)) {
                            if (*DumpFailures) {
                                failed++;
                                if (failed%1000 == 1) {
                                    cout << failed << endl;
                                }
                                if (failed >= 32000) {
                                myfile << fun2string(F) << endl;
                                }
                                if (failed > 64000) {
                                   stop = F;
                                }
                            } else {
                                 stop = F;
                            }
                        }
                    }
                }
        } // Parallel
        free(testp);

        fin_time_it = time(NULL);
        ShowDifTime(" Iteration time:", fin_time_it - start_time_it);
        if (!stop && failed == 0) {
            printf("Test sets found for all functions. Min test set length = %" PRIu64 "\n", testlen);
            break;
        } else {
            if (stop) {
                cout << "failed with function: " << fun2string(stop) << endl;
            }
            if (failed) {
                cout << "failed with " << failed << " functions\n";
            }
        }
    } // for testlen
    fin_time = time(NULL);
    ShowDifTime("Working time:", fin_time - start_time);
    if (sel_functions) {
        delete[] sel_functions;
    }
    if (*DumpFailures) {
        myfile.close();
    }
    return 0;
}
