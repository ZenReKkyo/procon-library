---
title:  Heavy Light Decomposition
documentation_of: //tree/hldecomposition.hpp
---
木をいくつかのパスに分解し、パスクエリに答える手法

根付き木において、頂点数が最も多い子に向かう辺を **Heavy Edge** , そうでない辺を **Light Edge** と呼ぶ。
任意のパスについて，そのパスが通るLight Edgeは高々 $O(\log N)$ 個であるので， 任意のパスを Heavy Edge からなる Heavy path の $O(\log N)$ 個の連結として表せる．

## 実装

DFS2回で各種情報を入手する．

DFS1回目で以下のものを計算する．

- $\mathrm{par}[v]$ : $v$ の親頂点
- $\mathrm{depth}[v]$ : $v$ の根からの深さ
- $\mathrm{size}[v]$ : $v$ を根とする部分木のサイズ

これを用いてHeavy Edgeを特定する．隣接リスト表現でHeavy Edgeを $\mathrm{Graph}[v][0]$ に設定しておくと便利．(2回目のDFSはheavy edge優先で潜る)

DFS2回目で以下のものを計算する．

- $\mathrm{vertex}[i]$ : 行きがけ順で $i$ 番目に訪れる頂点
- $\mathrm{id}[v]$ : $v$ を何番目に訪れるか
- $\mathrm{head}[v]$ : $v$ を通るheavy pathのうち最も根側にある頂点