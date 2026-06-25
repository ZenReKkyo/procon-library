#include <cmath>
#include <iomanip>
#include <iostream>

const double PI=std::acos(-1.0);

struct Point{
    double x,y;
    Point():x(0),y(0){}
    Point(double x,double y):x(x),y(y){}

    Point operator+(const Point& p)const{ return Point(x+p.x,y+p.y); }
    Point operator-(const Point& p)const{ return Point(x-p.x,y-p.y); }
    Point operator*(double k)const{ return Point(x*k,y*k); }
    Point operator/(double k)const{ return Point(x/k,y/k); }
    Point& operator+=(const Point& p){ x+=p.x; y+=p.y; return *this; }
    Point& operator-=(const Point& p){ x-=p.x; y-=p.y; return *this; }
    Point& operator*=(double k){ x*=k; y*=k; return *this; }
    Point& operator/=(double k){ x/=k; y/=k; return *this; }

    double norm()const{ return x*x+y*y; }
    double abs()const{ return std::sqrt(norm()); }
    double arg()const{ return std::atan2(y,x); }
};

double dot(const Point& a,const Point& b){ return a.x*b.x+a.y*b.y; }
double cross(const Point& a,const Point& b){ return a.x*b.y-a.y*b.x; }

std::istream& operator>>(std::istream& is,Point& p){
    is>>p.x>>p.y; return is;
}
std::ostream& operator<<(std::ostream& os,const Point& p){
    return os<<std::fixed<<std::setprecision(10)<<p.x<<" "<<p.y;
}

double distance(const Point& a,const Point& b){
    return (a-b).abs();
}

Point divide_internal(const Point& a,const Point& b,double m,double n){
    return (a*n+b*m)/(m+n);
}
Point divide_external(const Point& a,const Point& b,double m,double n){
    return (a*n-b*m)/(n-m);
}