#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <climits>
#include <cstring>

#define INF ((1 << 30) - 1)
// 2^30-1 ca sa nu fie overflow daca faci INF + INF

const int nMax = 100005;

using namespace std;

class DisjointSet {
private:
    int m_parinte[nMax] = {}, m_dimensiune[nMax] = {};

public:
    explicit DisjointSet(int n) {
        for (int i = 1; i <= n; i++) {
            m_parinte[i] = i;
            m_dimensiune[i] = 1;
        }
    }

    int cauta(int x) {
        while (x != m_parinte[x]) {
            x = m_parinte[x];
        }
        return x;
    }

    void uneste(int x, int y) {
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
};

class Graf {
private:
    int m_n, m_m;
    vector<int> m_listAd[nMax];
    vector<pair<int, int>> m_ponderatListaAd[nMax];
    int m_ponderatMatrice[105][105] = {};
    vector<vector<int>> m_listaMuchii;
    vector<pair<int, pair<int, int>>> m_listaMuchiiPonderat;

    // DFS - https://www.infoarena.ro/problema/dfs
    bool m_dfsViz[nMax] = {};

    // BFS - https://www.infoarena.ro/problema/bfs
    int m_bfsDist[nMax] = {};
    queue<int> m_bfsQueue;

    // CTC - https://www.infoarena.ro/problema/ctc
    int m_ctcId[nMax] = {}, m_ctcLow[nMax] = {}, m_ctcUltId = 0;
    bool m_ctcPeStiva[nMax] = {};
    list<list<int>> m_ctc;
    stack<int> m_ctcStack;

    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    list<list<int>> m_biconexComps;
    stack<int> m_biconexStack;
    int m_biconexLow[nMax] = {};

    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    map<pair<int, int>, bool> m_criticeToRemove;
    vector<vector<int>> m_critice;
    int m_criticeLow[nMax] = {}; // Id-ul nodului minim in care te poti intoarce din nodul i

    // Arbore partial de cost minim - https://www.infoarena.ro/problema/apm
    int m_apcmCost = 0;
    vector<pair<int, int>> m_apcmResult;

    // Bellman-Ford - https://infoarena.ro/problema/bellmanford
    vector<int> m_bellmanDist = vector<int>(nMax, INT_MAX);
    int m_bellmanPuneriInCoada[nMax] = {}, m_bellmanInQueue[nMax] = {};
    queue<int> m_bellmanQueue;
    bool m_bellmanCircuitCostNegativ = false;

    // Dijkstra - https://infoarena.ro/problema/dijkstra
    vector<int> m_dijkstraDist = vector<int>(nMax, INT_MAX);
    set<pair<int, int>> m_dijkstraMinSet; // "min".. doar e un ordered set (crescator)

    // Diametru arbore - https://www.infoarena.ro/problema/darb
    bool m_diametruViz[nMax] = {};
    int m_diametruNodMax = 0, m_diametruDistMax = 0;

    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    int m_royFloydDists[105][105] = {};


    // ---------------- Functii private ----------------
    void orientatCtcDFS(int x) {
        m_ctcStack.push(x);
        m_ctcPeStiva[x] = true;
        m_ctcId[x] = m_ctcLow[x] = ++m_ctcUltId;

        for (auto y: m_listAd[x]) {
            // Nu am explorat nodul pana acum (neavand vreun id)
            if (m_ctcId[y] == 0) {
                orientatCtcDFS(y);
            }

            // Am intalnit un nod care inca nu a fost atribuit unei componente conexe.
            // Poate nodul curent face parte din viitoarea componenta conexa, a carei (posibila) sursa
            // a fost gasita de y.
            if (m_ctcPeStiva[y]) {
                m_ctcLow[x] = min(m_ctcLow[x], m_ctcLow[y]);
            }
        }

        // Am ajuns la nodul de start al ctc-ului explorat in prezent
        if (m_ctcId[x] == m_ctcLow[x]) {
            list<int> compCurr;
            while (true) {
                auto y = m_ctcStack.top();
                m_ctcStack.pop();

                m_ctcPeStiva[y] = false;
                compCurr.push_back(y);

                if (y == x) break;
            }
            m_ctc.push_back(compCurr);
        }
    }

    void neorientatBiconexAdd(int x, int y) {
        // Creeaza o noua componenta pentru afisare
        list<int> comp;

        // Adauga in componenta toate nodurile pana la y, inclusiv y
        while (m_biconexStack.top() != y) {
            comp.push_back(m_biconexStack.top());
            m_biconexStack.pop();
        }
        comp.push_back(y);
        m_biconexStack.pop();

        // Adauga in componenta si pe x, separat (in caz ca e un gap in stack intre y si x)
        // ^ gap-ul poate aparea daca intalnim mai multe componente biconexe ce se intorc in acelasi nod
        comp.push_back(x);

        m_biconexComps.push_back(comp);
    }

