if __name__ == "__main__":
    aa, bb, cc = input().split()
    a = int(aa)
    b = int(bb)
    c = int(cc)
    if a <= b:
        if b <= c:
            print(a, b, c)
        elif a < c:
            print(a, c, b)
        else:
            print(c, a, b)
    else:
        if b > c:
            print(c, b, a)
        elif a < c:
            print(b, a, c)
        else:
            print(b, c, a)
             