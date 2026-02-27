import scqubits as scq


zp_yaml = """# zero-pi
branches:
- [JJ, 1,2, EJ = 10nA, 200pF]
- [JJ, 3,4, EJ, 20GHz]
- [L, 2,3, 10uH]
- [L, 4,1, 0.008]
- [C, 1,3, 0.02]
- [C, 2,4, 0.02]
"""
zero_pi = scq.Circuit(zp_yaml, from_file=False)
print(zero_pi.hamiltonian())