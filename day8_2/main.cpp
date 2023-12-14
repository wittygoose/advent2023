#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../include/Performance.hpp"

struct Node {
    std::string left_key;
    std::string right_key;
    Node* left = nullptr;
    Node* right = nullptr;
    bool is_finish = false;
    Node(std::string&& left_key_, std::string&& right_key_) : left_key(std::move(left_key_)), right_key(std::move(right_key_)) {}
};

int main() {
    std::ifstream file("../day8_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    uint64_t result = 0;
    std::string directions;
    if (std::getline(file, directions)) {
        std::string line;
        std::getline(file, line); // empty line
        std::unordered_map<std::string, Node> nodes;
        std::vector<Node*> start_nodes;
        // parse whole file and emplace nodes in map
        while (std::getline(file, line)) {
            auto result = nodes.emplace(std::make_pair(line.substr(0, 3), Node(line.substr(7, 3), line.substr(12, 3))));
            Node& node = result.first->second;
            char key_last_letter = result.first->first[2];
            if (key_last_letter == 'Z') node.is_finish = true;
            else if (key_last_letter == 'A') start_nodes.push_back(&node);
        }
        // link parsed nodes
        for (auto& pair : nodes) {
            auto left_it = nodes.find(pair.second.left_key);
            pair.second.left = &(left_it->second); // skip safety check
            auto right_it = nodes.find(pair.second.right_key);
            pair.second.right = &(right_it->second); // skip safety check
        }
        // calculate each way length
        std::vector<int> results;
        int longest_way = 0;
        for (int j = 0; j < start_nodes.size(); ++j) {
            auto* node = start_nodes[j];
            int count = 0;
            for (int i = 0; i < directions.size(); ++i) {
                if (directions[i] == 'R') node = node->right;
                else node = node->left;
                ++count;
                if (node->is_finish) break;
                else if (i + 1 == directions.size()) i = -1;
            }
            if (count > longest_way) longest_way = count;
            results.push_back(count);
        }
        result = longest_way;
        // brute force (todo: think of something smarter)
        while (true) {
            int good = 0;
            for (int& v : results) {
                if (result % v == 0) ++good;
                else break;
            }
            if (good == results.size()) break;
            else result += longest_way;
        }
    }
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}