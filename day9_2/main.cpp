#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/Performance.hpp"

inline void accumulate_digit(int64_t& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

int main() {
    std::ifstream file("../day9_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int64_t result = 0;
    for (std::string line; std::getline(file, line);) {
        std::vector<std::vector<int64_t>> data;
        data.emplace_back();
        std::vector<int64_t>& root = data.back();
        bool digit_finished = false;
        int64_t digit = 0;
        int sign = 1;
        // parse line and fill root
        for (int i = 0; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                accumulate_digit(digit, letter);
                if (i + 1 == line.size()) digit_finished = true;
            } else if (letter == '-') {
                sign = -1;
            } else {
                digit_finished = true;
            }
            if (digit_finished) {
                digit_finished = false;
                root.push_back(digit * sign);
                sign = 1;
                digit = 0;
            }
        }
        // go down
        int depth = 0;
        while (true) {
            data.emplace_back();
            std::vector<int64_t>& current = data[depth];
            std::vector<int64_t>& next = data.back();
            next.resize(current.size());
            int limit = depth;
            bool all_zeros = true;
            for (int i = current.size() - 1; i > limit; --i) {
                int64_t v = current[i] - current[i - 1];
                if (v != 0) {
                    all_zeros = false;
                    next[i] = v;
                }
            }
            if (all_zeros) {
                break;
            } else {
                ++depth;
            }
        }
        // go up
        while (depth >= 0) {
            std::vector<int64_t>& current = data[depth];
            std::vector<int64_t>& prev = data[depth + 1];
            int index = depth;
            int64_t value = current[index] - prev[index + 1];
            if (depth == 0) digit = value;
            else current[index] = value;
            --depth;
        }
        result += digit;
    }
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}