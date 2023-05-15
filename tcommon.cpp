#include "settings.h"
#include "tcommon.h"
#include <iostream>
#include <fstream>
#include "string.h"
#include <string>
#include <regex>

using namespace std;

/*
// буфер для формирования двоичных строк
char bins[65] = "00000000000000000000000000000000";


// число в строку в двоичной системе
char* binstr(uint64_t v, uint64_t num)
{
    uint64_t k = 0;
    //sprintf(bins,"%u %n", v,&k);
    for (uint64_t i = 0; i < num; i++) {
        bins[i+k] = ((v >> (num-1-i)) & 1) +'0';
    }
    bins[num+k] = 0;
    return bins;
}
*/


string bin2string(uint64_t v, uint64_t num)
{
    string s = "";
    for (uint64_t i = 0; i < num; i++) {       
        s += char(((v >> (num-1-i)) & 1) +'0');
    }
    return s;
}

uint64_t bin2u(const char * s)
{
    uint64_t res = 0;
    for (uint64_t i = 0; i < num_values; i++) {
        res = (res << 1) + (s[i] - '0');
    }
    return res;
}

uint64_t bin2u2(const char * s)
{
    uint64_t res = 0;
    for (uint64_t i = 0; i < num_values/2; i++) {
        res = (res << 1) + (s[i] - '0');
    }
    return res;
}

/*
// число в строку в двоичной системе, начиная с младшего бита
char* binaryLite(uint64_t v, uint64_t num)
{
    for (uint64_t i = 0; i < num; i++) {
        bins[i] = ((v >> i) & 1) +'0';
    }
    bins[num] = 0;
    return bins;
}

// вектор функции в строку
char* binfun(uint64_t v)
{
    return binstr(v, num_values);
}
*/

string fun2string(uint64_t v)
{
    return bin2string(v, num_values);
}

// печать всех функций в лвоичном виде
void Prinuint64_ts()
{
    for (uint64_t i = 1; i <= max_function; i++) {
        cout << fun2string(i) << " ";
    }
    cout << endl;
}

// печать одномерного массива
void PrintArr(const char *s, uint64_t *a, uint64_t size)
{
    printf("%s: ", s);
    for (uint64_t i = 0; i < size; i++) {
        if (i == size - 1)
            printf("%" PRIu64 , a[i]);
        else
            printf("%" PRIu64 ",", a[i]);
    }
    printf("\n");
}

// печать массива пар
void PrintPairs(const char *s, unsigned a[][2], unsigned size)
{
    printf("%s: ", s);
    for (unsigned i = 0; i < size; i++) {
        #ifdef UseMirror
        printf("(%u-%u) ", a[i][0], a[i][1]);
        #else
        printf("(%u,%u) ", a[i][0], a[i][1]);
        #endif
    }
    printf("\n");
}

// печать набора тестов
void PrintTestSet(uint64_t testmask)
{
    uint64_t t = testmask, t2 = t;
    uint64_t e = 0;
    cout << "тест:" ;
    while (t2 != 0) {
        if ((t2 & 1) != 0) {
            cout << " " << e ;
        }
        t2 >>= 1;
        e++;
    }
    cout << endl << "Набор значений переменных: (";

    e = 0;
    while (t != 0) {
        if ((t & 1) != 0) {
            cout << " " << bin2string(e,num_vars);
        }
        t >>= 1;
        e++;
    }

    cout << " ); " << endl << "Как вектор: " << bin2string(testmask,num_values) << endl;
}

