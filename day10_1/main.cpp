#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "../include/Performance.hpp"

struct Direction {
    int x;
    int y;
    std::array<char, 3> allowed_pipes;
    Direction(int x_, int y_, std::array<char, 3>&& allowed_pipes_) : x(x_), y(y_), allowed_pipes(std::move(allowed_pipes_)) {}
    bool operator==(const Direction& other) const {
        return x == other.x && y == other.y;
    }
    bool is_allowed_pipe(char& pipe) {
        for (char& p : allowed_pipes) {
            if (p == pipe) return true;
        }
        return false;
    }
};

Direction left(-1, 0, {'-', 'L', 'F'});
Direction right(1, 0, {'-', 'J', '7'});
Direction up(0, -1, {'|', '7', 'F'});
Direction down(0, 1, {'|', 'J', 'L'});

std::vector<Direction*> directions = {&left, &right, &up, &down};

struct Rat {
    int x = 0;
    int y = 0;
    int distance = 0;
    Direction* dir = nullptr;
};

inline void proceed(Rat& rat, std::vector<std::string>& map) {
    Direction& dir = *rat.dir;
    rat.x += dir.x;
    rat.y += dir.y;
    rat.distance++;
    char pipe = map[rat.y][rat.x];
    // change direction
    switch (pipe) {
        case 'L':
            rat.dir = dir == left ? &up : &right;
            break;
        case 'J':
            rat.dir = dir == right ? &up : &left;
            break;
        case 'F':
            rat.dir = dir == up ? &right : &down;
            break;
        case '7':
            rat.dir = dir == right ? &down : &left;
            break;
    }
}

int main() {
    std::ifstream file("../day10_1/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    std::vector<std::string> map;
    std::pair<int, int> start;
    bool start_found = false;
    int map_width;
    for (std::string line; std::getline(file, line);) {
        map.push_back(line);
        if (!start_found) {
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == 'S') {
                    start.first = i;
                    start.second = map.size() - 1;
                    start_found = true;
                    map_width = line.size();
                    break;
                }
            }
        }
    }
    // make first move
    Rat rat1;
    Rat rat2;
    std::array<Rat*, 2> rats = {&rat1, &rat2};
    int rat_index = 0;
    for (auto* dir : directions) {
        int x = start.first + dir->x;
        int y = start.second + dir->y;
        if (x >= 0 && x < map_width && y >= 0 && y < map.size()) {
            char pipe = map[y][x];
            if (dir->is_allowed_pipe(pipe)) {
                auto* r = rats[rat_index];
                r->dir = dir;
                r->x = start.first;
                r->y = start.second;
                proceed(*r, map);
                ++rat_index;
                if (rat_index >= rats.size()) break;
            }
        }
    }
    // move rats until they meet
    while (rat1.x != rat2.x || rat1.y != rat2.y) {
        proceed(rat1, map);
        proceed(rat2, map);
    }
    file.close();
    std::cout << "resualt equals to: " << rat1.distance << std::endl;
    return 0;
}