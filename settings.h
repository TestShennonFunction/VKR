#ifndef settings_h
#define settings_h

using namespace std;

// КОНСТАНТЫ, ПЕРЕМЕННЫЕ И ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВВОДА И ВЫВОДА
#define __STDC_FORMAT_MACROS 1
#include "cstdint"
#include <inttypes.h>
#include <string>
#include "time.h"


#define num_vars 5
#define DamageNum 1

#if DamageNum == 1
  // зеркало любых переменных
  #define AnyPermutations
  #define UseMirror
#elif DamageNum == 2
  // перестановка любых переменных
  #define AnyPermutations
#elif DamageNum == 3
  // зеркало от любой переменной до доследней
  #define UseMirror
#elif DamageNum == 4
  // Перестанока двух рядом стоящих переменных
  #define NearPermutations
#endif

//#define DebugPrint
//#define DebugPrintFun

#ifndef DebugPrint
#define Parallel
#endif


//--------------константы--------------

#define num_values ((uint64_t)1ULL << num_vars)
#define varsmask (num_values - 1)
#define max_tests (num_values-2)

//
//  2  3   4     5  num_vars
//  4  8  16    32  num_values
//  # 16 256 4млрд  num_functions


#ifdef AnyPermutations
  #define num_pairs (num_vars * (num_vars-1) / 2)
  #ifdef UseMirror
    #define DamageTypeRu "Зеркалирование области между любыми переменными"
    #define DamageTypeEn "Mirror area between any variables"
  #else
    #define DamageTypeRu "Перестановка любых 2 переменных"
    #define DamageTypeEn "Permutation of any 2 variables"
  #endif
#else
  #define num_pairs (num_vars-1)
  #ifdef NearPermutations
    #define DamageTypeRu "Перестановка 2 рядом стоящих переменных"
    #define DamageTypeEn "Permutation of 2 near variables"
  #else
    #define DamageTypeRu "Зеркалирование области до последней переменной"
    #define DamageTypeEn "Mirror area till last variable"
  #endif
#endif

#define VariablesRu "Количество переменных"
#define VariablesEn "Number of variables"

#define PairsRu "Пары для функций неисправности"
#define PairsEn "pairs"

#define VarSetsRu "Тестирующих наборов = "
#define VarSetsEn "var sets = "

#define SearchTestLenRu "Проверка длины теста = "
#define SearchTestLenEn "Saercing for test length = "

#define NumberOfTestsRu "Количество тестов = "
#define NumberOfTestsEn "Total number of tests = "

#define TestFoundRu "Тест найден."
#define TestFoundEn "Test found."

#define IterationTimeRu " Время итерации:"
#define IterationTimeEn " Iteration time:"

#define WorkingTimeRu "Время работы:"
#define WorkingTimeEn "Working time:"

#define FailedFunctionRu "Провал теста на функции: "
#define FailedFunctionEn "Test failed with function: "

#define FailedWithRu "Неудачное тестирование для функции: "
#define FailedWithEn "Test failed with: "

#define OnFunctionsRu " функциях"
#define OnFunctionsEn " functions"

#define TestLenRu "Длина теста"
#define TestLenEn "Test length"

#define SearchSuccessRu "Тесты найдены для всех функций. Длина теста = "
#define SearchSuccessEn "Test sets found for all functions. Min test set length = "

//--------------переменные--------------


extern uint64_t num_tests;

// количество булевских функций для num_vars
//extern uint64_t num_functions;

// количество проверяемых функций для num_vars
// фунции 0 и 1 не проверяются
extern uint64_t max_function;

extern uint64_t tests[max_tests];      // тесты (номера в таблице истинности)
extern unsigned pairs[num_pairs][2];   // переставляемые пары
// отображение значений в таблице истинности при перестановке значений
extern uint64_t FTOGmatr[num_pairs][num_values];

extern uint64_t *testp; // наборы тестов
extern uint64_t testcnt;

#endif // settings_h
