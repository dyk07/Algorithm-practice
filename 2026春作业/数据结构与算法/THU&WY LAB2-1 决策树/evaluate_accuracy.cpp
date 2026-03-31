#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <test_data_file> <prediction_file>" << endl;
        return 1;
    }

    // 读取测试集文件，获取真实类别
    ifstream testFin(argv[1]);
    if (!testFin) {
        cerr << "Error: Cannot open test data file: " << argv[1] << endl;
        return 1;
    }

    int n, m;
    testFin >> n >> m;
    vector<int> trueLabels(n);
    for (int i = 0; i < n; ++i) {
        // 跳过前 m 个属性值，读取最后一个类别
        int val;
        for (int j = 0; j < m; ++j) {
            testFin >> val; // 属性值，忽略
        }
        testFin >> trueLabels[i];
    }
    testFin.close();

    // 读取预测结果文件
    ifstream predFin(argv[2]);
    if (!predFin) {
        cerr << "Error: Cannot open prediction file: " << argv[2] << endl;
        return 1;
    }

    vector<int> predLabels;
    int label;
    while (predFin >> label) {
        predLabels.push_back(label);
    }
    predFin.close();

    if (predLabels.size() != trueLabels.size()) {
        cerr << "Error: Number of predictions (" << predLabels.size()
             << ") does not match number of test samples (" << trueLabels.size() << ")" << endl;
        return 1;
    }

    // 计算正确数
    int correct = 0;
    for (size_t i = 0; i < trueLabels.size(); ++i) {
        if (predLabels[i] == trueLabels[i]) {
            ++correct;
        }
    }

    double accuracy = 100.0 * correct / trueLabels.size();
    cout << fixed << setprecision(2);
    cout << "Accuracy: " << accuracy << "% (" << correct << "/" << trueLabels.size() << ")" << endl;

    return 0;
}