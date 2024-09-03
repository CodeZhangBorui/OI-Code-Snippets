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
 * @brief Splay 平衡树数据结构模板
 * @tparam SIZE 最大节点数量
 */
template <int SIZE>
struct Splay {
    int root; // 树的根节点
    int tree_size; // 树的大小
    int fa[SIZE]; // 节点的父亲
    int sons[SIZE][2]; // 节点的左右儿子
    int sub_size[SIZE]; // 子树的大小
    int val[SIZE]; // 节点的值
    int cnt[SIZE]; // 节点重复出现的次数
    
    /**
     * @brief 清除一个节点的信息
     * @param pos 节点
     */
    inline void clear_son(int pos) {
        sons[pos][0] = sons[pos][1] = fa[pos] = sub_size[pos] = cnt[pos] = val[pos] = 0;
    }

    /**
     * @brief 某个节点在其父节点中的位置
     * @param pos 节点
     * @return 0 为左儿子，1 为右儿子
     */
    inline int get_which(int pos) {
        return sons[fa[pos]][1] == pos;
    }
    
    /**
     * @brief 更新给定节点的子树大小信息，确保子树大小信息保持正确
     * @param pos 节点
     */
    void update(int pos) {
        if(!pos) return;
        sub_size[pos] = cnt[pos];
        if(sons[pos][0]) sub_size[pos] += sub_size[sons[pos][0]];
        if(sons[pos][1]) sub_size[pos] += sub_size[sons[pos][1]];
    }

    /**
     * @brief 旋转操作，本质是将某个节点上移一个位置
     * @param pos 节点
     */
    void rotate(int pos) {
        int father = fa[pos];
        int g_father = fa[father];
        int which_son = get_which(pos);
        sons[father][which_son] = sons[pos][which_son ^ 1];
        fa[sons[father][which_son]] = father;
        sons[pos][which_son ^ 1] = father;
        fa[father] = pos;
        fa[pos] = g_father;
        if(g_father) {
            sons[g_father][sons[g_father][1] == father] = pos;
        }
        update(father);
        update(pos);
    }

    /**
     * @brief Splay 操作，将某个节点旋转到根节点
     * @param pos 节点
     */
    void splay(int pos) {
        for(int father; (father = fa[pos]); rotate(pos)) {
            if(fa[father]) {
                rotate((get_which(pos) == get_which(father)) ? father : pos);
            }
        }
        root = pos;
    }

    /**
     * @brief 插入一个节点
     * @param v 节点的值
     */
    void insert(int v) {
        if(!root) {
            tree_size++;
            sons[tree_size][0] = sons[tree_size][1] = fa[tree_size] = 0;
            root = tree_size;
            sub_size[tree_size] = cnt[tree_size]++;
            val[tree_size] = v;
            return;
        }
        int now = root, father = 0;
        while(true) {
            if(val[now] == v) {
                cnt[now]++;
                update(now);
                update(father);
                splay(now);
                break;
            }
            father = now;
            now = sons[now][val[now] < v];
            if(!now) {
                tree_size++;
                sons[tree_size][0] = sons[tree_size][1] = 0;
                fa[tree_size] = father;
                sub_size[tree_size] = cnt[tree_size] = 1;
                sons[father][val[father] < v] = tree_size;
                val[tree_size] = v;
                update(father);
                splay(tree_size);
                break;
            }
        }
    }

    /**
     * @brief 按排名查找节点
     * @param rk 排名
     * @return 排名为 rk 的节点的值
     */
    int find_num(int rk) {
        int now = root;
        while(true) {
            if(sons[now][0] && rk <= sub_size[sons[now][0]]) {
                now = sons[now][0];
            } else {
                int temp = (sons[now][0] ? sub_size[sons[now][0]] : 0) + cnt[now];
                if(rk <= temp) return val[now];
                rk -= temp;
                now = sons[now][1];
            }
        }
    }

    /**
     * @brief 获取某个节点的排名
     * @param v 节点的值
     * @return 节点的排名
     */
    int get_rank(int v) {
        int now = root, ans = 0;
        while(now) {
            if(v < val[now]) {
                now = sons[now][0];
            } else {
                ans += (sons[now][0] ? sub_size[sons[now][0]] : 0);
                if(v == val[now]) {
                    splay(now);
                    return ans + 1;
                }
                ans += cnt[now];
                now = sons[now][1];
            }
        }
        return ans + 1;
    }

    /**
     * @brief 查找树的前驱
     * @return 树的前驱
     */
    int get_pre() {
        int now = sons[root][0];
        while(sons[now][1]) now = sons[now][1];
        return now;
    }

    /**
     * @brief 查找树的后继
     * @return 树的后继
     */
    int get_nxt() {
        int now = sons[root][1];
        while(sons[now][0]) now = sons[now][0];
        return now;
    }

    /**
     * @brief 求 v 的前驱（前驱定义为小于 v，且最大的数）
     * @param v
     * @return v 的前驱
     */
    int query_pre(int v) {
        insert(v);
        int res = val[get_pre()];
        remove(v);
        return res;
    }

    /**
     * @brief 求 v 的后继（后继定义为大于 v，且最小的数）
     * @param v
     * @return v 的后继
     */
    int query_nxt(int v) {
        insert(v);
        int res = val[get_nxt()];
        remove(v);
        return res;
    }


    /**
     * @brief 删除某个节点
     * @param v 节点的值
     */
    void remove(int v) {
        get_rank(v);
        if(cnt[root] > 1) {
            cnt[root]--;
            update(root);
            return;
        }
        if(!sons[root][0] && !sons[root][1]) {
            clear_son(root);
            root = 0;
            return;
        }
        if(!sons[root][0]) {
            int pre_root = root;
            root = sons[root][1];
            fa[root] = 0;
            clear_son(pre_root);
            return;
        }
        if(!sons[root][1]) {
            int pre_root = root;
            root = sons[root][0];
            fa[root] = 0;
            clear_son(pre_root);
            return;
        }
        int left_max = get_pre();
        int pre_root = root;
        splay(left_max);
        sons[root][1] = sons[pre_root][1];
        fa[sons[pre_root][1]] = root;
        clear_son(pre_root);
        update(root);
    }
};
Splay<N> tree;

// https://www.luogu.com.cn/problem/P3369
int main() {
    int n;
    n = read<int>();
    int opt, x;
    while (n--)
    {
        opt = read<int>(), x = read<int>();
        if(opt == 1) {
            tree.insert(x);
        } else if(opt == 2) {
            tree.remove(x);
        } else if(opt == 3) {
            printf("%d\n", tree.get_rank(x));
        } else if(opt == 4) {
            printf("%d\n", tree.find_num(x));
        } else if(opt == 5) {
            printf("%d\n", tree.query_pre(x));
        } else if(opt == 6) {
            printf("%d\n", tree.query_nxt(x));
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