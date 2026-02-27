#include <bits/stdc++.h>
using namespace std;

vector<int> concat(vector<int> a, vector<int> b){
    int l1 = a.size();
    int l2 = b.size();
    vector<int> ans(l1 + l2 - 1);
    ans[0] = 0;
    for(int i = 1; i < l1 + l2 - 1; i++){
        int tmp = a[l1 - 1] + b[l2 - 1];
        for(int j = 0; j < l1 && j <= i; j++){
            if(i - j < l2){
                tmp = min(tmp, a[j] + b[i - j]);
            }
        }
        ans[i] = tmp;
    }
    return ans;
}

vector<int> cal(vector<int> a, int i, int j){
    int l = j - i;
    if(l <= 1){
        return {0};
    }
    if(l == 2){
        return {0, a[i + 1] - a[i]};
    }

    int mid = i + l/2;
    int extra = a[mid] - a[mid - 1];

    vector<int> v1 = concat(cal(a, i, mid), cal(a, mid, j));
    vector<int> v2 = concat(cal(a, i, mid - 1), cal(a, mid + 1, j));

    int s1 = v1.size();
    int s2 = v2.size();
    int s = max(s1, s2 + 1);
    vector<int> ans(s);
    ans[0] = 0;
    if(s1 >= s2 + 1){
        for(int i = 1; i < s2 + 1; i++){
            ans[i] = min(v1[i], extra + v2[i - 1]);
        }
        for(int i = s2 + 1; i < s1; i++){
            ans[i] = v1[i];
        }
    }else{
        for(int i = 1; i < s1; i++){
            ans[i] = min(v1[i], extra + v2[i - 1]);
        }
        for(int i = s1; i < s2 + 1; i++){
            ans[i] = extra + v2[i - 1];
        }
    }

    return ans;
}

int main(){
    int n, m, k, sgm = 1;
    cin >> n >> m >> k;
    string str;
    cin >> str;
    bool b[n - 1];
    for(int i = 0; i < n - 1; i++){
        b[i] = !(str[i] == str[i + 1]);
        sgm += b[i];
    }
    
    int ans = 0, tmp = 0, mm = m;


   
    for(int tt = 0; tt < 4; tt++){
        if(tt == 1){ 
            if(m > 0 && n > k){
                tmp = (b[k - 1])? -1 : 1;
                b[k - 1] = !b[k - 1];
                mm = m - 1;
            }else{
                break;
            }
        }
        if(tt == 2){ 
            b[k - 1] = !b[k - 1];
            tmp = (b[n - k - 1])? -1 : 1;
            b[n - k - 1] = !b[n - k - 1];           
        }
        if(tt == 3){
            if(m > 1 && n != 2*k){
                b[k - 1] = !b[k - 1];
                tmp = (b[n - k - 1])? 1 : -1;
                tmp += (b[k - 1])? 1 : -1;
                mm = m - 2;
            }else{
                break;
            }
        }
        
        
        vector<int> sum = {0};
        vector<int> cur;
        for(int i = 0; i < k; i++){
            cur = {};
            for(int j = 0; j*k + i < n - 1; j++){
                if(!b[j*k + i]){
                    cur.push_back(j);
                }
            }
            sum = concat(sum, cal(cur, 0, cur.size()));
        }

        int s = sum.size();
        for(int i = 0; i < s - 1; i++){
            if(sum[i] <= mm && mm < sum[i + 1]){
                tmp += 2*i;
            }
        }
        if(mm >= sum[s - 1]){
            tmp += 2*(s - 1);
        }

        
        ans = max(ans, tmp);
    }

    cout << ans + sgm;




    return 0;
}