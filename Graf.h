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

    // dfs - https://www.infoarena.ro/problema/dfs
    void dfs(int k);

    // bfs - https://www.infoarena.ro/problema/bfs
    const auto &bfs(int start);

    static bool potiConstruiGraf(vector<int> grade);

    void construiesteApcm();

    const auto &getApcmResult();

    auto getApcmCost();

    void neorientatCitesteListaMuchii(ifstream &in);

    void neorientatListaMuchiiToListaAdiacenta();

    void neorientatCitesteListaAdiacenta(ifstream &in);

    int neorientatNrCompConexe();

    const auto &neorientatBiconexe();

    const auto &neorientatMuchiiCritice();

    int diametru();


    void orientatCitesteListaAdiacenta(ifstream &in);

    void orientatPonderatCitesteListaAdiacenta(ifstream &in);

    void orientatPonderatCitesteMatricePonderi(ifstream &in);

    const auto &orientatCtc();

    void orientatRuleazaBellmanFord(int start);

    bool circuitNegativBellmanFord();

    const auto &getBellmanFordDists();

    const auto &orientatRuleazaDijkstra(int start);

    void orientatRoyFloyd();

    const auto &orientatRoyFloydGetDists();

    void citesteInputFluxMaxim(ifstream &in);

    int orientatFluxMaximBFS(int start = 1);

    int orientatFluxMaxim();

    void citesteInputCicluHamiltonian(ifstream &in);

    int orientatCostMinimCicluHamiltonian();

    void neorientatCitesteMultigrafContorizat(ifstream &in);

    bool eCicluEuler();

    const auto &gasesteCicluEuler();

private:
    int m_n = 0, m_m = 0;

    // Reprezentari graf
    vector<vector<int>> m_listaAd;
    vector<vector<pair<int, int>>> m_ponderatListaAd;
    vector<vector<int>> m_ponderatMatrice;
    vector<vector<int>> m_listaMuchii;
    vector<pair<int, pair<int, int>>> m_listaMuchiiPonderat;

    // Constante
    static const int INF = ((1 << 30) - 1);
    // 2^30-1 ca sa nu fie overflow daca faci INF + INF

    // dfs - https://www.infoarena.ro/problema/dfs
    vector<bool> m_dfsViz;

    // bfs - https://www.infoarena.ro/problema/bfs
    vector<int> m_bfsDist;
    queue<int> m_bfsQueue;

    // CTC - https://www.infoarena.ro/problema/ctc
    static constexpr int ctcMax = 1;
    int m_ctcId[ctcMax] = {}, m_ctcLow[ctcMax] = {}, m_ctcUltId = 0;
    bool m_ctcPeStiva[ctcMax] = {};
    list<list<int>> m_ctc;
    stack<int> m_ctcStack;

    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    static constexpr int biconexMax = 1;
    list<list<int>> m_biconexComps;
    stack<int> m_biconexStack;
    int m_biconexLow[biconexMax] = {};

    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    static constexpr int criticeMax = 1;
    map<pair<int, int>, bool> m_criticeToRemove;
    vector<vector<int>> m_critice;
    int m_criticeLow[criticeMax] = {}; // Id-ul nodului minim in care te poti intoarce din nodul i

    // Arbore partial de cost minim - https://www.infoarena.ro/problema/apm
    int m_apcmCost = 0;
    vector<pair<int, int>> m_apcmResult;

    // Bellman-Ford - https://infoarena.ro/problema/bellmanford
    static constexpr int bellmanMax = 1;
    vector<int> m_bellmanDist = vector<int>(bellmanMax, INT_MAX);
    int m_bellmanPuneriInCoada[bellmanMax] = {}, m_bellmanInQueue[bellmanMax] = {};
    queue<int> m_bellmanQueue;
    bool m_bellmanCircuitCostNegativ = false;

    // Dijkstra - https://infoarena.ro/problema/dijkstra
    static constexpr int dijkstraMax = 1;
    vector<int> m_dijkstraDist = vector<int>(dijkstraMax, INT_MAX);
    set<pair<int, int>> m_dijkstraMinHeap; // "min".. doar e un ordered set (crescator)

    // Diametru arbore - https://www.infoarena.ro/problema/darb
    static constexpr int diametruVizMax = 1;
    bool m_diametruViz[diametruVizMax] = {};
    int m_diametruNodMax = 0, m_diametruDistMax = 0;

    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    static constexpr int royFloydMax = 1;
    int m_royFloydDists[royFloydMax][royFloydMax] = {};

    // Flux maxim - https://infoarena.ro/problema/maxflow
    static constexpr int fluxMaximMax = 1;
    int m_fluxMaximCapacitate[fluxMaximMax][fluxMaximMax] = {}, m_fluxMaximFlux[fluxMaximMax][fluxMaximMax] = {},
            m_fluxMaximParinti[fluxMaximMax] = {};
    queue<int> m_fluxMaximQueue;

    // Ciclu hamiltonian de cost minim - https://www.infoarena.ro/problema/hamilton
    static const int hamiltonMinimMax = 1;
    vector<pair<int, int>> m_hamiltonListaAd[hamiltonMinimMax];
    int m_hamiltonMinimDP[1 << hamiltonMinimMax][hamiltonMinimMax] = {};

    // Ciclu eulerian - https://www.infoarena.ro/problema/ciclueuler
    vector<pair<int, int>> m_multigrafListaAd[100005];
    vector<int> m_cicluEulerSol, m_cicluEulerStack;
    int m_cicluEulerGrade[100005] = {};
    bool m_cicluEulerFolMuchie[500005] = {};


    // ---------------- Functii private ----------------
    void dfsRecursive(int k);

    void orientatCtcDFS(int x);

    void neorientatBiconexAdd(int x, int y);

    void neorientatBiconexDfs(int x, int prev, int id);

    void neorientatMuchiiCriticeDfs(int x, int prev, int id);

    void diametruDFS(int x, int dist);

    void orientatRoyFloydSetup();
};

#endif //GRAFURI_GRAF_H
