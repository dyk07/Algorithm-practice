def f(x):
    s = 0
    while x > 0:
        s += x % 10
        x = x // 10
    return s


if __name__ == "__main__":
    x, y, z = map(int, input().split())
    s = f(x) + f(y) + f(z)
    while s > 9:
        s = f(s)
    print(s)