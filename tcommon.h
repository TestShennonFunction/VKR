#ifndef tcommon_h
#define tcommon_h

using namespace std;

// число в строку в двоичной системе
//char* binstr(uint64_t v, uint64_t num);
string bin2string(uint64_t v, uint64_t num);

// строку в двоичной системе в число
uint64_t bin2u(const char * s);
uint64_t bin2u2(const char * s);

// число в строку в двоичной системе, начиная с младшего бита
//char* binaryLite(uint64_t v, uint64_t num);

// вектор функции в строку
//char* binfun(uint64_t v);
string fun2string(uint64_t v);

// печать всех функций в лвоичном виде
void Prinuint64_ts();

// печать одномерного массива
void PrintArr(const char *s, uint64_t *a, uint64_t size);

// печать массива пар
void PrintPairs(const char *s, unsigned a[][2], unsigned size);
//void PrintArr2(const char *s, uint64_t a[][2], uint64_t size);

// печать набора тестов
void PrintTestSet(uint64_t testmask);

void ShowDifTime(const char *s, time_t mytime);
void ShowLocalTime(time_t mytime);

// Поиск провераяющего теста для функции F
// int SearchFunCheck(uint64_t F);

// Поиск диагностического теста для функции F
// int SearchFunDiag(uint64_t F);

// получение нового вектора значений лля перестановки
// передается вектор значений функции vec и массив переходов значений
inline uint64_t GVector(uint64_t F, uint64_t FTOG[])
{
    // крайние значения в векторе не меняются
    uint64_t G = F & ((1ULL << (num_values-1)) | 1);
    uint64_t *p = &FTOG[1];
    while (*p != num_values-1) {
        F >>= 1;
        G |= (F  & 1) << *(p++);
    }
    return G;
}

// Загрузка функций для тестирования
// Можно загрузить функции с текущим числом переменных и тестировать их
// Можно загрузить функции с числом переменных на 1 меньше и составить из них комбинации
uint64_t * LoadSubFunctions(string fun_name, uint64_t * num_functions);

/*
inline uint64_t GVector(uint64_t F, uint64_t FTOG[])
{
    // крайние значения в векторе не меняются
    uint64_t G = F & ((1ULL << (num_values-1)) | 1);    
    for (uint64_t i = 1; i < num_values - 1; i++) {
        F >>= 1;
        G |= (F & 1) << FTOG[i];
    }
    return G;
}
*/


#endif // tcommon_h
