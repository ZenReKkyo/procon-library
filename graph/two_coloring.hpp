vector<int>two_coloring(vector<vector<int>>&Graph){
	int N=Graph.size();
	vector<int>color(N,-1);
	int valid=1;
	auto dfs=[&](auto self,int now){
		for(auto &e:Graph[now]){
			if(color[e]==color[now]){
				valid=0;
				return;
			}
			if(color[e]==-1){
				color[e]=1-color[now];
				self(self,e);
			}
		}
	};
	rep(i,N){
		if(color[i]==-1){
			color[i]=0;
			dfs(dfs,i);
		}
	}
	if(valid==0)return {};
	return color;
}