#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <memory>

using namespace std;

// 决策树节点结构
struct TreeNode {
    bool isLeaf;                // 是否为叶子节点
    int label;                  // 叶子节点对应的类别（仅在 isLeaf 为 true 时有效）
    int splitAttr;              // 非叶子节点划分使用的属性索引
    unique_ptr<TreeNode> left;  // 属性值为 0 的分支
    unique_ptr<TreeNode> right; // 属性值为 1 的分支

    TreeNode() : isLeaf(false), label(-1), splitAttr(-1), left(nullptr), right(nullptr) {}
};

using Data = vector<vector<int>>; // 原始数据，每行 m+1 个整数

// --------------------------- 辅助函数 ---------------------------

// 统计给定索引集合中各类别出现的次数
map<int, int> countLabels(const vector<int>& indices, const Data& data) {
    map<int, int> freq;
    for (int idx : indices) {
        int label = data[idx].back();
        freq[label]++;
    }
    return freq;
}

// 计算熵
double entropy(const map<int, int>& freq, int total) {
    if (total == 0) return 0.0;
    double ent = 0.0;
    for (const auto& p : freq) {
        double prob = static_cast<double>(p.second) / total;
        ent -= prob * log2(prob);
    }
    return ent;
}

// 计算基尼指数
double gini(const map<int, int>& freq, int total) {
    if (total == 0) return 0.0;
    double g = 1.0;
    for (const auto& p : freq) {
        double prob = static_cast<double>(p.second) / total;
        g -= prob * prob;
    }
    return g;
}

// 获取多数类别
int majorityClass(const vector<int>& indices, const Data& data) {
    auto freq = countLabels(indices, data);
    int maxCount = -1, bestClass = -1;
    for (const auto& p : freq) {
        if (p.second > maxCount) {
            maxCount = p.second;
            bestClass = p.first;
        }
    }
    return bestClass;
}

// 根据属性值划分数据集
void splitData(const vector<int>& indices, const Data& data, int attr,
               vector<int>& leftIndices, vector<int>& rightIndices) {
    leftIndices.clear();
    rightIndices.clear();
    for (int idx : indices) {
        if (data[idx][attr] == 0)
            leftIndices.push_back(idx);
        else
            rightIndices.push_back(idx);
    }
}

// --------------------------- ID3 算法 ---------------------------

// 递归构建 ID3 决策树
unique_ptr<TreeNode> buildID3Recursive(const Data& data, const vector<int>& indices,
                                       const vector<bool>& availableAttrs) {
    unique_ptr<TreeNode> node(new TreeNode());

    // 停止条件 1：所有样本类别相同
    auto freq = countLabels(indices, data);
    if (freq.size() == 1) {
        node->isLeaf = true;
        node->label = freq.begin()->first;
        return node;
    }

    // 停止条件 2：没有可用属性或所有样本在剩余属性上取值相同
    bool allSame = true;
    for (size_t i = 0; i < availableAttrs.size(); ++i) {
        if (availableAttrs[i]) {
            int val = data[indices[0]][i];
            bool same = true;
            for (int idx : indices) {
                if (data[idx][i] != val) {
                    same = false;
                    break;
                }
            }
            if (!same) {
                allSame = false;
                break;
            }
        }
    }
    if (allSame || find(availableAttrs.begin(), availableAttrs.end(), true) == availableAttrs.end()) {
        node->isLeaf = true;
        node->label = majorityClass(indices, data);
        return node;
    }

    int total = indices.size();
    double currentEntropy = entropy(freq, total);

    int bestAttr = -1;
    double maxGain = -1.0;

    // 遍历所有可用属性，选择信息增益最大的
    for (size_t attr = 0; attr < availableAttrs.size(); ++attr) {
        if (!availableAttrs[attr]) continue;

        vector<int> leftIdx, rightIdx;
        splitData(indices, data, attr, leftIdx, rightIdx);

        // 要求划分后两个子集均非空
        if (leftIdx.empty() || rightIdx.empty()) continue;

        double condEntropy = 0.0;
        if (!leftIdx.empty()) {
            auto leftFreq = countLabels(leftIdx, data);
            condEntropy += (static_cast<double>(leftIdx.size()) / total) * entropy(leftFreq, leftIdx.size());
        }
        if (!rightIdx.empty()) {
            auto rightFreq = countLabels(rightIdx, data);
            condEntropy += (static_cast<double>(rightIdx.size()) / total) * entropy(rightFreq, rightIdx.size());
        }

        double gain = currentEntropy - condEntropy;
        if (gain > maxGain) {
            maxGain = gain;
            bestAttr = attr;
        }
    }

    // 如果没有找到合适的属性，返回叶子节点
    if (bestAttr == -1) {
        node->isLeaf = true;
        node->label = majorityClass(indices, data);
        return node;
    }

    // 构建内部节点
    node->isLeaf = false;
    node->splitAttr = bestAttr;

    vector<int> leftIdx, rightIdx;
    splitData(indices, data, bestAttr, leftIdx, rightIdx);

    // 更新可用属性集（移除当前属性）
    vector<bool> newAvailableAttrs = availableAttrs;
    newAvailableAttrs[bestAttr] = false;

    node->left = buildID3Recursive(data, leftIdx, newAvailableAttrs);
    node->right = buildID3Recursive(data, rightIdx, newAvailableAttrs);

    return node;
}

