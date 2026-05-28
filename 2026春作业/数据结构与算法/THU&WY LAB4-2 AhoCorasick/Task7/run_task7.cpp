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
std::vector<long long> matchAC(const std::string& text, const std::vector<std::string>& patterns);

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
                                         std::mt19937& rng, const std::string& prefix = "") {
    std::unordered_set<std::string> seen;
    seen.reserve(count * 2);
    std::vector<std::string> patterns;
    patterns.reserve(count);
    while (patterns.size() < count) {
        std::string p = prefix + random_string(len - prefix.size(), alphabet, rng);
        if (seen.insert(p).second) {
            patterns.push_back(std::move(p));
        }
    }
    return patterns;
}

TestCase build_case_ac_advantage(std::mt19937& rng) {
    const size_t text_len = 200000;
    const size_t pattern_count = 500;
    const size_t start_len = 50;

    TestCase tc;
    tc.name = "AC_advantage_many_patterns";
    tc.text = std::string(text_len, 'a');
    tc.patterns.reserve(pattern_count);
    for (size_t i = 0; i < pattern_count; ++i) {
        tc.patterns.push_back(std::string(start_len + i, 'a'));
    }
    return tc;
}

TestCase build_case_close(std::mt19937& rng) {
    const size_t text_len = 30000;
    const size_t pattern_len = 5;
    const size_t pattern_count = 10;
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

    TestCase tc;
    tc.name = "Close_small_k_small_Lmax";
    tc.text = random_string(text_len, alphabet, rng);
    tc.patterns = unique_patterns(pattern_count, pattern_len, alphabet, rng);
    return tc;
}

struct RunResult {
    double ms = 0.0;
    bool counts_match = false;
    size_t max_len = 0;
};

using MatchFunc = std::vector<long long> (*)(const std::string&, const std::vector<std::string>&);

RunResult measure(const TestCase& tc, MatchFunc func, const std::vector<long long>& baseline) {
    RunResult result;
    for (const auto& p : tc.patterns) {
        result.max_len = std::max(result.max_len, p.size());
    }

    const int repeats = 3;
    double best = 1e100;
    bool ok = true;
    for (int i = 0; i < repeats; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        auto counts = func(tc.text, tc.patterns);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        best = std::min(best, ms);
        if (counts != baseline) {
            ok = false;
        }
    }
    result.ms = best;
    result.counts_match = ok;
    return result;
}

void run_case(std::ofstream& out, const TestCase& tc) {
    auto baseline = matchA(tc.text, tc.patterns);

    RunResult res_a = measure(tc, matchA, baseline);
    RunResult res_b = measure(tc, matchB, baseline);
    RunResult res_c = measure(tc, matchAC, baseline);

    out << tc.name << ",A," << tc.text.size() << "," << tc.patterns.size() << ","
        << res_a.max_len << "," << res_a.ms << "," << (res_a.counts_match ? "yes" : "no")
        << "\n";
    out << tc.name << ",B," << tc.text.size() << "," << tc.patterns.size() << ","
        << res_b.max_len << "," << res_b.ms << "," << (res_b.counts_match ? "yes" : "no")
        << "\n";
    out << tc.name << ",AC," << tc.text.size() << "," << tc.patterns.size() << ","
        << res_c.max_len << "," << res_c.ms << "," << (res_c.counts_match ? "yes" : "no")
        << "\n";

    std::cout << tc.name << " : A=" << res_a.ms << " ms, B=" << res_b.ms
              << " ms, AC=" << res_c.ms << " ms" << "\n";
}

int main() {
    std::mt19937 rng(20260528);

    std::ofstream out("task7_results.csv");
    out << "test_name,algorithm,text_len,num_patterns,max_pattern_len,time_ms,counts_match\n";

    run_case(out, build_case_ac_advantage(rng));
    run_case(out, build_case_close(rng));

    return 0;
}
