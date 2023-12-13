#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/Performance.hpp"

inline void accumulate_digit(uint& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

inline uint divide_and_ceil(uint a, uint b) {
    return (a + b - 1) / b;
}

void fill_vector_with_values_from_line(std::vector<uint>& v, std::string& line) {
    uint digit = 0;
    bool digit_finished = false;
    for (int i = 11; i < line.size(); ++i) {
        char letter = line[i];
        if (std::isdigit(letter)) {
            accumulate_digit(digit, letter);
            if (i + 1 == line.size()) digit_finished = true;
        } else if (digit > 0) {
            digit_finished = true;
        }
        if (digit_finished) {
            v.push_back(digit);
            digit_finished = false;
            digit = 0;
        }
    }
}

int main() {
    std::ifstream file("../day6_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    uint result = 1; // :)
    std::vector<uint> times;
    std::vector<uint> distances;
    std::string line;
    if (std::getline(file, line)) fill_vector_with_values_from_line(times, line);
    if (std::getline(file, line)) fill_vector_with_values_from_line(distances, line);
    for (int i = 0; i < times.size(); ++i) {
        uint time = times[i];
        uint distance = distances[i];
        uint min_time = 0;
        while (min_time * (time - min_time) <= distance) {
            min_time = divide_and_ceil(distance, time - min_time);
        }
        uint max_time = time - min_time;
        uint variants = max_time - min_time + 1;
        result *= variants;
    }
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}