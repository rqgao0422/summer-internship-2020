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
    vector<fraction> b(0),c(1<<m,fraction(0,1));
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
        for(int j=1;j<(1<<k);j++){
            bool last=0;fraction pos=fraction(1,1);
            for(int l=0;l<k;l++)
                if(j&(1<<l)){
                    if(last) pos*=((-gamma)+1);
                    pos*=fraction(1,2);last=1;
                }else last=0;
            vector<fraction> _a(1<<m,fraction(1,1));
            for(int idx=0;idx<(1<<m);idx++)
                for(int l=0;l<k;l++)
                    if((j&(1<<l)) && ((idx>>p[l].first)&1)==p[l].second){
                        _a[idx]=fraction(0,1);
                        break;
                    }
            // for(int idx=0;idx<(1<<m);idx++)
                // fprint(stdout,_a[idx],' ');
            // fprint(stdout,pos,'\n');
            A.push_back(_a);
            b.push_back(pos);
        }
    }
    vector<fraction> _a(0);
    for(int idx=0;idx<(1<<m);idx++)
        _a.push_back(fraction(-1,1));
    A.push_back(_a);b.push_back(fraction(-1,1));
    auto result = LP(A,b,c);
    if(print && result.feasible){
        assert(result.x.size()==(1<<m));
        for(int idx=0;idx<(1<<m);idx++)
            fprint(stdout,result.x[idx],' ');
        puts("");
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
                bool last=0;fraction pos=fraction(1,1);
                for(int l=0;l<k;l++)
                    if(j&(1<<l)){
                        if(last) pos*=((-gamma)+1);
                        pos*=fraction(1,2);last=1;
                    }else last=0;
                for(int idx=0;idx<(1<<m);idx++){
                    bool isok=1;
                    for(int l=0;l<k;l++)
                        if((j&(1<<l)) && ((idx>>p[l].first)&1)==p[l].second){
                            isok=0;
                            break;
                        }
                    if(isok) pos-=result.x[idx];
                }
                if(zero(pos)){
                    printf("e_%d: ",elements[i]);
                    for(int l=0;l<k;l++)
                        if(j&(1<<l)) printf("%d ",p[l].first);
                    puts("");
                }
            }
        }
        for(int j=1;j<=m;j++){
            vector<fraction> sum((1<<m),fraction(0,1));
            for(int idx=0;idx<(1<<m);idx++)
                sum[idx&((1<<j)-1)] += result.x[idx];
            for(int idx=0;idx<(1<<j-1);idx++){
                fraction total = sum[idx] + sum[idx|(1<<j-1)];
                if(zero(total)) continue;
                for(int k=0;k<j-1;k++)
                    printf("%d",((idx>>k)&1)?a[k].second:a[k].first);
                putchar(':');
                fprint(stdout, sum[idx]/total, ' ');
                fprint(stdout, sum[idx | (1<<j-1)]/total, '\n');
            }
        }
        for(int j=1;j<=m;j++){
            vector<fraction> sum((1<<m),fraction(0,1));
            for(int idx=0;idx<(1<<m);idx++)
                sum[idx&((1<<j)-1)] += result.x[idx];
            for(int idx=0;idx<(1<<j);idx++){
                fraction total = sum[idx];
                for(int k=0;k<j;k++)
                    printf("%d",((idx>>k)&1)?a[k].second:a[k].first);
                putchar(':');
                fprint(stdout, total, ' ');puts("");
            }
        }
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
    bool g=check_feasibility(a,fraction(n,4*n-1)-fraction(1,10000000),true);
    return 0;
}