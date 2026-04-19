LAB3 Set 题目的代码参考。

目录 lib 下包含了经过整理后的 二叉搜索树（bst），伸展树 (splay)，跳表 (skiplist) 的示例代码头文件。以及这些头文件所依赖的其他头文件（例如 bintree, list）。
splay 的实现分为分为单旋和双旋两种，使用模板参数控制。
my_bst.cpp, my_splay_single.cpp, my_splay_double.cpp, my_skiplist.cpp 引用了上述头文件，基于三种数据结构实现了 set 接口。
此外我们还提供了基于 C++ set 容器的实现 my_stdset.cpp，用于生成小规模随机数据的 rand_input.cpp，用于验证代码正确性和辅助调试的 battle.cpp。

你需要首先阅读和理解代码，可以比对提供的代码和讲义中的示例代码，并分析二者的差异。
然后需要补全代码中的 lower_bound 函数实现部分。包括 lib/bst/bst.h，lib/splay/splay.h，lib/skiplist/skiplist.h 中的 lower_bound 函数实现。
以及在 lib/splay/splay.h 中的 splay 函数仅提供了双旋 splay 的实现，你需要补全单旋 splay 的实现。

对于编译，我们提供了 makefile 文件，运行 make 进行编译。可以使用 battle 对实现的正确性进行验证，battle 会使用 rand_input 生成随机测例，并将三种数据结构的输出结果和 my_stdset 的参考实现输出进行对比。
数据的生成也可以参考 rand_input.cpp。

