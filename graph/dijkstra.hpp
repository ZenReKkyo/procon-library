template <typename T>
vector<T> shortest_path(const vector<vector<pair<ll,T>>>& Graph,vector<int>s){
	int n=Graph.size();
	vector<T> dist(n,numeric_limits<T>::max());
	for(auto &e:s) dist[e]=0;
	priority_queue<pair<T,int>,vector<pair<T,int>>,greater<pair<T,int>>> pq;
	for(auto &e:s) pq.push({0,e});
	while(!pq.empty()){
		auto [d,v]=pq.top(); pq.pop();
		if(dist[v]<d)continue;
		for(auto [u,c]:Graph[v]){
			if(chmin(dist[u],dist[v]+c))pq.push({dist[u],u});
		}
	}
	return dist;
}

template <typename T>
vector<T> dense_shortest_path(const vector<vector<pair<ll,T>>>& Graph,vector<int>s){
    int n=Graph.size();
    vector<T> dist(n,numeric_limits<T>::max());
    vector<bool> used(n,false);
    for(auto &e:s) dist[e]=0;
    while(true){
        int v=-1;
        for(int i=0;i<n;i++){
            if(!used[i]&&(v==-1||dist[i]<dist[v])) v=i;
        }
        if(v==-1) break;
        used[v]=true;
        for(auto [u,c]:Graph[v]){
            chmin(dist[u],dist[v]+c);
        }
    }
    return dist;
}