#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<long long> matchA(const std::string& text, const std::vector<std::string>& patterns);
std::vector<long long> matchB(const std::string& text, const std::vector<std::string>& patterns);

struct TestCase {
    std::string name;
    std::string text;
    std::vector<std::string> patterns;
};

std::string random_string(size_t len, const std::string& alphabet, std::mt19937& rng) {
    std::uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);
    std::string s(len, 'a');
    for (size_t i = 0; i < len; ++i) {
        s[i] = alphabet[dist(rng)];
    }
    return s;
}

std::vector<std::string> unique_patterns(size_t count, size_t len, const std::string& alphabet,
                                         std::mt19937& rng) {
    std::unordered_set<std::string> seen;
    seen.reserve(count * 2);
    std::vector<std::string> patterns;
    patterns.reserve(count);
    while (patterns.size() < count) {
        std::string p = random_string(len, alphabet, rng);
        if (seen.insert(p).second) {
            patterns.push_back(std::move(p));
        }
    }
    return patterns;
}

TestCase build_case_a_faster(std::mt19937& rng) {
    const size_t text_len = 100000;
    const size_t pattern_len = 3;
    const size_t pattern_count = 2000;
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

    TestCase tc;
    tc.name = "A_faster_many_short_patterns";
    tc.text = random_string(text_len, alphabet, rng);
    tc.patterns = unique_patterns(pattern_count, pattern_len, alphabet, rng);
    return tc;
}

TestCase build_case_b_faster() {
    const size_t text_len = 100000;
    const size_t pattern_len = 2000;

    TestCase tc;
    tc.name = "B_faster_few_long_patterns";
    tc.text = std::string(text_len, 'a');
    tc.patterns.push_back(std::string(pattern_len, 'a'));
    tc.patterns.push_back(std::string(pattern_len - 1, 'a') + "b");
    return tc;
}

struct RunResult {
    double ms_a = 0.0;
    double ms_b = 0.0;
    bool counts_match = false;
    size_t max_len = 0;
};

RunResult run_one_case(const TestCase& tc) {
    RunResult result;
    for (const auto& p : tc.patterns) {
        result.max_len = std::max(result.max_len, p.size());
    }

    auto start_a = std::chrono::high_resolution_clock::now();
    const auto counts_a = matchA(tc.text, tc.patterns);
    auto end_a = std::chrono::high_resolution_clock::now();

    auto start_b = std::chrono::high_resolution_clock::now();
    const auto counts_b = matchB(tc.text, tc.patterns);
    auto end_b = std::chrono::high_resolution_clock::now();

    result.ms_a = std::chrono::duration<double, std::milli>(end_a - start_a).count();
    result.ms_b = std::chrono::duration<double, std::milli>(end_b - start_b).count();
    result.counts_match = (counts_a == counts_b);
    return result;
}

int main() {
    std::mt19937 rng(20260528);

    std::vector<TestCase> tests;
    tests.push_back(build_case_a_faster(rng));
    tests.push_back(build_case_b_faster());

    std::ofstream out("results.csv");
    out << "test_name,algorithm,text_len,num_patterns,max_pattern_len,time_ms,counts_match\n";

    for (const auto& tc : tests) {
        RunResult res = run_one_case(tc);

        out << tc.name << ",A," << tc.text.size() << "," << tc.patterns.size() << ","
            << res.max_len << "," << res.ms_a << "," << (res.counts_match ? "yes" : "no")
            << "\n";
        out << tc.name << ",B," << tc.text.size() << "," << tc.patterns.size() << ","
            << res.max_len << "," << res.ms_b << "," << (res.counts_match ? "yes" : "no")
            << "\n";

        std::cout << tc.name << " : A=" << res.ms_a << " ms, B=" << res.ms_b
                  << " ms, counts_match=" << (res.counts_match ? "yes" : "no") << "\n";
    }

    return 0;
}
