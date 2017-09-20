#include "stdafx.h"

using Int64 = long long;

std::vector<Int64> inputArray(std::istream& stream)
{
	auto result = std::vector<Int64>();

	while (true)
	{
		Int64 temp;
		stream >> temp;

		if (!stream)
			break;

		result.push_back(temp);
	}

	std::string temp;

	stream.clear();
	stream >> temp;

	if (temp == "*")
		return result;
	else
		throw std::runtime_error(
			"Список должен завершаться `*`, а также возможен некорректный ввод."
		);
}

std::tuple<Int64, Int64, Int64> processArray(std::vector<Int64>& data)
{
	auto sum = std::accumulate(
		data.cbegin(), 
		data.cend(), 
		Int64()
	);

	auto sumPositive = std::accumulate(
		data.cbegin(),
		data.cend(),
		Int64(),
		[](Int64 z, Int64 xs) { return xs >= 0 ? z + xs : z; }
	);

	auto min = *std::min_element(
		data.cbegin(),
		data.cend()
	);

	return{ sum, sumPositive, min };
}

int main()
{
	auto data = inputArray(std::cin);
	auto result = processArray(data);

	std::cout << "Sum of the elements: " << std::get<0>(result) << std::endl;
	std::cout << "Sum of the positive elements: " << std::get<1>(result) << std::endl;
	std::cout << "Minimum of the elements: " << std::get<2>(result) << std::endl;

#if _DEBUG
	std::system("pause");
#endif

	return 0;
}