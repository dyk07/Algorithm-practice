if __name__ == '__main__':
    n = int(input())
    time = []
    ans = []
    for i in range(n):
        time.append(list(map(int, input().split())))
    time.sort(key=lambda x: x[0])
    
    ans.append(time[0])
    for i in range(1, n):
        if time[i][0] <= ans[-1][1]:
            ans[-1][1] = max(ans[-1][1], time[i][1])
        else:
            ans.append(time[i])
            
    for i in ans:
        print(i[0], i[1])