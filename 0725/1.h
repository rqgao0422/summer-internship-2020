#include<bits/stdc++.h>
#include "history.h"
using namespace std;

enum PairState{
    unmatched = 0,
    ready,
    matched
};

struct State{
    PairState lastpair;
    State(PairState _pair=matched){
        lastpair=_pair;
    }
};

bool operator < (State a, State b){
    return a.lastpair<b.lastpair;
}
bool operator != (State a, State b){
    return a.lastpair!=b.lastpair;
}

bool check_feasibility(vector<pair<State,pair<int,long double> > > next_configurations){
    long double sum=0;
    for(auto tuple: next_configurations){
        sum+=tuple.second.second;
        if(tuple.second.first!=0 &&  tuple.second.first!=1) return false;
    }
    if(fabs(1-sum)>1e-7) return false;
    return true;
}

vector<pair<State,pair<int,long double> > > combine(vector<pair<State,pair<int,long double> > > next_configurations_prime){
    sort(next_configurations_prime.begin(),next_configurations_prime.end());
    vector<pair<State,pair<int,long double> > > next_configurations(0);
    for(auto tuple: next_configurations_prime){
        if(next_configurations.size() == 0 || tuple.first != next_configurations.back().first || tuple.second.first != next_configurations.back().second.first) 
            next_configurations.push_back(tuple);
        else
            next_configurations[next_configurations.size()-1].second.second += tuple.second.second;
    }
    return next_configurations;
}

vector<pair<State,pair<int,long double> > > algorithm(History history, State state, pair<int,int> p){
/*
    TODO
    Input:
        history: the history of pairs and selections
        state: local states of our algorithm
        p: the current pair
    
    Return: a vector consisting of all the possible configurations
        To describe a following configuration, you need to give the following local state, the selection of the current pair and the conditional probability of the configruation
        The second element must be 0 or 1, corresponding to selecting p.first or p.second
*/

    assert(p.first==1);
    vector<pair<State,pair<int,long double> > > next_configurations(0);
    const long double q = (sqrt(5)-1.)/2.;

    if(state.lastpair == matched){
        next_configurations.push_back(make_pair(State(unmatched),make_pair(0,0.5)));
        next_configurations.push_back(make_pair(State(unmatched),make_pair(1,0.5)));
    }
    else if(state.lastpair == ready){
        next_configurations.push_back(make_pair(State(matched),make_pair(history.selection.back()^1,1)));
    }
    else if(state.lastpair == unmatched){
        next_configurations.push_back(make_pair(State(matched),make_pair(history.selection.back()^1,q)));
        next_configurations.push_back(make_pair(State(ready),make_pair(0,0.5*(1.-q))));
        next_configurations.push_back(make_pair(State(ready),make_pair(1,0.5*(1.-q))));
    }
    else assert(0);

    assert(1 && check_feasibility(next_configurations));
    next_configurations = combine(next_configurations);
    assert(2 && check_feasibility(next_configurations));
    return next_configurations;
}