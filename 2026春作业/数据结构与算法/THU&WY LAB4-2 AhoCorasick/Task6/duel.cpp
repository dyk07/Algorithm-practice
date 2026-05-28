#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<long long> matchAC(const std::string& text, const std::vector<std::string>& patterns);

namespace {
constexpr int kAlphabet = 128; // ASCII

struct TrieNode {
    std::array<int, kAlphabet> next;
    std::vector<int> out;
    TrieNode() { next.fill(-1); }
};

std::vector<long long> matchA(const std::string& text, const std::vector<std::string>& patterns) {
    std::vector<long long> counts(patterns.size(), 0);
    if (patterns.empty() || text.empty()) {
        return counts;
    }

    size_t total_len = 0;
    size_t max_len = 0;
    for (const auto& p : patterns) {
        total_len += p.size();
        max_len = std::max(max_len, p.size());
    }

    std::vector<TrieNode> nodes;
    nodes.reserve(total_len + 1);
    nodes.emplace_back();

    for (size_t i = 0; i < patterns.size(); ++i) {
        const auto& p = patterns[i];
        int cur = 0;
        for (unsigned char ch : p) {
            int idx = static_cast<int>(ch);
            if (nodes[cur].next[idx] == -1) {
                nodes[cur].next[idx] = static_cast<int>(nodes.size());
                nodes.emplace_back();
            }
            cur = nodes[cur].next[idx];
        }
        nodes[cur].out.push_back(static_cast<int>(i));
    }

    const size_t n = text.size();
    for (size_t i = 0; i < n; ++i) {
        int cur = 0;
        size_t limit = std::min(max_len, n - i);
        for (size_t d = 0; d < limit; ++d) {
            unsigned char ch = static_cast<unsigned char>(text[i + d]);
            int idx = static_cast<int>(ch);
            int next = nodes[cur].next[idx];
            if (next == -1) {
                break;
            }
            cur = next;
            if (!nodes[cur].out.empty()) {
                for (int id : nodes[cur].out) {
                    counts[id] += 1;
                }
            }
        }
    }

    return counts;
}

std::vector<int> build_prefix(const std::string& pattern) {
    std::vector<int> pi(pattern.size(), 0);
    int j = 0;
    for (size_t i = 1; i < pattern.size(); ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j += 1;
        }
        pi[i] = j;
    }
    return pi;
}

std::vector<long long> matchB(const std::string& text, const std::vector<std::string>& patterns) {
    std::vector<long long> counts(patterns.size(), 0);
    if (patterns.empty() || text.empty()) {
        return counts;
    }

    for (size_t i = 0; i < patterns.size(); ++i) {
        const auto& p = patterns[i];
        if (p.empty()) {
            counts[i] = 0;
            continue;
        }
        const auto pi = build_prefix(p);
        int j = 0;
        for (size_t k = 0; k < text.size(); ++k) {
            while (j > 0 && text[k] != p[j]) {
                j = pi[j - 1];
            }
            if (text[k] == p[j]) {
                j += 1;
            }
            if (j == static_cast<int>(p.size())) {
                counts[i] += 1;
                j = pi[j - 1];
            }
        }
    }

    return counts;
}

std::string random_string(size_t len, const std::string& alphabet, std::mt19937& rng) {
    std::uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);
    std::string s(len, 'a');
    for (size_t i = 0; i < len; ++i) {
        s[i] = alphabet[dist(rng)];
    }
    return s;
}

bool run_one_test(std::mt19937& rng, int test_id) {
    const std::string alphabet = "abc";
    std::uniform_int_distribution<int> len_dist(1, 50);
    std::uniform_int_distribution<int> pat_cnt_dist(1, 25);
    std::uniform_int_distribution<int> pat_len_dist(1, 10);

    int text_len = len_dist(rng);
    int pat_count = pat_cnt_dist(rng);

    std::string text = random_string(static_cast<size_t>(text_len), alphabet, rng);
    std::vector<std::string> patterns;
    patterns.reserve(static_cast<size_t>(pat_count));
    for (int i = 0; i < pat_count; ++i) {
        int p_len = pat_len_dist(rng);
        patterns.push_back(random_string(static_cast<size_t>(p_len), alphabet, rng));
    }

    auto a = matchA(text, patterns);
    auto b = matchB(text, patterns);
    auto c = matchAC(text, patterns);

    if (a != b || a != c) {
        std::cout << "Mismatch at test " << test_id << "\n";
        std::cout << "text=" << text << "\n";
        for (size_t i = 0; i < patterns.size(); ++i) {
            std::cout << "p" << i << "=" << patterns[i] << " a=" << a[i] << " b=" << b[i]
                      << " c=" << c[i] << "\n";
        }
        return false;
    }

    return true;
}
}

int main() {
    std::mt19937 rng(20260528);

    const int tests = 300;
    for (int t = 1; t <= tests; ++t) {
        if (!run_one_test(rng, t)) {
            return 1;
        }
    }

    std::cout << "All tests passed: " << tests << "\n";
    return 0;
}
