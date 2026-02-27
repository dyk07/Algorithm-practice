#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
vector<string> t[N];
int m[N];

struct ord{
    int nex;
    int pre;
};

const int M = 3e5 + 5;
vector<vector<int>> cls = {{}};
ord xx[M], yy[M];
int id[M][2];
int num;
map<string, int> pret;
int pretty = 0;
map<string, int> cnt;
int init = 0;

double cal(){
    double s = 0;
    int ii = init;
    while(ii < cls.size()){
        pretty++;
        vector<int> i = cls[ii];
        int len = i.size();
        if(len == 1){
            if(ii == 0){
                init = yy[ii].nex;
            }else{
                yy[yy[ii].pre].nex = yy[ii].nex;
                yy[yy[ii].nex].pre = yy[ii].pre;
            }
        }else{
            for(int j = 0; j < len; j++){
                string str = t[id[i[j]][0]][id[i[j]][1]];
                if(pret[str] != pretty){
                    pret[str] = pretty;
                    cnt[str] = 1;
                    xx[j].nex = j + 1;
                    xx[j + 1].pre = j;
                }else{
                    cnt[str] += 1;
                    xx[xx[j].pre].nex = j + 1;
                    xx[j + 1].pre = xx[j].pre;
                }
            }
            int cur = 0;
            while(cur < len){
                int tmp = cnt[t[id[i[cur]][0]][id[i[cur]][1]]];
                s += -log2((double)tmp/len)*tmp;
                cur = xx[cur].nex;
            }
        }
        ii = yy[ii].nex;
    }
    return s;
}

void arrange(int k){
    int i = init;
    while(i < cls.size()){
        if(id[cls[i][0]][1] >= k){
            string val = t[id[cls[i][0]][0]][id[cls[i][0]][1] - k];
            int j = 1;
            while(j < cls[i].size()){
                if(id[cls[i][j]][1] >= k && t[id[cls[i][j]][0]][id[cls[i][j]][1] - k] == val){
                    j++;
                    continue;
                }
                break;
            }
            if(j < cls[i].size()){ 
                int cur = cls.size();
                int cntt = 0;    
                yy[cur].nex = cur + 1;
                yy[cur + 1].pre = cur;
                cls.push_back({});
                while(j < cls[i].size()){
                    if(id[cls[i][j]][1] >= k && t[id[cls[i][j]][0]][id[cls[i][j]][1] - k] == val){
                        cls[i][j - cntt] = cls[i][j];
                        j++;
                        continue;
                    }else{
                        cntt++;
                        cls[cur].push_back(cls[i][j]);
                        j++;
                    }
                }
                cls[i].resize(cls[i].size() - cntt);
            }
        }else{
            int j = 1;
            while(j < cls[i].size()){
                if(id[cls[i][j]][1] < k){
                    j++;
                    continue;
                }
                break;
            }
            if(j < cls[i].size()){ 
                int cur = cls.size();
                int cntt = 0;    
                yy[cur].nex = cur + 1;
                yy[cur + 1].pre = cur;
                cls.push_back({});
                while(j < cls[i].size()){
                    if(id[cls[i][j]][1] < k){
                        cls[i][j - cntt] = cls[i][j];
                        j++;
                        continue;
                    }else{
                        cntt++;
                        cls[cur].push_back(cls[i][j]);
                        j++;
                    }
                }
                cls[i].resize(cls[i].size() - cntt);
            }
        }
        i = yy[i].nex;
    }
}

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, MM = 0;
    cin >> n;
    string l;
    for(int i = 0; i < n; i++){
        cin >> m[i];
        MM = max(MM, m[i]);
        t[i].resize(m[i]);
        for(int j = 0; j < m[i]; j++){
            cin >> t[i][j];
        }
        cin >> l;
        for(int j = 0; j < m[i]; j++){
            if(l[j] == 'L'){
                pret[t[i][j]] = 0;
                cls[0].push_back(num);
                id[num][0] = i;
                id[num][1] = j;
                num++;
            }
        }
    }
    yy[0].nex = 1;
    yy[1].pre = 0;

    if(n == 1000){
        cout << MM << endl;
    }

    cout << fixed << setprecision(8)<< cal() << endl;
    double ans = 1;
    for(int k = 1; k < MM; k++){
        if(ans != 0){
            arrange(k);
            ans = cal();
        }
        if(n == 1000){
            if(k%10 == 0){
                cout << k << ' ';
            }
        }else{
            cout << fixed << setprecision(8) << ans << endl;
        }
    }


    return 0;
}