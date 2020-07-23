#include<bits/stdc++.h> 
using namespace std;

const int n=7;
const double eps=1e-7;

int num_rounds,x[105],y[105],lr[105],le[105],ri[105],selected[105],num[105];
double f[105];

void dfs(int round,double prob=1){
    if(round>num_rounds){
        for(int i=1;i<=n;i++)
            if(selected[i]==0) f[i]+=prob;
        return;
    }
    /*left*/
    if(!le[round]){
        selected[x[round]]++;lr[round]=0;
        dfs(round+1,prob*0.25);
        selected[x[round]]--;
        selected[y[round]]++;lr[round]=1;
        dfs(round+1,prob*0.25);
        selected[y[round]]--;
    }
    else if((lr[le[round]]==0?x[le[round]]:y[le[round]])!=x[round]){
        selected[x[round]]++;lr[round]=0;
        dfs(round+1,prob*0.5);
        selected[x[round]]--;
    }else{
        selected[y[round]]++;lr[round]=1;
        dfs(round+1,prob*0.5);
        selected[y[round]]--;
    }

    /*right*/
    if(!ri[round]){
        selected[x[round]]++;lr[round]=0;
        dfs(round+1,prob*0.25);
        selected[x[round]]--;
        selected[y[round]]++;lr[round]=1;
        dfs(round+1,prob*0.25);
        selected[y[round]]--;
    }
    else if((lr[ri[round]]==0?x[ri[round]]:y[ri[round]])==y[round]){
        selected[x[round]]++;lr[round]=0;
        dfs(round+1,prob*0.5);
        selected[x[round]]--;
    }else{
        selected[y[round]]++;lr[round]=1;
        dfs(round+1,prob*0.5);
        selected[y[round]]--;
    }
}

int main(){
    scanf("%d",&num_rounds);
    int now[105];memset(now,0,sizeof(now));
    for(int i=1;i<=num_rounds;i++){
        scanf("%d%d",&x[i],&y[i]);
        le[i]=now[x[i]];
        ri[i]=now[y[i]];
        now[x[i]]=now[y[i]]=i;
        num[x[i]]++;num[y[i]]++;
    }
    dfs(1);
    for(int j=1;j<=n;j++){
        if(num[j]==0){printf("ok ");continue;}
        
        if(num[j]==1){
            // printf("%d %d %.6lf\n",j,num[j],sum[j]);
            if(f[j]<=0.5+eps) printf("ok ");
            else printf("bad ");
            continue;
        }

        double l=0,r=1;
        while(l+eps<r){
            double mid=(l+r)/2.;
            if(pow((1-mid)/2.,num[j]-1)/2.>f[j]) l=mid; else r=mid;
        }
        // printf("%d %d %.6lf %.3lf\n",j,num[j],sum[j],r);
        printf("%.6lf ",r);
    }
    puts("");
    return 0;
}