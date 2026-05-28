#include <string>
#include <vector>

namespace {
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
