//# pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> P;
constexpr ll INF=4e18;
#define rep(i,n) for(ll i=0;i<(n);i++)
#define all(a) a.begin(),a.end()
template<typename T> bool chmin(T& a, T b){if(a > b){a = b; return true;} return false;}
template<typename T> bool chmax(T& a, T b){if(a < b){a = b; return true;} return false;}
#define compress(a) {sort(all(a));a.erase(unique(all(a)),a.end());}
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
#define ACLibrary_available 1
#ifdef ACLibrary_available
#include<atcoder/all>
using namespace atcoder;
//#define static_mod 1
//#define arbitrary_mod 2
//#define mod1000000007 3
#ifdef static_mod
constexpr ll mod=65537;
typedef static_modint<65537> mint;
#elif arbitrary_mod
typedef modint mint;
#elif mod1000000007
constexpr ll mod=1000000007;
typedef modint1000000007 mint;
#else
constexpr ll mod=998244353;
typedef modint998244353 mint;
#endif
#endif