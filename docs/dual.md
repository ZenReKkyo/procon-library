## 双対問題

$$
\begin{equation}
\tag{P}
       \begin{aligned}
           & \text{max}
               & {\bf c}^T{\bf x} \\
           & \text{s.t.}
               & A{\bf x} \le {\bf b} \\
       \end{aligned}
   \end{equation}
$$

このようなLP問題に対し、

$$
\begin{equation}
\tag{D}
       \begin{aligned}
           & \text{min}
               & {\bf y}^T{\bf b} \\
           & \text{s.t.}
               & {\bf y}^TA = {\bf c}^T \\
& & {\bf y} \geq \bf 0
       \end{aligned}
   \end{equation}
$$

で定まる問題もまたLPである。問題Pを主問題、問題Dを双対問題と呼ぶ。

適切な書き換えにより、以下のようにも表現できる。

- 双対の別表現
    
    以下の左右がそれぞれ双対として対応する。
    
    $$
    \begin{equation}
     \tag{P}      \begin{aligned}
               & \text{max}
                   & {\bf c}^T{\bf x} \\
               & \text{s.t.}
                   & A{\bf x} \le {\bf b} \\
             &  & x \geq \bf 0
           \end{aligned}
       \end{equation}
    $$
    
    $$
    \begin{equation}
     \tag{P}      \begin{aligned}
               & \text{max}
                   & {\bf c}^T{\bf x} \\
               & \text{s.t.}
                   & A{\bf x} = {\bf b} \\
             &  & x \geq \bf 0
           \end{aligned}
       \end{equation}
    $$
    
    $$
    \begin{equation}
    \tag{D}
           \begin{aligned}
               & \text{min}
                   & {\bf y}^T{\bf b} \\
               & \text{s.t.}
                   & {\bf y}^TA \geq {\bf c}^T \\
    & & {\bf y} \geq \bf 0
           \end{aligned}
       \end{equation}
    $$
    
    $$
    \begin{equation}
    \tag{D}
           \begin{aligned}
               & \text{min}
                   & {\bf y}^T{\bf b} \\
               & \text{s.t.}
                   & {\bf y}^TA \geq {\bf c}^T
           \end{aligned}
       \end{equation}
    $$
    
- 双対の双対は元の問題と一致する。