void ShowLocalTime(time_t mytime)
{
    struct tm * timeinfo;
    timeinfo = localtime(&mytime);
    printf( "[%d %d %d %d:%d:%d]", timeinfo->tm_mday,
            timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void ShowDifTime(const char *s, time_t mytime)
{
    printf("%s ", s);
    if (mytime <= 60) {
        printf("%" PRIu64 " s\n", (uint64_t) mytime);
    } else if (mytime < 3600) {
        printf("%" PRIu64 " m %" PRIu64 " s\n", (uint64_t) mytime / 60, (uint64_t) mytime % 60);
    } else {
        printf("%" PRIu64 " h %" PRIu64 " m %" PRIu64 " s\n", (uint64_t) mytime / 3600,(uint64_t) mytime % 3600 / 60, (uint64_t) mytime % 60);
    }
}

// удаление пробельных символов слева и справа
string trim(string s) {
    regex e("^\\s+|\\s+$");
    return regex_replace(s, e, "");
}

// Загрузка функций для тестирования
// Можно загрузить функции с текущим числом переменных и тестировать их
// Можно загрузить функции с числом переменных на 1 меньше и составить из них комбинации
uint64_t * LoadSubFunctions(string fun_name, uint64_t * num_functions)
{
    cout << "Loading functions from " << fun_name << endl;
    ifstream myfile (fun_name);
    if (!myfile.is_open()) {
        return NULL;
    }

    uint64_t n1 = 0;
    unsigned shift = num_values / 2;
    string line;
    while ( getline (myfile,line) ) {
        line = trim(line);
        if (line.size() > 0) {
            n1++;
        } else {
            continue;
        }
        if (line.size() != shift && line.size() != num_values) {
            cout << "Function vector size is " << line.size() << ". Expected size: " << shift << endl;
            cout << "'" << line << "'" << shift << endl;
            myfile.close();
            return NULL;
        } else {
             if (line.size() == num_values) {
                 shift = 0;
             }
        }
    }
    if (shift == 0) {
        cout << "Functions in file: " << n1 << "; shift =" << shift << endl;
    } else {
        cout << "Subfunctions in file: " << n1 << "; value len =" << shift  << endl;
    }

    uint64_t *sel_functions;
    uint64_t f;
    uint64_t *fs = new uint64_t[n1];
    uint64_t k = 0;

    myfile.clear();
    myfile.seekg(0);
    while ( getline (myfile,line) ) {
        if (shift) {
            f = bin2u2(line.c_str());
        } else {
            f = bin2u(line.c_str());
        }
        if (k < n1) {
            fs[k] = f;
            k++;
        } else {
            break;
        }
    }
    myfile.close();
    if (n1 == 0) {
        delete[] fs;
        return NULL;
    }
    cout << "Last functions in file is: " << fun2string(fs[n1-1]) << endl;

    if (shift == 0) {
        // файл с полными функциями
        *num_functions = k;
        return fs;
    }
    
    // файл с функциями на одну переменную меньше. Составляем полные функции
    n1 = k;
    k = 0;
    #ifdef Concat
    *num_functions = n1*n1*4;
    sel_functions = new uint64_t[*num_functions];
    cout << "Number of composite functions: " << *num_functions << endl;
    for (uint64_t i = 0; i < n1; i++) {
        for (uint64_t j = 0; j < n1; j++) {
            for (uint64_t m = 0; m < 4; m++) {
                sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (m << (shift-1));
            }
            /*
            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (0ll << 31) | (0ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (1ll << 31) | (0ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (0ll << 31) | (0ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (1ll << 31) | (0ll << 61) | (0ll << 62) | (1ll << 29);

            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (0ll << 31) | (0ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (1ll << 31) | (0ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (0ll << 31) | (0ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (1ll << 31) | (0ll << 61) | (1ll << 62) | (1ll << 29);

            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (0ll << 31) | (1ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (1ll << 31) | (1ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (0ll << 31) | (1ll << 61) | (0ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (1ll << 31) | (1ll << 61) | (0ll << 62) | (1ll << 29);

            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (0ll << 31) | (1ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|0) << shift) | fs[j] | (1ll << 31) | (1ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (0ll << 31) | (1ll << 61) | (1ll << 62) | (1ll << 29);
            sel_functions[k++] = ((fs[i]|1) << shift) | fs[j] | (1ll << 31) | (1ll << 61) | (1ll << 62) | (1ll << 29);
            */
        }
    }
    #else

    uint64_t rand_bits = 15;
    uint64_t max_mask = (1 << rand_bits)-1;
    *num_functions = n1*(1 << rand_bits)*2;
    sel_functions = new uint64_t[*num_functions];
    cout << "Number of composite functions: " << *num_functions << endl;
    for (uint64_t i = 0; i < n1; i++) {
        for (uint64_t j = 0; j <= max_mask; j++) {
                //sel_functions[k++] = (j << (num_values-rand_bits-1)) | fs[i]  | (0ll << (shift-1));
                //sel_functions[k++] = (j << (num_values-rand_bits-1)) | fs[i]  | (1ll << (shift-1));
                sel_functions[k++] = (j << (shift-rand_bits-1)) | (fs[i] << shift) | (0ll << (shift-1));
                sel_functions[k++] = (j << (shift-rand_bits-1)) | (fs[i] << shift) | (1ll << (shift-1));
        }
    }
    #endif

    cout << "Last composite function:   " << fun2string(sel_functions[*num_functions-1]) << endl;
    delete[] fs;
    return sel_functions;
}


