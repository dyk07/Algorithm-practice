## 任务1
https://chat.deepseek.com/share/402ig4rwxqpn0zolb4
回答：
1. vector
2. O(L)，可以降低。使用 std::deque 或 std::list 替代 std::vector；或改变存储顺序，按从蛇尾到蛇头顺序存储
3. O(L)，可以降低。使用std::list实现头部插入和尾部删除

## 任务2
https://chat.qwen.ai/s/t_7f4fb67d-9641-4cfa-888a-6f553abd6c44?fev=0.2.16
Task1的使用了全局的函数，函数指责单一，功能明确；且注释较多，易懂
而Task2全封装在class中，完全OOP
我更喜欢1，因为它过程式结构清晰，注释细致

## 任务3
bug如下：（行数均指修改前）
1. 169行 将 == 改为 !=
2. 119行 将 true 改为 false
3. 56行 将 drawmap 移动至 while 循环前
4. 181行 删除 score++
5. 198行 删除 system("cls")
https://chat.deepseek.com/share/qvghrwf474uhxieud3
LLM一次性成功修改了所有bug
简单局部的语法错误和逻辑漏洞，LLM可以迅速发现；但整体上复杂的原理性错误，LLM难以有效甄别。

## 任务4
见Task4.cpp
新增了{
难度递增，让蛇的移动速度不断加快；
在地图上生成随机道具（加分，缩短蛇身）；
暂停功能；
游戏结束后重玩；
最高分记录；
自动导航模式（AI自动决策）；
}

最终还是有概率出bug
一局结束后有概率直接卡退，而非进入结算界面
https://chat.deepseek.com/share/t1krab3veipo3q9pdy
原因大概是它没有考虑到很多现实问题，如计算机的内存性能等，导致没有及时地清内存之类的，导致容易卡退
但是实在找不到什么特别根本性的问题


## 任务5
https://chat.deepseek.com/share/uy2q8he4h6v4njloc1
总结：
要封装，拆分模块，策略模式，提高可拓展性，可测试性
要考虑设备兼容性，考虑实际运行的问题