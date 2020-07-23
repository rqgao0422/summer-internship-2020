#include<bits/stdc++.h> 
using namespace std;

int n=10,test_cases=5000000;
int num_rounds=20;
const double eps=1e-9;
int pw[105];

enum State{
	unchosen=0,
	chosen_unselected,
	chosen_selected
};

bool chosen[25];
int num[25];
double f[2][50000005];
bool vis[2][50000005];
int tot[2],q[2][50000005];

double findprob(int state,int x,int y){
    State s[n+5];
    double p=0;
    for(int i=0,t=state;i<n;i++)
        s[i]=(State)(t%3),t/=3;
    /*Fresh Random bit: x*/
    if(num[x]==0) p+=0.25;
    else{
        if(s[x]==unchosen || s[x]==chosen_unselected)
            p+=0.5;
    }

    /*Fresh Random bit: y*/
    if(num[y]!=0){
        if(s[y]==chosen_selected)
            p+=0.5;
    }
    else p+=0.25;

    return p;
}

int label(State *s){
    int sum=0;
    for(int i=0;i<n;i++)
        sum+=pw[i]*s[i];
    return sum;
}

int findstate(int state,int x,int y){
    State s[n+5];
    double p=0;
    for(int i=0,t=state;i<n;i++)
        s[i]=(State)(t%3),t/=3;
    s[x]=chosen_selected;
    if(s[y]!=unchosen) s[y]=chosen_unselected;
    return label(s);
}

double updateminimum(int i,double *R){
    double sum[n+5];
    for(int j=0;j<n;j++)
        sum[j]=1.;
    for(int j=0;j<pw[n];j++){
        State s[n+5];
        double p=0;
        for(int k=0,t=j;k<n;k++)
            s[k]=(State)(t%3),t/=3;
        for(int k=0;k<n;k++)
            if(s[k]!=unchosen)
                sum[k]-=f[i&1][j];
    }
    double ming=1.;
    for(int j=0;j<n;j++){
        if(num[j]==0) continue;
        
        if(num[j]==1){
            // printf("%d %d %.6lf\n",j,num[j],sum[j]);
            if(sum[j]<=0.5+eps) continue;
            return -1;
        }

        double l=0,r=1;
        while(l+eps<r){
            double mid=(l+r)/2.;
            if(pow((1-mid)/2.,num[j]-1)/2.>sum[j]) l=mid; else r=mid;
        }
        // printf("%d %d %.6lf %.3lf\n",j,num[j],sum[j],r);
        ming=min(ming,r);
        R[j]=r;
    }
    return ming;
}

inline void get_pair_case_1(int &x,int &y,bool refresh=0){
    static int now,maxn;
    if(refresh){now=1;maxn=n;return;}
    if(now>=maxn){
        do{
            x=rand()%n;y=rand()%n;
        }while(x==y);
    }else{
        x=now-1;y=now;
        now++;if(now==maxn) maxn--,now=1;
    }
}

int main(){
    n=3;
    while(n<=16){
    num_rounds=n*(n-1)/2;
	pw[0]=1;
	for(int i=1;i<=20;i++)
		pw[i]=pw[i-1]*3;
	// int T=test_cases;
    vector<pair<int,int> > history(0);
    double _r[n+5],r[n+5];
    memset(f,0,sizeof(f));
    memset(chosen,0,sizeof(chosen));
    memset(vis,0,sizeof(vis));
    memset(num,0,sizeof(num));
    tot[0]=1;tot[1]=0;
    q[0][1]=0;f[0][0]=1;vis[0][0]=1;
    double gamma=1;
    int round_id=0,x,y;
    get_pair_case_1(x,y,true);
	for(int i=1;i<=num_rounds;i++){
        memset(f[i&1],0,sizeof(f[i&1]));
        memset(vis[i&1],0,sizeof(vis[i&1]));
        tot[i&1]=0;
        get_pair_case_1(x,y);
		for(int _j=1;_j<=tot[i-1&1];_j++){
            int j=q[i-1&1][_j];
            double p=findprob(j,x,y);
            if(p>eps){
                int next_state=findstate(j,x,y);
                f[i&1][next_state]+=f[i-1&1][j]*p;
                if(!vis[i&1][next_state]){
                    vis[i&1][next_state]=1;
                    tot[i&1]++;q[i&1][tot[i&1]]=next_state;
                }
            }
            if(p<1-eps){
                int next_state=findstate(j,y,x);
                f[i&1][next_state]+=f[i-1&1][j]*(1-p);
                if(!vis[i&1][next_state]){
                    vis[i&1][next_state]=1;
                    tot[i&1]++;q[i&1][tot[i&1]]=next_state;
                }
            }
        }
        num[x]++;num[y]++;
        history.push_back(make_pair(x,y));
        // printf("%d %d\n",x,y);
        double temp=updateminimum(i,_r);
        if(gamma>temp){
            gamma=temp;
            round_id=i;
            memcpy(r,_r,sizeof(r));
         };
	}
    printf("gamma=%.6lf\n",gamma);
    n++;
    }
    // printf("gamma=%.6lf round_id=%d\n",gamma,round_id);
    /*int it=0;
    for(int it=0;it<round_id;it++){
        auto i=history[it];
        printf("%d-th pair: %d %d\n",it,i.first+1,i.second+1);
    }
    for(int i=0;i<n;i++)
        printf("%.10lf ",r[i]);puts("");*/
    
    return 0;
}