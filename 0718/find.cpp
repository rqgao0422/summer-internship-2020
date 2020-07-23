#include<bits/stdc++.h>
#include "simplex.h"
using namespace std;

int indexing(vector<int> a, int b){
    auto it = lower_bound(a.begin(), a.end(), b);
    if((*it)!=b) return -1;
    return it-a.begin();
}

bool check_feasibility(vector<pair<int,int> > a, long double gamma, bool print=false){
    int m=a.size();
    vector<int> elements(0);
    for(int i=0;i<m;i++){
        elements.push_back(a[i].first);
        elements.push_back(a[i].second);
    }
    sort(elements.begin(), elements.end());
    elements.erase(unique(elements.begin(),elements.end()),elements.end());
    int n=elements.size();
    vector<vector<long double> > A(0);
    vector<long double> b(0),c(1<<m,0);
    // printf("%d %.5Lf\n",n,gamma);
    for(int i=0;i<n;i++){
        vector<pair<int,int> > p(0);
        for(int j=0;j<m;j++){
            int x=a[j].first,y=a[j].second,id=0;
            if(y==elements[i]) swap(x,y),id=1;
            if(x!=elements[i]) continue;
            p.push_back(make_pair(j,id));
        }
        int k=p.size();
        // printf("%d %d\n",elements[i],k);
        for(int j=1;j<(1<<k);j++){
            bool last=0;long double pos=1.;
            for(int l=0;l<k;l++)
                if(j&(1<<l)){
                    if(last) pos*=1-gamma;
                    pos*=1/2.;last=1;
                }else last=0;
            vector<long double> _a(1<<m,0);
            for(int idx=0;idx<(1<<m);idx++){
                _a[idx]=1.;
                for(int l=0;l<k;l++)
                    if((j&(1<<l)) && ((idx>>p[l].first)&1)==p[l].second){
                        _a[idx]=0.;
                        break;
                    }
            }
            // for(int idx=0;idx<(1<<m);idx++)
                // printf("%.0Lf ",_a[idx]);
            // printf("%.5Lf\n",pos);
            A.push_back(_a);
            b.push_back(pos);
        }
    }
    vector<long double> _a(0);
    for(int idx=0;idx<(1<<m);idx++)
        _a.push_back(-1);
    A.push_back(_a);b.push_back(-1);
    auto result = LP(A,b,c);
    if(print){
        assert(result.x.size()==(1<<m));
        for(int idx=0;idx<(1<<m);idx++)
            printf("%.5Lf ",result.x[idx]);
        puts("");
    }
    return result.feasible;
}

long double calculate(vector<pair<int,int> > a, bool print=false){
    const long double eps=1e-6;
    long double l=0,r=1;
    while(l+eps<r){
        long double mid=(l+r)/2.;
        printf("%.5Lf %.5Lf %.5Lf\n",l,r,mid);
        if(check_feasibility(a,mid,false)) l=mid; else r=mid;
    }
    if(print){
        printf("best gamma: %.5Lf\n",l);
        check_feasibility(a,l,true);
    }
    return r;
}

int main(){
    srand(time(0));
    const int n=5,m=3;
    int T=1;
    long double gamma=1.;
    for(int _=1;_<=T;_++){
        vector<pair<int,int> > a(0);
        for(int j=1;j<=n;j++){
            int x,y;
            do{x=1;y=j%2+2;}while(x==y);
            a.push_back(make_pair(x,y));
        }
        double g=calculate(a,false);
        if(g<gamma){
            gamma=g;
            printf("%.5Lf\n",gamma);
            for(auto j:a)
                printf("%d %d\n",j.first,j.second);
        }
    }
    return 0;
}