#include <iostream>
#include <cstdio>
#include <algorithm>
#define INF 0x3f3f3f3f
#define N 100005
#define MOD 998'244'353
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief ST 表（Sparse Table，稀疏表）是用于解决 可重复贡献问题 的数据结构。
 * @tparam T 数据类型
 * @tparam SIZE 数据数量
 */
template<typename T, int SIZE>
struct SparseTable {
    int size = 0; // 当前数据个数
    T st_data[2][SIZE][21]; // ST 表数据
    int log_2[SIZE]; // 二进制对数（加速计算）

    /**
     * @brief 向 ST 表中添加一个数
     * @param v 预备添加的数
     */
    inline void push_back(int v) {
        size++;
        st_data[0][size][0] = v;
        st_data[1][size][0] = v;
    }
    
    /**
     * @brief 初始化 ST 表
     * @attention 需要在数据输入完毕后再初始化
     */
    void load() {
		for(int i = 2; i <= size; i++) {
			log_2[i] = log_2[i >> 1] + 1;
		}
        for(int j = 1; j <= 21; j++) {
            for(int i = 1; i + (1 << j) - 1 <= size; i++) {
                st_data[0][i][j] = max(st_data[0][i][j - 1], st_data[0][i + (1 << (j - 1))][j - 1]);
                st_data[1][i][j] = min(st_data[1][i][j - 1], st_data[1][i + (1 << (j - 1))][j - 1]);
            }
        } 
    }

    /**
     * @brief 获取 [l, r] 之间的最大值
     * @param l 左端点
     * @param r 右端点
     */
    T get_max(int l, int r) {
        int k = log_2[r - l + 1];
        return max(st_data[0][l][k], st_data[0][r - (1 << k) + 1][k]);
    }

    /**
     * @brief 获取 [l, r] 之间的最小值
     * @param l 左端点
     * @param r 右端点
     */
    T get_min(int l, int r) {
        int k = log_2[r - l + 1];
        return min(st_data[1][l][k], st_data[1][r - (1 << k) + 1][k]);
    }
};
SparseTable<int, N> st;

// https://www.luogu.com.cn/problem/P3865
int main() {
    int n, m;
	n = read<int>();
	m = read<int>();
    int val;
	for(int i = 1; i <= n; i++) {
		val = read<int>();
        st.push_back(val);
	}
	st.load();
	while(m--) {
		int l = read<int>();
		int r = read<int>();
		printf("%d\n", st.get_max(l, r));
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