#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file("../day1_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
        return 1;
    }
    int result = 0;
    for (std::string line; std::getline(file, line);) {
        char first_digit = '0', last_digit = '0';
        // start from beginning
        for (auto it = line.begin(); it != line.end(); ++it) {
            if (std::isdigit(*it)) {
                first_digit = *it;
                break;
            }
        }
        // start from end
        for (auto it = line.rbegin(); it != line.rend(); ++it) {
            if (std::isdigit(*it)) {
                last_digit = *it;
                break;
            }
        }
        result += (first_digit - '0') * 10 + (last_digit - '0');
    }
    file.close();
    std::cout << "result equals to: " << result << std::endl;
    return 0;
}