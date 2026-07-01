#ifndef DSU_H
#define DSU_H

#include <vector>

class DSU {
private:
    std::vector<int> parent;

public:
    DSU(int n) : parent(n) {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx != ry)
            parent[ry] = rx;
    }
};

#endif
