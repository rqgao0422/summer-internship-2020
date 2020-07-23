#include<bits/stdc++.h>
#define int128 __int128_t
using namespace std;

struct fraction{
    int128 x,y;
    fraction(int128 _x=0, int128 _y=1){
        int128 k=__gcd(_x,_y);
        if(k<0) k=-k;
        x=_x/k;y=_y/k;
        assert(y>0);
    }
};
fraction operator - (fraction a){return fraction(-a.x,a.y);}
fraction operator + (fraction a, fraction b){
    int128 x=a.x*b.y+a.y*b.x,y=a.y*b.y;
    return fraction(x,y);
}
inline void operator += (fraction &a, fraction b){a=a+b;}
inline fraction operator + (fraction &a, int128 b){return a+fraction(b,1);}
inline void operator += (fraction &a, int128 b){a=a+b;}
inline fraction operator - (fraction a, fraction b){
    int128 x=a.x*b.y-a.y*b.x,y=a.y*b.y;
    return fraction(x,y);
}
inline void operator -= (fraction &a, fraction b){a=a-b;}
inline fraction operator - (fraction &a, int128 b){return a-fraction(b,1);}
inline void operator -= (fraction &a, int128 b){a=a-b;}
inline fraction operator * (fraction a, fraction b){
    int128 x=a.x*b.x,y=a.y*b.y;
    return fraction(x,y);
}
inline void operator *= (fraction &a, fraction b){a=a*b;}
inline fraction operator * (fraction &a, int128 b){return a*fraction(b,1);}
inline void operator *= (fraction &a, int128 b){a=a*b;}
inline fraction operator / (fraction a, fraction b){
    int128 x=a.x*b.y,y=a.y*b.x;
    if(y<0) x=-x,y=-y;
    return fraction(x,y);
}
inline void operator /= (fraction &a, fraction b){a=a/b;}
inline fraction operator / (fraction &a, int128 b){return a/fraction(b,1);}
inline void operator /= (fraction &a, int128 b){a=a/b;}
inline bool operator == (fraction a, fraction b){
    int128 x=a.x*b.y,y=a.y*b.x;
    return x==y;
}
inline bool operator != (fraction a, fraction b){return !(a==b);}
inline bool operator < (fraction a, fraction b){
    int128 x=a.x*b.y,y=a.y*b.x;
    return x<y;
}
inline bool operator >= (fraction a, fraction b){return !(a<b);}
inline bool operator > (fraction a, fraction b){
    int128 x=a.x*b.y,y=a.y*b.x;
    return x>y;
}
inline bool operator <= (fraction a, fraction b){return !(a>b);}
inline void fprint(FILE *f, fraction a,char c='\n'){
    fprintf(f,"%lld/%lld%c",(long long)a.x,(long long)a.y,c);
}
inline bool zero(fraction a){return a.x==0;}
inline bool nonnegative(fraction a){return a.x>=0;}
inline bool negative(fraction a){return a.x<0;}
inline bool nonpositive(fraction a){return a.x<=0;}
inline bool positive(fraction a){return a.x>0;}