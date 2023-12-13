#include <iostream>
#include <fstream>
#include <string>
#include "../include/Performance.hpp"

inline void accumulate_digit(uint64_t& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

inline uint64_t divide_and_ceil(uint64_t a, uint64_t b) {
    return (a + b - 1) / b;
}

inline void accumulate_digit_from_line(uint64_t& digit, std::string& line) {
    digit = 0;
    for (int i = 11; i < line.size(); ++i) {
        char letter = line[i];
        if (std::isdigit(letter)) {
            accumulate_digit(digit, letter);
        }
    }
}

int main() {
    std::ifstream file("../day6_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    std::string line;
    uint64_t time;
    uint64_t distance;
    if (std::getline(file, line)) accumulate_digit_from_line(time, line);
    if (std::getline(file, line)) accumulate_digit_from_line(distance, line);
    uint64_t min_time = 0;
    while (min_time * (time - min_time) <= distance) {
        min_time = divide_and_ceil(distance, time - min_time);
    }
    uint64_t max_time = time - min_time;
    uint64_t result = max_time - min_time + 1;
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}