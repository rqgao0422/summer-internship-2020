#include <bits/stdc++.h>
#include "fraction.h"

#ifdef __WIN32
#define LLFORMAT "I64"
#else
#define LLFORMAT "ll"
#endif

using namespace std;

const fraction eps=fraction(1,1000000000);

struct SolutionOfLP{
    bool feasible;
    fraction result;
    vector<fraction> x;
    SolutionOfLP(bool _feasible, fraction _result=fraction(-1,1), vector<fraction> _x = (vector<fraction>){fraction(-1,1)}){
        feasible = _feasible;
        result = _result;
        x = _x;
    }
};

int simplex(vector<vector<fraction> > &a, vector<fraction> &b, vector<fraction> &c, vector<int> &basic) {
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
		assert(nonnegative(b[i]));
		int k = basic[i];
		assert(zero(a[i][k] - 1));
		for (int l = 0; l < m; ++l) if(l != i) assert(zero(a[l][k]));
		assert(zero(c[k]));
	}
	while(true) {
		int k = -1;
		for (int j = 0; j < n; ++j) if(negative(c[j])) {
			k = j;
			break;
		}
		if(k == -1) {
			fraction ans = fraction(0,1);
			for (int i = 0; i < m; ++i) ans += c[basic[i]] * b[i];
			return 0;
		}
		int l = -1;
		for (int i = 0; i < m; ++i) if(positive(a[i][k])) {
			if(l == -1) l = i;
			else {
				fraction ti = b[i] / a[i][k], tl = b[l] / a[l][k];
				if(ti < tl || (ti <= tl && basic[i] < basic[l])) l = i;
			}
		}
		if(l == -1) return -1;
		basic[l] = k;
		fraction tmp = fraction(1,1) / a[l][k];
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

SolutionOfLP LP(vector<vector<fraction> > _A, vector<fraction> _b, vector<fraction> _c) {
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
	vector<fraction> c(n + m, fraction(0, 1));
	for (int i = 0; i < n; ++i) {
		c[i] = _c[i];
		c[i] *= -1;
	}
	auto C = c;
	vector<vector<fraction> > a(m, vector<fraction>(n + m, fraction(0,1)));
	vector<fraction> b(m);
	vector<int> basic(m, -1), tmp;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) a[i][j]=_A[i][j];
		a[i][i + n] = 1;
		b[i] = _b[i];
		if(nonnegative(b[i])) basic[i] = i + n;
		else tmp.push_back(i);
	}
	if(!tmp.empty()) {
		sort(tmp.begin(), tmp.end(), [&](int i, int j) { return b[i] > b[j]; });
		vector<vector<fraction> > A;
		vector<fraction> B, C(n + m + 1, fraction(0,1));
		vector<int> Basic;
		for (int i: tmp) {
			vector<fraction> foo;
			for (int j = 0; j < n + m; ++j) foo.push_back(-a[i][j]);
			foo.push_back(fraction(1,1));
			fraction bar = -b[i];
			for (int i = 0; i < A.size(); ++i) {
				fraction tmp = foo[Basic[i]];
				for (int j = 0; j <= n + m; ++j) foo[j] -= tmp * A[i][j];
				bar -= tmp * B[i];
			}
			for (int j = n + m; j >= 0; --j) if(zero(foo[j] - 1)) {
				Basic.push_back(j);
				break;
			}
			for (int i = 0; i < A.size(); ++i) {
				fraction tmp = A[i][Basic.back()];
				for (int j = 0; j <= n + m; ++j) A[i][j] -= tmp * foo[j];
				B[i] -= tmp * bar;
			}
			A.push_back(foo);
			B.push_back(bar);
			assert(nonnegative(bar));
			assert(A.size() == Basic.size());
		}
		for (int i = 0; i < A.size(); ++i) if(Basic[i] == n + m) {
			for (int j = 0; j < n + m; ++j) C[j] = -A[i][j];
		}
		for (int i = 0; i < m; ++i) if(nonnegative(b[i])) {
			A.push_back(a[i]);
			A[A.size() - 1].push_back(0);
			B.push_back(b[i]);
			Basic.push_back(basic[i]);
		}
		assert(simplex(A, B, C, Basic) == 0);
		bool flag = true;
		for (int i = 0; i < m; ++i) if(Basic[i] == n + m) {
			if(positive(B[i])) {
				return SolutionOfLP(false);
			}
			int k = -1;
			for (int j = 0; j < n + m; ++j) if(!zero(A[i][j])) {
				k = j;
				break;
			}
			if(k != -1) {
				fraction tmp = fraction(1,1) / A[i][k];
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
			A.push_back(vector<fraction>(n + m, fraction(0,1)));
			A[A.size() - 1].push_back(fraction(1,1));
			B.push_back(fraction(0,1));
			Basic.push_back(n + m);
			for (int i = 0; i < A.size() - 1; ++i) {
				fraction tmp = A[i].back();
				for (int j = 0; j <= n + m; ++j) A[i][j] -= tmp * A[A.size() - 1][j];
				B[i] -= tmp * B.back();
			}
		}
		a = A;
		b = B;
		basic = Basic;
		c.push_back(fraction(0,1));
		for (int i = 0; i < a.size(); ++i) {
			fraction tmp = c[basic[i]];
			for (int j = 0; j <= n + m; ++j) c[j] -= tmp * a[i][j];
		}
	}
	auto foo = simplex(a, b, c, basic);
	if(foo == -1) return SolutionOfLP(true);
	else {
		fraction res = fraction(0,1);
		vector<fraction> ans(n, fraction(0,1));
		for (int i = 0; i < basic.size(); ++i) if(basic[i] < n) ans[basic[i]] = b[i];
		for (int j = 0; j < n; ++j) res -= C[j] * ans[j];
        return SolutionOfLP(true, res, ans);
	}
}