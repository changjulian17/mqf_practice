#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

int main() {
    int numCount;
    std::vector<int> numbers;
    std::vector<int> oddNumbers;
    int number;
    int sum = 0;
    std::ofstream outputFile;

    std::cout << "Enter the number of inputs: ";
    std::cin >> numCount;

    for (int i = 0; i < numCount; ++i) {
        std::cout << "Enter number " << i + 1 << ": ";
        std::cin >> number;
        numbers.push_back(number);
    }

    for (int num : numbers) {
        if (num % 2 != 0) {
            oddNumbers.push_back(num);
        }
    }

    if (!oddNumbers.empty()) {
        std::cout << *oddNumbers.begin() << std::endl;
        std::cout << *(oddNumbers.end() - 1) << std::endl;
        for (int num : oddNumbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        sum = std::accumulate(oddNumbers.begin(), oddNumbers.end(), 0);
    }

    outputFile.open("results.txt");
    if (outputFile.is_open()) {
        outputFile << "Sum of odd numbers: " << sum << std::endl;
        outputFile.close();
        std::cout << "Results written to results.txt" << std::endl;
    } else {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    return 0;
}
