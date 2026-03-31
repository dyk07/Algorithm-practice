#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <queue>

using namespace std;

// ==================== 游戏配置 ====================
const int WIDTH = 20;                    // 地图宽度（列数）
const int HEIGHT = 20;                   // 地图高度（行数）
const int INIT_LENGTH = 3;               // 初始蛇长
const int INIT_SPEED_MS = 200;           // 初始移动间隔（毫秒）
const int MIN_SPEED_MS = 80;             // 最快移动间隔（毫秒）
const int SPEED_DECREMENT = 10;          // 每3分减少的延迟（毫秒）
const int SCORE_PER_SPEED_STEP = 3;      // 每多少分加速一次
const int DYNAMIC_OBSTACLE_INTERVAL = 10000; // 动态障碍物生成间隔（毫秒）
const int DYNAMIC_OBSTACLE_DURATION = 5000;  // 动态障碍物持续时间（毫秒）

// 静态障碍物坐标
const vector<pair<int, int>> STATIC_OBSTACLES = {
    {5, 5}, {5, 6}, {5, 7},
    {14, 12}, {14, 13}, {14, 14},
    {8, 15}, {9, 15}, {10, 15}
};

// 道具类型（扩展为6种）
enum ItemType {
    ITEM_NONE = 0,
    ITEM_SCORE_BOOST,   // 加分道具
    ITEM_SHRINK,        // 缩短蛇身道具
    ITEM_SPEED_UP,      // 加速道具
    ITEM_SPEED_DOWN,    // 减速道具
    ITEM_REVERSE,       // 反向道具
    ITEM_INVINCIBLE     // 无敌道具
};

using Position = pair<int, int>;

// 方向向量
const Position DIR_UP    = {0, -1};
const Position DIR_DOWN  = {0, 1};
const Position DIR_LEFT  = {-1, 0};
const Position DIR_RIGHT = {1, 0};

// ==================== 全局变量 ====================
vector<Position> snake;          // 蛇身坐标，蛇头在 front
Position food;                   // 食物坐标
Position dir;                    // 当前移动方向
int score;                       // 得分
int highScore;                   // 最高分（本次运行）
bool gameOver;                   // 游戏结束标志
bool win;                        // 胜利标志（占满地图）
int gameSpeedMs;                 // 当前移动间隔（动态变化）
Position itemPos;                // 道具位置，(-1,-1)表示不存在
ItemType itemType;               // 当前道具类型
bool paused;                     // 暂停标志
bool autoMode;                   // 自动导航模式（作弊模式）

// 动态障碍物
vector<Position> dynamicObstacles; // 当前动态障碍物列表
int dynamicObstacleTimer;          // 距离下次生成动态障碍物的时间（毫秒）

// 状态效果（剩余毫秒）
int speedUpRemaining = 0;      // 加速剩余时间
int speedDownRemaining = 0;     // 减速剩余时间
int reverseRemaining = 0;       // 反向剩余时间
int invincibleRemaining = 0;     // 无敌剩余时间

// ==================== 函数声明 ====================
void initGame();
void drawMap();
Position generateFood();
bool isValidPosition(const Position& pos, bool checkItem = true, bool ignoreDynamic = false);
bool isCollision(const Position& newHead, bool isEating, bool invincible);
void processInput();
void updateGame();
bool showGameOver();
void adjustSpeedByScore();
void generateItem();
void applyItemEffect();
void updateHighScore();
void aiMove();
void updateStatusEffects();
void updateDynamicObstacles();
float getCurrentSpeedFactor();
int getCurrentDelay();
Position oppositeDir(const Position& d);

// ==================== 主函数 ====================
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    highScore = 0;

    bool playAgain = true;
    while (playAgain) {
        initGame();

        while (!gameOver) {
            processInput();      // 处理键盘输入
            if (!paused) {
                updateGame();    // 更新游戏逻辑
            }
            drawMap();           // 绘制画面
            Sleep(getCurrentDelay()); // 控制速度（应用速度效果）
        }

        updateHighScore();
        playAgain = showGameOver();
    }

    return 0;
}

