#include <iostream>
#include "settings.h"
#include "tcommon.h"
#include "init.h"

using namespace std;

// формирование массива наборов переменных. Набор - номер строки в таблице истинности
// Наборы из всех нулей или единиц ничего не проверяют. Поэтому не включаются в массив
uint64_t SetTests()
{
    for (uint64_t i = 1; i <= max_tests; i++) {
        tests[i-1] = i;
    }
    return max_tests;
}

/*
// попытка убрать инвертированные тесты
// есть 00011, убирает 11100
// тестирование показывает, что увеличиваютмя длины наборов
uint64_t SetLessTests()
{
    uint64_t count = 1;
    for (uint64_t i = 1; i < max_tests; i++) {
        uint64_t mask = tests[i];
        uint64_t notmask = mask ^ varsmask;
        int ok = 1;
        for (uint64_t j = 0; j < count; j++) {
            if (tests[j] == notmask) {
                ok = 0;
                break;
            }
        }
        //printf("%u %u %u %d\n",tests[i], notmask, count, ok);
        if (ok) {
            tests[count] = tests[i];
            count++;
        }
    }
    return count;
}
*/

#ifdef AnyPermutations
// формирование массива перестановок любых пар переменных
void SetPairs()
{
    int count = 0;
    for (int i = 0; i < num_vars-1; i++) {
        for (int j = i + 1; j < num_vars; j++) {
            pairs[count][0] = i;
            pairs[count][1] = j;
            count++;
        }
    }
}
#endif // AnyPermutations

#ifdef NearPermutations
// формирование массива перестановок пар переменных, стоящих рядом
void SetPairs()
{
    int count = 0;
    for (int i = 0; i < num_vars-1; i++) {
        pairs[count][0] = i;
        pairs[count][1] = i+1;
        count++;
    }
}
#endif // NearPermutations

#ifndef AnyPermutations
#ifdef UseMirror
// Зеркало
// формирование массива перестановок пар переменных
void SetPairs()
{
    int count = 0;
    for (int i = 0; i < num_vars-1; i++) {
        pairs[count][0] = i;
        pairs[count][1] = num_vars-1;
        count++;
    }
}
#endif // UseMirror
#endif // AnyPermutations


// заполнение для пары переменных смены значений в таблице истинности
// какой номер в векторе G будет иметь номер в векторе F,
// если переставит пару переменных.
// передаются номера начальной и конечной переменных
// заполняется массив сиены значений FTOG
void SetFTOG(uint64_t startvar, uint64_t finvar, uint64_t FTOG[])
{
    uint64_t newv;
    uint64_t swaps;
    #ifdef UseMirror
        swaps = (finvar-startvar+1)/2;
    #else
        swaps = 1;
    #endif
    //cout << startvar << " " << finvar << endl;
    for (uint64_t v = 0; v < num_values; v++) {
        newv = v;
        for (uint64_t k = 0; k < swaps; k++) {
            if (((v >> (startvar+k)) & 1) != ((v >> (finvar-k)) & 1)) {
                newv ^= (1 << (startvar+k)) ^ (1 << (finvar-k));
            }
            //cout << "  " << ">" << newv << " ";
        }
        //cout << " " << v << ">" << newv << " ";
        FTOG[v] = newv;
    }
    //cout << endl;
}

// заполнение матрицы смены значений в таблице истинности
void SetFTOGMatr()
{
    for (uint64_t i = 0; i < num_pairs; i++) {
        SetFTOG(pairs[i][0], pairs[i][1], &FTOGmatr[i][0]);
    }
}

void InitArrays()
{
    cout << endl << VariablesRu << ": " << num_vars << "; " << DamageTypeRu << endl;
    SetTests();
    //num_tests = SetLessTests();
    // чуть сокращаем количество тестов
    // 11110 редко бывает удачным
    //num_tests--;

    // генерация пар переменных
    SetPairs();

    // установка массивов отображения значений таблицы истинности
    SetFTOGMatr();

    // печать пар переменных
    PrintPairs(PairsRu , pairs, num_pairs);

    // печать массивов отображения значений
    for (int i=0; i<num_pairs; i++) {
        PrintArr("FTOG ",&FTOGmatr[i][0], num_values);
    }
    cout << VarSetsRu << num_tests;
    printf("; maxfun = %" PRIu64 "; pairs = %u\n",
           max_function, num_pairs);
    #ifdef DebugPrint
        // печать всех функций
        Prinuint64_ts();
    #endif // DebugPrint
}


