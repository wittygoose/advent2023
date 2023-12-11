#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "../include/Performance.hpp"

enum Direction {left, right, top_left, top_right, bottom_left, bottom_right};
std::array<Direction, 6> directions = {left, right, top_left, top_right, bottom_left, bottom_right};

inline void accumulate_digit(int& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

inline void try_accumulate_digit_horizontally(int& digit, std::string& line, int start_index, int direction, bool change_direction, int& digit_length_after_changing_direction) {
    digit_length_after_changing_direction = 0;
    int digit_length = 0;
    int digit_start_index = 9999;
    bool direction_changed = false;
    for (int i = start_index; i >= 0 && i < line.size(); i += direction) {
        char letter = line[i];
        if (std::isdigit(letter)) {
            ++digit_length;
            if (direction_changed) ++digit_length_after_changing_direction;
            if (digit_start_index > i) digit_start_index = i;
        } else {
            if (change_direction) {
                change_direction = false;
                direction_changed = true;
                direction *= -1;
                i = start_index;
            } else {
                break;
            }
        }
    }
    while (digit_length > 0) {
        accumulate_digit(digit, line[digit_start_index]);
        ++digit_start_index;
        --digit_length;
    }
}

int main() {
    std::ifstream file("../day3_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    int digit_counter = 0;
    int direction_counter = 0;
    int digit_length_after_changing_direction = 0;
    bool parse = true;
    std::array<int, 3> digits = {0, 0, 0};
    std::string prev_line, curr_line, next_line;
    if (std::getline(file, curr_line)) {
        do {
            if (!std::getline(file, next_line)) {
                parse = false;
                next_line.clear();
            }
            for (int i = 0; i < curr_line.size(); ++i) {
                char letter = curr_line[i];
                if (letter == '*') {
                    digits[0] = digits[1] = digits[2] = 0;
                    digit_counter = direction_counter = digit_length_after_changing_direction = 0;
                    while (digit_counter <= 2 && direction_counter < directions.size()) {
                        int& digit = digits[digit_counter];
                        while (direction_counter < directions.size()) {
                            Direction& dir = directions[direction_counter];
                            switch (dir) {
                                case left:
                                    try_accumulate_digit_horizontally(digit, curr_line, i - 1, -1, false, digit_length_after_changing_direction);
                                    break;
                                case right:
                                    try_accumulate_digit_horizontally(digit, curr_line, i + 1, 1, false, digit_length_after_changing_direction);
                                    break;
                                case top_left:
                                    try_accumulate_digit_horizontally(digit, prev_line, i - 1, -1, true, digit_length_after_changing_direction);
                                    break;
                                case top_right:
                                    // since we know the exact order of directions we can use such condition
                                    if (digit_length_after_changing_direction < 1) {
                                        try_accumulate_digit_horizontally(digit, prev_line, i + 1, 1, true, digit_length_after_changing_direction);
                                    }
                                    break;
                                case bottom_left:
                                    try_accumulate_digit_horizontally(digit, next_line, i - 1, -1, true, digit_length_after_changing_direction);
                                    break;
                                case bottom_right:
                                    // since we know the exact order of directions we can use such condition
                                    if (digit_length_after_changing_direction < 1) {
                                        try_accumulate_digit_horizontally(digit, next_line, i + 1, 1, true, digit_length_after_changing_direction);
                                    }
                                    break;
                            }
                            ++direction_counter;
                            if (digit > 0) break;
                        }
                        if (digit > 0) {
                            ++digit_counter;
                        }
                    }
                    // found exactly 2 digits adjacent to *
                    if (digit_counter == 2) {
                        result += digits[0] * digits[1];
                    }
                }
            }
            prev_line = std::move(curr_line);
            curr_line = std::move(next_line);
        } while (parse);
    }
    file.close();
    std::cout << "result equals to: " << result << std::endl;
    return 0;
}