// ==================== 初始化 ====================
void initGame() {
    // 初始化蛇
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;
    snake.clear();
    for (int i = 0; i < INIT_LENGTH; ++i) {
        snake.push_back({startX - i, startY});
    }
    dir = DIR_RIGHT;
    score = 0;
    gameOver = false;
    win = false;
    paused = false;
    autoMode = false;
    gameSpeedMs = INIT_SPEED_MS;

    // 初始化道具
    itemPos = {-1, -1};
    itemType = ITEM_NONE;

    // 初始化状态效果
    speedUpRemaining = 0;
    speedDownRemaining = 0;
    reverseRemaining = 0;
    invincibleRemaining = 0;

    // 初始化动态障碍物
    dynamicObstacles.clear();
    dynamicObstacleTimer = DYNAMIC_OBSTACLE_INTERVAL; // 10秒后生成第一批

    // 生成食物
    food = generateFood();
    if (food.first == -1) {
        win = true;
        gameOver = true;
    }
}

// ==================== 位置有效性检查 ====================
bool isValidPosition(const Position& pos, bool checkItem, bool ignoreDynamic) {
    // 边界检查（地图边界是墙，不可占用）
    if (pos.first <= 0 || pos.first >= WIDTH - 1 ||
        pos.second <= 0 || pos.second >= HEIGHT - 1) {
        return false;
    }

    // 蛇身检查
    if (find(snake.begin(), snake.end(), pos) != snake.end()) {
        return false;
    }

    // 静态障碍物检查
    if (find(STATIC_OBSTACLES.begin(), STATIC_OBSTACLES.end(), pos) != STATIC_OBSTACLES.end()) {
        return false;
    }

    // 动态障碍物检查
    if (!ignoreDynamic && find(dynamicObstacles.begin(), dynamicObstacles.end(), pos) != dynamicObstacles.end()) {
        return false;
    }

    // 食物检查
    if (pos == food) {
        return false;
    }

    // 道具检查
    if (checkItem && itemPos.first != -1 && pos == itemPos) {
        return false;
    }

    return true;
}

// ==================== 食物生成 ====================
Position generateFood() {
    const int MAX_TRIES = 10000;
    for (int tries = 0; tries < MAX_TRIES; ++tries) {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;
        Position cand = {x, y};

        if (isValidPosition(cand, true, false)) {
            return cand;
        }
    }
    return {-1, -1}; // 没有空位，胜利
}

// ==================== 道具生成 ====================
void generateItem() {
    if (itemPos.first != -1) return;

    // 40%概率生成道具
    if (rand() % 100 < 40) {
        const int MAX_TRIES = 1000;
        for (int tries = 0; tries < MAX_TRIES; ++tries) {
            int x = rand() % (WIDTH - 2) + 1;
            int y = rand() % (HEIGHT - 2) + 1;
            Position cand = {x, y};

            if (isValidPosition(cand, false, false)) {
                itemPos = cand;
                // 从6种道具中随机选择
                itemType = static_cast<ItemType>(rand() % 6 + 1); // 1~6
                return;
            }
        }
    }
}

// ==================== 道具效果应用 ====================
void applyItemEffect() {
    switch (itemType) {
        case ITEM_SCORE_BOOST:
            score += 3;
            adjustSpeedByScore();
            break;

        case ITEM_SHRINK:
            if (snake.size() > INIT_LENGTH) {
                int removeCount = min(2, (int)snake.size() - INIT_LENGTH);
                for (int i = 0; i < removeCount; ++i) {
                    snake.pop_back();
                }
            }
            break;

        case ITEM_SPEED_UP:
            speedUpRemaining = 3000;   // 持续3秒
            break;

        case ITEM_SPEED_DOWN:
            speedDownRemaining = 3000;
            break;

        case ITEM_REVERSE:
            reverseRemaining = 5000;   // 持续5秒
            break;

        case ITEM_INVINCIBLE:
            invincibleRemaining = 2000; // 持续2秒
            break;

        default:
            break;
    }

    // 道具被使用后清除
    itemPos = {-1, -1};
    itemType = ITEM_NONE;
}

