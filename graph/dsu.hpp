#include <algorithm>
#include <cassert>
#include <vector>
struct dsu{
    int N;
	int _components;
	std::vector<int>_parent,_size;
    dsu():N(0),_components(0){}
    dsu(int n): N(n),_components(n),_parent(n,-1),_size(n){
		for(int i=0;i<n;i++)_size[i]=1;
	}
	dsu(std::vector<std::vector<int>>g):N(g.size()),_components(g.size()),_parent(g.size(),-1),_size(g.size()){
		for(int i=0;i<N;i++){
			_size[i]=1;
			for(auto e:g[i]){
				merge(i,e);
			}
		}
	}
	int merge(int a,int b){
		assert(0<=a&&a<N);
		assert(0<=b&&b<N);
		a=leader(a);
		b=leader(b);
		if(a==b)return a;
		if(_size[a]>_size[b])std::swap(a,b);
		_parent[a]=b;
		_size[b]+=_size[a];
		_components--;
		return b;
	}   
	bool same(int a,int b){
		assert(0<=a&&a<N);
		assert(0<=b&&b<N);
		return leader(a)==leader(b);
	}
	int leader(int a){
		assert(0<=a&&a<N);
		if(_parent[a]==-1)return a;
		return _parent[a]=leader(_parent[a]);
	}
	int size(int a){
		assert(0<=a&&a<N);
		return _size[leader(a)];
	}
	int components(){
		return _components;
	}
	std::vector<std::vector<int>> groups(){
		std::vector<int>leader_buf(N),group_size(N);
		for(int i=0;i<N;i++){
			leader_buf[i]=leader(i);
			group_size[leader_buf[i]]++;
		}
		std::vector<std::vector<int>>result(N);
		for(int i=0;i<N;i++)result[i].reserve(group_size[i]);
		for(int i=0;i<N;i++)result[leader_buf[i]].push_back(i);
		result.erase(std::remove_if(result.begin(),result.end(),[&](const std::vector<int>&v){return v.empty();}),result.end());
		return result;
	}
};