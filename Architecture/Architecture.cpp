#include "stdafx.h"

// #include <iosfwd>
#include <iostream>
#include <locale>

int main()
{
    using std::cin;
    using std::cout;
    using std::locale;

    // ������� ������������� ������.
    //
    locale russian = locale("ru-RU");

    // ������������� �� ��� ����������.
    //
    locale::global(russian);

    int number;

    // ������ ������� �����.
    //
    cin >> number;

    // ������� ����� �� ���������� � �����.
    //
    cout << "����� ��������� " << number << ".";

    return 0;
}