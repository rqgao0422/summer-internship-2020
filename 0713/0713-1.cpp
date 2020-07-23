#include<bits/stdc++.h> 
using namespace std;

const int n=5,test_cases=5000000,num_rounds=20;
const double eps=1e-5;
int pw[105];

enum State{
	unchosen=0,
	chosen_unselected,
	chosen_selected
};

bool chosen[n+5];
int num[n+5];
double f[num_rounds+5][1005];
bool vis[num_rounds+5][1005];

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
                sum[k]-=f[i][j];
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

int main(){

    srand(time(0));
	pw[0]=1;
	for(int i=1;i<=20;i++)
		pw[i]=pw[i-1]*3;
	int T=test_cases;
    double Gamma=1.;
	while(T--){
        vector<pair<int,int> > history;
        double _r[n+5],r[n+5];
        memset(chosen,0,sizeof(chosen));
        memset(f,0,sizeof(f));
        memset(vis,0,sizeof(vis));
        memset(num,0,sizeof(num));
        f[0][0]=1;vis[0][0]=1;
        double gamma=1;
        int round_id=0;
		for(int i=1;i<=num_rounds;i++){
			int x,y;
			do{
				x=rand()%n;y=rand()%n;	
			}while(x==y);
			for(int j=0;j<pw[n];j++){
                if(!vis[i-1][j]) continue;
                double p=findprob(j,x,y);
                f[i][findstate(j,x,y)]+=f[i-1][j]*p;
                if(p>eps) vis[i][findstate(j,x,y)]=1;
                f[i][findstate(j,y,x)]+=f[i-1][j]*(1-p);
                if(p<1-eps) vis[i][findstate(j,y,x)]=1;
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
        if(gamma<Gamma){
            Gamma=gamma;
            printf("gamma=%.6lf round_id=%d\n",gamma,round_id);
            int it=0;
            for(int it=0;it<round_id;it++){
                auto i=history[it];
                printf("%d-th pair: %d %d\n",it,i.first+1,i.second+1);
            }
            for(int i=0;i<n;i++)
                printf("%.6lf ",r[i]);puts("");
        }
	}
    // printf("%.6lf\n",gamma);
    return 0;
}