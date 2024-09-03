#include <iostream>
#include <cstdio>
#define INF 0x3f3f3f3f
#define N 100005
#define MOD 998'244'353
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief 并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素。
 * @note 路径压缩
 * @tparam SIZE 数据数量
 */
template<int SIZE>
struct DisjointSet_ShortenPath {
    int fa[SIZE]; // 父亲节点

    DisjointSet_ShortenPath() {
        for(int i = 1; i <= SIZE; i++) {
            fa[i] = i;
        }
    }

    /**
     * @brief 查找元素所属的根父亲
     * @param x 元素节点的编号
     * @return 父节点的编号
     */
    int find(int x) {
        if(x == fa[x]) {
            return x;
        }
        return fa[x] = find(fa[x]);
    }

    /**
     * @brief 合并两个元素所属的集合（按秩合并）
     * @param x 一个元素
     * @param y 另外一个元素
     */
    void merge(int x, int y) {
        int fx = find(x);
        int fy = find(y);
        if(fx != fy) {
            fa[fx] = fy;
        }
    }

    /**
     * @brief 查询两个元素是否属于同一个集合
     * @param x 一个元素
     * @param y 另外一个元素
     * @return `bool` - 是否属于同一个集合
     */
    inline bool check(int x, int y) {
        return find(x) == find(y);
    }
};

/**
 * @brief 并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素。
 * @note 按秩合并
 * @tparam SIZE 数据数量
 */
template<int SIZE>
struct DisjointSet_UnionByRank {
    int fa[SIZE]; // 父亲节点
    int depth[SIZE]; // 深度

    DisjointSet_UnionByRank() {
        for(int i = 1; i <= SIZE; i++) {
            fa[i] = i;
            depth[i] = 1;
        }
    }

    /**
     * @brief 查找元素所属的根父亲
     * @param x 元素节点的编号
     * @return 父节点的编号
     */
    int find(int x) {
        if(x == fa[x]) {
            return x;
        }
        return find(fa[x]);
    }

    /**
     * @brief 合并两个元素所属的集合（按秩合并）
     * @param x 一个元素
     * @param y 另外一个元素
     */
    void merge(int x, int y) {
        int fx = find(x);
        int fy = find(y);
        if(fx == fy) {
            return;
        }
        if(depth[fx] < depth[fy]) {
            swap(fx, fy);
        }
        depth[fx] += depth[fy];
        fa[fy] = fx;
    }

    /**
     * @brief 查询两个元素是否属于同一个集合
     * @param x 一个元素
     * @param y 另外一个元素
     * @return `bool` - 是否属于同一个集合
     */
    inline bool check(int x, int y) {
        return find(x) == find(y);
    }
};
DisjointSet_ShortenPath<N> dsu;

// https://www.luogu.com.cn/problem/P3367
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
	for(int i = 1; i <= m; i++) {
		int mode, x, y;
		scanf("%d%d%d", &mode, &x, &y);
		if(mode == 1) {
			dsu.merge(x, y);
			continue;
		} else {
			if(dsu.check(x, y)) {
				puts("Y");
			} else {
				puts("N");
			}
		}
	}
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