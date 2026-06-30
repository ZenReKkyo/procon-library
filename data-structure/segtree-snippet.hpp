#include "../atcoder/segtree.hpp"
#include <algorithm>
#include <limits>
using namespace atcoder;

inline long long seg_max_op(long long a, long long b) { return std::max(a, b); }
inline long long seg_max_e() { return std::numeric_limits<long long>::min(); }
using SegMax = segtree<long long, seg_max_op, seg_max_e>;

inline long long seg_min_op(long long a, long long b) { return std::min(a, b); }
inline long long seg_min_e() { return std::numeric_limits<long long>::max(); }
using SegMin = segtree<long long, seg_min_op, seg_min_e>;

template <class T> T seg_sum_op(T a, T b) { return a + b; }
template <class T> T seg_sum_e() { return T(0); }
template <class T> using SegSum = segtree<T, seg_sum_op<T>, seg_sum_e<T>>;

template <class T> struct Affine {
    T a, b;  
};
// f(x)=ax+b
// op(f,g):g(f(x))=(g.a*f.a)*x+(g.a*f.b+g.b)
template <class T> Affine<T> seg_affine_op(Affine<T> f, Affine<T> g) {
    return Affine<T>{g.a * f.a, g.a * f.b + g.b};
}
template <class T> Affine<T> seg_affine_e() { return Affine<T>{T(1), T(0)}; }
template <class T>
using SegAffine = segtree<Affine<T>, seg_affine_op<T>, seg_affine_e<T>>;
