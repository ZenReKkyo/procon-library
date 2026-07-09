using ll=long long;
ll rem(ll a,ll b){
	return (a%b+b)%b;
}
ll quo(ll a,ll b){
	return (a-rem(a,b))/b;
}
ll upper_mult(ll x,ll M){
	if(x%M==0)return x;
	x+=rem(x,M);
	return x;
}
ll lower_mult(ll x,ll M){
	if(x%M==0)return x;
	x-=rem(x,M);
	return x;
}
ll sqrt_floor(ll x){
	ll lb=0,ub=2.83e9;
	while(ub-lb>1){
		ll mi=(ub+lb)/2;
		if(mi*mi<=x)lb=mi;
		else ub=mi;
	}
	return lb;
}
ll gcd(ll a,ll b){
	if(b==0)return a;
	return gcd(b,a%b);
}
ll extgcd(ll a,ll b,ll &x,ll &y){
	if(b==0){x=1;y=0;return a;}
	ll d=extgcd(b,a%b,y,x);
	y-=a/b*x;
	return d;
}
template<typename T>
T modular_equation(T a,T b,T m){
	//ax=b mod m
	T g=std::gcd(a,m);
	if(b%g!=0)return -1;
	a/=g;b/=g;m/=g;
	T x,y;
	extgcd(a,m,x,y);
	x=rem(x,m);
	return rem(x*b,m);
}