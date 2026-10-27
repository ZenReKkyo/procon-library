template<typename T>
struct rmq_sparse{
	//sparse table
	int N=0, K=0;
	vector<T> _a;
	vector<vector<T>> _t;
	rmq_sparse()=default;
	rmq_sparse(const vector<T>& a){
		N=a.size();
		K=0;
		while(1<<(K+1)<=N)K++;
		_t.resize(K+1);
		_t[0]=a;
		for(int k=0;k<K;k++){
			int m=N-(1<<(k+1))+1;
			_t[k+1].resize(m);
			for(int i=0;i<m;i++){
				_t[k+1][i]=min(_t[k][i],_t[k][i+(1<<k)]);
			}
		}
	}
	T query(int l,int r)const{
		//[L,R)
		assert(0<=l&&l<=r&&r<=N);
		if(l==r)return INF;
		int i=31-__builtin_clz(r-l);
		return min(_t[i][l],_t[i][r-(1<<i)]);
	}
};
