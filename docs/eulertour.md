---
title:  Euler Tour
documentation_of: //tree/eulertour.hpp
---

木の根から出発し、全ての頂点をDFSの要領で訪れたときの頂点列。

部分木に対するクエリを扱うことができる。DFS順で初めて頂点 $u$ が現れるところと最後に $u$ が現れるところの間の区間が元の木における $u$ を根とした部分木に対応している。

その他の応用例: LCA (最小共通祖先)

本ライブラリの実装は頂点属性のEuler Tourで、頂点に対するクエリと相性がいい


## 例題

https://atcoder.jp/contests/abc337/tasks/abc337_g