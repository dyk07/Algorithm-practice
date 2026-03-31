if __name__ == "__main__" :
    d = {}
    n = int(input())
    while n > 0:
        n -= 1
        a = int(input())
        if a in d.keys() :
            d[a] += 1
        else:
            d.update({a: 1})
    k = 0
    val = -1
    for i in d.keys() :
        if d[i] > val or (d[i] == val and i < k) :
            k = i
            val = d[i]
    print(k)