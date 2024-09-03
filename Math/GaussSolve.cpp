#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief 高斯消元法是求解线性方程组的经典算法，除了用于线性方程组求解外，还可以用于行列式计算、求矩阵的逆，以及其他计算机和工程方面。
 * @tparam MAXN 可预料的最大未知数个数
 */
template <int MAXN>
struct GaussSolver {
    int unknowns;            // 未知数个数
    double eps = 1e-6;       // 求解精度
    double nums[MAXN][MAXN]; // 系数矩阵
    double ans[MAXN];        // 答案数组

    /**
     * @brief 对方程组求解
     * @return 求解状态 0 为唯一解，-1 为无解，>=1 为自由元个数
     * @note 答案存于 `ans` 中
     */
    int solve() {
        int row = 0, col = 0;
        for (; row < unknowns && col < unknowns; row++, col++) {
            int maxrow = row;
            for (int i = row + 1; i < unknowns; i++) {
                if (abs(nums[i][col]) > abs(nums[maxrow][col])) {
                    maxrow = i;
                }
            }
            if (maxrow != row) {
                swap(nums[row], nums[maxrow]);
            }
            if (fabs(nums[row][col]) < eps) {
                row--;
                continue;
            }
            for (int i = row + 1; i < unknowns; i++) {
                if (fabs(nums[i][col]) > eps) {
                    double k = nums[i][col] / nums[row][col];
                    for (int j = col; j <= unknowns; j++) {
                        nums[i][j] -= nums[row][j] * k;
                    }
                    nums[i][col] = 0;
                }
            }
        }
        for (int i = row; i < unknowns; i++) {
            if (fabs(nums[i][col]) > eps) {
                return -1;
            }
        }
        if (row < unknowns) {
            return unknowns - row;
        }
        for (int i = unknowns - 1; i >= 0; i--) {
            for (int j = i + 1; j < unknowns; j++) {
                nums[i][unknowns] -= nums[i][j] * ans[j];
            }
            ans[i] = nums[i][unknowns] / nums[i][i];
        }
        return 0;
    }
};
GaussSolver<101> gs;

// https://www.luogu.com.cn/problem/P3389
int main()
{
    int n;
    scanf("%d", &n);
    gs.unknowns = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            scanf("%lf", &gs.nums[i][j]);
        }
    }
    int res = gs.solve();
    if(res == -1) {
        printf("No Solution\n");
        return 0;
    } else if(res > 0) {
        printf("Multiple Solution: %d free\n", res);
        return 0;
    }
    for (int i = 0; i < n; i++) {
        printf("x%d=%.2lf\n", i + 1, gs.ans[i]);
    }
}

template <class T>
inline T read()
{
    T x = 0, f = 1;
    char ch;
    ch = getchar();
    while (ch > '9' || ch < '0')
    {
        if (ch == '-')
            f = -f;
        ch = getchar();
    }
    while (ch <= '9' && ch >= '0')
    {
        x = (x << 1) + (x << 3) + (ch & 15);
        ch = getchar();
    }
    return x * f;
}