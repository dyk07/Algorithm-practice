def a(n):
    if n < 10 :
        return n
    
    s = 1
    while n != 0 :
        b = n%10
        if b != 0 :
            s *= b
        n = n//10
    return a(s)

if __name__ == "__main__" :
    n = int(input())
    print(a(n))