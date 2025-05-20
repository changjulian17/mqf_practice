// C++ program to demonstrate an input iterators

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm> // For std::iter_swap (uses more powerful iterators)

using namespace std;

// A simple function that "swaps" the values pointed to by two input iterators
template <typename InputIterator1, typename InputIterator2, typename T>
void input_iterator_conceptual_swap(InputIterator1 it1, InputIterator2 it2)
{
	// Input iterators primarily support reading (dereferencing) and incrementing.
	// We'll read the values pointed to and conceptually swap them in this context.

	T val1 = *it1; // Read the value pointed to by it1
	T val2 = *it2; // Read the value pointed to by it2

	std::cout << "Conceptual swap:" << std::endl;
	std::cout << "  Value at first iterator (before conceptual swap): " << val1 << std::endl;
	std::cout << "  Value at second iterator (before conceptual swap): " << val2 << std::endl;
	std::cout << "  (Input iterators themselves typically don't support direct writing for swapping)" << std::endl;
	std::cout << "  (We've conceptually read them. To actually swap in the source, "
			  << "we'd need output or bidirectional iterators)." << std::endl;
}

int main()
{
	// example 1: using input iterator
	if (false)
	{
		vector<int> v = {1, 2, 3};

		// Accessing the elements without using iterators
		cout << "Without iterators = ";
		int j;
		for (j = 0; j < 3; ++j)
		{
			cout << v[j] << " ";
		}

		// Accessing the elements using iterators forward
		cout << "\nWith iterators = ";
		vector<int>::iterator i;
		for (i = v.begin(); i != v.end(); ++i)
		{
			cout << *i << " ";
		}

		if (i == v.end())
			cout << "iterator points to end of container" << endl;
	}
	
	// example 2, dynamically add or delete from container
	if (0)
	{
		std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

		// Using a lambda with remove_if to conditionally "delete" (move to end)
		numbers.erase(std::remove_if(numbers.begin(), numbers.end(),
									 [](int n)
									 { return n % 2 == 0; }),
					  numbers.end());

		std::cout << "Odd numbers after 'deletion': ";
		for (int num : numbers)
		{
			std::cout << num << " ";
		}
		std::cout << std::endl; // Output: Odd numbers after 'deletion': 1 3 5

		return 0;
	}

	//example 3, swap item
	if (0)
	{
		std::vector<int> vec1 = {10};
		std::vector<int> vec2 = {20};

		// Get input iterators (which for vector::iterator also happen to be random access)
		std::vector<int>::iterator it_vec1 = vec1.begin();
		std::vector<int>::iterator it_vec2 = vec2.begin();

		input_iterator_conceptual_swap<std::vector<int>::iterator, std::vector<int>::iterator, int>(it_vec1, it_vec2);
		std::cout << std::endl;

		// Standard library's std::iter_swap (requires forward iterators at least)
		std::cout << " Using std::iter_swap (for demonstration - vector iterators are stronger):" << std::endl;
		std::cout << " Before swap: *it_vec1 = " << *it_vec1 << ", *it_vec2 = " << *it_vec2 << std::endl;
		std::iter_swap(it_vec1, it_vec2);
		std::cout << " After swap: *it_vec1 = " << *it_vec1 << ", *it_vec2 = " << *it_vec2 << std::endl;
		std::cout << " Vector contents: vec1[0] = " << vec1[0] << ", vec2[0] = " << vec2[0] << std::endl;

		return 0;
	}

	return 0;
}