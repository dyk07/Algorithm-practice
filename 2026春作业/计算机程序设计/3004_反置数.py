def f(x):
    s = 0
    while x != 0 :
        s = s * 10 + x % 10
        x = x//10
    return s


if __name__ == "__main__" :
    x, y = map(int, input().split())
    sum = f(x) + f(y)
    print(f(sum))