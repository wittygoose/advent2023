#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "../include/Performance.hpp"

int main() {
    std::ifstream file("../day1_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
    }
    std::array<std::string, 9> string_digits = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    std::array<char, 9> char_digits = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const int min_string_digit_size = 3; // hardcoded for simplicity
    const int max_backward_overlap = 1; // hardcoded for simplicity
    int result = 0;
    Performance perf;
    for (std::string line; std::getline(file, line);) {
        char first_digit = '0', last_digit = '0';
        int last_index_for_string_digit = (int)line.size() - min_string_digit_size; 
        int first_digit_index = 0;
        // forward
        for (int i = 0; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                first_digit = letter;
                first_digit_index = i;
                break;
            } else if (i <= last_index_for_string_digit) {
                for (int j = 0; j < string_digits.size(); ++j) {
                    std::string& digit = string_digits[j];
                    for (int k = 0; k < digit.size(); ++k) {
                        char ch = digit[k];
                        if (ch != line[i + k]) {
                            break;
                        } else if (k + 1 == digit.size()) {
                            first_digit = char_digits[j];
                            first_digit_index = i;
                            break;
                        }
                    }
                    if (first_digit != '0') {
                        break;
                    }
                }
                if (first_digit != '0') {
                    break;
                }
            }
        }
        // backward
        first_digit_index = std::max<int>(0, first_digit_index + 1 - max_backward_overlap);
        last_index_for_string_digit = first_digit_index + min_string_digit_size;
        for (int i = line.size() - 1; i >= first_digit_index; --i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                last_digit = letter;
                break;
            } else if (i >= last_index_for_string_digit) {
                for (int j = 0; j < string_digits.size(); ++j) {
                    std::string& digit = string_digits[j];
                    for (int k = digit.size() - 1, l = 0; k >= 0; --k, ++l) {
                        char ch = digit[k];
                        if (ch != line[i - l]) {
                            break;
                        } else if (k == 0) {
                            last_digit = char_digits[j];
                            break;
                        }
                    }
                    if (last_digit != '0') {
                        break;
                    }
                }
                if (last_digit != '0') {
                    break;
                }
            }
        }
        result += (first_digit - '0') * 10 + (last_digit - '0');
    }
    file.close();
    std::cout << "result equals to: " << result << std::endl;
    return 0;
}