#include <array>
#include <queue>
#include <string>
#include <vector>

namespace {
constexpr int kAlphabet = 128;

struct Node {
    std::array<int, kAlphabet> next;
    int fail = 0;
    std::vector<int> out;
    Node() { next.fill(-1); }
};
}

std::vector<long long> matchAC(const std::string& text, const std::vector<std::string>& patterns) {
    std::vector<long long> counts(patterns.size(), 0);
    if (patterns.empty() || text.empty()) {
        return counts;
    }

    std::vector<Node> nodes;
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

    std::queue<int> q;
    for (int c = 0; c < kAlphabet; ++c) {
        int nxt = nodes[0].next[c];
        if (nxt != -1) {
            nodes[nxt].fail = 0;
            q.push(nxt);
        } else {
            nodes[0].next[c] = 0;
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        int fail = nodes[v].fail;

        for (int c = 0; c < kAlphabet; ++c) {
            int nxt = nodes[v].next[c];
            if (nxt != -1) {
                nodes[nxt].fail = nodes[fail].next[c];
                const auto& out = nodes[nodes[nxt].fail].out;
                if (!out.empty()) {
                    nodes[nxt].out.insert(nodes[nxt].out.end(), out.begin(), out.end());
                }
                q.push(nxt);
            } else {
                nodes[v].next[c] = nodes[fail].next[c];
            }
        }
    }

    int state = 0;
    for (unsigned char ch : text) {
        int idx = static_cast<int>(ch);
        state = nodes[state].next[idx];
        if (!nodes[state].out.empty()) {
            for (int id : nodes[state].out) {
                counts[id] += 1;
            }
        }
    }

    return counts;
}
