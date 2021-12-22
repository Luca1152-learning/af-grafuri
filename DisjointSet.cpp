#include "DisjointSet.h"

DisjointSet::DisjointSet(int n) {
    // Redimensioneaza
    m_parinte.resize(n + 1);
    m_dimensiune.resize(n + 1);

    for (int i = 1; i <= n; i++) {
        m_parinte[i] = i;
        m_dimensiune[i] = 1;
    }
}

int DisjointSet::cauta(int x) {
    while (x != m_parinte[x]) {
        x = m_parinte[x];
    }
    return x;
}

void DisjointSet::uneste(int x, int y) {
    int parinteX = cauta(x), parinteY = cauta(y);

    // Uneste arborele mai mic la arborele mai mare, pentru o complexitate mai buna
    if (m_dimensiune[parinteX] >= m_dimensiune[parinteY]) {
        m_parinte[parinteY] = parinteX;
        m_dimensiune[parinteX] += m_dimensiune[parinteY];
    } else {
        m_parinte[parinteX] = parinteY;
        m_dimensiune[parinteY] += m_dimensiune[parinteX];
    }
}
