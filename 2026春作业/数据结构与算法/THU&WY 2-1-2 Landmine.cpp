#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>


int A[2005][2005], s = 0, n, m, N, M;
std::vector<int> v;


int va(int a, int b) {
    a = std::min(a, M);
    return ((long long)a*b)%10007;
}


void cal(int i, int j) {
    for (int l = v.size() - 1; l >= 1; l--) {
        s = (s + va(i - v[l - 1], A[v[l]][j] - A[v[l - 1]][j]))%10007;
    }
    s = (s + va(i + 1, A[v[0]][j]))%10007;
}


int main() {
    scanf("%d%d%d%d", &n, &m, &N, &M);
    for (int i = 0; i < m; i++) {
        int tmp = 0;
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
            if (!A[i][j]) {
                while (tmp <= j) {
                    A[i][tmp] = std::min(j - tmp, N);
                    tmp++;
                }
            }
        }
        while (tmp < n) {
            A[i][tmp] = std::min(n - tmp, N);
            tmp++;
        }
    }


    for (int j = 0; j < n; j++) {
        v.clear();
        for (int i = 0; i < m; i++) {
            while (!v.empty() && A[v.back()][j] >= A[i][j]) {
                v.pop_back();
            }
            v.push_back(i);
            cal(i, j);
        }
    }

    printf("%d", s);

    return 0;
}