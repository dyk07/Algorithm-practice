if "__main__" == __name__:
    n = list()
    m = input().split()
    for c in m:
        if c.isdigit():
            n.append(int(c))
        else:
            if c == "+":
                n.append(n.pop() + n.pop())
            elif c == "-":
                n.append(-n.pop() + n.pop())
            elif c == "*":
                n.append(n.pop() * n.pop())
            elif c == "/":
                n.append((1 / n.pop() * n.pop())) 
    print(int(n[0]))