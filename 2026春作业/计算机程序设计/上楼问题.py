if __name__ == "__main__":
    n = int(input())
    if n == 1:
        print(1)
    elif n == 2:
        print(2)
    else:
        a = 1
        b = 2
        c = 4
        for i in range(4, n + 1):
            a, b, c = b, c, a + b + c
        print(c)