    void neorientatBiconexDfs(int x, int prev, int id) {
        // Initializam low-ul (nodul cel mai de sus din parcurgerea DFS in care putem ajunge)
        // si punem nodul curent pe stack
        m_biconexLow[x] = id;
        m_biconexStack.push(x);

        for (auto y: m_listAd[x]) {
            // Ignoram cazul in care ne intoarcem din nodul in care am plecat
            if (y == prev) continue;

            // Nodul y nu a fost vizitat => viziteaza-l
            if (!m_biconexLow[y]) {
                // Viziteaza-l si actualizeaza low
                neorientatBiconexDfs(y, x, id + 1);
                m_biconexLow[x] = min(m_biconexLow[x], m_biconexLow[y]);

                // Am ajuns la originea ciclului / am dat peste un nod de mai jos din parcurgerea
                // DFS la care nu mai putem ajunge altfel (=> componenta biconexa)
                if (m_biconexLow[y] >= id) {
                    neorientatBiconexAdd(x, y);
                }
            }
                // Nodul y a fost vizitat => doar actualizeaza min-ul in caz ca e nevoie,
                // fara sa risti sa afisezi o componenta biconexa de doua ori
            else {
                m_biconexLow[x] = min(m_biconexLow[x], m_biconexLow[y]);
            }
        }
    }

    void neorientatMuchiiCriticeDfs(int x, int prev, int id) {
        m_criticeLow[x] = id;

        for (auto y: m_listAd[x]) {
            // Nu te intoarce in nodul din care ai plecat
            if (y == prev) continue;

            // Ruleaza DFS in continuare, cu un id mai mare
            if (m_criticeLow[y] == 0) neorientatMuchiiCriticeDfs(y, x, id + 1);

            // Nodul vizitat din cel curent face parte dintr-un ciclu,
            // asa ca trebuie sa excludem muchia x-y
            if (m_criticeLow[y] < id + 1) {
                m_criticeToRemove[{x, y}] = m_criticeToRemove[{y, x}] = true;
            }

            // Actualizeaza low-ul nodului curent
            m_criticeLow[x] = min(m_criticeLow[x], m_criticeLow[y]);
        }
    }

    void diametruDFS(int x, int dist) {
        if (dist > m_diametruDistMax) {
            m_diametruDistMax = dist;
            m_diametruNodMax = x;
        }
        m_diametruViz[x] = 1;
        for (auto &y: m_listAd[x]) {
            if (!m_diametruViz[y]) {
                diametruDFS(y, dist + 1);
            }
        }
    }

    void orientatRoyFloydSetup() {
        for (int i = 1; i <= m_n; i++) {
            for (int j = 1; j <= m_n; j++) {
                if (i == j) {
                    m_royFloydDists[i][j] = 0;
                } else if (m_ponderatMatrice[i][j] == 0) {
                    m_royFloydDists[i][j] = INF;
                } else {
                    m_royFloydDists[i][j] = m_ponderatMatrice[i][j];
                }
            }
        }
    }

public:
    // ---------------- Interfata publica ----------------
    explicit Graf(int n = 0, int m = 0) : m_n(n), m_m(m) {}


    /*************** Algoritmi generali ***************/
    void ponderatCitesteListaMuchii(ifstream &in) {
        for (int i = 0; i < m_m; i++) {
            int x, y, c;
            in >> x >> y >> c;
            m_listaMuchiiPonderat.push_back({c, {x, y}});
        }
    }

    void DFS(int k) {
        m_dfsViz[k] = true;

        for (auto x: m_listAd[k]) {
            if (!m_dfsViz[x]) {
                DFS(x);
            }
        }
    }

    const auto &BFS(int start) {
        m_bfsQueue.push(start);
        m_bfsDist[start] = 1;

        while (!m_bfsQueue.empty()) {
            int curr = m_bfsQueue.front();
            m_bfsQueue.pop();

            for (auto i: m_listAd[curr]) {
                if (m_bfsDist[i] == 0) {
                    m_bfsDist[i] = m_bfsDist[curr] + 1;
                    m_bfsQueue.push(i);
                }
            }
        }
        return m_bfsDist;
    }

    static bool potiConstruiGraf(vector<int> grade) {
        // Algoritmul Havel-Hakimi
        while (true) {
            sort(grade.begin(), grade.end(), greater<int>());

            if (grade[0] == 0) break;
            if (grade[0] > grade.size() - 1) return false;

            int maxVal = grade[0];
            for (int i = 1; i <= maxVal; i++) {
                grade[0]--;
                grade[i]--;
                if (grade[i] < 0) return false;
            }
        }

        return true;
    }

