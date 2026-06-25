//# pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using ld=long double;
using ull=unsigned long long;
typedef pair<ll,ll> P;
#define vec vector
#define pb emplace_back
#define fs first
#define sc second
#define pcnt __builtin_popcountll
#define rep(i,n) for(ll i=0;i<(n);i++)
#define rng(i,s,t) for(ll i=s;i<t;i++)
#define len(a) int((a).size())
#define all(a) a.begin(),a.end()
#define compress(a) {sort(all(a));a.erase(unique(all(a)),a.end());}
constexpr ll INF=4e18;
constexpr ld EPS=1e-11;
template<typename T> bool chmin(T& a, T b){if(a > b){a = b; return true;} return false;}
template<typename T> bool chmax(T& a, T b){if(a < b){a = b; return true;} return false;}
template<typename itr> void printvec(itr begin, itr end) {
    for (itr p = begin; p < end; p++){
		if(p!=begin)cout<<" "; cout << *p;
	}
    cout << endl;
}
template <class T> T makevec(T value) {return value;}
template <class T, class... Args,
          std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
auto makevec(std::size_t n, Args... args) {
    auto inner = makevec<T>(args...);
    return std::vector<decltype(inner)>(n, inner);
}