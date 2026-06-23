// competitive-verifier: PROBLEM http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_7_A

#include "../../template/template.hpp"
#include "../../graph/bipartitematching.hpp"

int main(){
	ll x,y,e;
	cin>>x>>y>>e;
	BipartiteMatching matching(x+y);
	rep(i,e){
		ll xi,yi;
		cin>>xi>>yi;
		matching.add_edge(xi,x+yi);
	}
	cout<<matching.bipartite_matching()<<endl;
}
