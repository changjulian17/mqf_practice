#include <iostream>

// Function that returns a pointer to an integer
int* createInteger() {
    // Dynamically allocate memory for an integer
    int* ptr = new int;     // new key word is used to allocate memory on the heap or stacking memory. 
                            // It is persistent until it is deleted.
    
    // Initialize the value
    *ptr = 42;

    // Return the pointer
    return ptr;
}

int main() {
    // Call the function and get the returned pointer
    int* myIntegerPtr = createInteger();
    std::cout << "Pointer address: " << myIntegerPtr << std::endl;

    // Access the value through the pointer
    std::cout << "Value: " << *myIntegerPtr << std::endl;

    // Don't forget to free the allocated memory to avoid memory leaks
    delete myIntegerPtr;

    return 0;
}