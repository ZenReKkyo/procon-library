#include <vector>
template<typename T>
struct enumeration{
	std::vector<T> _fact,_finv,_inv;
	void init(int n){
		_fact.assign(n+1,1);
		_finv.assign(n+1,1);
		_inv.assign(n+1,1);
		for(int i=2;i<=n;i++){
			_fact[i]=_fact[i-1]*T(i);
		}
		_finv[n]=T(1)/_fact[n];
		for(int i=n-1;i>=2;i--){
			_finv[i]=_finv[i+1]*T(i+1);
		}
		for(int i=2;i<=n;i++){
			_inv[i]=_finv[i]*_fact[i-1];
		}
	}
	enumeration(int n){init(n);}

	T fact(int n){return _fact[n];}
	T finv(int n){return _finv[n];}
	T inv(int n){return _inv[n];}
	
	T binom(int r,int c){
		if(r<c||r<0||c<0)return T(0);
		return _fact[r]*_finv[c]*_finv[r-c];
	}
};