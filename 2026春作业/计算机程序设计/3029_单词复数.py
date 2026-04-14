def f(x):
    if x[-1] in ['s', 'x', 'z'] or x[-2:] in ['ch', 'sh']:
        return x + 'es'
    if x[-1] == 'y' and not x[-2] in ['a', 'e', 'i', 'o', 'u']:
        return x[:-1] + 'ies'
    return x + 's'


if __name__ == "__main__":
    x = input()
    print(f(x))