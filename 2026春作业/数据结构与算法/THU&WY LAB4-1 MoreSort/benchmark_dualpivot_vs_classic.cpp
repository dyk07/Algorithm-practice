#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "classical_quicksort.cpp"
#include "DualPivotQsort.cpp"

static double time_classic(const std::vector<int> &data, unsigned seed) {
    std::vector<int> work = data;
    std::srand(seed);
    auto start = std::chrono::high_resolution_clock::now();
    classic_quicksort(work.data(), 0, static_cast<int>(work.size()));
    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(work.begin(), work.end())) {
        std::cerr << "classic_quicksort failed\n";
        std::exit(1);
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

static double time_dual(const std::vector<int> &data, unsigned seed) {
    std::vector<int> work = data;
    std::srand(seed);
    auto start = std::chrono::high_resolution_clock::now();
    dual_pivot_quicksort(work.data(), 0, static_cast<int>(work.size()));
    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(work.begin(), work.end())) {
        std::cerr << "dual_pivot_quicksort failed\n";
        std::exit(1);
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    const std::vector<int> sizes = {20000, 50000, 100000, 200000};
    const int trials = 5;
    const uint32_t seed = 987654321u;

    LIMIT_INSERTION = 10;
    DUAL_LIMIT_INSERTION = 10;

    const std::string csvName = "dual_pivot_vs_classic.csv";
    std::ofstream out(csvName);
    if (!out) {
        std::cerr << "Failed to open output file: " << csvName << "\n";
        return 1;
    }
    out << "n,classic_ms,dual_ms\n";

    std::cout << "Benchmarking classic vs dual-pivot quicksort...\n";
    std::cout << "trials=" << trials << "\n";

    for (int n : sizes) {
        std::mt19937 rng(seed + static_cast<uint32_t>(n));
        std::uniform_int_distribution<int> dist(-1000000000, 1000000000);

        std::vector<std::vector<int>> datasets;
        datasets.reserve(trials);
        for (int t = 0; t < trials; ++t) {
            std::vector<int> data(n);
            for (int i = 0; i < n; ++i) {
                data[i] = dist(rng);
            }
            datasets.push_back(std::move(data));
        }

        double totalClassic = 0.0;
        double totalDual = 0.0;
        for (int t = 0; t < trials; ++t) {
            unsigned runSeed = static_cast<unsigned>(seed + n * 1000 + t);
            totalClassic += time_classic(datasets[t], runSeed);
            totalDual += time_dual(datasets[t], runSeed);
        }

        double avgClassic = totalClassic / trials;
        double avgDual = totalDual / trials;
        out << n << "," << avgClassic << "," << avgDual << "\n";
        std::cout << "n=" << n << ": classic=" << avgClassic << " ms, dual=" << avgDual << " ms\n";
    }

    std::cout << "CSV saved to " << csvName << "\n";
    return 0;
}
