// Step 1
#include <iostream>
#include <fstream>

int main(){
    int numCount;
    std::vector<int> numbers;
    std::vector<int> oddNumbers;
    int number;
    int sum = 0;
    std::ofstream outputFile;
    // Step 2
    std::cout << "Enter the number of inputs: ";
    std::cin >> numCount;

    for (int i = 0; i < numCount; ++i) {
        std::cout << "Enter number " << i + 1 << ": ";
        std::cin >> number;
        numbers.push_back(number);
    }
    // Step 3
    for (int num : numbers) {
        if (num % 2 != 0) {
            sum += num;
        }
    }

    // Step 4
    try{
        outputFile.open("results.txt");
        if (outputFile.is_open()) {
            outputFile << "Sum of odd numbers: " << sum << std::endl;
            outputFile.close();
            std::cout << "Data written to the file 'results.txt'." << std::endl;
            return 0;
        }
        else {
            std::cerr << "Error opening the file for writing." << std::endl;
            return 1;
        }
    }
    catch(std::exception& e){
        std::cerr << "Error opening the file for writing: " << e.what() << std::endl;
        return 1;
    }
}