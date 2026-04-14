if __name__ == "__main__" :
    c = input()
    n = len(c)
    s = 0
    i = 0
    while i < n :
        s += int(c[n - i - 1]) * 2**i
        i += 1
    print(s)