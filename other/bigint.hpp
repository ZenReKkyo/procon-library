// bigint.hpp  ---  多倍長整数ライブラリ
//
//   * 符号付き（正負）整数
//   * 加減乗除・剰余( +  -  *  /  %  )   … 切り捨て除算（C++の / % と同じ符号規則）
//   * 入出力 ( operator<< / operator>> / to_string / コンストラクタ(string) )
//   * 約 2*10^6 桁までを実用的な速度でサポート
//        - 乗算 : Goldilocks 素数 (p = 2^64 - 2^32 + 1) を用いた単一素数 NTT   O(n log n)
//        - 除算 : Newton 法による逆数近似 → 乗算に帰着                        O(M(n))
//
// C++17, 単体ヘッダ。 使い方は bigint_test.cpp を参照。
#pragma once
#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cassert>

class BigInt {
public:
    // ---- 内部表現 ----------------------------------------------------------
    // magnitude: 基数 BASE = 10^8 の little-endian。 上位の余分な 0 は持たない。
    // 値 0 は mag が空・sign=+1 で表す。
    using u32 = uint32_t;
    using u64 = uint64_t;
    using Mag = std::vector<u32>;

    static constexpr u64 BASE      = 100000000ULL; // 10^8
    static constexpr int BASE_DIG  = 8;

    int sign = 1;   // +1 または -1（0 のときは +1）
    Mag mag;        // magnitude

    // ---- 構築 --------------------------------------------------------------
    BigInt() {}
    BigInt(int v) { *this = fromLL((long long)v); }
    BigInt(long long v) { *this = fromLL(v); }
    BigInt(const std::string& s) { *this = fromString(s); }
    BigInt(const char* s) { *this = fromString(std::string(s)); }

    static BigInt fromLL(long long v) {
        BigInt r;
        if (v == 0) return r;
        unsigned long long u;
        if (v < 0) { r.sign = -1; u = (unsigned long long)(-(v + 1)) + 1ULL; }
        else       { r.sign = 1;  u = (unsigned long long)v; }
        while (u) { r.mag.push_back((u32)(u % BASE)); u /= BASE; }
        return r;
    }

    bool isZero() const { return mag.empty(); }

    // =======================================================================
    //  比較
    // =======================================================================
    static int cmpMag(const Mag& a, const Mag& b) {
        if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
        for (size_t i = a.size(); i-- > 0;)
            if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
        return 0;
    }
    // 符号込み比較   a<b:-1  a==b:0  a>b:1
    int cmp(const BigInt& o) const {
        if (isZero() && o.isZero()) return 0;
        if (sign != o.sign) return sign < o.sign ? -1 : 1;
        int c = cmpMag(mag, o.mag);
        return sign > 0 ? c : -c;
    }
    bool operator==(const BigInt& o) const { return cmp(o) == 0; }
    bool operator!=(const BigInt& o) const { return cmp(o) != 0; }
    bool operator< (const BigInt& o) const { return cmp(o) <  0; }
    bool operator<=(const BigInt& o) const { return cmp(o) <= 0; }
    bool operator> (const BigInt& o) const { return cmp(o) >  0; }
    bool operator>=(const BigInt& o) const { return cmp(o) >= 0; }

    BigInt operator-() const { BigInt r = *this; if (!r.isZero()) r.sign = -sign; return r; }
    BigInt abs() const { BigInt r = *this; r.sign = 1; return r; }

    // =======================================================================
    //  magnitude の加減算（符号なし）
    // =======================================================================
    static void trim(Mag& a) { while (!a.empty() && a.back() == 0) a.pop_back(); }

    static Mag addMag(const Mag& a, const Mag& b) {
        Mag r;
        r.reserve(std::max(a.size(), b.size()) + 1);
        u64 carry = 0;
        for (size_t i = 0; i < a.size() || i < b.size() || carry; i++) {
            u64 cur = carry;
            if (i < a.size()) cur += a[i];
            if (i < b.size()) cur += b[i];
            r.push_back((u32)(cur % BASE));
            carry = cur / BASE;
        }
        trim(r);
        return r;
    }
    // a >= b が前提
    static Mag subMag(const Mag& a, const Mag& b) {
        Mag r;
        r.reserve(a.size());
        long long borrow = 0;
        for (size_t i = 0; i < a.size(); i++) {
            long long cur = (long long)a[i] - borrow - (i < b.size() ? (long long)b[i] : 0);
            if (cur < 0) { cur += BASE; borrow = 1; } else borrow = 0;
            r.push_back((u32)cur);
        }
        trim(r);
        return r;
    }

