#include <iostream>
#include <cstdio>
#include <algorithm>
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief 线性基是指一组线性无关向量，这些向量的线性组合可以生成向量空间中的所有向量。它用于描述向量空间的维数和结构。
 * @tparam SIZE 最大线性基的大小（`log2(N)`）
 */
template<int SIZE>
struct Basis {
    ll data[SIZE + 1]; // 存储线性基中的向量
    ll tmp[SIZE + 1]; // 临时数组，用于在某些操作（如 `query_k_min`）中进行计算
    bool flag = false; // 标志位，指示是否有向量无法插入线性基中

    /**
     * @brief 插入一个新的向量到线性基中
     * @param v 要插入的向量
     */
    void insert(ll v) {
        for(int i = SIZE; ~i; i--) {
            if(v & (1ll << i)) {
                if(!data[i]) {
                    data[i] = v;
                    return;
                } else {
                    v ^= data[i];
                }
            }
        }
        flag = true;
    }

    /**
     * @brief 检查一个向量是否可以由线性基中的向量线性组合得到
     * @param v 要检查的向量
     * @return `true` 如果可以由线性基中的向量线性组合得到
     * @return `false` 如果不可以由线性基中的向量线性组合得到
     */
    inline bool check(ll v) {
        for(int i = SIZE; ~i; i--) {
            if(v & (1ll << i)) {
                if(!data[i]) {
                    return false;
                } else {
                    v ^= data[i];
                }
            }
        }
        return true;
    }

    /**
     * @brief 查询线性基中能够表示的最大向量
     * @return `ll` - 线性基中能够表示的最大向量
     */
    ll query_max() {
        ll res = 0;
        for(int i = SIZE; ~i; i--) {
            res = max(res, res ^ data[i]);
        }
        return res;
    }

    /**
     * @brief 查询线性基中能够表示的最小非零向量
     * @return `ll` - 线性基中能够表示的最小非零向量，如果没有非零向量则返回 0
     */
    ll query_min() {
        if(flag) {
            return 0;
        }
        for(int i = 0; i <= SIZE; i++) {
            if(data[i]) {
                return data[i];
            }
        }
    }

    /**
     * @brief 查询线性基中能够表示的第 k 小的向量
     * @param k 第 k 小的向量
     * @return `ll` - 线性基中能够表示的第 k 小的向量，如果不存在则返回 -1
     */
    ll query_k_min(ll k) {
        ll res = 0, cnt = 0;
        k -= flag;
        if(!k) {
            return 0;
        }
        for(int i = 0; i <= SIZE; i++) {
            for(int j = i - 1; ~j; j--) {
                if(data[i] & (1ll << j)) {
                    data[i] ^= data[j];
                }
            }
            if(data[i]) {
                tmp[cnt++] = data[i];
            }
        }
        if(k >= (1ll << cnt)) {
            return -1;
        }
        for(int i = 0; i < cnt; i++) {
            if(k & (1ll << i)) {
                res ^= tmp[i];
            }
        }
        return res;
    }
};
Basis<61> basis;

// https://www.luogu.com.cn/problem/P3812
int main() {
    int n;
    scanf("%d", &n);
    ll x;
    for(int i = 1; i <= n; i++) {
        x = read<ll>();
        basis.insert(x);
    }
    printf("%lld\n", basis.query_max());
    return 0;
}

template <class T>
inline T read() {
    T x = 0, f = 1;
    char ch;
    ch = getchar();
    while (ch > '9' || ch < '0') {
        if (ch == '-')
            f = -f;
        ch = getchar();
    }
    while (ch <= '9' && ch >= '0') {
        x = (x << 1) + (x << 3) + (ch & 15);
        ch = getchar();
    }
    return x * f;
}