#ifndef all_test_h
#define all_test_h

// ����� ����������� ������� ����������� �����
// ��� ���� ������� ����������� ������� �����
// ����� ������� ������� ������, ��� ��� ������ "��� ������ - ���� �����"

// ������� ������ �������� ������� ��� 3-4-5 ����������
int all_test(uint64_t StartLen, uint64_t FinLen, const char * FunctionsFile, const char * DumpFailures);

// ������� ������������ ������ �������� ������� ��� 6 ����������
int allfuns_test_6();

// �������� ������ ������ testvector ������ ��� ���� ������� � ����� �������� � ������� ��������
int quickchecktest(uint64_t testvector);

#endif // all_test_h
