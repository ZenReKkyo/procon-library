template<typename T>
struct static_sum{
    std::vector<ll>_data;
    static_sum(std::vector<T>v){
        _data.resize(v.size()+1);
        rep(i,v.size())_data[i+1]=_data[i]+v[i];
    }
    T sum(int l,int r){//closed interval
        return _data[r]-_data[l];
    }
};