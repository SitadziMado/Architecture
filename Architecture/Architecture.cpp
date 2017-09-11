#include "stdafx.h"

#include <iosfwd>
#include <locale>
#include <clocale>

int main()
{
    using std::wcin;
    using std::wcout;
    using std::locale;

    // Создаем россияновскую локаль.
    locale russian = locale("ru-RU.CP1251");

    // Устанавливаем ее как глобальную.
    locale::global(russian);

    int number;

    // Вводим обычный номер.
    wcin >> number;

    // Выводим текст на российском и номер.
    wcout << L"Номер равняется " << number << L".";

    return 0;
}