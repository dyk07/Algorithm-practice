#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// 执行系统命令并返回执行时间（毫秒）
long long run_test(string cmd) {
    auto start = high_resolution_clock::now();
    int status = system(cmd.c_str());
    auto stop = high_resolution_clock::now();
    if (status != 0) return -1; 
    return duration_cast<milliseconds>(stop - start).count();
}

// 生成测试数据
void generate_data(int type, int N, string filename) {
    ofstream out(filename);
    out << N << endl;

    if (type == 1) { 
        // 测例1: 随机平衡 - 三者表现相近
        for (int i = 0; i < N; ++i) {
            out << rand() % 3 << " " << rand() % (N * 5) << endl;
        }
    } 
    else if (type == 2) { 
        // 测例2: 顺序插入 - 体现 BST 劣势
        for (int i = 0; i < N; ++i) out << "0 " << i << endl;
        for (int i = 0; i < 1000; ++i) out << "2 " << N - 1 << endl;
    }
    else if (type == 3) { 
        // 测例3: 专门对比 Double vs Single
        // 构造极长链
        for (int i = 0; i < N / 2; ++i) out << "0 " << i << endl; 
        for (int i = 0; i < N / 2; ++i) {
            if (i % 2 == 0) out << "2 " << 0 << endl;
            else out << "2 " << N - 1 << endl;
        }
    }
    else if (type == 4) { 
        // 测例4: 极端局部性 - 体现 Splay 优势
        for (int i = 0; i < N; ++i) out << "0 " << i << endl;
        vector<int> hot_keys;
        for (int i = 0; i < N/100; ++i) hot_keys.push_back(rand() % N);
        for (int i = 0; i < N; ++i) {
            out << "2 " << hot_keys[i % hot_keys.size()] << endl;
        }
    }
    out.close();
}

int main() {
    srand(time(0));
    system("if not exist input mkdir input");
    system("if not exist output mkdir output");

    vector<string> progs = {"my_bst", "my_skiplist", "my_splay_double", "my_splay_single"};
    vector<string> cases = {"1.Random_Balance", "2.BST_Weakness", "3.Double_vs_Single", "4.Splay_Locality"};
    
    // 调整各测例规模
    int sizes[] = {100000, 40000, 200000, 80000}; 

    ofstream report("report.txt");
    report << left << setw(20) << "TestCase" << setw(18) << "Program" << "Time(ms)" << endl;
    report << string(55, '-') << endl;

    for (int i = 0; i < 4; ++i) {
        string input_file = "input/case" + to_string(i+1) + ".in";
        cout << ">> Generating " << cases[i] << "..." << endl;
        generate_data(i + 1, sizes[i], input_file);

        for (const string& prog : progs) {
            // 特殊逻辑：测试 3 只要测试 Splay 两兄弟
            if (i == 2 && (prog == "my_bst" || prog == "my_skiplist")) {
                continue; 
            }

            cout << "   Running " << prog << "... ";
            string cmd = prog + ".exe < " + input_file + " > output/" + prog + ".out";
            
            long long t = run_test(cmd);
            
            cout << (t < 0 ? "Failed" : to_string(t) + " ms") << endl;
            report << left << setw(20) << cases[i] << setw(18) << prog;
            
            if (t > 20000) report << ">20000 (Timeout)" << endl;
            else if (t < 0) report << "Error/Fail" << endl;
            else report << t << endl;
        }
        report << string(55, '-') << endl;
    }

    report.close();
    cout << "\n[Done] Results saved to report.txt" << endl;
    return 0;
}