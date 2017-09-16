#include "stdafx.h"

#include <functional>

#include "Vector.h"

int main()
{
    using std::cin;
    using std::cout;
    using geometry::Vector;
    using std::function;

    auto first = Vector();
    auto second = Vector();
    auto input = [&first, &second] {
        return (cin >> first >> second).good();
    };

    cout << "Enter vector coordinates separated by space:" << std::endl;
    
    while (!input())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Please enter valid vector coordinates." << std::endl;
    }

    cout << "Result: " << first << " * " << second << " = " << first * second;
    
    return 0;
}