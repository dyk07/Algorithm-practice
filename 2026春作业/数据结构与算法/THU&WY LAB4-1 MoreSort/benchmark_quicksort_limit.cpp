#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "classical_quicksort.cpp"

static double run_once(const std::vector<int> &data, int limit, unsigned seed, bool verify) {
	std::vector<int> work = data;
	LIMIT_INSERTION = limit;
	std::srand(seed);

	auto start = std::chrono::high_resolution_clock::now();
	classic_quicksort(work.data(), 0, static_cast<int>(work.size()));
	auto end = std::chrono::high_resolution_clock::now();

	if (verify && !std::is_sorted(work.begin(), work.end())) {
		std::cerr << "Sort failed for limit=" << limit << "\n";
		std::exit(1);
	}

	std::chrono::duration<double, std::milli> elapsed = end - start;
	return elapsed.count();
}

int main() {
	const int n = 200000;
	const int trials = 8;
	const int minLimit = 0;
	const int maxLimit = 64;
	const int step = 4;
	const uint32_t seed = 123456789u;

	std::mt19937 rng(seed);
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

	const std::string csvName = "limit_benchmark.csv";
	std::ofstream out(csvName);
	if (!out) {
		std::cerr << "Failed to open output file: " << csvName << "\n";
		return 1;
	}
	out << "limit,avg_ms\n";

	std::cout << "Benchmarking quicksort insertion limit...\n";
	std::cout << "n=" << n << ", trials=" << trials << "\n";
	std::cout << "Results (limit, avg_ms):\n";

	bool verify = true;
	for (int limit = minLimit; limit <= maxLimit; limit += step) {
		double totalMs = 0.0;
		for (int t = 0; t < trials; ++t) {
			unsigned runSeed = static_cast<unsigned>(seed + limit * 1000 + t);
			totalMs += run_once(datasets[t], limit, runSeed, verify);
			verify = false;
		}
		double avgMs = totalMs / trials;
		out << limit << "," << avgMs << "\n";
		std::cout << limit << ", " << avgMs << "\n";
	}

	std::cout << "CSV saved to " << csvName << "\n";
	return 0;
}
