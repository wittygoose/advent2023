#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <map>
#include "../include/Performance.hpp"

enum HandPower {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

struct Hand {
    std::array<char, 5> cards;
    int bid;
    HandPower power = HighCard;
};

inline void accumulate_digit(int& digit, char ch) {
    digit = digit * 10 + (ch - '0');
}

bool compare_hand(Hand& left, Hand& right) {
    if (left.power == right.power) {
        for (int i = 0; i < left.cards.size(); ++i) {
            char& left_card = left.cards[i];
            char& right_card = right.cards[i];
            if (left_card == right_card) continue;
            else return left_card < right_card;
        }
    }
    return left.power < right.power;
}

int main() {
    std::ifstream file("../day7_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    std::vector<Hand> hands;
    int result = 0;
    std::map<char, int> counts;
    for (std::string line; std::getline(file, line);) {
        int bid = 0;
        bool collect_bid = false;
        counts.clear();
        hands.emplace_back();
        Hand& hand = hands.back();
        for (int i = 0; i < line.size(); ++i) {
            char letter = line[i];
            if (collect_bid) {
                accumulate_digit(bid, letter);
            } else if (letter == ' ') {
                collect_bid = true;
                bid = 0;
            } else {
                if (letter == 'A') letter = 'Z';
                else if (letter == 'K') letter = 'Y';
                else if (letter == 'Q') letter = 'X';
                else if (letter == 'J') letter = 'W';
                else if (letter == 'T') letter = 'V';
                hand.cards[bid] = letter;
                counts[letter]++;
                ++bid;
            }
        }
        hand.bid = bid;
        // calculate power (consider different approach like sorted array)
        int pairs = 0;
        bool three = false, four = false, five = false;
        for (auto& pair : counts) {
            if (pair.second == 2) ++pairs;
            else if (pair.second == 3) three = true;
            else if (pair.second == 4) four = true;
            else if (pair.second == 5) five = true;
        }
        if (five) hand.power = HandPower::FiveOfAKind; 
        else if (four) hand.power = HandPower::FourOfAKind;
        else if (three && pairs > 0) hand.power = HandPower::FullHouse;
        else if (three) hand.power = HandPower::ThreeOfAKind;
        else if (pairs > 1) hand.power = HandPower::TwoPair;
        else if (pairs > 0) hand.power = HandPower::OnePair;
    }
    std::sort(hands.begin(), hands.end(), compare_hand);
    int rank = 1;
    for (auto& hand : hands) {
        result += hand.bid * rank;
        ++rank;
    }
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}