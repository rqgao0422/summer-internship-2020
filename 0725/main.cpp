#include<bits/stdc++.h>
#include "2.h"
#define SIZE(a) ((int)(a).size())
using namespace std;

const long double eps=1e-7;

vector<pair<int,int> > pairs;
map<vector<int>, long double> distribution;
vector<int> elements;

inline void dfs(int round, long double prob, History history, State state){
    if(round==SIZE(pairs)){
        // printf("%.6Lf\n",prob);
        distribution[history.selection] += prob;
        return;
    }
    auto next_states = algorithm(history, state, pairs[round]);
    for(auto tuple: next_states){
        State next_state = tuple.first;
        int selection = tuple.second.first;
        long double next_prob = prob * tuple.second.second;
        History next_history = history;
        next_history.pairs.push_back(pairs[round]);
        next_history.selection.push_back(selection);
        dfs(round+1,next_prob,next_history,next_state);
    }
}

bool check(long double gamma){
    int n=SIZE(pairs);
    for(auto e: elements){
        vector<pair<int,int> > appearance(0);
        for(int i=0;i<n;i++)
            if(e == pairs[i].first) appearance.push_back(make_pair(i,0));
            else if(e == pairs[i].second) appearance.push_back(make_pair(i,1));
        int k = SIZE(appearance);
        for(int i=1;i<(1<<k);i++){
            bool last=false;
            long double prob=1.,total=0;
            for(int j=0;j<k;j++)
                if(i&(1<<j)){
                    if(last) prob*=1-gamma;
                    last=true; prob*=.5;
                }else last=false;
            for(auto v:distribution){
                bool isok=true;
                for(int j=0;j<k;j++)
                    if((i&(1<<j))&&v.first[appearance[j].first]==appearance[j].second) isok=false;
                if(isok) total+=v.second;
            }
            if(total-eps>prob) return false;
        }
    }
    return true;
}

int main(){
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        int x,y;scanf("%d%d",&x,&y);
        pairs.push_back(make_pair(x,y));
        elements.push_back(x);
        elements.push_back(y);
    }
    dfs(0,1.0,History(),State());
    sort(elements.begin(),elements.end());
    elements.erase(unique(elements.begin(),elements.end()),elements.end());
    long double l=-0.1,r=1;
    while(l+eps<r){
        long double mid=(l+r)/2.;
        if(check(mid)) l=mid; else r=mid;
    }
    if(r<-eps) puts("Not an OCS"); else 
    printf("%.6Lf\n",r);
    return 0;
}
