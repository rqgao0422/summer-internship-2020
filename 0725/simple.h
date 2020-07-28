#include<bits/stdc++.h>
#include "history.h"
using namespace std;

struct State{
    bool nothing;
    State(){
        nothing=false;
    }
};

bool operator < (State a, State b){
    return a.nothing<b.nothing;
}
bool operator != (State a, State b){
    return a.nothing!=b.nothing;
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
    vector<pair<State,pair<int,long double> > > next_configurations(0);

    next_configurations.push_back(make_pair(state, make_pair(0, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(1, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(0, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(1, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(0, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(1, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(0, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(1, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(0, 0.1)));
    next_configurations.push_back(make_pair(state, make_pair(1, 0.1)));

    assert(check_feasibility(next_configurations));
    next_configurations = combine(next_configurations);
    assert(check_feasibility(next_configurations));
    assert(next_configurations.size() == 2);
    return next_configurations;
}