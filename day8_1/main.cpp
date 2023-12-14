#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
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
    std::ifstream file("../day8_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    std::string directions;
    if (std::getline(file, directions)) {
        std::string line;
        std::getline(file, line); // empty line
        std::unordered_map<std::string, Node> nodes;
        // parse whole file and emplace nodes in map
        while (std::getline(file, line)) {
            auto result = nodes.emplace(std::make_pair(line.substr(0, 3), Node(line.substr(7, 3), line.substr(12, 3))));
            Node& node = result.first->second;
            node.is_finish = result.first->first == "ZZZ";
        }
        // link parsed nodes
        for (auto& pair : nodes) {
            auto left_it = nodes.find(pair.second.left_key);
            pair.second.left = &(left_it->second); // skip safety check
            auto right_it = nodes.find(pair.second.right_key);
            pair.second.right = &(right_it->second); // skip safety check
        }
        // count moves
        Node* current_node = &(nodes.find("AAA")->second);
        for (int i = 0; i < directions.size(); ++i) {
            if (directions[i] == 'R') current_node = current_node->right;
            else current_node = current_node->left;
            ++result;
            if (current_node->is_finish) break;
            else if (i + 1 == directions.size()) i = -1;
        }
    }
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}