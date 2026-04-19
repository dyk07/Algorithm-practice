t = """2	4	6	8	9	7	5	3
3	4	6	7	8	4	3	2
5	8	4	9	0	4	3	2
4	3	2	1	6	7	8	9
4	5	6	7	8	9	0	7
2	3	4	6	7	8	9	6
1	2	3	4	5	6	7	8
0	9	8	7	6	5	4	3""".split()

def f(a, b):
    return int(t[(8-a)*8 + (b-1)])

if "__main__" == __name__:
    s = 0
    n = int(input())
    a, b = map(int, input().split())
    s += f(a, b)
    while n > 1:
        n -= 1
        x, y = map(int, input().split())
        if x == a:
            if y > b:
                for i in range(b+1, y+1):
                    s += f(a, i)
            else:
                for i in range(y, b):
                    s += f(a, i)
        elif y == b:
            if x > a:
                for i in range(a+1, x+1):
                    s += f(i, b)
            else:
                for i in range(x, a):
                    s += f(i, b)
        a, b = x, y
    print(s)