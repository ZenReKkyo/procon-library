#include <cassert>
#include <cstddef>
#include <functional>
#include <map>
#include <queue>
#include <vector>

// 大きい方から k 個の値を管理するデータ構造。
//   - topKSum() : 大きい方から k 個の値の総和
//   - kth()     : 大きい方から k 番目の値
//   - insert(x) : 要素 x を追加
//   - erase(x)  : 要素 x があれば 1 つ削除し、成否を返す
//   - set_k(k)  : 現在の k を変更する
// priority_queue を 2 本 (in / out) 使い、各々を遅延削除で erasable にしている。
//   in  : 上位 k 個を保持する最小ヒープ (先頭 = 上位 k 個の中で最小 = k 番目)
//   out : 残りを保持する最大ヒープ         (先頭 = 上位 k 個の外で最大)
template <typename T, typename Compare = std::less<T>, typename RCompare = std::greater<T>>
struct TopKSum {
    size_t k;
    T sum;

    // 遅延削除付きヒープ。del に削除予約を積み、先頭が一致したら捨てる。
    template <typename C>
    struct ErasableHeap {
        std::priority_queue<T, std::vector<T>, C> raw, del;
        void push(const T& x) { raw.push(x); }
        void erase(const T& x) { del.push(x); }
        void prune() {
            while (!del.empty() && raw.top() == del.top()) raw.pop(), del.pop();
        }
        const T& top() { prune(); return raw.top(); }
        void pop() { prune(); raw.pop(); }
        size_t size() const { return raw.size() - del.size(); }
        bool empty() const { return size() == 0; }
    };

    ErasableHeap<RCompare> in;  // 最小ヒープ
    ErasableHeap<Compare> out;  // 最大ヒープ
    std::map<T, int> cnt;       // 存在判定 (erase の成否) 用

    TopKSum(size_t k = 0) : k(k), sum(0) {}

    size_t size() const { return in.size() + out.size(); }

    // in の要素数を min(k, size()) に合わせる。
    void modify() {
        while (in.size() > k) {
            T p = in.top(); in.pop();
            sum -= p;
            out.push(p);
        }
        while (in.size() < k && !out.empty()) {
            T p = out.top(); out.pop();
            sum += p;
            in.push(p);
        }
    }

    // 要素 x を追加する。
    void insert(const T& x) {
        cnt[x]++;
        in.push(x);
        sum += x;
        modify();  // in が k を超えたら最小要素を out へ落とす
    }

    // 要素 x を 1 つ削除する。存在すれば true。
    bool erase(const T& x) {
        auto it = cnt.find(x);
        if (it == cnt.end()) return false;
        if (--it->second == 0) cnt.erase(it);
        // x が boundary (= in.top()) 以上なら in 側、そうでなければ out 側。
        if (in.size() && !Compare()(x, in.top())) {
            in.erase(x);
            sum -= x;
        } else {
            out.erase(x);
        }
        modify();
        return true;
    }

    // 大きい方から k 個の値の総和。
    T topKSum() const { return sum; }

    // 大きい方から k 番目の値 (size() >= k が前提)。
    T kth() {
        assert(size() >= k && k > 0);
        return in.top();
    }

    // 現在の k を変更する。
    void set_k(size_t nk) {
        k = nk;
        modify();
    }
};
