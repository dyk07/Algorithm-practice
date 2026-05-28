#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

static const int LIMIT_INSERTION = 10;

static inline void swap_with_pos(int array[], int pos[], int i, int j) {
    if (i == j) {
        return;
    }
    int ai = array[i];
    int aj = array[j];
    array[i] = aj;
    array[j] = ai;
    pos[ai] = j;
    pos[aj] = i;
}

static void insertionSort(int array[], int pos[], int L, int R) {
    for (int i = L + 1; i < R; ++i) {
        for (int j = i; j > L; --j) {
            if (array[j] < array[j - 1]) {
                swap_with_pos(array, pos, j, j - 1);
            } else {
                break;
            }
        }
    }
}

static int select_pivot_value(int L, int R, int kPercent) {
    return L + (R - L) * kPercent / 100;
}

static int partition_by_value(int array[], int pos[], int lo, int hi, int pivotValue) {
    int pivotIndex = pos[pivotValue];
    swap_with_pos(array, pos, lo, pivotIndex);

    int mi = lo;
    for (int i = lo + 1; i < hi; ++i) {
        if (array[i] < pivotValue) {
            swap_with_pos(array, pos, ++mi, i);
        }
    }
    swap_with_pos(array, pos, lo, mi);
    return mi;
}

static void quicksort_by_rank(int array[], int pos[], int lo, int hi, int L, int R, int kPercent) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    if (n <= LIMIT_INSERTION) {
        insertionSort(array, pos, lo, hi);
        return;
    }

    int pivotValue = select_pivot_value(L, R, kPercent);
    int mi = partition_by_value(array, pos, lo, hi, pivotValue);

    if (mi - lo > 1) {
        quicksort_by_rank(array, pos, lo, mi, L, pivotValue - 1, kPercent);
    }
    if (hi - (mi + 1) > 1) {
        quicksort_by_rank(array, pos, mi + 1, hi, pivotValue + 1, R, kPercent);
    }
}

static double run_once(const std::vector<int> &data, int kPercent, bool verify) {
    std::vector<int> work = data;
    std::vector<int> pos(work.size() + 1);
    for (int i = 0; i < static_cast<int>(work.size()); ++i) {
        pos[work[i]] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    quicksort_by_rank(work.data(), pos.data(), 0, static_cast<int>(work.size()), 1,
                      static_cast<int>(work.size()), kPercent);
    auto end = std::chrono::high_resolution_clock::now();

    if (verify && !std::is_sorted(work.begin(), work.end())) {
        std::cerr << "Sort failed for k=" << kPercent << "\n";
        std::exit(1);
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    const int n = 200000;
    const int trials = 5;
    const uint32_t seed = 246813579u;
    const std::vector<int> ks = {10, 20, 30, 40, 50, 60, 90};

    std::mt19937 rng(seed);

    std::vector<std::vector<int>> datasets;
    datasets.reserve(trials);
    for (int t = 0; t < trials; ++t) {
        std::vector<int> data(n);
        for (int i = 0; i < n; ++i) {
            data[i] = i + 1;
        }
        std::shuffle(data.begin(), data.end(), rng);
        datasets.push_back(std::move(data));
    }

    const char *csvName = "pivot_rank_benchmark.csv";
    std::ofstream out(csvName);
    if (!out) {
        std::cerr << "Failed to open output file: " << csvName << "\n";
        return 1;
    }
    out << "n,k,avg_ms\n";

    std::cout << "Benchmarking pivot rank selection...\n";
    std::cout << "n=" << n << ", trials=" << trials << "\n";

    bool verify = true;
    for (int k : ks) {
        double totalMs = 0.0;
        for (int t = 0; t < trials; ++t) {
            totalMs += run_once(datasets[t], k, verify);
            verify = false;
        }
        double avgMs = totalMs / trials;
        out << n << "," << k << "," << avgMs << "\n";
        std::cout << "k=" << k << ": " << avgMs << " ms\n";
    }

    std::cout << "CSV saved to " << csvName << "\n";
    return 0;
}
