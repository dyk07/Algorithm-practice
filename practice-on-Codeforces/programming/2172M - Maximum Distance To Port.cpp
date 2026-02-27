#include <bits/stdc++.h>
using namespace std;



int main() {
    
    
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> p(n);
    
    for(int i = 0; i < n;i++){
        cin >> p[i];
    }
    vector<vector<int>> a(n);
    while(m--){
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        a[u].push_back(v);
        a[v].push_back(u);
    }

    
    vector<int> val(n, 0);
    vector<int> dis(n,-1);
    queue<int> q;
    dis[0] = 0;
    q.push(0);

    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v : a[u]){
            if(dis[v] == -1){
                dis[v] = dis[u] + 1;
                int tm = p[v] - 1;
                val[tm] = dis[v];
                q.push(v);
            }
        }
        
    }

    for(int i = 0; i < k; i++){
        cout << val[i] << ' ';
    }

    return 0;
}

