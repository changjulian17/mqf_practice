#include <iostream>
#include <deque>
#include <string>

int main()
{
    // Create an empty deque of strings
    std::deque<std::string> myDeque;

    // Add elements to the back
    myDeque.push_back("World");
    myDeque.push_back("!");

    // Add elements to the front
    myDeque.push_front("Hello");

    std::cout << "Elements in the deque: ";
    // Iterate through the deque using a range-based for loop
    for (const auto &element : myDeque)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl; // Output: Hello World !

    // Access elements using index (random access)
    std::cout << "First element: " << myDeque[0] << std::endl;                 // Output: Hello
    std::cout << "Last element: " << myDeque[myDeque.size() - 1] << std::endl; // Output: !

    // Access the front and back elements directly
    std::cout << "Front element: " << myDeque.front() << std::endl; // Output: Hello
    std::cout << "Back element: " << myDeque.back() << std::endl;   // Output: !

    // Remove elements from the front
    myDeque.pop_front();
    std::cout << "After pop_front(): ";
    for (const auto &element : myDeque)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl; // Output: World !

    // Remove elements from the back
    myDeque.pop_back();
    std::cout << "After pop_back(): ";
    for (const auto &element : myDeque)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl; // Output: World

    // Check if the deque is empty
    if (myDeque.empty())
    {
        std::cout << "The deque is now empty." << std::endl;
    }
    else
    {
        std::cout << "The deque still has " << myDeque.size() << " element(s)." << std::endl;
    }

    // Add more elements
    myDeque.push_front("First");
    myDeque.push_back("Last");

    std::cout << "Current deque: ";
    for (const auto &element : myDeque)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl; // Output: First Last

    return 0;
}