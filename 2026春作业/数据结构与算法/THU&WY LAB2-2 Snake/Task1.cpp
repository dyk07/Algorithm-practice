#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

// 游戏配置
const int WIDTH = 20;          // 地图宽度（列数）
const int HEIGHT = 20;         // 地图高度（行数）
const int INIT_LENGTH = 3;     // 初始蛇长
const int SPEED_MS = 1000;      // 移动间隔（毫秒）

// 障碍物坐标（内部，避开边界）
const vector<pair<int, int>> OBSTACLES = {
    {5, 5}, {5, 6}, {5, 7},
    {14, 12}, {14, 13}, {14, 14},
    {8, 15}, {9, 15}, {10, 15}
};

using Position = pair<int, int>;

// 方向向量
const Position DIR_UP    = {0, -1};
const Position DIR_DOWN  = {0, 1};
const Position DIR_LEFT  = {-1, 0};
const Position DIR_RIGHT = {1, 0};

// 全局变量
vector<Position> snake;      // 蛇身坐标，蛇头在 front
Position food;               // 食物坐标
Position dir;                // 当前移动方向
int score;                   // 得分
bool gameOver;               // 游戏结束标志
bool win;                    // 胜利标志（占满地图）

// 函数声明
void initGame();
void drawMap();
Position generateFood();
bool isCollision(const Position& newHead, bool isEating);
void processInput();
void updateGame();
void showGameOver();

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    initGame();

    while (!gameOver) {
        processInput();      // 处理键盘输入
        updateGame();        // 更新游戏逻辑
        drawMap();           // 绘制画面
        Sleep(SPEED_MS);     // 控制速度
    }

    showGameOver();
    return 0;
}

// 初始化游戏
void initGame() {
    // 初始蛇身：水平放置，蛇头在右
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;
    snake.clear();
    for (int i = 0; i < INIT_LENGTH; ++i) {
        snake.push_back({startX - i, startY});
    }
    dir = DIR_RIGHT;          // 初始方向向右
    score = 0;
    gameOver = false;
    win = false;

    // 生成食物（避开蛇身和障碍物）
    food = generateFood();
    if (food.first == -1) {
        // 如果一开始就没有空间，游戏胜利（但一般不会）
        win = true;
        gameOver = true;
    }
}

// 生成随机食物（避开蛇身、障碍物和边界）
Position generateFood() {
    // 最大尝试次数（防止无限循环）
    const int MAX_TRIES = 10000;
    for (int tries = 0; tries < MAX_TRIES; ++tries) {
        int x = rand() % (WIDTH - 2) + 1;   // 1 到 WIDTH-2
        int y = rand() % (HEIGHT - 2) + 1;  // 1 到 HEIGHT-2
        Position cand = {x, y};

        // 检查是否与蛇身重合
        if (find(snake.begin(), snake.end(), cand) != snake.end())
            continue;
        // 检查是否与障碍物重合
        if (find(OBSTACLES.begin(), OBSTACLES.end(), cand) != OBSTACLES.end())
            continue;

        return cand; // 有效食物位置
    }
    // 如果没有空位，返回 (-1,-1) 表示胜利
    return {-1, -1};
}

// 碰撞检测
bool isCollision(const Position& newHead, bool isEating) {
    // 边界碰撞（地图边界是墙）
    if (newHead.first <= 0 || newHead.first >= WIDTH - 1 ||
        newHead.second <= 0 || newHead.second >= HEIGHT - 1) {
        return true;
    }

    // 障碍物碰撞
    if (find(OBSTACLES.begin(), OBSTACLES.end(), newHead) != OBSTACLES.end()) {
        return true;
    }

    // 自身碰撞
    if (isEating) {
        // 吃食物时，蛇身会增长，新头不能与任何原有蛇身重合
        for (const auto& seg : snake) {
            if (seg == newHead)
                return true;
        }
    } else {
        // 不吃食物时，尾部会移除，所以新头不能与除尾部外的蛇身重合
        for (size_t i = 0; i < snake.size() - 1; ++i) {
            if (snake[i] == newHead)
                return true;
        }
    }
    return false;
}

// 处理键盘输入（非阻塞）
void processInput() {
    if (_kbhit()) {
        int ch = _getch();
        // 处理小写字母
        switch (ch) {
            case 'w': case 'W':
                if (dir != DIR_DOWN) dir = DIR_UP;
                break;
            case 's': case 'S':
                if (dir != DIR_UP) dir = DIR_DOWN;
                break;
            case 'a': case 'A':
                if (dir != DIR_RIGHT) dir = DIR_LEFT;
                break;
            case 'd': case 'D':
                if (dir != DIR_LEFT) dir = DIR_RIGHT;
                break;
            default:
                break;
        }
    }
}

// 更新游戏逻辑
void updateGame() {
    // 计算新蛇头位置
    Position newHead = {snake[0].first + dir.first, snake[0].second + dir.second};

    // 判断是否吃到食物
    bool isEating = (newHead == food);

    // 碰撞检测（传入是否吃食物的标志）
    if (isCollision(newHead, isEating)) {
        gameOver = true;
        return;
    }

    // 执行移动
    if (isEating) {
        // 吃食物：新头插入头部，尾部保留（长度+1）
        snake.insert(snake.begin(), newHead);
        score++;
        // 生成新食物
        food = generateFood();
        if (food.first == -1) {
            // 地图已满，胜利
            win = true;
            gameOver = true;
        }
    } else {
        // 普通移动：新头插入头部，移除尾部
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }
}

// 绘制地图（清屏后重绘）
void drawMap() {
    system("cls");  // 清屏

    // 显示分数
    cout << "Score: " << score << "   (WASD to move)\n";

    // 绘制上边界
    for (int x = 0; x < WIDTH; ++x) cout << "#";
    cout << "\n";

    // 绘制地图内容
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // 边界墙
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                cout << "#";
            }
            // 障碍物
            else if (find(OBSTACLES.begin(), OBSTACLES.end(), make_pair(x, y)) != OBSTACLES.end()) {
                cout << "#";
            }
            // 食物
            else if (x == food.first && y == food.second) {
                cout << "F";
            }
            // 蛇身
            else {
                auto it = find(snake.begin(), snake.end(), make_pair(x, y));
                if (it != snake.end()) {
                    if (it == snake.begin()) cout << "H";  // 蛇头
                    else cout << "S";                      // 蛇身
                } else {
                    cout << " ";  // 空格
                }
            }
        }
        cout << "\n";
    }

    // 绘制下边界（实际上已经包含在循环中，但为了整齐再画一行）
    for (int x = 0; x < WIDTH; ++x) cout << "#";
    cout << "\n";
}

// 游戏结束画面
void showGameOver() {
    system("cls");
    if (win) {
        cout << "Congratulations! You win!\n";
    } else {
        cout << "Game Over!\n";
    }
    cout << "Final Score: " << score << "\n";
    cout << "Press any key to exit...";
    _getch();
}