    // magnitude を小さな整数倍 ( 0 <= m < BASE )
    static Mag mulSmall(const Mag& a, u64 m) {
        Mag r;
        if (m == 0 || a.empty()) return r;
        r.reserve(a.size() + 1);
        u64 carry = 0;
        for (size_t i = 0; i < a.size(); i++) {
            u64 cur = (u64)a[i] * m + carry;
            r.push_back((u32)(cur % BASE));
            carry = cur / BASE;
        }
        while (carry) { r.push_back((u32)(carry % BASE)); carry /= BASE; }
        trim(r);
        return r;
    }

    // 基数 BASE での桁シフト  a * BASE^k
    static Mag shlLimbs(const Mag& a, size_t k) {
        if (a.empty()) return a;
        Mag r(a.size() + k, 0);
        std::copy(a.begin(), a.end(), r.begin() + k);
        return r;
    }
    // a を BASE^k で割った商（下位 k 桁を落とす）
    static Mag shrLimbs(const Mag& a, size_t k) {
        if (k >= a.size()) return Mag();
        return Mag(a.begin() + k, a.end());
    }

    // =======================================================================
    //  NTT (Goldilocks 素数  p = 2^64 - 2^32 + 1)
    // =======================================================================
    static constexpr u64 MOD = 0xFFFFFFFF00000001ULL; // 2^64 - 2^32 + 1
    static constexpr u64 G   = 7;                     // 原始根

    static inline u64 addmod(u64 a, u64 b) { u64 s = a + b; if (s < a || s >= MOD) s -= MOD; return s; }
    static inline u64 submod(u64 a, u64 b) { return a >= b ? a - b : a + MOD - b; }
    static inline u64 mulmod(u64 a, u64 b) { return (u64)((__uint128_t)a * b % MOD); }
    static u64 powmod(u64 a, u64 e) {
        u64 r = 1; a %= MOD;
        while (e) { if (e & 1) r = mulmod(r, a); a = mulmod(a, a); e >>= 1; }
        return r;
    }
    static u64 invmod(u64 a) { return powmod(a, MOD - 2); }

