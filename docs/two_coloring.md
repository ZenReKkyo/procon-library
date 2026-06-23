---
title:  2彩色(二部グラフの彩色)
documentation_of: //graph/two_coloring.hpp
---

グラフが2彩色可能か（=グラフが二部グラフか）の判定は 頂点数$N$, 辺数 $M$ に対し $O(N+M)$ で可能である．

DFSを行い，二部グラフでない場合は空のvectorを返す．二部グラフである場合は連結成分ごとに適当な塗り分けの結果を返す．

## 手動verify

- [ABC327-D](https://atcoder.jp/contests/abc327/submissions/76913636)