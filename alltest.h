#ifndef all_test_h
#define all_test_h

// ПОИСК ТЕСТИРУЮЩИХ НАБОРОВ МИНИМАЛЬНОЙ ДЛИНЫ
// Для всех функций подбирается единный набор
// Здесь перебор намного больше, чем для случая "для каждой - свой набор"

// функция поиска тестовых наборов для 3-4-5 переменных
int all_test(uint64_t StartLen, uint64_t FinLen, const char * FunctionsFile, const char * DumpFailures);

// функция сокращенного поиска тестовых наборов для 6 переменных
int allfuns_test_6();

// проверка набора тестов testvector только для всех функций с одной единицей в векторе значений
int quickchecktest(uint64_t testvector);

#endif // all_test_h
