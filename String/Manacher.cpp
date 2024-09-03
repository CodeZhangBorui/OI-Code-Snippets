#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define N 11000007
#define MOD 998'244'353
#define ll long long
template <class T>
inline T read();
using namespace std;
char data[N << 1];
int len, pos[N << 1];

/**
 * @brief Manacher 算法是一种在 O(n) 时间复杂度内求解字符串中最长回文子串长度及位置的算法。
 * @note 其基本思想是在遍历字符串的同时，利用已知的回文信息（回文半径数组）来加速对后续回文串的判断。
 * @tparam MAXLEN 
 */
template<int MAXLEN>
struct Manacher {
    int len; // 字符串长度
    char data[MAXLEN << 1]; // 带“隔板”的适合于 Manacher 算法的字符串
    int pos[MAXLEN << 1]; // 回文半径数组
    int max_len; // 最大回文子串的长度

    /**
     * @brief 读取一个字符串，转化为适用于 Manacher 算法的字符串
     */
    void read() {
        data[0] = '$'; // 首位加上一个特殊字符，防止越界
        data[1] = '#'; // 每个字符之间插入一个特殊字符（挡板），确保所有的对称中心都位于一个字符上
        len = 1;
        char ch = getchar();
        while(ch < 'a' || ch > 'z') {
            ch = getchar();
        }
        for(int i = 1; ch >= 'a' && ch <= 'z'; i++) {
            data[++len] = ch;
            data[++len] = '#';
            ch = getchar();
        }
        data[++len] = '!'; // 末尾加上一个特殊字符，防止越界
    }

    /**
     * @brief 求解回文字符串
     * @note 对于在 i 位置开始的回文串，长度为 `pos[i] - 1`
     * @note 所有的回文串长度中的最大值存储于 max_len
     */
    void solve() {
        for(int t = 1, r = 0, mid = 0; t <= len; t++) {
            if(t <= r) {
                pos[t] = min(pos[(mid << 1) - t], r - t + 1);
            }
            while (data[t - pos[t]] == data[t + pos[t]]) {
                pos[t]++;
            }
            if(pos[t] + t > r) {
                r = pos[t] + t - 1;
                mid = t;
            }
            max_len = max(max_len, pos[t] - 1);
        }
    }
};
Manacher<N> manacher;

// https://www.luogu.com.cn/problem/P3805
int main() {
    manacher.read();
    manacher.solve();
    printf("%d\n", manacher.max_len); // 输出最大回文子串的长度
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