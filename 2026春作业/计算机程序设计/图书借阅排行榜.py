if __name__ == "__main__":
    n = int(input())
    d = {}
    for _ in range(n):
        k, v = input().split()
        if k in d:
            d[k] += int(v)
        else:
            d[k] = int(v)
    s = sorted(d.items(), key=lambda x: (-x[1], x[0]))
    
    for k, v in s[:3]:
        print(k, v)