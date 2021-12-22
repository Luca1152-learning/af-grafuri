#ifndef GRAFURI_DISJOINTSET_H
#define GRAFURI_DISJOINTSET_H

#include <vector>

using namespace std;

class DisjointSet {
public:
    explicit DisjointSet(int n);

    int cauta(int x);

    void uneste(int x, int y);

private:
    vector<int> m_parinte, m_dimensiune;
};

#endif //GRAFURI_DISJOINTSET_H
