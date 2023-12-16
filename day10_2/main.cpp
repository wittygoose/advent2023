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
    bool is_vertical() {
        return y != 0;
    }
    bool is_horizontal() {
        return x != 0;
    }
};

Direction left(-1, 0, {'-', 'L', 'F'});
Direction right(1, 0, {'-', 'J', '7'});
Direction up(0, -1, {'|', '7', 'F'});
Direction down(0, 1, {'|', 'J', 'L'});

std::vector<Direction*> directions = {&left, &right, &up, &down};

struct Tile {
    char pipe;
    bool is_main_loop = false;
    Tile(char& pipe_) : pipe(pipe_) {}
};

struct Rat {
    int x = 0;
    int y = 0;
    int distance = 0;
    Direction* dir = nullptr;
};

void proceed(Rat& rat, std::vector<std::vector<Tile>>& map) {
    Direction& dir = *rat.dir;
    rat.x += dir.x;
    rat.y += dir.y;
    rat.distance++;
    Tile& tile = map[rat.y][rat.x];
    tile.is_main_loop = true;
    // change direction
    switch (tile.pipe) {
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
    std::ifstream file("../day10_2/input.txt");
    if (!file.is_open()) {
        std::cerr << "failed to open input.txt" << std::endl;
        return 1;
    }
    Performance perf;
    int result = 0;
    std::vector<std::vector<Tile>> map;
    std::pair<int, int> start;
    bool start_found = false;
    int map_width;
    for (std::string line; std::getline(file, line);) {
        map.emplace_back();
        std::vector<Tile>& row = map.back();
        int y = map.size() - 1;
        for (int i = 0; i < line.size(); ++i) {
            char letter = line[i];
            row.emplace_back(letter);
            if (!start_found && letter == 'S') {
                start.first = i;
                start.second = y;
                map_width = line.size();
                start_found = true;
            }
        }
    }
    // make first move
    Rat rat;
    Direction* start_dir = nullptr;
    for (auto* dir : directions) {
        int x = start.first + dir->x;
        int y = start.second + dir->y;
        if (x >= 0 && x < map_width && y >= 0 && y < map.size()) {
            Tile& neighbor = map[y][x];
            if (dir->is_allowed_pipe(neighbor.pipe)) {
                start_dir = dir;
                rat.dir = dir;
                rat.x = start.first;
                rat.y = start.second;
                proceed(rat, map);
                break;
            }
        }
    }
    // mark main loop
    while (rat.x != start.first || rat.y != start.second) {
        proceed(rat, map);
    }
    // replace start tile with corret pipe
    Tile& start_tile = map[start.second][start.first];
    if (start_dir->is_horizontal()) {
        if (rat.dir->is_horizontal()) {
            start_tile.pipe = '-';
        } else {
            if (rat.dir == &up) {
                start_tile.pipe = start_dir->x > 0 ? 'F' : '7';
            } else {
                start_tile.pipe = start_dir->x > 0 ? 'L' : 'J';
            }
        }
    } else {
        if (rat.dir->is_vertical()) {
            start_tile.pipe = '|';
        } else {
            if (rat.dir == &right) {
                start_tile.pipe = start_dir->y > 0 ? '7' : 'J';
            } else {
                start_tile.pipe = start_dir->y > 0 ? 'F' : 'L';
            }
        }
    }
    // remove all garbage making it a dot so that only pipes from main loop will remain
    for (int i = 0; i < map.size(); ++i) {
        std::vector<Tile>& row = map[i];
        for (int j = 0; j < row.size(); ++j) {
            Tile& tile = row[j];
            if (!tile.is_main_loop) {
                tile.pipe = '.';
            }
        }
    }
    // find everything outside
    bool within = false;
    bool up = false;
    int outside = rat.distance;
    for (int i = 0; i < map.size(); ++i) {
        std::vector<Tile>& row = map[i];
        for (int j = 0; j < row.size(); ++j) {
            Tile& tile = row[j];
            if (tile.pipe == '|') {
                within = !within;
            } else if (tile.pipe == 'L') {
                up = true;
            } else if (tile.pipe == 'F') {
                up = false;
            } else if (tile.pipe == '7' || tile.pipe == 'J') {
                char reverse = up ? '7' : 'J';
                if (tile.pipe == reverse) {
                    within = !within;
                }
                up = false;
            }
            if (!within && !tile.is_main_loop) {
                ++outside;
            }
        }
    }
    int map_size = map.size() * map_width;
    result = map_size - outside;
    file.close();
    std::cout << "resualt equals to: " << result << std::endl;
    return 0;
}