    void construiesteApcm() {
        // Algoritmul lui Kruskal: luam muchiile cu cel mai mic cost, cat timp nu
        // se creeaza cicluri.
        sort(m_listaMuchiiPonderat.begin(), m_listaMuchiiPonderat.end());

        // Foloseste un disjoint set pentru a sti daca muchia de la x la y va crea
        // un ciclu (valoarea fiecarui nod reprezentand componenta conexa din care
        // face parte).
        DisjointSet ds(m_n);

        for (auto &m: m_listaMuchiiPonderat) {
            int c = m.first, x = m.second.first, y = m.second.second;
            if (ds.cauta(x) != ds.cauta(y)) {
                ds.uneste(x, y);

                // Adauga la rezultat muchia gasita
                m_apcmCost += c;
                m_apcmResult.push_back({x, y});
            }
        }
    }

    const auto &getApcmResult() {
        return m_apcmResult;
    }

    auto getApcmCost() {
        return m_apcmCost;
    }

    /*************** Grafuri neorientate ***************/
    void neorientatCitesteListaMuchii(ifstream &in) {
        for (int i = 0; i < m_m; i++) {
            int x, y;
            in >> x >> y;
            m_listaMuchii.push_back({x, y});
        }
    }

    void neorientatListaMuchiiToListaAdiacenta() {
        for (auto &e: m_listaMuchii) {
            m_listAd[e[0]].push_back(e[1]);
            m_listAd[e[1]].push_back(e[0]);
        }
    }

    void neorientatCitesteListaAdiacenta(ifstream &in) {
        for (int i = 0; i < m_m; i++) {
            int x, y;
            in >> x >> y;
            m_listAd[x].push_back(y);
            m_listAd[y].push_back(x);
        }
    }

    int neorientatNrCompConexe() {
        int nrComp = 0;
        for (int i = 1; i <= m_n; i++) {
            if (!m_dfsViz[i]) {
                nrComp++;
                DFS(i);
            }
        }
        return nrComp;
    }

    const auto &neorientatBiconexe() {
        for (int i = 1; i <= m_n; i++) {
            if (!m_biconexLow[i]) {
                neorientatBiconexDfs(i, -1, 1);
            }
        }
        return m_biconexComps;
    }

    const auto &neorientatMuchiiCritice() {
        neorientatMuchiiCriticeDfs(0, -1, 1);

        // In rezultat, punem muchiile ce nu au fost marcate ca trebuind sa fie sterse
        for (auto &e: m_listaMuchii) {
            if (!m_criticeToRemove[{e[0], e[1]}]) {
                m_critice.push_back(e);
            }
        }

        return m_critice;
    }

    int diametru() {
        diametruDFS(1, 1);
        m_diametruDistMax = 0;
        memset(m_diametruViz, 0, sizeof(m_diametruViz));
        diametruDFS(m_diametruNodMax, 1);

        return m_diametruDistMax;
    }


    /*************** Grafuri orientate ***************/
    void orientatCitesteListaAdiacenta(ifstream &in) {
        for (int i = 0; i < m_m; i++) {
            int x, y;
            in >> x >> y;
            m_listAd[x].push_back(y);
        }
    }

    void orientatPonderatCitesteListaAdiacenta(ifstream &in) {
        for (int i = 0; i < m_m; i++) {
            int x, y, c;
            in >> x >> y >> c;
            m_ponderatListaAd[x].push_back({y, c});
        }
    }

    void orientatPonderatCitesteMatricePonderi(ifstream &in) {
        for (int i = 1; i <= m_n; i++) {
            for (int j = 1; j <= m_n; j++) {
                int p;
                in >> p;
                m_ponderatMatrice[i][j] = p;
            }
        }
    }

    void orientatRoyFloyd() {
        orientatRoyFloydSetup();

        // Nodul k = nodul pe care incercam sa il integram in drumul de la i la j
        for (int k = 1; k <= m_n; k++) {
            // Verificam toate drumurile daca pot fi scurtate folosind k drept nod intermediar
            // (i -> ... -> k -> ... -> j)
            for (int i = 1; i <= m_n; i++) {
                for (int j = 1; j <= m_n; j++) {
                    if (m_royFloydDists[i][k] + m_royFloydDists[k][j] < m_royFloydDists[i][j]) {
                        m_royFloydDists[i][j] = m_royFloydDists[i][k] + m_royFloydDists[k][j];
                    }
                }
            }
        }
    }

    const auto &orientatRoyFloydGetDists() {
        return m_royFloydDists;
    }
};

int main() {
    // Input rapid
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // I/O
    ifstream in("royfloyd.in");
    ofstream out("royfloyd.out");

    int n;
    in >> n;

    Graf g(n, 0);
    g.orientatPonderatCitesteMatricePonderi(in);
    in.close();

    g.orientatRoyFloyd();

    // TODO
    const auto &dists = g.orientatRoyFloydGetDists();
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            out << dists[i][j] << " ";
        }
        out << "\n";
    }

    out.close();
    return 0;
}