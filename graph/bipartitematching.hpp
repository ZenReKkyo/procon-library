#include "../graph/two_coloring.hpp"
#include "atcoder/maxflow.hpp"
using namespace atcoder;
struct BipartiteMatching{
	int n;
	int s,t;
	vector<int> color;
	vector<pair<int,int>> es;//es[i].first->es[i].second
	mf_graph<int> graph;
	vector<int> eid;  // ids of es[i] on mf_graph
	int flow_val;
	bool built;

	BipartiteMatching(int n):n(n),color(n,-1),built(false){}

	BipartiteMatching(vector<vector<int>> &g):n(g.size()),built(false){
		auto coloring=two_coloring(g);
		assert(coloring.size()!=0);
		color=coloring;
		for(int u=0;u<n;u++){
			for(int v:g[u]){
				if(color[u]==0)es.push_back({u,v});
			}
		}
	}

	void add_edge(int u,int v){
		assert(!(color[u]==1||color[v]==0));
		color[u]=0;
		color[v]=1;
		es.push_back({u,v});
	}

	void build(){
		if(built)return;
		built=true;
		s=n;t=n+1;
		graph=mf_graph<int>(n+2);
		for(int i=0;i<n;i++){
			if(color[i]==0)graph.add_edge(s,i,1);
			else if(color[i]==1)graph.add_edge(i,t,1);
		}
		eid.resize(es.size());
		for(int i=0;i<es.size();i++){
			eid[i]=graph.add_edge(es[i].first,es[i].second,1);
		}
		flow_val=graph.flow(s,t);
	}

	int bipartite_matching(){
		build();
		return flow_val;
	}

	vector<pair<int,int>> matching_edge(){
		build();
		vector<pair<int,int>> res;
		for(int i=0;i<es.size();i++){
			auto e=graph.get_edge(eid[i]);
			if(e.flow>0)res.push_back(es[i]);
		}
		return res;
	}

	vector<int> min_vertex_cover(){
		build();
		auto vis=graph.min_cut(s);
		vector<int> res;
		rep(i,n){
			if(color[i]==0){
				if(!vis[i])res.push_back(i);
			}
			else if(color[i]==1){
				if(vis[i])res.push_back(i);
			}
		}
		return res;
	}

	vector<int> max_independent_set(){
		build();
		auto mvc=min_vertex_cover();
		vector<char> in_cover(n,0);
		for(int x:mvc)in_cover[x]=1;
		vector<int> res;
		rep(i,n)if(!in_cover[i])res.push_back(i);
		return res;
	}
};