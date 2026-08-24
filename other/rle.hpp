template <typename T>
vector<pair<T,int>>rle(vector<T> V){
	vector<pair<T,int>>ans;
	int cnt=0;
	T now=V[0];
	for(auto &e:V){
		if(now==e)cnt++;
		else{
			ans.push_back({now,cnt});
			now=e;
			cnt=1;
		}
	}
	ans.push_back({now,cnt});
	return ans;
}