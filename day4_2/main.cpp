#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <deque>
#include "../include/Performance.hpp"

inline void accumulate_digit(int& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

int main() {
    std::ifstream file("../day4_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    int start_index = 5;
    std::deque<int> queue;
    std::unordered_set<int> winner_digits;
    std::string line;
    if (std::getline(file, line)) {
        // adjust start_index to skip useless stuff at each line
        for (; start_index < line.size(); ++start_index) {
            if (line[start_index] == ':') {
                start_index += 2;
                break;
            }
        }
        do {
            winner_digits.clear();
            int digit = 0;
            int index = 0;
            bool collecting = true;
            bool digit_finished = false;
            int copies = 1;
            if (!queue.empty()) {
                copies += queue.front();
                queue.pop_front();
            }
            for (int i = start_index; i < line.size(); ++i) {
                char letter = line[i];
                if (std::isdigit(letter)) {
                    accumulate_digit(digit, letter);
                    if (i + 1 == line.size()) digit_finished = true;
                } else if (digit > 0) {
                    digit_finished = true;
                } else if (letter == '|') {
                    collecting = false;
                }
                if (digit_finished) {
                    digit_finished = false;
                    if (collecting) {
                        winner_digits.insert(digit);
                    } else {
                        auto it = winner_digits.find(digit);
                        if (it != winner_digits.end()) {
                            ++index;
                            if (queue.size() < index) queue.push_back(copies);
                            else queue[index - 1] += copies;
                            winner_digits.erase(it);
                        }
                    }
                    digit = 0;
                }
            }
            result += copies;
        } while (std::getline(file, line));
    }
    file.close();
    std::cout << "results equals to: " << result << std::endl;
    return 0;
}