map<ll,ll>factorization(ll n){
	map<ll,ll>res;
	for(ll i=2;i*i<=n;i++){
		while(n%i==0){
			n/=i;
			res[i]++;
		}
	}
	if(n>1)res[n]++;
	return res;
}
vector<ll>divisor(ll n){
	vector<ll>res;
	for(ll i=1;i*i<=n;i++){
		if(n%i==0){
			res.push_back(i);
			if(i*i!=n)res.push_back(n/i);
		}
	}
	sort(all(res));
	return res;
}
bool is_prime(ll n){
	for(ll i=2;i*i<=n;i++){
		if(n%i==0)return false;
	}
	return true;
}
vec<ll>sieve(ll N){
	vec<bool>is_prime(N+1,true);
	is_prime[0]=is_prime[1]=false;
	vec<ll>res;
	for(int i=2;i<=N;i++){
		if(is_prime[i]){
			res.emplace_back(i);
			for(int j=i+i;j<=N;j+=i)is_prime[i]=false;
		}
	}
	return res;
}

ll primitive_root(ll p){
	vec<ll>factors;
	auto mp=factorization(p-1);
	for(auto &e:mp)factors.emplace_back(e.first);
	random_device rd;
	mt19937 mt(rd());
	while(1){
		ll a=mt()%(p-1)+1;
		int flg=0;
		for(auto &q:factors){
			if(pow_mod(a,(p-1)/q,p)==1){
				flg=1;
			}
		}
		if(flg==0){
			return a;
		}
	}
}
