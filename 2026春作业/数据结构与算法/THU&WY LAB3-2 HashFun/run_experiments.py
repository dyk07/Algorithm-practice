import os
import subprocess
import time
import sys

# Configuration
TIMEOUT_SECONDS = 30.0 
COMPILER = "g++"
HASHTABLE_SRC = ["main.cpp", "hashtable.cpp"]
HASHTABLE_EXE = "hashtable_app"

MAX_N = 100000

DATASETS = [
    {
        "name": "1 (Extreme Load)", 
        "N": MAX_N, 
        "M": 20000, 
        "desc": "Tests collision handling at 97% capacity."
    },
    {
        "name": "2 (Massive Queries)", 
        "N": 20000, 
        "M": 100000, 
        "desc": "Tests raw hashing speed (computation vs I/O)."
    },
    {
        "name": "3 (XOR Attack)", 
        "N": 30000, 
        "M": 100000, 
        "desc": "Uses anagrams to break bad_hashing (XOR logic)."
    }
]

def generate_adversarial_data(filename, N, M):
    """Generates anagrams that have the same XOR sum to break bad_hashing."""
    with open(filename, 'w') as f:
        # Generate anagrams of 'abcdefgh'
        base = "abcdefgh"
        import itertools
        perms = itertools.islice(itertools.permutations(base), N)
        words = []
        for p in perms:
            word = "".join(p)
            words.append(word)
            f.write(f"0 {word} 100\n")
        
        # Query the same words
        for i in range(M):
            f.write(f"1 {words[i % len(words)]}\n")
        f.write("2\n")

def run_test(h_id, c_id):
    hash_exe = f"./{HASHTABLE_EXE}" if os.name != 'nt' else f"{HASHTABLE_EXE}.exe"
    start_time = time.time()
    try:
        with open("test.in", "r") as infile:
            subprocess.run(
                [hash_exe, str(h_id), str(c_id)],
                stdin=infile,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                timeout=TIMEOUT_SECONDS
            )
        return time.time() - start_time
    except subprocess.TimeoutExpired:
        return None

def main():
    # Compile
    subprocess.run([COMPILER] + HASHTABLE_SRC + ["-o", HASHTABLE_EXE, "-O3"]) # Using O3 for max speed
    
    results = []
    for ds in DATASETS:
        print(f"\nPreparing {ds['name']}...")
        if "Attack" in ds['name']:
            generate_adversarial_data("test.in", ds['N'], ds['M'])
        else:
            # Use your original generator for normal datasets
            gen_exe = f"./generator" if os.name != 'nt' else "generator.exe"
            subprocess.run([gen_exe, str(ds['N']), str(ds['M'])])

        for h_id in [1, 2]: # bad_hashing, good_hashing
            for c_id in [0, 1, 2]: # linear, quadratic, overflow
                h_name = "bad_hash" if h_id == 1 else "good_hash"
                c_name = ["linear", "quad", "overflow"][c_id]
                
                print(f"  Running {h_name} + {c_name}...", end="", flush=True)
                elapsed = run_test(h_id, c_id)
                
                if elapsed is None:
                    print(" TIMEOUT")
                    res_str = f"> {TIMEOUT_SECONDS}s"
                else:
                    print(f" {elapsed:.4f}s")
                    res_str = f"{elapsed:.4f}s"
                
                results.append((ds['name'], h_name, c_name, res_str))

    # Output Markdown Table
    print("\n| Dataset | Hashing | Collision | Time |")
    print("| :--- | :--- | :--- | :--- |")
    for r in results:
        print(f"| {r[0]} | {r[1]} | {r[2]} | {r[3]} |")

if __name__ == "__main__":
    main()