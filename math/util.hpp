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