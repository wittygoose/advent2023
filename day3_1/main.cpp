#include <iostream>
#include <fstream>
#include <string>
#include "../include/Performance.hpp"

inline void accumulate_digit(int& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

inline bool is_valid_symbol(char ch) {
    return ch != '.' && !std::isdigit(ch);
}

inline bool check_line_for_adjacent_symbols(std::string& line, int digit_start_index, int digit_length) {
    int final_index = std::min<int>(digit_start_index + digit_length + 1, line.size());
    for (int j = std::max<int>(0, digit_start_index - 1); j < final_index; ++j) {
        if (is_valid_symbol(line[j])) {
            return true;
        }
    }
    return false;
}

int main() {
    std::ifstream file("../day3_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    int digit = 0;
    int digit_start_index = -1;
    int digit_length = 0;
    bool is_valid_digit = false;
    bool parse = true;
    std::string prev_line, curr_line, next_line;
    if (std::getline(file, curr_line)) {
        do {
            if (!std::getline(file, next_line)) {
                parse = false;
                next_line.clear();
            }
            for (int i = 0; i < curr_line.size(); ++i) {
                char letter = curr_line[i];
                bool digit_finished = false;
                if (std::isdigit(letter)) {
                    if (digit_start_index == -1) {
                        digit_start_index = i;
                        digit_length = 0;
                        digit = 0;
                        is_valid_digit = false;
                        // if prev letter was a valid symbol then digit is valid
                        if (i > 0 && is_valid_symbol(curr_line[i - 1])) {
                            is_valid_digit = true;
                        }
                    }
                    ++digit_length;
                    accumulate_digit(digit, letter);
                    // last letter on the line
                    if (i + 1 == curr_line.size()) {
                        digit_finished = true;
                    }
                } else if (digit_start_index != -1) {
                    digit_finished = true;
                }
                if (digit_finished) {
                    // check current symbol
                    if (!is_valid_digit) {
                        is_valid_digit = is_valid_symbol(letter);
                    }
                    // left and right symbol from digit was . now we have to check prev and next lines
                    if (!is_valid_digit) {
                        // prev line
                        is_valid_digit = check_line_for_adjacent_symbols(prev_line, digit_start_index, digit_length);
                        // next line
                        if (!is_valid_digit) {
                            is_valid_digit = check_line_for_adjacent_symbols(next_line, digit_start_index, digit_length);
                        }
                    }
                    digit_start_index = -1;
                    if (is_valid_digit) {
                        result += digit;
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