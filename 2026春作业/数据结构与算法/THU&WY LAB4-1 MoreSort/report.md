# THU&WY LAB 4-1 MoreSort

## 任务1
实验数据生成方式：
封装在 `run_once` 函数中，生成8组随机数，每组200000个，范围在[-10^9, 10^9]之间。

测试过程：
运行 `benchmark_quicksort_limit.cpp` 程序，对每个limit值进行测试，记录排序的平均时间

结果：
计入 `limit_benchmark.csv`，符合预期
当limit再20~30之间时，排序效率最高

## 任务2
IntroSort.cpp
比较：special_case_intro_vs_classic.cpp
结果：special_case_results.csv
结论：IntroSort在特殊情况下表现远好于QuickSort

## 任务3
DualPivotQsort.cpp
比较：benchmark_dualpivot_vs_classic.cpp
结果：dual_pivot_vs_classic.csv
结论：DualPivotQuickSort在平均情况下表现略劣于经典QuickSort（可能由于未充分优化）
复杂度：O(nlogn)，但常数较大，且在某些情况下可能退化为O(n^2)

## 任务4
benchmark_pivot_rank.cpp
结果：pivot_rank_benchmark.csv
结论：
- 实际并非50%最快
- 并不对称
原因分析：
- 只在 array[i] < pivot 时发生交换与 pos 更新。
k=10 时条件成立约 10%，k=90 时成立约 90%，导致 k=90 触发大量交换与 pos 更新，时间更长
- 当条件几乎总为真/假时，预测更容易稳定（低损失）；当真假比例接近 50% 时预测最差，反而变慢

## 任务5
- “三数取中杀手”序列的真实存在。Introsort 既拥有堆排序 $O(N \log N)$ 的最坏情况时间保障，又能在实际运行中几乎总是比直接使用堆排序快得多。
- 虽然 Introsort 是为了解决 Quicksort 的最坏情况而生，但作者幽默地指出，你可以制造一种“双面序列”（Two-faced sequences）来反将它一军：先构造一段“三数取中杀手”序列，诱骗 Introsort 达到深度限制并切换到堆排序。结果 Introsort 会因为“疑心太重”而切换到堆排序，但对于接下来的随机序列，继续用快速排序其实反而更快。虽然这不会导致性能崩溃，但确实能让 Introsort 在这种特殊情况下跑得比原版快速排序稍微慢一点。