#ifndef utils_h
#define utils_h

// Вспомогательные функции

// скачивается наборы тестов и проверяются на длине минус 1
int CheckFile();
int mainstat();
// проверка набора тестов для всех функций и всех перестановок пар переменных
int stattest(uint64_t testvector);

// проверка конкретного тестового набора на всех функциях
int check_test_345();

// проверка набора подтестов, получаемых из заданного теста
// цель либо показать, что уменьшение длины тестового набора невозможна
// либо найти набор меньшей длины
int checksubtests(uint64_t testvector1);

#endif // utils_h
