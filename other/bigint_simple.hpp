// bigint_simple.hpp  ---  素直な(愚直な)多倍長整数ライブラリ
//
//   実装の簡単さ最優先。 計算量は O(n^2) 程度だが 1000 桁までなら十分高速。
//   * 符号付き整数
//   * 加減乗除・剰余 ( + - * / % )   … 0 方向丸め（C++の / % と同じ符号規則）
//   * 入出力 ( operator<< / operator>> / to_string / コンストラクタ(string) )
//
//   内部表現: 基数 10（1 桁ずつ）を little-endian の vector<int> で保持。
//   高速版が必要なら bigint.hpp を使うこと。
#ifndef BIGINT_SIMPLE_HPP
#define BIGINT_SIMPLE_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>

class BigIntSimple {
public:
    int sign = 1;             // +1 / -1 （0 のときは +1）
    std::vector<int> d;       // 各桁 0..9, little-endian, 上位の 0 は持たない（0 は空）

    using BI = BigIntSimple;

    // ---- 構築 --------------------------------------------------------------
    BigIntSimple() {}
    BigIntSimple(long long v) {
        if (v < 0) { sign = -1; }
        unsigned long long u = (v < 0) ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
        while (u) { d.push_back((int)(u % 10)); u /= 10; }
    }
    BigIntSimple(const std::string& s) { *this = fromString(s); }
    BigIntSimple(const char* s) { *this = fromString(std::string(s)); }

    bool isZero() const { return d.empty(); }
    static void trim(std::vector<int>& a) { while (!a.empty() && a.back() == 0) a.pop_back(); }

    // ---- 比較 --------------------------------------------------------------
    // magnitude 比較   a<b:-1 a==b:0 a>b:1
    static int cmpMag(const std::vector<int>& a, const std::vector<int>& b) {
        if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
        for (size_t i = a.size(); i-- > 0;)
            if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
        return 0;
    }
    int cmp(const BI& o) const {
        if (isZero() && o.isZero()) return 0;
        if (sign != o.sign) return sign < o.sign ? -1 : 1;
        int c = cmpMag(d, o.d);
        return sign > 0 ? c : -c;
    }
    bool operator==(const BI& o) const { return cmp(o) == 0; }
    bool operator!=(const BI& o) const { return cmp(o) != 0; }
    bool operator< (const BI& o) const { return cmp(o) <  0; }
    bool operator<=(const BI& o) const { return cmp(o) <= 0; }
    bool operator> (const BI& o) const { return cmp(o) >  0; }
    bool operator>=(const BI& o) const { return cmp(o) >= 0; }

    BI operator-() const { BI r = *this; if (!r.isZero()) r.sign = -sign; return r; }
    BI abs() const { BI r = *this; r.sign = 1; return r; }

