#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, sz, leftPos, rightPos, activeCnt, lastY;

    explicit DSU(int n)
        : parent(n + 1), sz(n + 1, 1), leftPos(n + 1), rightPos(n + 1),
          activeCnt(n + 1, 0), lastY(n + 1, 1) {
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            leftPos[i] = i;
            rightPos[i] = i;
        }
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    void unite(int x, int y, int curY) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return;
        }
        if (sz[x] < sz[y]) {
            swap(x, y);
        }
        parent[y] = x;
        sz[x] += sz[y];
        leftPos[x] = min(leftPos[x], leftPos[y]);
        rightPos[x] = max(rightPos[x], rightPos[y]);
        activeCnt[x] += activeCnt[y];
        lastY[x] = curY;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n + 1), h(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    for (int i = 1; i < n; ++i) {
        cin >> h[i];
    }

    vector<vector<int>> deactivateAt(n + 2), openEdgeAt(n + 2);

    for (int i = 1; i <= n; ++i) {
        if (a[i] >= 1) {
            deactivateAt[a[i] + 1].push_back(i);
        }
    }

    for (int i = 1; i < n; ++i) {
        int y = h[i] + 1;
        openEdgeAt[y].push_back(i);
    }

    DSU dsu(n);
    for (int i = 1; i <= n; ++i) {
        dsu.activeCnt[i] = (a[i] >= 1) ? 1 : 0;
    }

    for (int i = 1; i < n; ++i) {
        if (h[i] == 0) {
            dsu.unite(i, i + 1, 1);
        }
    }

    vector<long long> diff(n + 3, 0);

    auto finalizeRoot = [&](int x, int y) {
        int rt = dsu.find(x);
        if (dsu.lastY[rt] >= y) {
            return;
        }
        int duration = y - dsu.lastY[rt];
        if (duration > 0 && dsu.activeCnt[rt] > 0) {
            int l = dsu.rightPos[rt] - dsu.activeCnt[rt] + 1;
            int r = dsu.rightPos[rt];
            diff[l] += duration;
            diff[r + 1] -= duration;
        }
        dsu.lastY[rt] = y;
    };

    vector<int> touched;
    vector<int> mark(n + 1, 0);
    int stamp = 0;

    for (int y = 2; y <= n + 1; ++y) {
        ++stamp;
        touched.clear();

        for (int idx : deactivateAt[y]) {
            int rt = dsu.find(idx);
            if (mark[rt] != stamp) {
                mark[rt] = stamp;
                touched.push_back(rt);
            }
        }

        for (int e : openEdgeAt[y]) {
            int rt1 = dsu.find(e);
            int rt2 = dsu.find(e + 1);
            if (mark[rt1] != stamp) {
                mark[rt1] = stamp;
                touched.push_back(rt1);
            }
            if (mark[rt2] != stamp) {
                mark[rt2] = stamp;
                touched.push_back(rt2);
            }
        }

        for (int rt : touched) {
            finalizeRoot(rt, y);
        }

        for (int idx : deactivateAt[y]) {
            int rt = dsu.find(idx);
            --dsu.activeCnt[rt];
        }

        for (int e : openEdgeAt[y]) {
            dsu.unite(e, e + 1, y);
        }
    }

    for (int i = 1; i <= n; ++i) {
        if (dsu.find(i) == i) {
            finalizeRoot(i, n + 1);
        }
    }

    long long cur = 0;
    for (int i = 1; i <= n; ++i) {
        cur += diff[i];
        cout << cur << ' ';
    }

    return 0;
}