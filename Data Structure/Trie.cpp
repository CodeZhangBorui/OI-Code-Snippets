#include <iostream>
#include <cstdio>
#include <cstring>
#define INF 0x3f3f3f3f
#define N 3000005
#define MOD 998'244'353
#define ll long long
template <class T>
inline T read();
using namespace std;

/**
 * @brief Trie（字典树）是一种树形数据结构，
 * @tparam SIZE 最大节点数
 */
template<int SIZE>
struct Trie {
    int Next[SIZE][70]; // 存储 Trie 中每个节点的子节点
    int cnt[SIZE]; // 记录每个节点被访问的次数，表示以该节点为终点的字符串数量
    int tot; // 记录当前节点的总数，用于为新节点分配唯一的索引

    /**
     * @brief 将字符转换为索引。
     * @note 大写字母 'A' 到 'Z' 映射到 0 到 25
     * @note 小写字母 'a' 到 'z' 映射到 26 到 51
     * @note 数字 '0' 到 '9' 映射到 52 到 61
     * @param ch 要转换的字符
     * @return `int` - 转换后的索引
     */
    int char2int(char ch) {
        if(ch >= 'A' && ch <= 'Z') {
            return ch - 'A';
        } else if(ch >= 'a' && ch <= 'z') {
            return ch - 'a' + 26;
        } else {
            return ch - '0' + 52;
        }
    }

    /**
     * @brief 将字符串插入到 Trie 中。
     * @param str C 风格字符串
     * @param length 字符串长度
     */
    void insert(char str[], int length) {
        int pos = 0;
        for(int i = 0; i < length; i++) {
            int ch = char2int(str[i]);
            if(!Next[pos][ch]) {
                Next[pos][ch] = ++tot;
            }
            pos = Next[pos][ch];
            cnt[pos]++;
        }
    }

    /**
     * @brief 将字符串插入到 Trie 中。
     * @param str C 风格字符串
     * @note 调用 `insert(str, strlen(str))` 实现
     */
    void insert(char str[]) {
        insert(str, strlen(str));
    }

    /**
     * @brief 查找字符串 str 在 Trie 中出现的次数 / 前缀。
     * @note 一个字符串 t 是 str 的前缀当且仅当从 str 的末尾删去若干个（可以为 0 个）连续的字符后与 t 相同。
     * @param str C 风格字符串
     * @param length 字符串长度
     * @return `int` - 字符串 str 在 Trie 中出现的次数
     */
    int find(char str[], int length) {
        int pos = 0;
        for(int i = 0; i < length; i++) {
            int ch = char2int(str[i]);
            if(!Next[pos][ch]) {
                return 0;
            }
            pos = Next[pos][ch];
        }
        return cnt[pos];
    }

    /**
     * @brief 查找字符串 str 在 Trie 中出现的次数 / 前缀。
     * @note 一个字符串 t 是 str 的前缀当且仅当从 str 的末尾删去若干个（可以为 0 个）连续的字符后与 t 相同。
     * @param str C 风格字符串
     * @return `int` - 字符串 str 在 Trie 中出现的次数
     * @note 调用 `find(str, strlen(str))` 实现
     */
    int find(char str[]) {
        return find(str, strlen(str));
    }
    
    /**
     * @brief 清空 Trie，将所有节点和计数器重置。
     */
    void clear() {
        for(int i = 0; i <= tot; i++) {
            for(int j = 0; j < 70; j++) {
                Next[i][j] = 0;
            }
            cnt[i] = 0;
        }
        tot = 0;
    }
};

Trie<N> trie;
char inp[N];

// https://www.luogu.com.cn/problem/P8306
int main() {
    int T;
    scanf("%d", &T);
    while (T--)
    {
        int n, q;
        trie.clear();
        scanf("%d%d", &n, &q);
        for(int i = 1; i <= n; i++) {
            scanf("%s", inp);
            trie.insert(inp);
        }
        for(int i = 1; i <= q; i++) {
            scanf("%s", inp);
            printf("%d\n", trie.find(inp));
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