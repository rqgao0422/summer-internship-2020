#include <bits/stdc++.h>

#ifdef __WIN32
#define LLFORMAT "I64"
#else
#define LLFORMAT "ll"
#endif

using namespace std;

const long double eps=1e-9;

struct SolutionOfLP{
    bool feasible;
    long double result;
    vector<long double> x;
    SolutionOfLP(bool _feasible, long double _result=-1, vector<long double> _x = (vector<long double>){-1}){
        feasible = _feasible;
        result = _result;
        x = _x;
    }
};

int simplex(vector<vector<long double> > &a, vector<long double> &b, vector<long double> &c, vector<int> &basic) {
/*
 * Solving the LP problem of:
 *		minimize c^Tx
 *		with Ax=b
 *		basic[i] denotes the index of basic variables for a[i][...]
 *	Return:
 *		-1 if solution is unbounded, 0 otherwise
 *		basic variables for Ax=b will denoted by ''basic'', the corresponding solution will be basic variables taking corresponding b value and nonbasic variables taking 0
 */
	int m = b.size(), n = c.size();
	for (int i = 0; i < m; ++i) {
		assert(b[i] > -eps);
		int k = basic[i];
		assert(-eps < a[i][k] - 1 && a[i][k] - 1 < eps);
		for (int l = 0; l < m; ++l) if(l != i) assert(-eps < a[l][k] && a[l][k] < eps);
		assert(-eps < c[k] && c[k] < eps);
	}
	while(true) {
		int k = -1;
		for (int j = 0; j < n; ++j) if(c[j] < -eps) {
			k = j;
			break;
		}
		if(k == -1) {
			long double ans = 0;
			for (int i = 0; i < m; ++i) ans += c[basic[i]] * b[i];
			return 0;
		}
		int l = -1;
		for (int i = 0; i < m; ++i) if(a[i][k] > eps) {
			if(l == -1) l = i;
			else {
				long double ti = b[i] / a[i][k], tl = b[l] / a[l][k];
				if(ti < tl - eps || (ti < tl + eps && basic[i] < basic[l])) l = i;
			}
		}
		if(l == -1) return -1;
		basic[l] = k;
		long double tmp = 1 / a[l][k];
		for (int j = 0; j < n; ++j) a[l][j] *= tmp;
		b[l] *= tmp;
		for (int i = 0; i < m; ++i) if(i != l) {
			tmp = a[i][k];
			for (int j = 0; j < n; ++j) a[i][j] -= tmp * a[l][j];
			b[i] -= tmp * b[l];
		}
		tmp = c[k];
		for (int j = 0; j < n; ++j) c[j] -= tmp * a[l][j];
	}
}

SolutionOfLP LP(vector<vector<long double> > _A, vector<long double> _b, vector<long double> _c) {
/*
    Linear Programming:
        max  c'x
        s.t. Ax<=b
             x>=0
    Return value: whether
    a: m*n matrix 
    b: vector of length m
    c: vector of length n
*/
    int n = _c.size();
    int m = _b.size();
	vector<long double> c(n + m, 0);
	for (int i = 0; i < n; ++i) {
		c[i] = _c[i];
		c[i] *= -1;
	}
	auto C = c;
	vector<vector<long double> > a(m, vector<long double>(n + m, 0));
	vector<long double> b(m);
	vector<int> basic(m, -1), tmp;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) a[i][j]=_A[i][j];
		a[i][i + n] = 1;
		b[i] = _b[i];
		if(b[i] > -eps) basic[i] = i + n;
		else tmp.push_back(i);
	}
	if(!tmp.empty()) {
		sort(tmp.begin(), tmp.end(), [&](int i, int j) { return b[i] > b[j]; });
		vector<vector<long double> > A;
		vector<long double> B, C(n + m + 1, 0);
		vector<int> Basic;
		for (int i: tmp) {
			vector<long double> foo;
			for (int j = 0; j < n + m; ++j) foo.push_back(-a[i][j]);
			foo.push_back(1);
			long double bar = -b[i];
			for (int i = 0; i < A.size(); ++i) {
				long double tmp = foo[Basic[i]];
				for (int j = 0; j <= n + m; ++j) foo[j] -= tmp * A[i][j];
				bar -= tmp * B[i];
			}
			for (int j = n + m; j >= 0; --j) if(-eps < foo[j] - 1 && foo[j] - 1 < eps) {
				Basic.push_back(j);
				break;
			}
			for (int i = 0; i < A.size(); ++i) {
				long double tmp = A[i][Basic.back()];
				for (int j = 0; j <= n + m; ++j) A[i][j] -= tmp * foo[j];
				B[i] -= tmp * bar;
			}
			A.push_back(foo);
			B.push_back(bar);
			assert(bar > -eps);
			assert(A.size() == Basic.size());
		}
		for (int i = 0; i < A.size(); ++i) if(Basic[i] == n + m) {
			for (int j = 0; j < n + m; ++j) C[j] = -A[i][j];
		}
		for (int i = 0; i < m; ++i) if(b[i] > -eps) {
			A.push_back(a[i]);
			A[A.size() - 1].push_back(0);
			B.push_back(b[i]);
			Basic.push_back(basic[i]);
		}
		assert(simplex(A, B, C, Basic) == 0);
		bool flag = true;
		for (int i = 0; i < m; ++i) if(Basic[i] == n + m) {
			if(B[i] > eps) {
				return SolutionOfLP(false);
			}
			int k = -1;
			for (int j = 0; j < n + m; ++j) if(A[i][j] > eps || A[i][j] < -eps) {
				k = j;
				break;
			}
			if(k != -1) {
				long double tmp = 1 / A[i][k];
				Basic[i] = k;
				for (int j = 0; j <= n + m; ++j) A[i][j] *= tmp;
				B[i] *= tmp;
				for (int l = 0; l < m; ++l) if(l != i) {
					tmp = A[l][k];
					for (int j = 0; j <= n + m; ++j) A[l][j] -= tmp * A[i][j];
					B[l] -= tmp * B[i];
				}
			}
			else flag = false;
			break;
		}
		if(flag) {
			A.push_back(vector<long double>(n + m, 0));
			A[A.size() - 1].push_back(1);
			B.push_back(0);
			Basic.push_back(n + m);
			for (int i = 0; i < A.size() - 1; ++i) {
				long double tmp = A[i].back();
				for (int j = 0; j <= n + m; ++j) A[i][j] -= tmp * A[A.size() - 1][j];
				B[i] -= tmp * B.back();
			}
		}
		a = A;
		b = B;
		basic = Basic;
		c.push_back(0);
		for (int i = 0; i < a.size(); ++i) {
			long double tmp = c[basic[i]];
			for (int j = 0; j <= n + m; ++j) c[j] -= tmp * a[i][j];
		}
	}
	auto foo = simplex(a, b, c, basic);
	if(foo == -1) return SolutionOfLP(true);
	else {
		long double res = 0;
		vector<long double> ans(n, 0);
		for (int i = 0; i < basic.size(); ++i) if(basic[i] < n) ans[basic[i]] = b[i];
		for (int j = 0; j < n; ++j) res -= C[j] * ans[j];
        return SolutionOfLP(true, res, ans);
	}
}