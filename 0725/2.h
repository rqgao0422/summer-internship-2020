#include<bits/stdc++.h>
#include "history.h"
#define Configurations vector<pair<State,pair<int,long double> > >
using namespace std;

const int max_pairs = 15;

enum PairState{
    unmatched = 0,
    ready,
    matched
};

struct State{
    vector<PairState> lastpair;
    vector<int> lastselection;
    State(vector<PairState> _pair=vector<PairState>(max_pairs, matched), vector<int> _selection=vector<int>(max_pairs, 0)){
        lastpair=_pair;
        lastselection=_selection;
    }
};

bool operator < (State a, State b){
    return a.lastpair<b.lastpair || (a.lastpair == b.lastpair && a.lastselection == b.lastselection);
}
bool operator != (State a, State b){
    return a.lastpair!=b.lastpair || a.lastselection!=b.lastselection;
}

bool check_feasibility(Configurations next_configurations){
    long double sum=0;
    for(auto tuple: next_configurations){
        sum+=tuple.second.second;
        if(tuple.second.first!=0 &&  tuple.second.first!=1) return false;
    }
    if(fabs(1-sum)>1e-7) return false;
    return true;
}

Configurations combine(Configurations next_configurations_prime){
    sort(next_configurations_prime.begin(),next_configurations_prime.end());
    Configurations next_configurations(0);
    for(auto tuple: next_configurations_prime){
        if(next_configurations.size() == 0 || tuple.first != next_configurations.back().first || tuple.second.first != next_configurations.back().second.first) 
            next_configurations.push_back(tuple);
        else
            next_configurations[next_configurations.size()-1].second.second += tuple.second.second;
    }
    return next_configurations;
}

inline State modify_state(State state, int e, PairState ps, int sel){
    state.lastpair[e] = ps;
    state.lastselection[e] = sel;
    return state;
}

inline void add_channel(State state, History history, int e, int selection, double prob, Configurations &next_configurations){
    const long double q = (sqrt(5)-1.)/2.;
    if(state.lastpair[e] == matched){
        next_configurations.push_back(make_pair(modify_state(state,e,unmatched,0),make_pair(0^selection,0.5*prob)));
        next_configurations.push_back(make_pair(modify_state(state,e,unmatched,1),make_pair(1^selection,0.5*prob)));
    }
    else if(state.lastpair[e] == ready){
        next_configurations.push_back(make_pair(modify_state(state,e,matched,state.lastselection[e]^1),make_pair(state.lastselection[e]^1^selection,1*prob)));
    }
    else if(state.lastpair[e] == unmatched){
        next_configurations.push_back(make_pair(modify_state(state,e,matched,state.lastselection[e]^1),make_pair(state.lastselection[e]^1^selection,q*prob)));
        next_configurations.push_back(make_pair(modify_state(state,e,ready,0),make_pair(0^selection,0.5*(1.-q)*prob)));
        next_configurations.push_back(make_pair(modify_state(state,e,ready,1),make_pair(1^selection,0.5*(1.-q)*prob)));
    }
    else assert(0);
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

    add_channel(state, history, p.first, 0, 0.5, next_configurations);
    add_channel(state, history, p.second, 1, 0.5, next_configurations);

    assert(1 && check_feasibility(next_configurations));
    next_configurations = combine(next_configurations);
    assert(2 && check_feasibility(next_configurations));
    return next_configurations;
}