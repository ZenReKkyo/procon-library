// competitive-verifier: PROBLEM http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A

#include "../../template/template.hpp"
#include "../../graph/dijkstra.hpp"

int main(){
	ll V,E,r;
	cin>>V>>E>>r;
	vector<vector<pair<ll,ll>>> Graph(V);
	rep(i,E){
		ll s,t,d;
		cin>>s>>t>>d;
		Graph[s].push_back({t,d});
	}
	auto res=shortest_path(Graph,{int(r)});
	rep(i,V){
		if(res[i]==numeric_limits<ll>::max())cout<<"INF"<<endl;
		else cout<<res[i]<<endl;
	}
}