// ==================== 动态障碍物管理 ====================
void updateDynamicObstacles() {
    if (dynamicObstacleTimer > 0) {
        dynamicObstacleTimer -= getCurrentDelay(); // 减去一帧的时间
        return;
    }

    // 时间到，生成新的动态障碍物
    dynamicObstacles.clear();

    // 随机生成1~3个障碍物
    int count = rand() % 3 + 1;
    for (int i = 0; i < count; ++i) {
        const int MAX_TRIES = 1000;
        for (int tries = 0; tries < MAX_TRIES; ++tries) {
            int x = rand() % (WIDTH - 2) + 1;
            int y = rand() % (HEIGHT - 2) + 1;
            Position cand = {x, y};

            // 不能与蛇身、食物、道具、静态障碍物、已有动态障碍物重叠
            if (isValidPosition(cand, true, true)) {
                dynamicObstacles.push_back(cand);
                break;
            }
        }
    }

    // 重置计时器，下一次生成在 DYNAMIC_OBSTACLE_INTERVAL 毫秒后
    dynamicObstacleTimer = DYNAMIC_OBSTACLE_INTERVAL;
}

// ==================== 状态效果更新 ====================
void updateStatusEffects() {
    int frameTime = getCurrentDelay(); // 使用当前延迟作为时间步长

    if (speedUpRemaining > 0) speedUpRemaining -= frameTime;
    if (speedDownRemaining > 0) speedDownRemaining -= frameTime;
    if (reverseRemaining > 0) reverseRemaining -= frameTime;
    if (invincibleRemaining > 0) invincibleRemaining -= frameTime;

    // 防止负值
    if (speedUpRemaining < 0) speedUpRemaining = 0;
    if (speedDownRemaining < 0) speedDownRemaining = 0;
    if (reverseRemaining < 0) reverseRemaining = 0;
    if (invincibleRemaining < 0) invincibleRemaining = 0;
}

// ==================== 速度相关 ====================
// 获取速度因子（加速/减速效果乘积）
float getCurrentSpeedFactor() {
    float factor = 1.0f;
    if (speedUpRemaining > 0) factor *= 1.5f;
    if (speedDownRemaining > 0) factor *= 0.5f;
    return factor;
}

// 获取当前实际延迟（毫秒）
int getCurrentDelay() {
    int baseDelay = gameSpeedMs;
    float factor = getCurrentSpeedFactor();
    int delay = static_cast<int>(baseDelay / factor);
    if (delay < 1) delay = 1; // 确保最小延迟
    return delay;
}

// 根据分数调整基础速度（难度递增）
void adjustSpeedByScore() {
    int newSpeed = INIT_SPEED_MS - (score / SCORE_PER_SPEED_STEP) * SPEED_DECREMENT;
    newSpeed = max(MIN_SPEED_MS, newSpeed);
    gameSpeedMs = newSpeed;
}

// ==================== 方向辅助 ====================
Position oppositeDir(const Position& d) {
    if (d == DIR_UP) return DIR_DOWN;
    if (d == DIR_DOWN) return DIR_UP;
    if (d == DIR_LEFT) return DIR_RIGHT;
    if (d == DIR_RIGHT) return DIR_LEFT;
    return d;
}

// ==================== 碰撞检测 ====================
bool isCollision(const Position& newHead, bool isEating, bool invincible) {
    // 边界碰撞（无敌不豁免边界）
    if (newHead.first <= 0 || newHead.first >= WIDTH - 1 ||
        newHead.second <= 0 || newHead.second >= HEIGHT - 1) {
        return true;
    }

    // 无敌状态下忽略障碍物和自身碰撞
    if (invincible) {
        return false;
    }

    // 静态障碍物碰撞
    if (find(STATIC_OBSTACLES.begin(), STATIC_OBSTACLES.end(), newHead) != STATIC_OBSTACLES.end()) {
        return true;
    }

    // 动态障碍物碰撞
    if (find(dynamicObstacles.begin(), dynamicObstacles.end(), newHead) != dynamicObstacles.end()) {
        return true;
    }

    // 自身碰撞
    if (isEating) {
        // 吃食物时，新头不能与任何原有蛇身重合
        for (const auto& seg : snake) {
            if (seg == newHead) return true;
        }
    } else {
        // 不吃食物时，新头不能与除尾部外的蛇身重合
        for (size_t i = 0; i < snake.size() - 1; ++i) {
            if (snake[i] == newHead) return true;
        }
    }

    return false;
}

