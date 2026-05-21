x = 0
y = 0
i_s = {}

def run(ins):
    global x, y
    l = len(ins)
    i = 0
    while i < l:
        if ins[i] == 'CALL':
            i += 1
            run(i_s[ins[i]])
        elif ins[i] == 'U':
            y += 1
        elif ins[i] == 'D':
            y -= 1
        elif ins[i] == 'R':
            x += 1
        elif ins[i] == 'L':
            x -= 1
        i += 1

if __name__ == '__main__':
    
    F = int(input())
    for i in range(F):
        a = input().split()
        i_s[a[1]] = a[2:]
    
    M = int(input())
    ins = input().split()
    
    run(ins)
    t = (x, y)
    print(t)