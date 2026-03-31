if __name__ == "__main__" :
    a = list(map(int, input().split()))
    i = 0
    ma = 0
    mi = 100
    while i < 10 :
        ma = max(a[i], ma)
        mi = min(a[i], mi)
        i += 1
    s = - mi - ma
    i = 0
    while i < 10 :
        s += a[i]
        i += 1
    g = s//8
    if s < g*8 + 4:
        print(g)
    else:
        print(g + 1)
        