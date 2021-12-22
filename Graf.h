#ifndef GRAFURI_GRAF_H
#define GRAFURI_GRAF_H

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
#include "DisjointSet.h"

class Graf {
public:
    explicit Graf(int n = 0, int m = 0);


    // Citire
    void ponderatCitesteListaMuchii(ifstream &in);

    void neorientatCitesteListaMuchii(ifstream &in);

    void neorientatListaMuchiiToListaAdiacenta();

    void neorientatCitesteListaAdiacenta(ifstream &in);

    void orientatCitesteListaAdiacenta(ifstream &in);

    void orientatPonderatCitesteListaAdiacenta(ifstream &in);

    void orientatPonderatCitesteMatricePonderi(ifstream &in);

    void neorientatCitesteMultigrafContorizat(ifstream &in);

    pair<vector<vector<int>>, vector<vector<int>>> citesteInputFluxMaxim(ifstream &in);

    void citesteInputCicluHamiltonian(ifstream &in);


    // dfs - https://www.infoarena.ro/problema/dfs
    int nrCompConexe();

    // bfs - https://www.infoarena.ro/problema/bfs
    auto bfs(int start);

    // Havel-Hakimi
    static bool potiConstruiGraf(vector<int> grade);

    // Arbore partial de cost minim - https://www.infoarena.ro/problema/apm
    pair<int, vector<pair<int, int>>> apcm();

    // CTC - https://www.infoarena.ro/problema/ctc
    auto ctc();

    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    auto biconexe();

    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    auto muchiiCritice();

    // Diametru arbore - https://www.infoarena.ro/problema/darb
    int diametru();

    // Bellman-Ford - https://infoarena.ro/problema/bellmanford
    pair<bool, vector<int>> bellmanFord(int start);

    // Dijkstra - https://infoarena.ro/problema/dijkstra
    auto dijkstra(int start);

    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    auto royFloyd();

    // Flux maxim - https://infoarena.ro/problema/maxflow
    int fluxMaxim(vector<vector<int>> &capacitate, vector<vector<int>> &flux);

    // Ciclu hamiltonian de cost minim - https://www.infoarena.ro/problema/hamilton
    int hamiltonCostMinim();

    // Ciclu eulerian - https://www.infoarena.ro/problema/ciclueuler
    bool areCicluEulerian();

    auto cicluEulerian();

private:
    int m_n = 0, m_m = 0;

    // Reprezentari graf
    vector<vector<int>> m_listaAd;
    vector<vector<pair<int, int>>> m_ponderatListaAd;
    vector<vector<int>> m_ponderatMatrice;
    vector<vector<int>> m_listaMuchii;
    vector<pair<int, pair<int, int>>> m_listaMuchiiPonderat;
    vector<vector<pair<int, int>>> m_multigrafListaAd;
    vector<vector<pair<int, int>>> m_hamiltonListaAd;

    // Constante
    static constexpr int INF = ((1 << 30) - 1);
    // 2^30-1 ca sa nu fie overflow daca faci INF + INF

    // ---------------- Functii private ----------------
    // dfs - https://www.infoarena.ro/problema/dfs
    void nrCompConexeDfs(int k, vector<int> &viz);

    // CTC - https://www.infoarena.ro/problema/ctc
    void ctcDfs(
            int x, int &ultId, vector<int> &id, vector<int> &low, vector<bool> &peStiva,
            stack<int> &st, list<list<int>> &ctc
    );

    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    void biconexAdd(int x, int y, list<list<int>> &comps, stack<int> &st);

    void biconexDfs(
            int x, int prev, int id, list<list<int>> &comps, stack<int> &st,
            vector<int> &low
    );

    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    void muchiiCriticeDfs(
            int x, int prev, int id, map<pair<int, int>, bool> &toRemove,
            vector<vector<int>> &critice, vector<int> &low
    );

    // Diametru arbore - https://www.infoarena.ro/problema/darb
    void diametruDfs(int x, int dist, vector<bool> &viz, int &nodMax, int &distMax);

    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    void royFloydSetup(vector<vector<int>> &dists);

    // Flux maxim - https://infoarena.ro/problema/maxflow
    int fluxMaximBfs(
            int start, vector<vector<int>> &capacitate, vector<vector<int>> &flux,
            vector<int> &parinti, queue<int> &q
    );
};

#endif //GRAFURI_GRAF_H