    // ---- magnitude 演算（符号なし） ---------------------------------------
    static std::vector<int> addMag(const std::vector<int>& a, const std::vector<int>& b) {
        std::vector<int> r;
        int carry = 0;
        for (size_t i = 0; i < a.size() || i < b.size() || carry; i++) {
            int cur = carry;
            if (i < a.size()) cur += a[i];
            if (i < b.size()) cur += b[i];
            r.push_back(cur % 10);
            carry = cur / 10;
        }
        trim(r);
        return r;
    }
    // a >= b が前提
    static std::vector<int> subMag(const std::vector<int>& a, const std::vector<int>& b) {
        std::vector<int> r;
        int borrow = 0;
        for (size_t i = 0; i < a.size(); i++) {
            int cur = a[i] - borrow - (i < b.size() ? b[i] : 0);
            if (cur < 0) { cur += 10; borrow = 1; } else borrow = 0;
            r.push_back(cur);
        }
        trim(r);
        return r;
    }
    // magnitude を 1 桁 m (0..9) 倍
    static std::vector<int> mulSmall(const std::vector<int>& a, int m) {
        std::vector<int> r;
        if (m == 0) return r;
        int carry = 0;
        for (size_t i = 0; i < a.size(); i++) {
            int cur = a[i] * m + carry;
            r.push_back(cur % 10);
            carry = cur / 10;
        }
        while (carry) { r.push_back(carry % 10); carry /= 10; }
        trim(r);
        return r;
    }
    // 筆算による乗算  O(n*m)
    static std::vector<int> mulMag(const std::vector<int>& a, const std::vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        std::vector<int> r(a.size() + b.size(), 0);
        for (size_t i = 0; i < a.size(); i++) {
            int carry = 0;
            for (size_t j = 0; j < b.size(); j++) {
                int cur = r[i + j] + a[i] * b[j] + carry;
                r[i + j] = cur % 10;
                carry = cur / 10;
            }
            size_t k = i + b.size();
            while (carry) { int cur = r[k] + carry; r[k] = cur % 10; carry = cur / 10; k++; }
        }
        trim(r);
        return r;
    }
    // 筆算による除算  A / B → (商, 余り)。 各位の商を 0..9 の試し引きで決める。
    static std::pair<std::vector<int>, std::vector<int>> divModMag(
            const std::vector<int>& A, const std::vector<int>& B) {
        assert(!B.empty());
        if (cmpMag(A, B) < 0) return {{}, A};
        std::vector<int> q(A.size(), 0), rem;
        for (size_t i = A.size(); i-- > 0;) {
            // rem = rem*10 + A[i]  （基数 10 なので先頭に桁を挿入するだけ）
            rem.insert(rem.begin(), A[i]);
            trim(rem);
            // B*(k) <= rem を満たす最大の k (0..9) を求める
            int k = 0;
            while (k < 9 && cmpMag(mulSmall(B, k + 1), rem) <= 0) k++;
            q[i] = k;
            rem = subMag(rem, mulSmall(B, k));
        }
        trim(q);
        return {q, rem};
    }

    // ---- 符号込み演算 ------------------------------------------------------
    static BI make(int sign, std::vector<int> m) {
        BI r; trim(m);
        if (m.empty()) return r;          // 0
        r.sign = sign; r.d = std::move(m);
        return r;
    }
    BI operator+(const BI& o) const {
        if (sign == o.sign) return make(sign, addMag(d, o.d));
        int c = cmpMag(d, o.d);
        if (c == 0) return BI();
        if (c > 0) return make(sign,   subMag(d, o.d));
        else       return make(o.sign, subMag(o.d, d));
    }
    BI operator-(const BI& o) const { return *this + (-o); }
    BI operator*(const BI& o) const {
        if (isZero() || o.isZero()) return BI();
        return make(sign * o.sign, mulMag(d, o.d));
    }
    // 0 方向丸め除算（商は 0 方向、余りは被除数と同符号）
    std::pair<BI, BI> divmod(const BI& o) const {
        assert(!o.isZero() && "division by zero");
        auto pr = divModMag(d, o.d);
        return {make(sign * o.sign, pr.first), make(sign, pr.second)};
    }
    BI operator/(const BI& o) const { return divmod(o).first; }
    BI operator%(const BI& o) const { return divmod(o).second; }

    BI& operator+=(const BI& o) { return *this = *this + o; }
    BI& operator-=(const BI& o) { return *this = *this - o; }
    BI& operator*=(const BI& o) { return *this = *this * o; }
    BI& operator/=(const BI& o) { return *this = *this / o; }
    BI& operator%=(const BI& o) { return *this = *this % o; }

    // ---- 入出力 ------------------------------------------------------------
    static BI fromString(const std::string& s) {
        int sg = 1; size_t i = 0;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) i++;
        if (i < s.size() && (s[i] == '+' || s[i] == '-')) { if (s[i] == '-') sg = -1; i++; }
        size_t end = i;
        while (end < s.size() && s[end] >= '0' && s[end] <= '9') end++;
        std::vector<int> m;                        // little-endian: 下位桁から詰める
        for (size_t j = end; j-- > i;) m.push_back(s[j] - '0');
        trim(m);
        return make(sg, m);
    }
    std::string toString() const {
        if (isZero()) return "0";
        std::string s;
        if (sign < 0) s.push_back('-');
        for (size_t i = d.size(); i-- > 0;) s.push_back((char)('0' + d[i]));
        return s;
    }
    friend std::ostream& operator<<(std::ostream& os, const BI& x) { return os << x.toString(); }
    friend std::istream& operator>>(std::istream& is, BI& x) {
        std::string s; is >> s; x = BI::fromString(s); return is;
    }
};

#endif // BIGINT_SIMPLE_HPP
