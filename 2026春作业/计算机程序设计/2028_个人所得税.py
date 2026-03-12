if __name__ == "__main__":
    n = int(input())
    t = 0
    if n <= 3500:
        print(0.0)
    else:
        n -= 3500
        if n <= 500:
            print(f"{n * 0.05:.1f}")
        else:
            t += 25
            if n <= 2000:
                print(f"{t + (n - 500) * 0.1:.1f}")
            else:
                t += 150
                if n <= 5000:
                    print(f"{t + (n - 2000) * 0.15:.1f}")
                else:
                    t += 450
                    if n <= 20000:
                        print(f"{t + (n - 5000) * 0.2:.1f}")
                    else:
                        t += 3000
                        if n <= 40000:
                            print(f"{t + (n - 20000) * 0.25:.1f}")
                        else:
                            t += 5000
                            if n <= 60000:
                                print(f"{t + (n - 40000) * 0.3:.1f}")
                            else:
                                t += 6000
                                print(f"{t + (n - 60000) * 0.5:.1f}")