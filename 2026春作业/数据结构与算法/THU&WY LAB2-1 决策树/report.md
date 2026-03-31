## 1.2.3节的小练习
在jpg文件中

## 实现思路
1. Data structure
    TreeNode: node of the decision tree
    Data: 2D integer vector, storing datasets
2. Auxiliary function
    countLabels
    entropy/gini
    majorityClass
    splitData
3. ID3 realization
    停止条件：当前样本集类别完全一致，或无可用的属性（availableAttrs 全为false），或所有样本在剩余属性上取值相同则创建叶子节点，类别为多数类。

    属性选择：遍历所有可用属性，计算划分后的条件熵，选择信息增益最大的属性作为当前节点划分属性。要求划分后两个子集均非空。

    递归：将当前属性标记为已用，分别在左右子集上递归构建子树。
4. CART realization
    停止条件：当前样本集类别完全一致，或无法找到可行的划分（所有属性划分都导致空子集）则创建叶子节点。

    属性选择：遍历所有属性，计算划分后的基尼指数，选择基尼指数最小的属性。同样要求划分后子集非空。

    递归：不删除属性（CART 允许重复使用），分别在左右子集上递归构建。
5. Prediction function
    Recursively judge from the root node.
6. Main function
    接收命令行参数：训练集文件、测试集文件。
    读取训练集，构建 ID3 树和 CART 树。
    读取测试集（仅使用前 m 个属性），对每个样本分别用两棵树预测。
    将预测结果逐行写入 result_ID3.out 和 result_CART.out。
7. Calculate accuracy
    run evaluate_accuracy.cpp

## 报告表现
利用evaluate_accuracy.cpp进行自动比较，得到：
ID3 Accuracy: 91.25% (365/400)
CART Accuracy: 90.75% (363/400)

## 思考题
1. 二分划分，比如将职业分为两个大致相等的子集
2. 选择一个分割点，比如气温以25°C为界
3. 二分通用性更强，适合CART等算法，但分支含义可能不直观，可能对是部分语义；多叉直观清晰，但不支持CART，且容易产生稀疏子集
4. 二值：是否要带伞 属性：天气（晴雨），风力（强弱），是否有约
    多离散值：选择交通工具 属性：季节，距离，是否有急事
    连续值：是否要穿外套 属性：温度、风速、湿度