#include<bits/stdc++.h>
using namespace std;

struct History{
    vector<pair<int,int> > pairs;
    vector<int> selection;
    History(){
        vector<pair<int,int> >(0).swap(pairs);
        vector<int>(0).swap(selection);
    }
};

