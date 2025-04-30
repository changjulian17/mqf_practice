#include <iostream>
#include <vector>

using namespace std;

// Function that returns a reference to a vector
std::vector<int>& getVector() {
    // Declare a static vector (for the sake of example)
    static std::vector<int> myVector = {1, 2, 3, 4, 5};     // this is a template class
    // static means the value will live in memory until the program ends
    // std::vector<int> myVector = {1, 2, 3, 4, 5};

    // Return a reference to the vector
    return myVector;
};

class test{
public:
    int x;
};

const test& getTestRef(const test& t) {
    return t;
}

int main() {
    // Call the function and get the returned reference
    std::vector<int>& vecRef = getVector();

    // Modify the vector through the reference
    vecRef.push_back(6);

    // Print the modified vector
    for (int value : vecRef) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    //tricky example
    if (1) {
        test* t1 = new test();      // data in heap
        test t2 = test();           // data in stack
        cout<<&t2<<endl;        // address of t2
        t2.x=15;            // instantiating x in t2
        t1->x = 10;         // instantiating x in t1 uses arrow
        auto t_ref = getTestRef(t2);    // why is the reference able to change to a value, isnt the reference a diff data type?
        cout << t_ref.x <<endl;
        t_ref.x = 20;
        cout << "pointer t1 x: "<< t2.x <<endl;
        cout << "t1_ref x: " << t_ref.x <<endl;
        delete t1;
    }

    return 0;
}