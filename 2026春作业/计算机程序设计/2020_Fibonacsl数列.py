if __name__ == "__main__" :
    f1, f2, a, b, n = map(int, input().split())
    if n == 1 :
        print(f1)
    else:
        for i in range(3, n + 1) :
            f1, f2 = f2, (a * f1 + b * f2)
        print(f2)