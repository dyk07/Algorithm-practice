if __name__ == "__main__":
    n = int(input())
    d = {}
    for _ in range(0, n):
        v = input().split()
        for k in v:
            if k in d:
                d[k] += 1
            else:
                d[k] = 1
    r = [k for k, v in d.items() if v == n]
    if not r:
        print("None")
    else:
        r.sort()
        print(' '.join(r))
    r  = [k for k, v in d.items() if v == 1]
    if not r:
        print("None")
    else:
        r.sort()
        print(' '.join(r))