if __name__ == "__main__":
    n = int(input())
    d = {}
    e = {}
    for i in range(0, n):
        p = input().split()
        m = int(p[0])
        for j in range(1, m + 1):
            k, v = p[2*j-1], p[2*j]
            if k in d:
                d[k] = max(d[k], float(v))
                if e[k] == i - 1:
                    e[k] = -1
                elif e[k] != -1:
                    e[k] = i
            else:
                d[k] = float(v)
                e[k] = i
    mm = 0
    r = [k for k, v in e.items() if v == -1]
    r.sort()
    s = ''
    for k in r:
        s += k + ' '
        mm = max(mm, d[k])
    print(s)
    print(f"{mm:.1f}")