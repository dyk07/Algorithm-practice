# 哈希表性能测试结果

| Dataset | Hashing | Collision | Time |
| :--- | :--- | :--- | :--- |
| 1 (Extreme Load) | bad_hash | linear | 7.8072s |
| 1 (Extreme Load) | bad_hash | quad | 7.5845s |
| 1 (Extreme Load) | bad_hash | overflow | 3.2852s |
| 1 (Extreme Load) | good_hash | linear | 7.2886s |
| 1 (Extreme Load) | good_hash | quad | 7.3939s |
| 1 (Extreme Load) | good_hash | overflow | 3.1167s |
| 2 (Massive Queries) | bad_hash | linear | 4.9792s |
| 2 (Massive Queries) | bad_hash | quad | 4.0637s |
| 2 (Massive Queries) | bad_hash | overflow | 3.6634s |
| 2 (Massive Queries) | good_hash | linear | 3.8128s |
| 2 (Massive Queries) | good_hash | quad | 4.0498s |
| 2 (Massive Queries) | good_hash | overflow | 3.0723s |
| 3 (XOR Attack) | bad_hash | linear | 6.7632s |
| 3 (XOR Attack) | bad_hash | quad | 8.3617s |
| 3 (XOR Attack) | bad_hash | overflow | 4.7044s |
| 3 (XOR Attack) | good_hash | linear | 5.9130s |
| 3 (XOR Attack) | good_hash | quad | 8.0995s |
| 3 (XOR Attack) | good_hash | overflow | 4.5837s |