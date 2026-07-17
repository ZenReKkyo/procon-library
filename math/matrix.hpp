
template<typename T>
struct Matrix{
	vector<vector<T>> A;
	Matrix(){}
	Matrix(int n,int m):A(n,vector<T>(m,0)){}
	Matrix(int n):A(n,vector<T>(n,0)){};
	int height() const{
		return A.size();
	}
	int width() const{
		return A[0].size();
	}
	vector<T> &operator[](int i){
		return A[i];
	}
	const vector<T> &operator[](int i) const{
		return A[i];
	}
	static Matrix I(int n){
		Matrix mat(n);
		for(int i=0;i<n;i++)mat[i][i]=1;
		return mat;
	}
	Matrix &operator+=(const Matrix &B){
		int h=height(),w=width();
		assert(h==B.height()&&w==B.width());
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				(*this)[i][j]+=B[i][j];
			}
		}
		return (*this);
	}
	Matrix &operator*=(const Matrix &B){
		int h=height(),w=B.width(),p=width();
		assert(p==B.height());
		vector<vector<T>>C(h,vector<T>(w,0));
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				for(int k=0;k<p;k++){
					C[i][j]+=(*this)[i][k]*B[k][j];
				}
			}
		}
		A.swap(C);
		return (*this);
	}
	Matrix operator+(const Matrix &B) const{
		Matrix C(*this);
		C+=B;
		return C;
	}
	Matrix operator*(const Matrix &B) const{
		Matrix C(*this);
		C*=B;
		return C;
	}
	Matrix pow(ll n) const{
		assert(height()==width());
		Matrix res=I(height()),x(*this);
		while(n>0){
			if(n&1)res*=x;
			x*=x;
			n>>=1;
		}
		return res;
	}
};