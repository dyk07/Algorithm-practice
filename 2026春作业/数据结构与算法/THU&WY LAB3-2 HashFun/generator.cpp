#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>

#define MAX_LINE 40

std::string get_first_word(const std::string& line) {
    size_t pos = line.find(' ');
    return line.substr(0, pos);
}

int main(int argc, char* argv[]) {
    int N = argc > 1 ? std::atoi(argv[1]) : 1;//插入次数
    int M = argc > 2 ? std::atoi(argv[2]) : 1;//查询次数

    freopen("test.in", "w", stdout);
    freopen("poj.txt", "r", stdin);

    srand(time(NULL));

    std::vector<std::string> reservoir;
    char line[MAX_LINE];
    int i = 0;

    while (fgets(line, MAX_LINE, stdin)) {
        i++;
        if (i <= N) {
            reservoir.push_back(std::string(line));
        } else {
            int r = rand() % i;
            if (r < N) {
                reservoir[r] = std::string(line);
            }
        }
    }

    if (i < N) N = i;

    for (int j = 0; j < N; j++) {
        printf("%d %s", 0, reservoir[j].c_str());
    }

    for (int j = 0; j < M; j++) {
        int r = rand() % N;
        printf("%d %s\n", 1, get_first_word(reservoir[r]).c_str());
    }

    printf("%d\n", 2);

    return 0;
}