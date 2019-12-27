#include <bits/stdc++.h>
using namespace std;

struct edge {
    int to, cap, rev;
};

const int N = 2e4; // 記得都要兩倍
const int INF = 1e9;

vector<edge> G[N];
// ty: 狗的性別
// v: 狗變性的cost
int ty[N], v[N], level[N], iter[N];

void add_edge(int from, int to, int cap) {
    G[from].push_back((edge){to, cap, (int) G[to].size()});
    G[to].push_back((edge){from, 0, (int) G[from].size() - 1});
}

void bfs(int s) {
    memset(level, -1, sizeof(level));
    queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto e : G[u]) {
            if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }
}

int dfs(int u, int t, int f) {
    if (u == t) {
        return f;
    }
    for (int i = iter[u]; i < (int) G[u].size(); i++) {
        edge &e = G[u][i];
        if (e.cap > 0 && level[u] < level[e.to]) {
            int d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0;
    while (true) {
        bfs(s);
        if (level[t] < 0) {
            return flow;
        }
        memset(iter, 0, sizeof(iter));
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
}

int main() {
    int n, m, g, ans;
    // n: n個狗
    // m: rich folks
    // g: 要陪給"朋友"的錢
    scanf("%d %d %d", &n, &m, &g);
    // ty: 每隻狗的sex, 0: female(false), 1:male(true)
    for (int i = 1; i <= n; i++) {
        scanf("%d", &ty[i]);
    }
    // v: 把每隻狗變性的成本
    for (int i = 1; i <= n; i++) {
        scanf("%d", &v[i]);
    }


    int s = 0, t = n + m + 1;

    for (int i = 1; i <= n; i++) {
        if (!ty[i]) {
            // ty[i] == 0 female
            add_edge(s, i, v[i]);
        } else {
            // ty[i] == 1 male
            add_edge(i, t, v[i]);
        }
    }

    ans = 0;

    for (int i = 1, typ, w, k, fr; i <= m; i++) {
        scanf("%d %d %d", &typ, &w, &k);
        ans += w;
        for (int j = 1, id; j <= k; j++) {
            // j-th dog
            scanf("%d", &id);
            if (!typ) {
                // ty[i] == 0 female
                add_edge(i + n, id, INF);
            } else {
                // ty[i] == 1 male
                add_edge(id, i + n, INF);
            }
        }
        scanf("%d", &fr);
        if (!typ) {
            // 母狗
            add_edge(s, i + n, w + g * fr);
        } else {
            // 公狗
            add_edge(i + n, t, w + g * fr);
        }
    }

    printf("%d\n", ans - max_flow(s, t));
    return 0;
}