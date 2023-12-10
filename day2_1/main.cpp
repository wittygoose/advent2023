#include <iostream>
#include <fstream>
#include <string>
#include "../include/Performance.hpp"

// first -> max_value; second -> word length
std::pair<int, int> red = {12, 3};
std::pair<int, int> green = {13, 5};
std::pair<int, int> blue = {14, 4};
std::pair<int, int> unsupported = {0, 999};

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
    std::ifstream file("../day2_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    for (std::string line; std::getline(file, line);) {
        int game_id = 0;
        int i = 5;
        // find game_id first
        for (;; ++i) {
            char letter = line[i];
            if (letter == ':') break;
            else accumulate_digit(game_id, letter);
        }
        // parse the rest
        int color_digit = 0;
        i += 2;
        for (; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                accumulate_digit(color_digit, letter);
            } else { // space
                ++i;
                std::pair<int, int>& color = get_color_data_by_char(line[i]);
                if (color.first < color_digit) {
                    game_id = 0;
                    break;
                } else {
                    color_digit = 0;
                    i += color.second + 1;
                }
            }
        }
        // add result
        result += game_id;
    }
    file.close();
    std::cout << "result equals to: " << result << std::endl;
    return 0;
}