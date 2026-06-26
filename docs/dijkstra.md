---
title:  最短経路(負辺なし)
documentation_of: //graph/dijkstra.hpp
---

## 概要
負辺のない重み付きグラフで単一/複数ソース最短経路を求める。グラフがsparseの場合、「距離が確定済みの頂点から最も近いもの」をheapを用いて取り出すことで効率よく距離を確定できる。

もしグラフがdenseな場合、heapに $O(N^2)$本の頂点が入りうる。そのため、代わりに毎回未確定の頂点で距離が最小のものを線型探索することで計算量を $O(N^2)$ にできる。
## 計算量

- `shortest_path` の場合 $O(M\log N)$ 
- `dense_shortest_path` の場合 $O(N^2)$