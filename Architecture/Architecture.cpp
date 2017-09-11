#include "stdafx.h"

#include <iosfwd>
#include <locale>
#include <clocale>

int main()
{
    using std::wcin;
    using std::wcout;
    using std::locale;

    // ������� ������������� ������.
    locale russian = locale("ru-RU.CP1251");

    // ������������� �� ��� ����������.
    locale::global(russian);

    int number;

    // ������ ������� �����.
    wcin >> number;

    // ������� ����� �� ���������� � �����.
    wcout << L"����� ��������� " << number << L".";

    return 0;
}