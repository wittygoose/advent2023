#pragma once

#include <iostream>
#include <string>
#include <chrono>

struct Performance {
    Performance() {}

    ~Performance() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<float>(end - start).count();
        std::cout << "time: " << std::to_string(duration) << " seconds" << std::endl;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
};