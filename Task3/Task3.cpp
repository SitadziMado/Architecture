#include "stdafx.h"

using Uint64 = unsigned long long;

template <Uint64 Number> struct Fibonacci;

template <>
struct Fibonacci<0>
{
	static constexpr Uint64 value = 1;
};

template <>
struct Fibonacci<1>
{
	static constexpr Uint64 value = 1;
};

template <Uint64 Number>
struct Fibonacci
{
	static constexpr Uint64 value = 
		Fibonacci<Number - 1>::value + Fibonacci<Number - 2>::value;
};

std::vector<Uint64> fibonacci(Uint64 number)
{
	switch (number)
	{
	case 0:
		return{ 1 };

	case 1:
		return{ 1, 1 };

	default:
		break;
	}

	auto result = std::vector<Uint64>{ 1, 1 };

	Uint64 cur = 0ULL;

	for (int i = 2; i < number; ++i)
	{
		cur = result.back() + *(result.rbegin() + 1);
		result.push_back(cur);
	}

	return result;
}

void recursiveFibonacciImpl(std::vector<Uint64>& result, Uint64 number, Uint64 counter)
{
	if (counter < number)
	{
		auto it = result.rbegin();
		auto last = *it;
		auto preLast = *(it + 1);
		result.push_back(last + preLast);
		recursiveFibonacciImpl(result, number, counter + 1);
	}
}

std::vector<Uint64> recursiveFibonacci(Uint64 number)
{
	switch (number)
	{
	case 0:
		return{ 1 };

	case 1:
		return{ 1, 1 };

	default:
		break;
	}

	std::vector<Uint64> result{ 1,1 };
	recursiveFibonacciImpl(result, number, 2);

	return result;
}

int main()
{
	using std::cin;
	using std::cout;

	Uint64 number;

	cout << "Input a length of a Fibonacci sequence: ";
	cin >> number;

	while (!cin)
	{
		cout << "Need a natural number." << std::endl;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin >> number;
	}

	auto iterationBased = fibonacci(number);
	auto recursiveBased = recursiveFibonacci(number);

	cout << "Compile-time based Fibonacci (15): " << Fibonacci<15>::value << std::endl;
	cout << "Iteration based Fibonacci: " << std::endl;

	std::for_each(
		iterationBased.cbegin(),
		iterationBased.cend(),
		[](Uint64 x) { cout << x << "; "; }
	);

	cout << std::endl << "Recursive based Fibonacci: " << std::endl;
	
	std::for_each(
		recursiveBased.cbegin(),
		recursiveBased.cend(),
		[](Uint64 x) { cout << x << "; "; }
	);

	std::system("pause");

    return 0;
}