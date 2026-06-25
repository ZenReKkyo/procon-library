---
title:  Union-Find
documentation_of: //graph/dsu.hpp
---

## 概要

素集合（互いに素な集合）を管理するデータ構造。union by size と経路圧縮により、各クエリを償却 $O(\alpha(N))$ で処理する（$\alpha$ はアッカーマン関数の逆関数）。

ほぼACLibraryの実装と同じだが、連結成分数を管理する機能とグラフをコンストラクタの引数に受け取る機能が追加されている。

- `dsu(int n)` 頂点数 $n$ で初期化する。初期状態では各頂点がそれぞれ独立した連結成分をなす。
- `dsu(vector<vector<int>> &g)` 隣接リスト表現のグラフ $g$ で初期化する。$g$ の各辺で結ばれた頂点を同じ連結成分にまとめる。
- `merge(int a, int b)` $a$ を含む連結成分と $b$ を含む連結成分を統合し、統合後の代表元を返す。
- `same(int a, int b)` $a$ と $b$ が同じ連結成分に属するかを返す。
- `leader(int a)` $a$ が属する連結成分の代表元を返す。
- `size(int a)` $a$ が属する連結成分の頂点数を返す。
- `components()` 連結成分の個数を返す。
- `groups()` 連結成分ごとに頂点をまとめた `vector<vector<int>>` を返す。

## 計算量

- 各クエリ 償却 $O(\alpha(N))$