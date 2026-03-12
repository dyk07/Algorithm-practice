#include <bits/stdc++.h>
using namespace std;



int main(){
    int t;
    cin>>t;
    int a[t];
    int tt = t;
    while(tt>0){
        tt--;
        cin>>a[tt];
    }

    sort(a,a+t);

    int m = 1, cur = 1;

    for(int i=1;i<t;i++){
        if(a[i] == a[i-1]){
            cur += 1;
        }else{
            m = std::max(m,cur);
            cur = 1;
        }
    }
    cout<<max(m,cur);



    return 0;
}