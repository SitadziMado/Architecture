#include "stdafx.h"

#include "BigInt.h"

int main()
{
    using std::cin;
    using std::cout;

    auto num = BigUint(123);
    auto fromString = BigUint::parse("123");
    auto lit = "1000000000000000000000000000000000000000000000000000000000000000"_BU;

    auto sum = num + fromString;
    auto mul = num * fromString;

    auto str1 = sum.toString();
    auto str2 = mul.toString();
    auto str3 = lit.toString();

    return 0;
}