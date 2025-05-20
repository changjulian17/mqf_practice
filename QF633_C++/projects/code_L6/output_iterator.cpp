// C++ program to demonstrate an output iterators
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main()
{
	if (false)
	{
		std::vector<int> numbers = {10, 20, 30, 40, 50};

		std::cout << "Printing numbers using ostream_iterator: ";

		// Create an ostream_iterator that writes integers to std::cout,
		// followed by a space after each element.
		std::ostream_iterator<int> output_it(std::cout, " ");

		// Iterate through the vector and assign each element to the output iterator.
		// This effectively writes each element to std::cout followed by a space.
		for (int num : numbers)
		{
			*output_it = num;
			++output_it;
		}
		std::cout << std::endl;
		// Output: Printing numbers using ostream_iterator: 10 20 30 40 50

		std::cout << "Printing numbers again using std::copy and ostream_iterator: ";
		// std::copy takes a range (begin and end iterators) and an output iterator.
		// It copies each element from the input range to the location pointed to by
		// the output iterator, incrementing the output iterator after each write.
		std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, ", "));
		std::cout << std::endl;
		// Output: Printing numbers again using std::copy and ostream_iterator: 10, 20, 30, 40, 50,

		return 0;
	}

	return 0;
}