#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#define INF 0x3f3f3f3f
#define N 100005
#define MOD 998'244'353
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief 线段树的一个一个节点
 * @tparam T 数据类型
 */
template<typename T>
struct SegmentTreeNode {
    int l; // 区间左端点
	int r; // 区间右端点
    T sum; // 区间和
	T tag; // 懒标记
	int len; // 区间长度
    SegmentTreeNode operator+(const SegmentTreeNode &b) const {
        return {l, b.r, sum + b.sum, 0, len + b.len};
    }
};

/**
 * @brief 线段树是一种用于存储区间或线段信息的数据结构。它允许查询所存储的区间中哪些包含给定的点。
 * @tparam T 数据类型
 * @tparam SIZE 数据数量
 */
template<typename T, int SIZE>
struct SegmentTreeIA {
    int length; // 线段树的长度
    T data[SIZE]; // 原始数据
    SegmentTreeNode<T> stree[SIZE << 2]; // 线段树节点

    /**
     * @brief 向线段树原始数据中添加一个数
     * @param v 
     */
    inline void push_back(T v) {
        data[++length] = v;
    }

    /**
     * @brief 将当前节点的值更新为其左子节点和右子节点值的和，以确保父节点的值始终是其子节点值的和。
     * @param root 要处理的节点
     */
    inline void pushup(int root) {
        stree[root] = stree[root << 1] + stree[root << 1 | 1];
    }

    /**
     * @brief 线段树的建树操作
     * @param now 当前节点的索引，在递归过程中，它会不断地左移和右移以表示左右子节点的位置。
     * @param l 表示当前节点所表示的区间的左边界
     * @param r 表示当前节点所表示的区间的右边界
     * @note 递归起始参数为 `build(1, 1, length)`
     * @note 当 `l` 等于 `r` 时，表示这是一个叶子节点。
     */
    void build(int now, int l, int r) {
        if(l == r) {
            stree[now] = {l, l, data[l], 0, 1};
            return;
        }
        int mid = (l + r) >> 1;
        build(now << 1, l, mid);
        build(now << 1 | 1, mid + 1, r);
        pushup(now);
    }

    /**
     * @brief 线段树的建树操作
     * @note 调用 `build(1, 1, length)` 实现
     */
    inline void build() {
        build(1, 1, length);
    }

    /**
     * @brief 为节点添加懒标记
     * @param now 要标记的节点
     * @param tag 懒标记的值
     * @note 懒惰标记，简单来说，就是通过延迟对节点信息的更改，从而减少可能不必要的操作次数。
     * 每次执行修改时，我们通过打标记的方法表明该节点对应的区间在某一次操作中被更改，但不更新该节点的子节点的信息。
     * 实质性的修改则在下一次访问带有标记的节点时才进行。
     */
    void lazytag(int now, T tag) {
        if(stree[now].l == stree[now].r) {
            stree[now].sum += tag;
            return;
        }
        stree[now].tag += tag;
        stree[now].sum += tag * stree[now].len;
    }

    /**
     * @brief 懒标记下传操作
     * @param now 要下传的节点
     * @note 下传操作的作用是将当前节点的懒标记传递给其子节点，并清除当前节点的懒标记。
     */
    void pushdown(int now) {
        lazytag(now << 1, stree[now].tag);
        lazytag(now << 1 | 1, stree[now].tag);
        stree[now].tag = 0;
    }

    /**
     * @brief 线段树区间加
     * @param now 当前节点的索引，在递归过程中，它会不断地左移和右移以表示左右子节点的位置。
     * @param l 表示当前节点所表示的区间的左边界
     * @param r 表示当前节点所表示的区间的右边界
     * @param v 要加的值
     * @note 递归起始参数为 `modify(1, l, r, v)`
     * @note 当 `l` 等于 `r` 时，表示这是一个叶子节点。
     */
    void modify(int now, int l, int r, T v) {
        if(stree[now].tag) {
            pushdown(now);
        }
        if(stree[now].l >= l && stree[now].r <= r) {
            // 当前节点完全在区间内
            lazytag(now, v);
            return;
        } else if(stree[now].l > r || stree[now].r < l) {
            // 当前节点完全不在区间内
            return;
        } else {
            // 将区间分为两端递归操作
            modify(now << 1, l, r, v);
            modify(now << 1 | 1, l, r, v);
			pushup(now);
        }
    }
    
    /**
     * @brief 线段树区间加
     * @param l 区间左端点
     * @param r 区间右端点
     * @param v 要加的值
     * @note 调用 `modify(1, l, r, v)` 实现
     */
    inline void modify(int l, int r, T v) {
        modify(1, l, r, v);
    }

    /**
     * @brief 线段树区间求和
     * @param now 当前节点的索引，在递归过程中，它会不断地左移和右移以表示左右子节点的位置。
     * @param l 表示当前节点所表示的区间的左边界
     * @param r 表示当前节点所表示的区间的右边界
     * @return `T` - [l, r] 的区间和
     * @note 递归起始参数为 `query(1, l, r)`
     * @note 当 `l` 等于 `r` 时，表示这是一个叶子节点。
     */
    T query(int now, int l, int r) {
        if(stree[now].tag) {
            // 如果当前节点有懒标记，先下传
            pushdown(now);
        }
        if(stree[now].l >= l && stree[now].r <= r) {
            // 当前节点完全在区间内
			return stree[now].sum;
        } else if(stree[now].l > r || stree[now].r < l) {
            // 当前节点完全不在区间内
            return 0;
        } else {
            // 将区间分为两端递归操作
            return query(now << 1, l, r) + query(now << 1 | 1, l, r);
        }
    }

    /**
     * @brief 线段树区间求和
     * @param l 区间左端点
     * @param r 区间右端点
     * @return `T` - [l, r] 的区间和
     * @note 调用 `query(1, l, r)` 实现
     */
    inline T query(int l, int r) {
        return query(1, l, r);
    }
};
SegmentTreeIA<ll, N> segtree;

// https://www.luogu.com.cn/problem/P3372
int main() {
	int n, m;
	ll inp;
	scanf("%d%d", &n, &m);
	for(int i = 1; i <= n; i++) {
		inp = read<ll>();
		segtree.push_back(inp);
	}
	segtree.build();
	ll opt, x, y, k;
	for(int i = 1; i <= m; i++) {
		opt = read<ll>();
		if(opt == 1) {
			x = read<ll>();
			y = read<ll>();
			k = read<ll>();
			segtree.modify(x, y, k);
		} else {
			x = read<ll>();
			y = read<ll>();
			printf("%lld\n", segtree.query(x, y));
		}
	}
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