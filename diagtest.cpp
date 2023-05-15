#include <iostream>
#include <fstream>
#include "settings.h"
#include "tcommon.h"
#include "combinations.h"
#include "init.h"
#include "diagtest.h"
#include <string.h>
#define __STDC_FORMAT_MACROS 1
#include "cstdint"
#include <inttypes.h>
#ifdef Parallel
#include "omp.h"
#define MAX 8
#endif // Parallel
// Поиск диагностического теста для функции F
uint64_t SearchFunDiag(uint64_t F)
{
    uint64_t test_found;
    uint64_t G, fs[num_pairs]; // Функция неисправноси и массив функций неисправности
    uint64_t testvector;
    //#define ShowTests
    #ifdef ShowTests
        uint64_t cnt = 0;  // подсчет хороших тестов
    #endif
    // пробегаем по всем тестам
    for (uint64_t testi = 0; testi < testcnt; testi++) {
        testvector = testp[testi];
        test_found = 1;
        // Проверяющий тест
        for (uint64_t k = 0; k < num_pairs; k++) {
            // получение вектора значений функции неисправности
            G = GVector(F, &FTOGmatr[k][0]);
            if (F != G) { // если вектор значений изменился
                if (((G ^ F) & testvector) == 0) {
                    test_found = 0;
                    break; // дальше не проверяем, одного провала достаточно
                }
            }
            fs[k] = G;
        } // for k
        if (!test_found) {
            continue;
        }
        // Проверяющий тест подошел, проверка на диагностический
        for (uint64_t k = 0; k < num_pairs; k++) {
            F = fs[k];
            for (uint64_t i = 0; i < k; i++) {
                // получение вектора значений другой функции неисправности
                G = fs[i];
                if (F != G) { // если вектор значений изменился
                    if (((G ^ F) & testvector) == 0) {
                        test_found = 0;
                        break; // дальше не проверяем, одного провала достаточно
                    }
                }
            }
            if (!test_found) {
                break;
            }
        } // for k
        if (test_found) {
            #ifdef ShowTests
                cnt++;
                PrintTestSet(testvector); // может быть очень много вывода
            #else
                return 1;
            #endif
        }
    } // for testi
    #ifdef ShowTests
        cout << "cnt=" << cnt << endl ;
        return cnt;
    #else
        return 0;
    #endif
}

using namespace std;
// Приск диагностического теста для множества функций
// Для каждой функции ищется свой диагностический тест
// Если задан FunctionsFile, то рассматриваются только функции из файла
// или их комбинации
// Иначе рассматриваются все функции var_nums переменных
// Если задан DumpFailures, то вывод в файл функций,
// для которых не удалось найти диагностического теста заданной длины
// Этот файл можно использовать для составления функций,
// содержащих на одну пееменную больше
// StartLen - стартовая длина тестового набора
// FinLenLen - предельная длина тестового набора
// Поиск распараллевается на потоки (количество потоков задается в settings.h)
int diag_test(uint64_t StartLen, uint64_t FinLen, 
              const char * FunctionsFile, const char * DumpFailures)
{
    ofstream myfile;
    if (*DumpFailures) {
        myfile.open(DumpFailures);
        if (!myfile.is_open()) {
            cout << "error open file to write: " << DumpFailures << endl;
            return 0;
        }
    }

    cout << "Диагностический тест" << endl;
    time_t start_time, fin_time, start_time_it, fin_time_it;
    start_time = time(NULL);
    ShowLocalTime(start_time);
    InitArrays();

    // num_vars = 3, num_values=8, max_function=127, num_functions=63
    // в цикле будет f=(i+1)* 2,i=0,1,...,62, f=2,4,...,126
    // функции 0,1 и >=128 нет смысла рассматривать. 1......1 
    // крайие единицы не переставляются
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
        unsigned failed = 0; // Количество провальных функций
        cout << SearchTestLenRu << testlen << ".  ";
        start_time_it = time(NULL);
        stop = 0;
        // Получение всех тестов длины testlen в количестве С(k,n)
        testp = PrepareCombinations(testlen, &testcnt, num_tests);
        cout << NumberOfTestsRu << testcnt << "." << endl;
        #ifdef Parallel
        #pragma omp parallel num_threads(MAX) private(F)
        #endif // Parallel
        {
                #ifdef Parallel
                #pragma omp for schedule(static)
                #endif // Parallel
                for (uint64_t i = 0; i < num_functions; i++) {
                    if (!stop) {
                        F = (i + 1) * 2;
                        if (sel_functions) {
                            F = sel_functions[i];
                        }
                        if (!SearchFunDiag(F)) {
                            // Дамп провальных функций в файл
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
                        } // if (!SearchFunDiag(F))
                    } // if (!stop)
                } // for ш
        } // Parallel
        free(testp);

        fin_time_it = time(NULL);
        ShowDifTime(IterationTimeRu, fin_time_it - start_time_it);
        if (!stop && failed == 0) {
            cout << SearchSuccessRu << testlen << endl;
            break;
        } else {
            if (stop) {
                cout << FailedFunctionRu << fun2string(stop) << endl;
            }
            if (failed) {
                cout << FailedWithRu << failed << OnFunctionsRu << endl;
            }
        }
    } // for testlen
    fin_time = time(NULL);
    ShowDifTime(WorkingTimeRu, fin_time - start_time);
    if (sel_functions) {
        delete[] sel_functions;
    }
    if (*DumpFailures) {
        myfile.close();
    }
    return 0;
}
