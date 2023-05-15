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
#include "anytest.h"
#include "alltest.h"
#include "diagtest.h"
#include "utils.h"

using namespace std;

int testany()
{
        #ifdef UseMirror
            #ifdef AnyPermutations
            // Зеркало любых переменных
            cout << "permutation any 2 variables with mirror" << endl;
            if (num_vars==6) {
                // Проверить трудные функции 6 переменных на тестах длины 6
                any_test(6,6,"var6_len6_any_mirror.txt","");

                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // с цельююю нахождения хотя бы одной трудной функции 6 переменных
                // any_test(5,5,"var5_len5_any_mirror.txt","");

                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // и создать файл с трудными функциями 6 переменных
                // any_test(5,5,"var5_len5_any_mirror.txt","var6_len6_any_mirror.txt");
            } else if (num_vars==5) {
                // полный поиск минимальной длины тестовых наборов для всех функций 5 переменных
                any_test(1,6,"","");
                // diag_test(1,10,"","");

                // Проверить трудные функции 5 переменных на тестах длины 5
                // any_test(5,5,"var5_len5_any_mirror.txt","");

                // синтезировать функции 5 переменных из трудных функций 4 переменных
                // с цельююю нахождения хотя бы одной трудной функции 5 переменных
                //any_test(4,4,"var4_len4_any_mirror.txt","");

                // создать файл с трудными функциями 5 переменных
                //any_test(4,4,"","var5_len5_any_mirror.txt");
            } else if (num_vars==4) {
                // полный поиск минимальной длины тестовых наборов для всех функций 4 переменных
                //any_test(1,6,"","");
                diag_test(1,6,"","");

                // создать файл с трудными функциями 4 переменных
                // any_test(3,3,"","var4_len4_any_mirror.txt");
            } else { // 2,3,4 переменные
                // полный поиск минимальной длины тестовых наборов для всех функций
                any_test(1,6,"","");
            }
            #else
            // Зеркало от какой-то переменной до последней
            cout << "permutation any variable with last with mirror" << endl;
            if (num_vars==6) {
                // Проверить трудные функции 6 переменных на тестах длины 6
                //any_test(4,4,"var5_len4_any_mirror_last3.txt","");
                diag_test(1,10,"var5_len4_any_mirror_last.txt","");

                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // с цельююю нахождения хотя бы одной трудной функции 6 переменных
                // any_test(5,5,"var5_len5_any_mirror.txt","");

                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // и создать файл с трудными функциями 6 переменных
                // any_test(5,5,"var5_len5_any_mirror.txt","var6_len6_last_mirror.txt");
            } else if (num_vars==5) {
                any_test(1,10,"","");
                //any_test(3,3,"","var5_len4_last_mirror_last3.txt");
                //diag_test(1,10,"","");

                // создать файл с трудными функциями 5 переменных
                //any_test(4,4,"","var5_len5_last_mirror_last.txt");
            } else {
                //diag_test(1,10,"","");
                any_test(1,6,"","");
            }
            #endif
        #else
            #ifdef NearPermutations
            // перестановка 2 переменных, стоящих рядом
            cout << "any near" << endl;
            if (num_vars==6) {
                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // с цельююю нахождения хотя бы одной трудной функции 6 переменных
                diag_test(1,10,"var5_len4_any_near12.txt","");
                //any_test(4,4,"var5_len4_any_near12.txt","");
            } else if (num_vars==5) {
                // полный поиск минимальной длины тестовых наборов для всех функций 5 переменных
                any_test(1,6,"","");
                //diag_test(1,10,"","");

                // создать файл с трудными функциями 5 переменных
                //any_test(3,3,"","var5_len4_any_near1.txt");
            } else if (num_vars==4) {
                any_test(1,6,"","");
            } else {
                any_test(1,6,"","");
            }
            #else
            // перестановка любых 2 переменных без зеркала
            cout << "any pair, no mirror" << endl;
            if (num_vars==6) {
                diag_test(1,16,"","");
                // синтезировать функции 6 переменных из трудных функций 5 переменных
                // с цельююю нахождения хотя бы одной трудной функции 6 переменных
                //any_test(6,6,"var5_len5_any_nomirror.txt","");
            } else if (num_vars==5) {
                // полный поиск минимальной длины тестовых наборов для всех функций 5 переменных
                any_test(1,6,"","");
                // diag_test(1,10,"","");

                // создать файл с трудными функциями 5 переменных
                // any_test(4,4,"","var5_len5_any_nomirror.txt");
            } else if (num_vars==4) {
                any_test(1,6,"","");
            } else {
                any_test(1,6,"","");
            }
            #endif
        #endif
    return 0;
}

int main()
{
    diag_test(1,15,"","");
    //any_test(1,15,"","");
    //all_test(1,24,"","");
    //testany();
    //testall();
 /*
    CheckFile();
    mainstat();
*/
   return 0;
}