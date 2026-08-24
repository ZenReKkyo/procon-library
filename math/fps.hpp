template <typename mint>
struct FPS:vector<mint>{
	using vector<mint>::vector;
	using vector<mint>::operator=;
	void ntt(){
		atcoder::internal::butterfly(*this);
	}
	void intt(){
		atcoder::internal::butterfly_inv(*this);
	}

	FPS &operator+=(const FPS &r){
		if(r.size()>this->size())this->resize(r.size());
		for(int i=0;i<r.size();i++)(*this)[i]+=r[i];
		return *this;
	}
	FPS &operator+=(const mint &r){
		if(this->empty())this->resize(1);
		(*this)[0]+=r;
		return *this;
	}
	FPS &operator-=(const FPS &r){
		if(r.size()>this->size())this->resize(r.size());
		for(int i=0;i<r.size();i++)(*this)[i]-=r[i];
		return *this;
	}
	FPS &operator-=(const mint &r){
		if(this->empty())this->resize(1);
		(*this)[0]-=r;
		return *this;
	}
	FPS &operator*=(const FPS &r){
		*this=convolution(*this,r);
		return *this;
	}
	FPS &operator*=(const mint &r){
		for(int i=0;i<this->size();i++)(*this)[i]*=r;
		return *this;
	}
	FPS &operator/=(const FPS &r){
		if(this->size()<r.size()){
			this->clear();
			return *this;
		}
		int n=this->size()-r.size()+1;
		if(r.size()<=64){
			FPS f(*this),g(r);
			g.shrink();
			mint coeff=g.back().inverse();
			for(auto &e:g)e*=coeff;
			int deg=(int)f.size()-(int)g.size()+1;
			int gs=g.size();
			FPS quo(deg);
			for(int i=deg-1;i>=0;i--){
				quo[i]=f[i+gs-1];
				for(int j=0;j<gs;j++)f[i+j]-=quo[i]*g[j];
			}
			*this=quo*coeff;
			this->resize(n,mint(0));
			return *this;
		}
		return *this=((*this).rev().pre(n)*r.rev().inv(n)).pre(n).rev();
	}
	FPS &operator%=(const FPS &r){
		*this-=*this/r*r;
		shrink();
		return *this;
	}

	FPS operator+(const FPS &r)const{return FPS(*this)+=r;}
	FPS operator+(const mint &r) const{return FPS(*this)+=r;}
	FPS operator-(const FPS &r)const{return FPS(*this)-=r;}
	FPS operator-(const mint &r)const{return FPS(*this)-=r;}
	FPS operator*(const FPS &r)const{return FPS(*this)*=r;}
	FPS operator*(const mint &r)const{return FPS(*this)*=r;}
	FPS operator/(const FPS &r)const{return FPS(*this)/=r;}
	FPS operator%(const FPS &r)const{return FPS(*this)%=r;}
	FPS operator-()const{
		FPS ret(this->size());
		for(int i=0;i<this->size();i++)ret[i]=-(*this)[i];
		return ret;
	}
	void shrink(){
		while(this->size()&&this->back()==mint(0))this->pop_back();
	}
	FPS rev() const{
		FPS ret(*this);
		reverse(begin(ret),end(ret));
		return ret;
	}
	FPS pre(int sz) const{
		FPS ret(begin(*this),begin(*this)+min((int)this->size(),sz));
		if(ret.size()<sz)ret.resize(sz);
		return ret;
	}
	FPS operator>>(int sz)const{
		if(this->size()<=sz)return {};
		FPS ret(*this);
		ret.erase(ret.begin(),ret.begin()+sz);
		return ret;
	}
	FPS operator<<(int sz)const{
		FPS ret(*this);
		ret.insert(ret.begin(),sz,mint(0));
		return ret;
	}

	FPS inv(int deg=-1) const{
		int n = this->size();
		if(deg == -1) deg = n;
		assert(n > 0 && (*this)[0] != mint(0));

		FPS ret(1);
		ret[0] = (*this)[0].inv();

		int i = 1;
		while(i < deg){
			int sz = i * 4; 
			FPS f = this->pre(min(n, 2 * i));
			f.resize(sz);
			FPS g = ret;
			g.resize(sz);
			f.ntt();
			g.ntt();
			mint _2(2);
			for(int k = 0; k < sz; k++){
				g[k] *= (_2 - f[k] * g[k]);
			}
			g.intt();
			mint inv_sz = mint(1) / mint(sz);
			ret.resize(2 * i);
			for(int k = 0; k < 2 * i; k++) {
				ret[k] = g[k] * inv_sz;
			}            
			i *= 2;
		}
		ret.resize(deg);
		return ret;
	}

