---
title:  二部グラフの最大マッチング
documentation_of: //graph/bipartitematching.hpp
---

## 概要

二部グラフの最大マッチングを求める。
グラフを白と黒の2色に塗り分け、sourceから白の頂点、白から黒の頂点、黒からsinkの頂点にそれぞれ流量1の辺を張ったグラフでの最大流がそのまま最大マッチングに対応する。

- `BipartiteMatching(int n)` 頂点数 $n$ で初期化する
- `BipartiteMatching(vector<vector<int>> &g)` グラフ $g$ で初期化する。
- `add_edge(int u,int v)` $u$がsource側、 $v$ がsink側であるような辺$u \to v$ を追加する
- `bipartite_matching()` マッチングの個数を返す
-  `matching_edge()` マッチングの辺を返す
- `min_vertex_cover()` 二部グラフの最小点カバーを返す
- `max_independent_set()` 二部グラフの最大独立集合を返す

## 最大独立集合・最小点カバー

一般のグラフにおいて，|Max Independent Set|=V-|Min Vertex Cover| が常に成立する．
> Proof. $S$ を Independent set とする．任意の辺 $(u,v) \in E$ について, $u,v$ の両方が $S$ に含まれていることは Independent set の定義からあり得ない．よって，少なくとも片方は $S$ に含まれていない．したがって，辺 $e \in E$ の頂点のうち少なくとも1つは $V \setminus S$ に含まれる．故に，$V \setminus S$ はVertex Cover をなす．
> 同様に，$C$ をVertex Coverとすれば $V \setminus C$ がIndependent Set をなす．
> 以上の議論より，$|C_{\min}| \le V-|S_{\max}|$ かつ $|S_{\max}| \ge V-|C_{\min}|$ なので，$|S_{\max}|+|C_{\min}|=V$ 

また，二部グラフにおいて|Min Vertex Cover|=|Max Matching| が成立する．（Kőnig の定理）
ここで、Max MatchingからMin Vertex Coverを復元することを考える。実は、残余グラフ上のsource側でsから到達不可能な点およびsink側でsから到達可能な点の和集合がMin Vertex Coverとなる。Min Cutで白のsink側と黒のsource側とも言い換えられる。

> Proof. 以降source 側を単に $S$, sink側を $T$ と書く。二部グラフの辺であって、(白, $S$ )と(黒, $T$ )を結ぶ辺は存在しない。存在した場合カットの条件に反する。よって、全ての二部グラフの辺は $(S,S), (T,S), (T,T)$ のどれかである。
> 白かつ $T$ の頂点および黒かつ $S$ の頂点は必ずマッチングの端点である。 そうでないと $S,T$ それぞれの内部でaugmenting pathが存在することになり最大マッチングに反する。

## 計算量

- $O(VE)$