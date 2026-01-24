#include <iostream>
using namespace std;

template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    Pair(T1 a, T2 b) {
        first=a;
        second=b;
    }

    T1 GetFirst() {
        return first;
    }

    T2 GetSecond() {
        return second;
    }

    T2 operator[](T1 a) {
        return first == a ? second : T2();
    }

};

int main() {
    
    Pair<int, double> pair1(10, 3.14);
    cout << "First: " << pair1.GetFirst() << ", Second: " << pair1.GetSecond() << endl;

    Pair<string, bool> pair2("hello", true);
    cout << "First: " << pair2.GetFirst() << ", Second: " << pair2.GetSecond() << endl;


    auto result = pair1[10]; // Accessing the second element using the first element
    cout << "Result: " << result << endl; // Should print 3.14

    return 0;
}