// 对外接口：使用 ID3 算法构建决策树
unique_ptr<TreeNode> build_ID3(const Data& data, int m) {
    vector<int> allIndices(data.size());
    for (size_t i = 0; i < data.size(); ++i) allIndices[i] = i;
    vector<bool> availableAttrs(m, true);
    return buildID3Recursive(data, allIndices, availableAttrs);
}

// --------------------------- CART 算法 ---------------------------

// 递归构建 CART 决策树
unique_ptr<TreeNode> buildCARTRecursive(const Data& data, const vector<int>& indices) {
    unique_ptr<TreeNode> node(new TreeNode());

    // 停止条件 1：所有样本类别相同
    auto freq = countLabels(indices, data);
    if (freq.size() == 1) {
        node->isLeaf = true;
        node->label = freq.begin()->first;
        return node;
    }

    int total = indices.size();
    int m = static_cast<int>(data[0].size()) - 1; // 属性个数

    int bestAttr = -1;
    double minGini = 1e9;

    // 遍历所有属性，选择基尼指数最小的
    for (int attr = 0; attr < m; ++attr) {
        vector<int> leftIdx, rightIdx;
        splitData(indices, data, attr, leftIdx, rightIdx);

        // 要求划分后两个子集均非空
        if (leftIdx.empty() || rightIdx.empty()) continue;

        double giniIdx = 0.0;
        if (!leftIdx.empty()) {
            auto leftFreq = countLabels(leftIdx, data);
            giniIdx += (static_cast<double>(leftIdx.size()) / total) * gini(leftFreq, leftIdx.size());
        }
        if (!rightIdx.empty()) {
            auto rightFreq = countLabels(rightIdx, data);
            giniIdx += (static_cast<double>(rightIdx.size()) / total) * gini(rightFreq, rightIdx.size());
        }

        if (giniIdx < minGini) {
            minGini = giniIdx;
            bestAttr = attr;
        }
    }

    // 如果没有找到合适的属性，返回叶子节点
    if (bestAttr == -1) {
        node->isLeaf = true;
        node->label = majorityClass(indices, data);
        return node;
    }

    // 构建内部节点
    node->isLeaf = false;
    node->splitAttr = bestAttr;

    vector<int> leftIdx, rightIdx;
    splitData(indices, data, bestAttr, leftIdx, rightIdx);

    // CART 允许重复使用属性，因此不删除属性
    node->left = buildCARTRecursive(data, leftIdx);
    node->right = buildCARTRecursive(data, rightIdx);

    return node;
}

// 对外接口：使用 CART 算法构建决策树
unique_ptr<TreeNode> build_CART(const Data& data) {
    vector<int> allIndices(data.size());
    for (size_t i = 0; i < data.size(); ++i) allIndices[i] = i;
    return buildCARTRecursive(data, allIndices);
}

// --------------------------- 决策树预测 ---------------------------

// 使用决策树对单个样本（属性向量）进行预测
int predict(const TreeNode* node, const vector<int>& sample) {
    if (node->isLeaf) {
        return node->label;
    }
    int attr = node->splitAttr;
    if (sample[attr] == 0) {
        return predict(node->left.get(), sample);
    } else {
        return predict(node->right.get(), sample);
    }
}

// --------------------------- 主函数 ---------------------------

int main(int argc, char* argv[]) {
    // 检查参数个数
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <train_file> <test_file>" << endl;
        return 1;
    }

    // 读取训练集
    ifstream trainFin(argv[1]);
    if (!trainFin) {
        cerr << "Error: Cannot open train file: " << argv[1] << endl;
        return 1;
    }

    int n_train, m;
    trainFin >> n_train >> m;
    Data trainData(n_train, vector<int>(m + 1));
    for (int i = 0; i < n_train; ++i) {
        for (int j = 0; j < m + 1; ++j) {
            trainFin >> trainData[i][j];
        }
    }
    trainFin.close();

    // 构建决策树
    unique_ptr<TreeNode> id3Tree = build_ID3(trainData, m);
    unique_ptr<TreeNode> cartTree = build_CART(trainData);

    // 读取测试集
    ifstream testFin(argv[2]);
    if (!testFin) {
        cerr << "Error: Cannot open test file: " << argv[2] << endl;
        return 1;
    }

    int n_test, m_test;
    testFin >> n_test >> m_test;
    if (m_test != m) {
        cerr << "Warning: test set attribute count (" << m_test
             << ") differs from train set (" << m << ")" << endl;
    }

    Data testData(n_test, vector<int>(m + 1));
    for (int i = 0; i < n_test; ++i) {
        for (int j = 0; j < m + 1; ++j) {
            testFin >> testData[i][j];
        }
    }
    testFin.close();

    // 输出预测结果
    ofstream id3Out("result_ID3.out");
    ofstream cartOut("result_CART.out");
    if (!id3Out || !cartOut) {
        cerr << "Error: Cannot create output files." << endl;
        return 1;
    }

    for (int i = 0; i < n_test; ++i) {
        // 提取属性部分（前 m 个值）
        vector<int> sample(testData[i].begin(), testData[i].begin() + m);
        int predID3 = predict(id3Tree.get(), sample);
        int predCART = predict(cartTree.get(), sample);
        id3Out << predID3 << endl;
        cartOut << predCART << endl;
    }

    id3Out.close();
    cartOut.close();

    cout << "Prediction completed. Results saved to result_ID3.out and result_CART.out" << endl;

    return 0;
}