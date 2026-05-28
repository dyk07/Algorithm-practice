#include <array>
#include <algorithm>
#include <string>
#include <vector>

namespace {
constexpr int kAlphabet = 128;

struct TrieNode {
    std::array<int, kAlphabet> next;
    std::vector<int> out;
    TrieNode() { next.fill(-1); }
};
}

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
