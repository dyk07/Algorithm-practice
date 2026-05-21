class Student:
    def __init__(self, Name, Age, Type):
        self.Name = Name
        self.Age = Age
        self.Type = Type
        
class Undergraduate(Student):
    def __init__(self, Name, Age, Type, Specialty):
        super().__init__(Name, Age, Type)
        self.Specialty = Specialty
        
class Graduate(Student):
    def __init__(self, Name, Age, Type, Direction):
        super().__init__(Name, Age, Type)
        self.Direction = Direction
        

def main():
    n = int(input())
    d = dict()
    for _ in range(n):
        info = input().split()
        if info[2] == 'Undergraduate':
            d[info[0]] = Undergraduate(info[0], info[1], info[2], info[3])
        else:
            d[info[0]] = Graduate(info[0], info[1], info[2], info[3])
            
    m = int(input())
    for _ in range(m):
        info = input().split()
        if info[0] in d:
            if info[1] == 'Name':
                print(d[info[0]].Name)
            elif info[1] == 'Age':
                print(d[info[0]].Age)
            elif info[1] == 'Type':
                print(d[info[0]].Type)
            elif info[1] == 'Specialty' and d[info[0]].Type == 'Undergraduate':
                print(d[info[0]].Specialty)
            elif info[1] == 'Direction' and d[info[0]].Type == 'Graduate':
                print(d[info[0]].Direction)
            else:
                print('none')
        else:
            print('none')
            

if __name__ == "__main__":
    main()