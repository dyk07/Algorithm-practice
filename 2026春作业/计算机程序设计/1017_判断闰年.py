if __name__ == "__main__":
    n = int(input())
    if n%4 != 0 :
        print("N")
    elif n%100 != 0:
        print("Y")
    elif n%400 != 0:
        print("N")
    else:
        print("Y")        
        