	FPS diff() const{
		const int n=this->size();
		FPS ret(max(0,n-1));
		mint _1(1),coeff(1);
		for(int i=1;i<n;i++){
			ret[i-1]=(*this)[i]*coeff;
			coeff+=_1;
		}
		return ret;
	}
	FPS integral() const{
		const int n=this->size();
		FPS ret(n+1);
		ret[0]=mint(0);
		if(n>0)ret[1]=mint(1);
		auto mod=mint::mod();
		for(int i=2;i<=n;i++){
			ret[i]=(-ret[mod%i])*(mod/i);
		}
		for(int i=0;i<n;i++){
			ret[i+1]*=(*this)[i];
		}
		return ret;
	}
	FPS log(int deg=-1) const{
		assert(!(*this).empty()&&(*this)[0]==mint(1));
		if(deg==-1)deg=this->size();
		return (this->diff()*this->inv(deg)).pre(deg-1).integral();
	}
	FPS exp(int deg=-1) const{
		assert((*this).size()==0||(*this)[0]==mint(0));
		if(deg==-1)deg=(int)this->size();
		if(deg<=0)return FPS();
		const int n=(int)this->size();
		auto at=[&](int i)->mint{ return i<n?(*this)[i]:mint(0); };
		if(deg==1){ FPS r(1); r[0]=mint(1); return r; }

		const long long MOD=mint::mod();
		std::vector<mint> iv(2*deg+2);
		iv[1]=mint(1);
		for(int i=2;i<2*deg+2;i++) iv[i]=-iv[MOD%i]*mint(MOD/i);

		auto ntt_=[](FPS &v){ atcoder::internal::butterfly(v); };
		auto intt_=[](FPS &v){
			atcoder::internal::butterfly_inv(v);
			mint c=mint(1)/mint((int)v.size());
			for(auto &e:v)e*=c;
		};

		FPS b(2),c(1),z1,z2(2);
		b[0]=mint(1); b[1]=at(1);
		c[0]=mint(1);
		z2[0]=mint(1); z2[1]=mint(1);

		for(int m=2;m<deg;m*=2){
			FPS y=b; y.resize(2*m); ntt_(y);

			z1=z2;
			{
				FPS z(m);
				for(int i=0;i<m;i++) z[i]=y[i]*z1[i];
				intt_(z);
				for(int i=0;i<m/2;i++) z[i]=mint(0);
				ntt_(z);
				for(int i=0;i<m;i++) z[i]*=-z1[i];
				intt_(z);
				c.insert(c.end(),z.begin()+m/2,z.end());
			}
			z2=c; z2.resize(2*m); ntt_(z2);

			FPS x(m);
			for(int i=0;i+1<m;i++) x[i]=at(i+1)*mint(i+1);
			ntt_(x);
			for(int i=0;i<m;i++) x[i]*=y[i];
			intt_(x);
			for(int i=0;i+1<m;i++) x[i]-=b[i+1]*mint(i+1);
			x.resize(2*m);
			for(int i=0;i+1<m;i++){ x[m+i]=x[i]; x[i]=mint(0); }

			ntt_(x);
			for(int i=0;i<2*m;i++) x[i]*=z2[i];
			intt_(x);

			FPS e(2*m);
			for(int k=m;k<2*m;k++) e[k]=x[k-1]*iv[k]+at(k);
			ntt_(e);
			for(int i=0;i<2*m;i++) e[i]*=y[i];
			intt_(e);

			b.resize(2*m);
			for(int i=m;i<2*m;i++) b[i]=e[i];
		}
		return FPS(b.begin(),b.begin()+deg);
	}
	FPS pow(long long k,int deg=-1){
		int n=this->size();
		if(deg==-1)deg=n;
		if(k==0){
			FPS ret(deg);
			if(deg)ret[0]=1;
			return ret;
		}
		for(int i=0;i<n;i++){
			if((*this)[i]!=mint(0)){
				mint rev=mint(1)/(*this)[i];
				FPS ret=(((*this*rev)>>i).log(deg)*k).exp(deg);
				ret*=(*this)[i].pow(k);
				ret=(ret<<(i*k)).pre(deg);
				if(ret.size()<deg)ret.resize(deg,mint(0));
				return ret;
			}
			if((long long)(i+1)*k>=deg)return FPS(deg,mint(0));
		}
		return FPS(deg,mint(0));
	}
};
