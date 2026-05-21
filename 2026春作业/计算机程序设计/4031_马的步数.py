def f(x, y, s):
    if x >= 0 and x < n and y >= 0 and y < n and a[x][y] == -1:
        if a[x][y] == -1:
            a[x][y] = s
            b.append((x, y))

if __name__ == "__main__" :
    n = int(input())
    x1, y1 = map(int, input().split())
    x2, y2 = map(int, input().split())
    a = [[-1 for _ in range(1, n + 1)] for _ in range(1, n + 1)]
    a[x1 - 1][y1 - 1] = 0
    b = []
    b.append((x1 - 1, y1 - 1))
    while b:
        x, y = b.pop(0)
        f(x + 1, y + 2, a[x][y] + 1)
        f(x + 2, y + 1, a[x][y] + 1)
        f(x + 2, y - 1, a[x][y] + 1)
        f(x + 1, y - 2, a[x][y] + 1)
        f(x - 1, y - 2, a[x][y] + 1)
        f(x - 2, y - 1, a[x][y] + 1)
        f(x - 2, y + 1, a[x][y] + 1)
        f(x - 1, y + 2, a[x][y] + 1)
        if a[x2 - 1][y2 - 1] != -1:
            break
    print(a[x2 - 1][y2 - 1])