// ==================== AI 自动导航 ====================
void aiMove() {
    Position head = snake[0];

    // BFS 寻路（避开静态障碍物、动态障碍物、蛇身，但允许经过即将释放的蛇尾）
    vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
    vector<vector<Position>> parent(HEIGHT, vector<Position>(WIDTH, {-1, -1}));
    queue<Position> q;

    visited[head.second][head.first] = true;
    q.push(head);

    const vector<Position> dirs = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
    bool found = false;

    while (!q.empty() && !found) {
        Position cur = q.front(); q.pop();
        int cx = cur.first, cy = cur.second;

        for (const auto& d : dirs) {
            int nx = cx + d.first, ny = cy + d.second;
            Position nxt = {nx, ny};

            if (nx <= 0 || nx >= WIDTH - 1 || ny <= 0 || ny >= HEIGHT - 1) continue;

            // 静态障碍物
            if (find(STATIC_OBSTACLES.begin(), STATIC_OBSTACLES.end(), nxt) != STATIC_OBSTACLES.end())
                continue;

            // 动态障碍物（AI 会避开，除非无敌，但无敌状态下 AI 也可以无视，但为简单，AI 始终避开）
            if (find(dynamicObstacles.begin(), dynamicObstacles.end(), nxt) != dynamicObstacles.end())
                continue;

            // 蛇身（允许经过蛇尾）
            bool isTail = (!snake.empty() && nxt == snake.back() && nxt != head);
            if (!isTail && find(snake.begin(), snake.end(), nxt) != snake.end())
                continue;

            if (!visited[ny][nx]) {
                visited[ny][nx] = true;
                parent[ny][nx] = cur;
                q.push(nxt);

                if (nxt == food) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (found) {
        // 回溯得到第一步
        Position cur = food;
        Position step = cur;
        while (true) {
            Position prev = parent[cur.second][cur.first];
            if (prev == head) {
                step = cur;
                break;
            }
            cur = prev;
        }

        Position targetDir;
        if (step.first > head.first) targetDir = DIR_RIGHT;
        else if (step.first < head.first) targetDir = DIR_LEFT;
        else if (step.second > head.second) targetDir = DIR_DOWN;
        else targetDir = DIR_UP;

        // 如果当前有反向效果，则 AI 计算的方向需要再取反
        if (reverseRemaining > 0) {
            dir = oppositeDir(targetDir);
        } else {
            dir = targetDir;
        }
        return;
    }

    // 无路径时，安全优先策略
    const vector<Position> dirsOrder = {dir, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
    for (const auto& d : dirsOrder) {
        Position newHead = {head.first + d.first, head.second + d.second};
        if (newHead.first > 0 && newHead.first < WIDTH - 1 &&
            newHead.second > 0 && newHead.second < HEIGHT - 1 &&
            find(STATIC_OBSTACLES.begin(), STATIC_OBSTACLES.end(), newHead) == STATIC_OBSTACLES.end() &&
            find(dynamicObstacles.begin(), dynamicObstacles.end(), newHead) == dynamicObstacles.end() &&
            find(snake.begin(), snake.end(), newHead) == snake.end()) {
            if (reverseRemaining > 0) {
                dir = oppositeDir(d);
            } else {
                dir = d;
            }
            return;
        }
    }
}

// ==================== 键盘输入处理 ====================
void processInput() {
    if (_kbhit()) {
        int ch = _getch();
        switch (ch) {
            case 'w': case 'W':
                if (!autoMode && dir != DIR_DOWN) dir = DIR_UP;
                break;
            case 's': case 'S':
                if (!autoMode && dir != DIR_UP) dir = DIR_DOWN;
                break;
            case 'a': case 'A':
                if (!autoMode && dir != DIR_RIGHT) dir = DIR_LEFT;
                break;
            case 'd': case 'D':
                if (!autoMode && dir != DIR_LEFT) dir = DIR_RIGHT;
                break;
            case 'p': case 'P':
                paused = !paused;
                break;
            case 'n': case 'N':
                autoMode = !autoMode;
                break;
            default:
                break;
        }
    }
}

// ==================== 游戏逻辑更新 ====================
void updateGame() {
    // 更新状态效果和动态障碍物
    updateStatusEffects();
    updateDynamicObstacles();

    // 自动导航模式
    if (autoMode) {
        aiMove();
    }

    // 计算实际移动方向（考虑反向效果）
    Position actualDir = (reverseRemaining > 0) ? oppositeDir(dir) : dir;
    Position newHead = {snake[0].first + actualDir.first, snake[0].second + actualDir.second};

    bool isEating = (newHead == food);
    bool invincible = (invincibleRemaining > 0);

    if (isCollision(newHead, isEating, invincible)) {
        gameOver = true;
        return;
    }

    // 移动
    if (isEating) {
        snake.insert(snake.begin(), newHead);
        score++;
        adjustSpeedByScore();

        food = generateFood();
        if (food.first == -1) {
            win = true;
            gameOver = true;
            return;
        }

        generateItem(); // 吃完食物后可能生成道具
    } else {
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }

    // 吃道具
    if (itemPos.first != -1 && newHead == itemPos) {
        applyItemEffect();
    }
}

// ==================== 画面绘制 ====================
void drawMap() {
    system("cls");

    // 状态栏
    cout << "Score: " << score << "   High Score: " << highScore
         << "   Base Speed: " << gameSpeedMs << " ms"
         << "   Speed Factor: " << getCurrentSpeedFactor();
    if (paused) cout << "   [PAUSED]";
    if (autoMode) cout << "   [AUTO MODE]";
    if (invincibleRemaining > 0) cout << "   [INVINCIBLE]";
    if (reverseRemaining > 0) cout << "   [REVERSE]";
    cout << "\nWASD to move, P pause, N auto-mode\n";

    // 道具信息
    cout << "Item: ";
    if (itemPos.first != -1) {
        switch (itemType) {
            case ITEM_SCORE_BOOST: cout << "+3 points (B)"; break;
            case ITEM_SHRINK:      cout << "Shrink (S)"; break;
            case ITEM_SPEED_UP:    cout << "Speed Up (U)"; break;
            case ITEM_SPEED_DOWN:  cout << "Speed Down (D)"; break;
            case ITEM_REVERSE:     cout << "Reverse (R)"; break;
            case ITEM_INVINCIBLE:  cout << "Invincible (I)"; break;
            default: cout << "?"; break;
        }
    } else {
        cout << "None";
    }
    cout << "\n";

    // 绘制上边界
    for (int x = 0; x < WIDTH; ++x) cout << "#";
    cout << "\n";

    // 绘制地图内容
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // 边界
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                cout << "#";
            }
            // 静态障碍物
            else if (find(STATIC_OBSTACLES.begin(), STATIC_OBSTACLES.end(), make_pair(x, y)) != STATIC_OBSTACLES.end()) {
                cout << "#";
            }
            // 动态障碍物
            else if (find(dynamicObstacles.begin(), dynamicObstacles.end(), make_pair(x, y)) != dynamicObstacles.end()) {
                cout << "%";   // 用 % 区分动态墙
            }
            // 食物
            else if (x == food.first && y == food.second) {
                cout << "F";
            }
            // 道具
            else if (itemPos.first != -1 && x == itemPos.first && y == itemPos.second) {
                char itemChar;
                switch (itemType) {
                    case ITEM_SCORE_BOOST: itemChar = 'B'; break;
                    case ITEM_SHRINK:      itemChar = 'S'; break;
                    case ITEM_SPEED_UP:    itemChar = 'U'; break;
                    case ITEM_SPEED_DOWN:  itemChar = 'D'; break;
                    case ITEM_REVERSE:     itemChar = 'R'; break;
                    case ITEM_INVINCIBLE:  itemChar = 'I'; break;
                    default: itemChar = '?'; break;
                }
                cout << itemChar;
            }
            // 蛇身
            else {
                auto it = find(snake.begin(), snake.end(), make_pair(x, y));
                if (it != snake.end()) {
                    if (it == snake.begin()) cout << "H";
                    else cout << "S";
                } else {
                    cout << " ";
                }
            }
        }
        cout << "\n";
    }

    // 绘制下边界
    for (int x = 0; x < WIDTH; ++x) cout << "#";
    cout << "\n";
}

// ==================== 结束画面 ====================
void updateHighScore() {
    if (score > highScore) highScore = score;
}

bool showGameOver() {
    system("cls");
    if (win) {
        cout << "Congratulations! You win!\n";
    } else {
        cout << "Game Over!\n";
    }
    cout << "Final Score: " << score << "\n";
    cout << "High Score: " << highScore << "\n";
    cout << "Press 'R' to replay, any other key to exit...";

    while (true) {
        int ch = _getch();
        if (ch == 'r' || ch == 'R') return true;
        else return false;
    }
}