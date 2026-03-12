#include <bits/stdc++.h> 
using namespace std;

struct pt{ int x; int y; };

double dis(pt a, pt b){ 
    double tmp = (double)a.x*b.y - (double)a.y*b.x;
    double t1 = a.x - b.x;
    double t2 = a.y - b.y;
    return (double)tmp*tmp/(t1*t1 + t2*t2); 
}

long long cross(pt a, pt b, pt c){
    return  (long long)(b.x - a.x)*(c.y - b.y) - (long long)(b.y - a.y)*(c.x - b.x);
}

pt o;

int main(){ 
    int n, r; 
    cin >> n >> r; 
    pt p[n]; 
    for(int i = 0; i < n; i++){ 
        cin >> p[i].x >> p[i].y; } 
    sort(p, p+n, [](pt x, pt y){ return x.y < y.y || x.y == y.y && x.x < y.x; }); 
    o = p[0];
    
    sort(p + 1, p + n, [](pt x, pt y){ return cross(o, x, y) > 0 || cross(o, x, y) == 0 && abs(o.x - x.x) + x.y < abs(o.x - y.x) + y.y; });

    vector<pt> cl = {o, p[1]};
    int cr = 1;
    for(int i = 2; i < n ; i++){
        if(cross(cl[cr-1], cl[cr], p[i]) == 0){
            cl[cr] = p[i];
            continue;
        } 
        while(cross(cl[cr - 1], cl[cr], p[i]) < 0){
            cl.pop_back();
            cr -= 1;
        }
        cl.push_back(p[i]);
        cr += 1;
    }
    while(cross(cl[cr - 1], cl[cr], o) < 0){
        cl.pop_back();
        cr -= 1;
    }
    cl.push_back(o);
    
    double d2 = (double)r*r;

    for(int i = 0; i <= cr; i++){
        if((double)cl[i].x*cl[i+1].y > (double)cl[i+1].x*cl[i].y){
            d2 = min(d2, dis(cl[i], cl[i+1]));
        }else{d2 = 0; break;}
    }

    cout << setprecision(10) << (double)r*r*acos(sqrt(d2) / r) - sqrt((double)d2 * ((double)r*r - d2));





    return 0;   
}