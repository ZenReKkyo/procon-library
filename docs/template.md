---
title:  Basic Template
documentation_of: //template/template.hpp
---

## QCFium法

```cpp
# pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")
```

いわゆる「QCFium法」https://speakerdeck.com/tatyam_prime/ding-shu-bei-gao-su-hua-noji-shu?slide=24

それぞれ「AVX2命令（SIMD命令などの含まれる拡張命令セット）を使ってよい」「最適化レベルをO3にする」「ループアンローリングを行う」ことを表している。

AVX命令はIntel/AMDのCPUには対応しているが**Appleシリコンには対応していない**ので注意！

手元で実行する際、コマンドライン側も命令セットを揃えなければならない。さもないとincludeした標準ライブラリの内部で衝突が起こる。https://claude.ai/share/26539c86-7c3b-4968-8dc0-8610c72af784

Codeforcesの場合、AVXを強制しようとするとCompile Errorを吐くので注意 https://codeforces.com/blog/entry/133046?#comment-1188224

## chmin/chmax

```cpp
template<typename T> bool chmin(T& a, T b){if(a > b){a = b; return true;} return false;}
template<typename T> bool chmax(T& a, T b){if(a < b){a = b; return true;} return false;}
```

代入し、max/minの更新が発生したかのbool値を返す。

## 座標圧縮

```cpp
#define compress(a) {sort(all(a));a.erase(unique(all(a)),a.end());}
```

vectorをソートし重複を削除したものを返す。

## 高次元vector生成

```cpp
template <class T> T makevec(T value) {return value;}
template <class T, class... Args,
          std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
auto makevec(std::size_t n, Args... args) {
    auto inner = makevec<T>(args...);
    return std::vector<decltype(inner)>(n, inner);
}
```

各次元の要素数および初期値を順に宣言することで高次元vectorを生成できる。