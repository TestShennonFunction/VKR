#include "settings.h"
#include <iostream>
#include <fstream>
#include "string.h"
#include <string>

using namespace std;

uint64_t num_tests = max_tests;
uint64_t *testp = NULL; // наборы тестов
uint64_t testcnt = 0;


// количество булевских функций для num_bits
// uint64_t num_functions = (uint64_t)1ULL << num_values;

// количество проверяемых функций для num_vars
// функции 0 и 1 не проверяются
uint64_t max_function = ((uint64_t)1 << (num_values-1)) - 1;
/*
bits = 2
values = 4
functions = 16
1,3,5,7
0111
*/

uint64_t tests[max_tests];      // тесты (номера в таблице истинности)
unsigned pairs[num_pairs][2];   // переставляемые пары
// отображение значений в таблице истинности при перестановке значений
uint64_t FTOGmatr[num_pairs][num_values];
