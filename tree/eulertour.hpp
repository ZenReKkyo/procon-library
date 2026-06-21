struct EulerTour{
    int N;
    vector<int>vs,depth,id,id_depth,id_rev;
    vector<vector<int>> G;

    EulerTour(int n,vector<vector<int>>&g){
        N=n;
        G=g;
        vs.resize(2*N-1);//DFSでの訪問順
        depth.resize(2*N-1);// 訪問順での根からの深さ
        id.resize(N); //id[i]:iが初めて現れるindex
        id_depth.resize(N);//頂点iの深さ
				id_rev.resize(N); //iが最後に現れるindex
        int k=0;
        dfs(0,-1,0,k);
    }

    void dfs(int now,int par,int d,int& k){
        id[now]=k;
		id_rev[now]=k;
        id_depth[now]=d;
        vs[k]=now;
        depth[k++]=d;
        for(auto e:G[now]){
            if(e==par)continue;
            dfs(e,now,d+1,k);
			id_rev[now]=k;
            vs[k]=now;
            depth[k++]=d;
        }
    }
};