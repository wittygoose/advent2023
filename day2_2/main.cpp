#include <iostream>
#include <fstream>
#include <string>
#include "../include/Performance.hpp"

// first -> current max_value; second -> word length
std::pair<int, int> red = {0, 3};
std::pair<int, int> green = {0, 5};
std::pair<int, int> blue = {0, 4};
std::pair<int, int> unsupported = {0, 999};

inline void clear_colors() {
    red.first = green.first = blue.first = 0;
}

inline void accumulate_digit(int& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

inline std::pair<int, int>& get_color_data_by_char(char ch) {
    switch (ch) {
        case 'r':
            return red;
        case 'g':
            return green;
        case 'b':
            return blue;
        default:
            std::cout << ch << " is unsupported color char" << std::endl;
            return unsupported;
    }
}

int main() {
    std::ifstream file("../day2_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    for (std::string line; std::getline(file, line);) {
        // find body index
        int i = 6;
        for (;; ++i) {
            char letter = line[i];
            if (letter == ':') break;
        }
        i += 2;
        // parse body
        clear_colors();
        int digit = 0;
        for (; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                accumulate_digit(digit, letter);
            } else {
                ++i;
                std::pair<int, int>& color = get_color_data_by_char(line[i]);
                if (digit > color.first) {
                    color.first = digit;
                }
                digit = 0;
                i += color.second + 1;
            }
        }
        result += red.first * green.first * blue.first;
    }
    file.close();
    std::cout << "result equals to: " << result << std::endl;
    return 0;
}