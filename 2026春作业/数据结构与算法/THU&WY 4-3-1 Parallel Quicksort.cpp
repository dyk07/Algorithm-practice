#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

vector<vector<int>> cd;
vector<int> me;
int N, M;
long long cnt = 0;

int median(vector<int> &v) {
    int idx = (static_cast<int>(v.size()) - 1) / 2;
    nth_element(v.begin(), v.begin() + idx, v.end());
    return v[idx];
}

int main() {
    scanf("%d%d", &N, &M);
    cd.resize(N);
    for (int i = 0; i < N; i++) {
        cd[i].resize(M);
        for (int j = 0; j < M; j++) {
            scanf("%d", &cd[i][j]);
        }
    }

    int rounds = 0;
    for (int tmp = N; tmp > 1; tmp >>= 1) {
        rounds++;
    }

    vector<pair<int, int>> groups;
    groups.emplace_back(0, N);
    for (int r = 0; r < rounds; r++) {
        vector<pair<int, int>> next_groups;
        for (const auto &g : groups) {
            int l = g.first;
            int rr = g.second;
            int len = rr - l;
            int half = len / 2;

            me.clear();
            for (int i = l; i < rr; i++) {
                if (!cd[i].empty()) {
                    me.push_back(median(cd[i]));
                }
            }

            if (!me.empty()) {
                int a = median(me);
                for (int i = 0; i < half; i++) {
                    int left = l + i;
                    int right = l + half + i;
                    vector<int> tmp_left, tmp_right;
                    tmp_left.reserve(cd[left].size() + cd[right].size());
                    tmp_right.reserve(cd[left].size() + cd[right].size());

                    for (int v : cd[left]) {
                        if (v <= a) {
                            tmp_left.push_back(v);
                        } else {
                            cnt++;
                            tmp_right.push_back(v);
                        }
                    }
                    for (int v : cd[right]) {
                        if (v <= a) {
                            cnt++;
                            tmp_left.push_back(v);
                        } else {
                            tmp_right.push_back(v);
                        }
                    }
                    cd[left].swap(tmp_left);
                    cd[right].swap(tmp_right);
                }
            }

            next_groups.emplace_back(l, l + half);
            next_groups.emplace_back(l + half, rr);
        }

        groups.swap(next_groups);
        for (int i = 0; i < N; i++) {
            printf("%d ", static_cast<int>(cd[i].size()));
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++) {
        long long s = 0;
        for (int v : cd[i]) {
            s += v;
        }
        printf("%lld ", s);
    }
    printf("\n%lld\n", cnt);

    return 0;
}