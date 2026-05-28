#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "classical_quicksort.cpp"
#include "IntroSort.cpp"

static std::vector<int> build_worst_case_max(int n, unsigned seed) {
    std::vector<int> out(n, -1);
    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i) {
        pos[i] = i;
    }

    std::srand(seed);
    int next = n;
    int lo = 0;
    int hi = n;
    while (hi - lo > 1) {
        int pivotIndex = lo + std::rand() % (hi - lo);
        std::swap(pos[lo], pos[pivotIndex]);
        std::swap(pos[lo], pos[hi - 1]);
        out[pos[hi - 1]] = next--;
        --hi;
    }
    if (n > 0) {
        out[pos[lo]] = next;
    }
    return out;
}

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

static double time_intro(const std::vector<int> &data, unsigned seed) {
    std::vector<int> work = data;
    std::srand(seed);
    auto start = std::chrono::high_resolution_clock::now();
    intro_sort(work.data(), 0, static_cast<int>(work.size()));
    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(work.begin(), work.end())) {
        std::cerr << "intro_sort failed\n";
        std::exit(1);
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main(int argc, char **argv) {
    int n = 50000;
    unsigned seed = 123456789u;
    if (argc >= 2) {
        n = std::atoi(argv[1]);
    }
    if (argc >= 3) {
        seed = static_cast<unsigned>(std::strtoul(argv[2], nullptr, 10));
    }

    std::vector<int> data = build_worst_case_max(n, seed);

    LIMIT_INSERTION = 1;
    INTRO_LIMIT_INSERTION = 1;

    double classicMs = time_classic(data, seed);
    double introMs = time_intro(data, seed);

    const char *csvName = "special_case_results.csv";
    std::ofstream out(csvName);
    if (!out) {
        std::cerr << "Failed to open output file: " << csvName << "\n";
        return 1;
    }
    out << "n,seed,classic_ms,intro_ms\n";
    out << n << "," << seed << "," << classicMs << "," << introMs << "\n";

    std::cout << "n=" << n << ", seed=" << seed << "\n";
    std::cout << "classic_quicksort ms: " << classicMs << "\n";
    std::cout << "intro_sort ms: " << introMs << "\n";
    std::cout << "CSV saved to " << csvName << "\n";
    return 0;
}
