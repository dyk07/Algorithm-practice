if __name__ == "__main__" :
    s = input()
    i = 0
    cr = 0
    ma = 0
    for j in s :
        if i <= ord(j) :
            cr += 1
        else :
            ma = max(ma, cr)
            cr = 1
        i = ord(j)
    print(ma)