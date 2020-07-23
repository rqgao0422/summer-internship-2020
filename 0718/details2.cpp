#include<bits/stdc++.h>
#include "simplex_fraction.h"
using namespace std;

int indexing(vector<int> a, int b){
    auto it = lower_bound(a.begin(), a.end(), b);
    if((*it)!=b) return -1;
    return it-a.begin();
}

bool check_feasibility(vector<pair<int,int> > a, fraction gamma, bool print=false){
    int m=a.size();
    // printf("%d\n",m);
    vector<int> elements(0);
    for(int i=0;i<m;i++){
        elements.push_back(a[i].first);
        elements.push_back(a[i].second);
    }
    sort(elements.begin(), elements.end());
    elements.erase(unique(elements.begin(),elements.end()),elements.end());
    int n=elements.size();
    vector<vector<fraction> > A(0);
    vector<fraction> b(0),c((1<<m)+1,fraction(0,1));
    c[(1<<m)]=fraction(1,1);
    // fprint(stdout, -gamma, '\n');
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
        for(int j=0;j<k;j++){
            vector<fraction> _a((1<<m)+1,fraction(0,1));
            for(int idx=0;idx<(1<<m);idx++)
                if(((idx>>p[j].first)&1)!=p[j].second) _a[idx]=fraction(1,1);
            A.push_back(_a);
            b.push_back(fraction(1,2));
        }
        for(int j=1;j<k;j++){
            vector<fraction> _a((1<<m)+1,fraction(0,1));
            _a[(1<<m)]=fraction(1,4);
            for(int idx=0;idx<(1<<m);idx++)
                if(((idx>>p[j].first)&1)!=p[j].second && ((idx>>p[j-1].first)&1)!=p[j-1].second) 
                    _a[idx]=fraction(1,1);
            A.push_back(_a);
            b.push_back(fraction(1,4));
        }
    }
    vector<fraction> _a(0);
    for(int idx=0;idx<(1<<m);idx++)
        _a.push_back(fraction(-1,1));
    _a.push_back(fraction(0,1));
    A.push_back(_a);b.push_back(fraction(-1,1));
    auto result = LP(A,b,c);
    if(print){
        assert(result.x.size()==(1<<m)+1);
        fprint(stdout,result.result,'\n');
        for(int idx=0;idx<(1<<m);idx++)
            fprint(stdout,result.x[idx],' ');
        puts("");
    }
    return result.feasible;
}

int main(){
    srand(time(0));
    const int n=2;
    vector<pair<int,int> > a(0);
    for(int j=1;j<=n*2+1;j++){
        int x,y;
        do{x=1;y=(j+1)%2+2;}while(x==y);
        a.push_back(make_pair(x,y));
        // printf("%d %d\n",x,y);
    }
    double g=check_feasibility(a,fraction(n,n*4-1),true);
    return 0;
}