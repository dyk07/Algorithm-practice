#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
private:
    int width, height;
    vector<Point> snake;
    Point food;
    char direction;
    bool gameOver;
    int score;
    vector<Point> obstacles;

public:
    SnakeGame(int w = 20, int h = 10) : width(w), height(h), direction('d'), gameOver(false), score(0) {
        snake.push_back({5, 5});
        snake.push_back({4, 5});
        snake.push_back({3, 5});
        srand(time(0));
        generateFood();
        generateObstacles(5);
    }

    void generateFood() {
        do {
            food = {rand() % width, rand() % height};
        } while (isOnSnake(food) || isOnObstacle(food));
    }

    void generateObstacles(int count) {
        for (int i = 0; i < count; ++i) {
            Point obs;
            do {
                obs = {rand() % width, rand() % height};
            } while (isOnSnake(obs) || isOnObstacle(obs) || obs == food);
            obstacles.push_back(obs);
        }
    }

    bool isOnSnake(const Point& p) {
        for (const auto& s : snake)
            if (s == p) return true;
        return false;
    }

    bool isOnObstacle(const Point& p) {
        for (const auto& o : obstacles)
            if (o == p) return true;
        return false;
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'w': case 'W': if (direction != 's') direction = 'w'; break;
                case 's': case 'S': if (direction != 'w') direction = 's'; break;
                case 'a': case 'A': if (direction != 'd') direction = 'a'; break;
                case 'd': case 'D': if (direction != 'a') direction = 'd'; break;
                case 'q': case 'Q': gameOver = true; break;
            }
        }
    }

    void update() {
        Point head = snake.front();
        switch (direction) {
            case 'w': head.y--; break;
            case 's': head.y++; break;
            case 'a': head.x--; break;
            case 'd': head.x++; break;
        }

        // 撞墙检测
        if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height) {
            gameOver = true;
            return;
        }

        // 撞自身检测
        if (isOnSnake(head)) {
            gameOver = true;
            return;
        }

        // 撞障碍物检测
        if (isOnObstacle(head)) {
            gameOver = true;
            return;
        }

        snake.insert(snake.begin(), head);

        if (head == food) {
            score += 10;
            generateFood();
        } else {
            snake.pop_back();
        }
    }

    void draw() {
        system("cls");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Point p = {x, y};
                if (p == snake.front()) cout << 'O';
                else if (isOnSnake(p)) cout << 'o';
                else if (p == food) cout << '*';
                else if (isOnObstacle(p)) cout << '#';
                else cout << ' ';
            }
            cout << endl;
        }
        cout << "Score: " << score << "  |  Press 'q' to quit" << endl;
    }

    void run() {
        while (!gameOver) {
            input();
            update();
            draw();
            Sleep(150); // 控制速度
        }
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}