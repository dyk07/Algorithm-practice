#include <bits/stdc++.h>
#include <vector>
#include <utility>
using namespace std;

pair<vector<int>, vector<int>> merge(const vector<int> l_s, const vector<int> l_e, const vector<int> r_s, const vector<int> r_e){
    vector<int> result_s, result_e;
    size_t i = 0, j = 0;
    while(i < l_s.size() and j < r_s.size()){
        if (l_e[i] < r_e[j]){
            result_s.push_back(l_s[i]);
            result_e.push_back(l_e[i++]);
                }else{result_s.push_back(r_s[j]);
            result_e.push_back(r_e[j++]);}
    }
    while(i < l_s.size()){
        result_s.push_back(l_s[i]);
            result_e.push_back(l_e[i++]);
    }
    while(j < r_s.size()){
        result_s.push_back(r_s[j]);
            result_e.push_back(r_e[j++]);
    }
    return {result_s, result_e};
}

pair<vector<int>, vector<int>> mergesort(const vector<int> s, const vector<int> e){
    size_t si = s.size();
    if (si <= 1){
        return {s, e};
    }
    vector<int> ls(s.begin(),s.begin()+si/2);
    vector<int> le(e.begin(),e.begin()+si/2);
    vector<int> rs(s.begin()+si/2,s.begin()+si);
    vector<int> re(e.begin()+si/2,e.begin()+si);
    auto [lsf, lef] = mergesort(ls, le);
    auto [rsf, ref] = mergesort(rs, re);
    auto [sf, ef] = merge(lsf, lef, rsf, ref);
    return {sf, ef};
}


int main() {
    int tt;
    cin >>  tt;
    while (tt--){

        int n, m ,L , V, d, v, a, tempo;
        cin >> n >> m >> L >> V;
        vector<int> Start, End;
        int nn = n;
        while(nn--){
            cin >> d >> v >> a;
            if (a == 0) {
                if (v > V){
                    Start.push_back(d);
                    End.push_back(L);
                }
            }else if (a > 0) {
                if (v > V){
                    Start.push_back(d);
                    End.push_back(L);
                }
                if (v <= V){
                    tempo = (V*V - v*v) / (2*a) + 1;
                    if (tempo <= L - d){
                        Start.push_back(tempo + d);
                        End.push_back(L);
                    }    
                }
            }else{
                if (v > V){
                    Start.push_back(d);
                    tempo = (v*v-V*V-1)/(2*(-a));
                    if(tempo + d <= L){
                        End.push_back(tempo + d);
                    }else{
                        End.push_back(L); 
                }
            }}

        }

        vector<int> p(m);
        for (int i = 0; i < m; i++){
            cin >> p[i];
        }
        sort(p.begin(), p.end());

        /*vector<int> gs, ge;
        auto [E, S] = mergesort(End, Start);

        unsigned long long siz = S.size(), cj = 0;
        int ci = 0;
        while(ci < m and cj < siz){
            while(p[ci] > E[cj]){
                cj++;
            }
            if (S[cj] <= p[ci]){
                gs.push_back(S[cj]);
                ge.push_back(E[cj]);
                cj++;
            }else{ci++;}
        }
        */
       auto [sorted_Start, sorted_End] = mergesort(Start, End);
        vector<int> gs, ge;
        size_t interval_idx = 0;
        
        for (int i = 0; i < m && interval_idx < sorted_Start.size(); i++) {
            // 找到第一个包含当前检查点的区间
            while (interval_idx < sorted_Start.size() && 
                   sorted_End[interval_idx] < p[i]) {
                interval_idx++;
            }
            
            if (interval_idx < sorted_Start.size() && 
                sorted_Start[interval_idx] <= p[i] && 
                p[i] <= sorted_End[interval_idx]) {
                gs.push_back(sorted_Start[interval_idx]);
                ge.push_back(sorted_End[interval_idx]);
                interval_idx++;  // 这个区间已被使用
            }
        }

         cout << gs.size() << endl ;
         auto [sgs, sge] = mergesort(gs, ge);
         int val = 0;
         int fi = 0;
         size_t fj = 0;
         while(fi < m && fj < sgs.size()){
            while(fi < m && p[fi] <= sge[fj]){
                fi++;
            }
            val++;
            if(fi == m){
                break;
            }
            while(fj < sgs.size() && sgs[fj] <= p[fi-1]){
                fj++;
            }

         }
      

         cout << m-val;

         
           




        }




    return 0;
}