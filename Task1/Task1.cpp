#include "stdafx.h"

#include <iosfwd>
#include <limits>
#include <stdexcept>

using Uint64 = unsigned long long;

Uint64 factorial(Uint64 number)
{
	auto result = 1ULL;

	while (number > 0)
	{
		result *= number--;
	}

	return result;
}

Uint64 recursiveFactorial(Uint64 number)
{
	if (!number)
		return 1;

	return number * recursiveFactorial(number - 1);
}

template <Uint64 Number> struct Factorial;

template <>
struct Factorial<0>
{
	static constexpr Uint64 value = 1ULL;
};

template <Uint64 Number>
struct Factorial
{
	static constexpr Uint64 value = Number * Factorial<Number - 1>::value;
};

int main()
{
    using std::cin;
    using std::cout;

	auto compileTime = Factorial<10>::value;

	Uint64 number;
	Uint64 iterationBased;
	Uint64 recursiveBased;

	cout << "Input a number: ";
	cin >> number;

	while (!cin)
	{
		cout << "Need a natural number." << std::endl;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin >> number;
	}

	iterationBased = factorial(number);
	recursiveBased = recursiveFactorial(number);

	cout << "Compile-time based factorial (10): " << Factorial<10>::value << std::endl;
	cout << "Iteration based factorial: " << iterationBased << std::endl;
	cout << "Recursive based factorial: " << recursiveBased << std::endl;

	std::system("pause");

    return 0;
}