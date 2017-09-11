#include "stdafx.h"

// #include <iosfwd>
#include <iostream>
#include <locale>

int main()
{
    using std::cin;
    using std::cout;
    using std::locale;

    // Создаем россияновскую локаль.
    //
    locale russian = locale("ru-RU");

    // Устанавливаем ее как глобальную.
    //
    locale::global(russian);

    int number;

    // Вводим обычный номер.
    //
    cin >> number;

    // Выводим текст на российском и номер.
    //
    cout << "Номер равняется " << number << ".";

    return 0;
}