    static void ntt(std::vector<u64>& a, bool inv) {
        size_t n = a.size();
        for (size_t i = 1, j = 0; i < n; i++) {
            size_t bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) std::swap(a[i], a[j]);
        }
        for (size_t len = 2; len <= n; len <<= 1) {
            // len 次の原始根
            u64 w = powmod(G, (MOD - 1) / len);
            if (inv) w = invmod(w);
            for (size_t i = 0; i < n; i += len) {
                u64 wn = 1;
                for (size_t j = 0; j < len / 2; j++) {
                    u64 u = a[i + j];
                    u64 v = mulmod(a[i + j + len / 2], wn);
                    a[i + j]           = addmod(u, v);
                    a[i + j + len / 2] = submod(u, v);
                    wn = mulmod(wn, w);
                }
            }
        }
        if (inv) {
            u64 ninv = invmod(n);
            for (auto& x : a) x = mulmod(x, ninv);
        }
    }

    // magnitude 同士の乗算
    static constexpr size_t SCHOOL_MUL = 40; // これ以下は筆算
    static Mag mulMag(const Mag& a, const Mag& b) {
        if (a.empty() || b.empty()) return Mag();
        if (std::min(a.size(), b.size()) <= SCHOOL_MUL) return mulSchool(a, b);
        return mulNTT(a, b);
    }

    static Mag mulSchool(const Mag& a, const Mag& b) {
        std::vector<u64> c(a.size() + b.size(), 0);
        for (size_t i = 0; i < a.size(); i++) {
            u64 carry = 0;
            u64 ai = a[i];
            for (size_t j = 0; j < b.size(); j++) {
                u64 cur = c[i + j] + ai * b[j] + carry;
                c[i + j] = cur % BASE;
                carry = cur / BASE;
            }
            size_t k = i + b.size();
            while (carry) { u64 cur = c[k] + carry; c[k] = cur % BASE; carry = cur / BASE; k++; }
        }
        Mag r(c.begin(), c.end());
        // c は既に < BASE
        Mag rr; rr.reserve(r.size());
        for (auto v : r) rr.push_back((u32)v);
        trim(rr);
        return rr;
    }

    static Mag mulNTT(const Mag& a, const Mag& b) {
        // 基数 10^8 の各桁を 10^4 の 2 桁に分解
        const u64 B4 = 10000;
        size_t la = a.size() * 2, lb = b.size() * 2;
        size_t need = la + lb; // >= la+lb-1
        size_t sz = 1;
        while (sz < need) sz <<= 1;

        std::vector<u64> fa(sz, 0), fb(sz, 0);
        for (size_t i = 0; i < a.size(); i++) { fa[2 * i] = a[i] % B4; fa[2 * i + 1] = a[i] / B4; }
        for (size_t i = 0; i < b.size(); i++) { fb[2 * i] = b[i] % B4; fb[2 * i + 1] = b[i] / B4; }

        ntt(fa, false); ntt(fb, false);
        for (size_t i = 0; i < sz; i++) fa[i] = mulmod(fa[i], fb[i]);
        ntt(fa, true);

        // 各係数 < (10^4)^2 * sz < MOD なので厳密。基数 10^4 で繰り上げ。
        std::vector<u32> d4(sz + 2, 0);
        u64 carry = 0;
        for (size_t i = 0; i < sz; i++) {
            u64 cur = fa[i] + carry;
            d4[i] = (u32)(cur % B4);
            carry = cur / B4;
        }
        size_t idx = sz;
        while (carry) { d4[idx++] = (u32)(carry % B4); carry /= B4; }
        // 10^4 の 2 桁を 10^8 の 1 桁へ
        Mag r((idx + 1) / 2 + 1, 0);
        for (size_t i = 0; i < r.size(); i++) {
            u64 lo = (2 * i     < d4.size()) ? d4[2 * i]     : 0;
            u64 hi = (2 * i + 1 < d4.size()) ? d4[2 * i + 1] : 0;
            r[i] = (u32)(lo + hi * B4);
        }
        trim(r);
        return r;
    }

    // =======================================================================
    //  除算  (magnitude)
    // =======================================================================
    // A を 1 桁 d (1<=d<BASE) で割る → (商, 余り)
    static std::pair<Mag, u64> divSmall(const Mag& a, u64 d) {
        Mag q(a.size(), 0);
        u64 cur = 0;
        for (size_t i = a.size(); i-- > 0;) {
            cur = cur * BASE + a[i];
            q[i] = (u32)(cur / d);
            cur %= d;
        }
        trim(q);
        return {q, cur};
    }

    // R = floor(BASE^(2n) / B) の入口。
    static Mag reciprocal(const Mag& B) { return recipExact(B); }

    // 1 桁精度の過小評価の初期値。  B < (Btop+1)*BASE^(n-1) より
    //   floor(BASE^2/(Btop+1)) * BASE^(n-1) <= floor(BASE^(2n)/B).
    static Mag cheapSeed(const Mag& B) {
        size_t n = B.size();
        u64 seedVal = (BASE * BASE) / ((u64)B[n - 1] + 1);
        Mag s;
        s.push_back((u32)(seedVal % BASE));
        if (seedVal / BASE) s.push_back((u32)(seedVal / BASE));
        Mag y = shlLimbs(s, n - 1);
        trim(y);
        return y;
    }

    // 再帰的倍精度 Newton:  D (n 桁, 最上位!=0) の floor(BASE^(2n)/D) を厳密に返す。
    //   計算量 T(n) = T(n/2) + O(M(n)) = O(M(n))。
    //   要点: 上位部を「切り上げた」Dup = Dhi+1 の逆数を種にすると seed <= 真値 が保証され、
    //         下からの単調収束 + O(1) 補正で正しく詰められる（種の絶対誤差爆発を回避）。
    static Mag recipExact(const Mag& D) {
        size_t n = D.size();
        if (n <= 4) return recipRefine(D, cheapSeed(D));
        size_t k = n / 2;
        Mag Dhi = shrLimbs(D, k);                 // 上位 n-k 桁 = floor(D/BASE^k)
        Mag Dup = addMag(Dhi, Mag{1});            // 切り上げ
        Mag Xh;
        if (Dup.size() > Dhi.size())              // 桁上がり(Dhi が全て BASE-1): Dup = BASE^(n-k)
            Xh = shlLimbs(Mag{1}, Dhi.size());    //   → floor(BASE^(2(n-k))/BASE^(n-k)) = BASE^(n-k)
        else
            Xh = recipExact(Dup);                 // floor(BASE^(2(n-k))/Dup)
        Mag seed = shlLimbs(Xh, k);               // seed <= 真値 が保証される
        return recipRefine(D, seed);
    }

    // seed (<= 真値) から Newton 逆数反復を下から回し floor(BASE^(2n)/B) を厳密に返す。
    //   seed が n/2 桁精度なら数回で収束。1 桁精度なら O(log n) 回。
    static Mag recipRefine(const Mag& B, Mag y) {
        size_t n = B.size();
        Mag T(2 * n + 1, 0); T[2 * n] = 1;        // BASE^(2n)
        Mag twoT = addMag(T, T);
        //   y_{k+1} = floor( y_k * (2T - B*y_k) / BASE^(2n) )   （下から単調増加）
        for (int it = 0; it < 200; it++) {
            Mag By = mulMag(B, y);
            if (cmpMag(By, twoT) > 0) By = twoT;   // 安全（seed<=真値 なら通常起きない）
            Mag e  = subMag(twoT, By);
            Mag yn = shrLimbs(mulMag(y, e), 2 * n);
            trim(yn);
            if (cmpMag(yn, y) <= 0) break;         // 増加が止まった → 収束
            y = yn;
        }
        // 厳密化。 seed<=真値 かつ Newton は下から詰めるので y<=真値、下方向補正は不要、
        // 上方向は O(1) 回。
        Mag one(1, 1);
        while (cmpMag(mulMag(B, y), T) > 0) y = subMag(y, one);
        while (cmpMag(mulMag(B, addMag(y, one)), T) <= 0) y = addMag(y, one);
        return y;
    }

    // U ( <= 2n 桁 ) を B ( n 桁 ) で割る。 R = reciprocal(B) を利用。→ (商, 余り)
    static std::pair<Mag, Mag> div2n(const Mag& U, const Mag& B, const Mag& R, size_t n) {
        // qhat = floor(U*R / BASE^(2n))
        Mag qhat = shrLimbs(mulMag(U, R), 2 * n);
        trim(qhat);
        Mag r;
        {
            Mag qb = mulMag(qhat, B);
            // qhat <= floor(U/B) なので U >= qb
            r = subMag(U, qb);
        }
        Mag one(1, 1);
        // 補正 (たかだか数回)
        while (cmpMag(r, B) >= 0) { r = subMag(r, B); qhat = addMag(qhat, one); }
        return {qhat, r};
    }

    // 一般の magnitude 除算  A / B → (商, 余り)
    static std::pair<Mag, Mag> divModMag(const Mag& A, const Mag& B) {
        assert(!B.empty());
        if (cmpMag(A, B) < 0) return {Mag(), A};
        size_t n = B.size();
        if (n == 1) {
            auto pr = divSmall(A, B[0]);
            Mag r;
            if (pr.second) r.push_back((u32)pr.second);
            return {pr.first, r};
        }
        Mag R = reciprocal(B);

        // A を n 桁ごとのチャンクに分け、基数 BASE^n で筆算除算
        size_t C = (A.size() + n - 1) / n; // チャンク数
        Mag Q(C * n, 0);
        Mag r; // 現在の余り (< B, 高々 n 桁)
        for (size_t ci = C; ci-- > 0;) {
            // A_i = A[ci*n .. ci*n+n-1]
            Mag Ai(n, 0);
            for (size_t k = 0; k < n; k++) {
                size_t idx = ci * n + k;
                if (idx < A.size()) Ai[k] = A[idx];
            }
            // U = r * BASE^n + A_i   (下位 n 桁 = Ai, 上位 = r)
            Mag U = Ai;
            U.resize(n, 0);
            for (size_t k = 0; k < r.size(); k++) U.push_back(r[k]);
            trim(U);

            auto pr = div2n(U, B, R, n);
            Mag qi = pr.first;
            r = pr.second;
            // qi (< BASE^n, <= n 桁) をスロット ci に配置
            for (size_t k = 0; k < qi.size() && k < n; k++) Q[ci * n + k] = qi[k];
        }
        trim(Q);
        trim(r);
        return {Q, r};
    }

    // =======================================================================
    //  符号込み演算
    // =======================================================================
    static BigInt make(int sign, Mag m) {
        BigInt r;
        trim(m);
        if (m.empty()) { r.sign = 1; return r; }
        r.sign = sign; r.mag = std::move(m);
        return r;
    }

    BigInt operator+(const BigInt& o) const {
        if (sign == o.sign) return make(sign, addMag(mag, o.mag));
        int c = cmpMag(mag, o.mag);
        if (c == 0) return BigInt();
        if (c > 0) return make(sign,   subMag(mag, o.mag));
        else       return make(o.sign, subMag(o.mag, mag));
    }
    BigInt operator-(const BigInt& o) const { return *this + (-o); }
    BigInt operator*(const BigInt& o) const {
        if (isZero() || o.isZero()) return BigInt();
        return make(sign * o.sign, mulMag(mag, o.mag));
    }

    // 切り捨て除算（商は 0 方向へ丸め、余りは被除数と同符号）
    std::pair<BigInt, BigInt> divmod(const BigInt& o) const {
        assert(!o.isZero() && "division by zero");
        auto pr = divModMag(mag, o.mag);
        BigInt q = make(sign * o.sign, pr.first);
        BigInt r = make(sign, pr.second);
        return {q, r};
    }
    BigInt operator/(const BigInt& o) const { return divmod(o).first; }
    BigInt operator%(const BigInt& o) const { return divmod(o).second; }

    BigInt& operator+=(const BigInt& o) { return *this = *this + o; }
    BigInt& operator-=(const BigInt& o) { return *this = *this - o; }
    BigInt& operator*=(const BigInt& o) { return *this = *this * o; }
    BigInt& operator/=(const BigInt& o) { return *this = *this / o; }
    BigInt& operator%=(const BigInt& o) { return *this = *this % o; }

    // =======================================================================
    //  入出力
    // =======================================================================
    static BigInt fromString(const std::string& s) {
        BigInt r;
        size_t i = 0;
        int sg = 1;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) i++;
        if (i < s.size() && (s[i] == '+' || s[i] == '-')) { if (s[i] == '-') sg = -1; i++; }
        size_t start = i;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') i++;
        size_t end = i; // [start,end) が数字列
        // 上位の 0 を飛ばさずとも下からまとめれば良い
        Mag m;
        long long pos = (long long)end;
        while (pos > (long long)start) {
            long long lo = std::max((long long)start, pos - BASE_DIG);
            u32 v = 0;
            for (long long k = lo; k < pos; k++) v = v * 10 + (s[k] - '0');
            m.push_back(v);
            pos = lo;
        }
        trim(m);
        return make(sg, m);
    }

    std::string toString() const {
        if (isZero()) return "0";
        std::string s;
        if (sign < 0) s.push_back('-');
        s += std::to_string(mag.back());
        for (size_t i = mag.size() - 1; i-- > 0;) {
            std::string t = std::to_string(mag[i]);
            s.append(BASE_DIG - t.size(), '0');
            s += t;
        }
        return s;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& x) { return os << x.toString(); }
    friend std::istream& operator>>(std::istream& is, BigInt& x) {
        std::string s; is >> s; x = BigInt::fromString(s); return is;
    }
};

#endif // BIGINT_HPP
