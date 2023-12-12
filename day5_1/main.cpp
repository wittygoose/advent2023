#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../include/Performance.hpp"

struct Range {
    uint start;
    uint finish;
    uint destination;
    Range(uint s, uint f, uint d) : start(s), finish(f), destination(d) {}
};

void accumulate_digit(uint& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

bool compare_ranges(Range& left, Range& right) {
    return left.start < right.start;
}

Range* get_range(std::vector<Range>& ranges, uint value) {
    for (auto& r : ranges) {
        if (value < r.start) {
            return nullptr;
        } else if (value >= r.start && value <= r.finish) {
            return &r;
        }
    }
    return nullptr;
}

int main() {
    std::ifstream file("../day5_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    uint digit = 0;
    bool digit_finished = false;
    std::vector<uint> current_values;
    std::string line;
    if (std::getline(file, line)) {
        // collect initial values (seeds)
        for (int i = 7; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                accumulate_digit(digit, letter);
                if (i + 1 == line.size()) digit_finished = true;
            } else {
                digit_finished = true;
            }
            if (digit_finished) {
                current_values.push_back(digit);
                digit = 0;
                digit_finished = false;
            }
        }
        // main parsing
        std::getline(file, line); // skip first empty line
        std::vector<Range> ranges;
        bool parse = true;
        bool new_ranges = true;
        do {
            if (!std::getline(file, line)) {
                parse = false;
                line.clear();
            }
            if (line.empty()) {
                new_ranges = true;
                // sort ascending by start
                std::sort(ranges.begin(), ranges.end(), compare_ranges);
                // replace values by using range
                for (int i = 0; i < current_values.size(); ++i) {
                    uint& value = current_values[i];
                    auto* range = get_range(ranges, value);
                    if (range != nullptr) {
                        value = range->destination + (value - range->start);
                    }
                }
            } else {
                if (new_ranges) {
                    new_ranges = false;
                    ranges.clear();
                    continue; // skip ranges header
                }
                digit = 0;
                digit_finished = false;
                int digit_index = 0;
                ranges.emplace_back(0, 0, 0);
                Range& range = ranges.back();
                for (int i = 0; i < line.size(); ++i) {
                    char letter = line[i];
                    if (std::isdigit(letter)) {
                        accumulate_digit(digit, letter);
                        if (i + 1 == line.size()) digit_finished = true;
                    } else {
                        digit_finished = true;
                    }
                    if (digit_finished) {
                        if (digit_index == 0) range.destination = digit;
                        else if (digit_index == 1) range.start = digit;
                        else range.finish = range.start + digit - 1;
                        digit = 0;
                        digit_finished = false;
                        ++digit_index;
                    }
                }
            }
        } while (parse);
    }
    std::sort(current_values.begin(), current_values.end());
    uint result = current_values[0];
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}