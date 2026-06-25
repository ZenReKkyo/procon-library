// competitive-verifier: PROBLEM https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A

#include "../../template/template.hpp"
#include "../../graph/dsu.hpp"

int main(){
	int n,q;
	cin>>n>>q;
	dsu d(n);
	rep(i,q){
		int t;
		cin>>t;
		if(t==0){
			int u,v;
			cin>>u>>v;
			d.merge(u,v);
		}
		else{
			int u,v;
			cin>>u>>v;
			cout<<(d.same(u,v)?"1":"0")<<endl;
		}
	}
}