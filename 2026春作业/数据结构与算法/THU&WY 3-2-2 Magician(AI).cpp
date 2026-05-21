#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Operation {
    char type;   // 'H' or 'Q'
    int a, b;    // H: [a,b], Q: a
};

class SegmentTree {
public:
    explicit SegmentTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tag.assign(max(1, 4 * n + 5), 0);
    }

    void rangeAdd(int ql, int qr) {
        if (n == 0 || ql > qr) return;
        rangeAdd(1, 1, n, ql, qr);
    }

    int pointQuery(int idx) const {
        if (n == 0) return 0;
        return pointQuery(1, 1, n, idx);
    }

private:
    int n{};
    vector<int> tag; // merge 到该节点的翻转次数（懒标记）

    void rangeAdd(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            ++tag[node];
            return;
        }
        int mid = l + ((r - l) >> 1);
        if (ql <= mid) rangeAdd(node << 1, l, mid, ql, qr);
        if (qr > mid)  rangeAdd(node << 1 | 1, mid + 1, r, ql, qr);
    }

    int pointQuery(int node, int l, int r, int idx) const {
        if (l == r) return tag[node];
        int mid = l + ((r - l) >> 1);
        if (idx <= mid) return tag[node] + pointQuery(node << 1, l, mid, idx);
        return tag[node] + pointQuery(node << 1 | 1, mid + 1, r, idx);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n; // 题目给到 < 2^31
    int m;
    if (!(cin >> n >> m)) return 0;

    vector<Operation> ops;
    ops.reserve(m);

    // 只离散化所有查询点，节点数 O(m)
    vector<int> queryPos;
    queryPos.reserve(m);

    for (int k = 0; k < m; ++k) {
        char t;
        cin >> t;
        if (t == 'H') {
            int i, j;
            cin >> i >> j;
            ops.push_back({t, i, j});
        } else { // 'Q'
            int i;
            cin >> i;
            ops.push_back({t, i, 0});
            queryPos.push_back(i);
        }
    }

    sort(queryPos.begin(), queryPos.end());
    queryPos.erase(unique(queryPos.begin(), queryPos.end()), queryPos.end());

    const int K = static_cast<int>(queryPos.size());
    SegmentTree st(K);

    string out;
    out.reserve(static_cast<size_t>(m) * 8);

    for (const auto& op : ops) {
        if (op.type == 'H') {
            if (K == 0) continue;

            auto itL = lower_bound(queryPos.begin(), queryPos.end(), op.a);
            auto itR = upper_bound(queryPos.begin(), queryPos.end(), op.b);

            if (itL < itR) {
                int l = static_cast<int>(itL - queryPos.begin()) + 1; // 1-indexed
                int r = static_cast<int>(itR - queryPos.begin());     // 1-indexed
                st.rangeAdd(l, r);
            }
        } else { // 'Q'
            int idx = static_cast<int>(
                lower_bound(queryPos.begin(), queryPos.end(), op.a) - queryPos.begin()
            ) + 1;
            int ans = st.pointQuery(idx);
            out += to_string(ans);
            out.push_back('\n');
        }
    }

    cout << out;
    return 0;
}