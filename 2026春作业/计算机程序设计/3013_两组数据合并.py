if "__main__" == __name__:
    m, n = map(int, input().split())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))
    c = list()
    i = 0
    j = 0
    
    while i < m and j < n:
        if a[i] < b[j]:
            c.append(a[i])
            i += 1
        elif a[i] > b[j]:
            c.append(b[j])
            j += 1
        else:
            c.append(a[i])
            i += 1
            j += 1
            
    c.extend(a[i:])
    c.extend(b[j:])
    print(*c)

