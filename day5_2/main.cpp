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

inline void accumulate_digit(uint& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

bool compare_ranges(Range& left, Range& right) {
    return left.start < right.start;
}

int main() {
    std::ifstream file("../day5_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    uint digit = 0;
    bool digit_finished = false;
    std::vector<Range> current_ranges;
    std::vector<Range> current_ranges2;
    std::vector<Range>* current_ranges_ptr = nullptr;
    std::string line;
    bool first = true;
    if (std::getline(file, line)) {
        // collect initial ranges (seeds)
        for (int i = 7; i < line.size(); ++i) {
            char letter = line[i];
            if (std::isdigit(letter)) {
                accumulate_digit(digit, letter);
                if (i + 1 == line.size()) digit_finished = true;
            } else {
                digit_finished = true;
            }
            if (digit_finished) {
                if (first) {
                    current_ranges.emplace_back(digit, 0, 0);
                } else {
                    auto& r = current_ranges.back();
                    r.finish = r.start + digit;
                }
                first = !first;
                digit = 0;
                digit_finished = false;
            }
        }
        // main parsing
        std::getline(file, line); // skip first empty line
        std::vector<Range> ranges;
        std::sort(current_ranges.begin(), current_ranges.end(), compare_ranges); // ascending order
        current_ranges_ptr = &current_ranges;
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
                // ugly ranges split
                std::vector<Range>& next_ranges = current_ranges_ptr == &current_ranges ? current_ranges2 : current_ranges;
                for (auto& r : *current_ranges_ptr) {
                    bool fully_splited = false;
                    for (int i = 0; i < ranges.size(); ++i) {
                        auto& range = ranges[i];
                        if (r.start < range.start) { // range starts to the left
                            if (r.finish >= range.start) {
                                next_ranges.emplace_back(r.start, (range.start == 0 ? 0 : range.start - 1), 0);
                                r.start = range.start;
                                --i;
                            } else {
                                next_ranges.emplace_back(r.start, r.finish, 0);
                                fully_splited = true;
                                break;
                            }
                        } else if (r.start >= range.start) {
                            if (r.finish >= range.finish) {
                                if (range.finish > r.start) {
                                    uint start = range.destination + (r.start - range.start);
                                    next_ranges.emplace_back(start, start + (range.finish - r.start), 0);
                                    r.start = range.finish + 1; // range.finish is included
                                    if (r.start >= r.finish) {
                                        fully_splited = true;
                                        break;
                                    }
                                }
                            } else {
                                uint start = range.destination + (r.start - range.start);
                                next_ranges.emplace_back(start, start + (r.finish - r.start), 0);
                                fully_splited = true;
                                break; // fully splited range    
                            }
                        }
                    }
                    if (!fully_splited) {
                        next_ranges.emplace_back(r.start, r.finish, 0);
                    }
                }
                current_ranges_ptr->clear();
                current_ranges_ptr = &next_ranges;
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
                        else {
                            range.finish = range.start + digit;
                            if (range.finish > 0) range.finish -= 1;
                        }
                        digit = 0;
                        digit_finished = false;
                        ++digit_index;
                    }
                }
            }
        } while (parse);
    }
    std::sort(current_ranges_ptr->begin(), current_ranges_ptr->end(), compare_ranges); // ascending order
    uint result = (*current_ranges_ptr)[0].start;
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}