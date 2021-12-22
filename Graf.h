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
    static constexpr int INF = ((1 << 30) - 1);
    // 2^30-1 ca sa nu fie overflow daca faci INF + INF

    // dfs - https://www.infoarena.ro/problema/dfs
    vector<bool> m_dfsViz;

    // bfs - https://www.infoarena.ro/problema/bfs
    vector<int> m_bfsDist;
    queue<int> m_bfsQueue;

    // CTC - https://www.infoarena.ro/problema/ctc
    vector<int> m_ctcId, m_ctcLow;
    int m_ctcUltId = 0;
    vector<bool> m_ctcPeStiva;
    list<list<int>> m_ctc;
    stack<int> m_ctcStack;

    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    list<list<int>> m_biconexComps;
    stack<int> m_biconexStack;
    vector<int> m_biconexLow;

    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    map<pair<int, int>, bool> m_criticeToRemove;
    vector<vector<int>> m_critice;
    vector<int> m_criticeLow; // Id-ul nodului minim in care te poti intoarce din nodul i

    // Arbore partial de cost minim - https://www.infoarena.ro/problema/apm
    int m_apcmCost = 0;
    vector<pair<int, int>> m_apcmResult;

    // Bellman-Ford - https://infoarena.ro/problema/bellmanford
    vector<int> m_bellmanDist, m_bellmanPuneriInCoada, m_bellmanInQueue;
    queue<int> m_bellmanQueue;
    bool m_bellmanCircuitCostNegativ = false;

    // Dijkstra - https://infoarena.ro/problema/dijkstra
    vector<int> m_dijkstraDist;
    set<pair<int, int>> m_dijkstraMinHeap; // "min".. doar e un ordered set (crescator)

    // Diametru arbore - https://www.infoarena.ro/problema/darb
    vector<bool> m_diametruViz;
    int m_diametruNodMax = 0, m_diametruDistMax = 0;

    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    vector<vector<int>> m_royFloydDists;

    // Flux maxim - https://infoarena.ro/problema/maxflow
    vector<vector<int>> m_fluxMaximCapacitate, m_fluxMaximFlux;
    vector<int> m_fluxMaximParinti;
    queue<int> m_fluxMaximQueue;

    // Ciclu hamiltonian de cost minim - https://www.infoarena.ro/problema/hamilton
    vector<vector<pair<int, int>>> m_hamiltonListaAd;
    vector<vector<int>> m_hamiltonMinimDP;

    // Ciclu eulerian - https://www.infoarena.ro/problema/ciclueuler
    vector<vector<pair<int, int>>> m_multigrafListaAd;
    vector<int> m_cicluEulerSol, m_cicluEulerStack;
    vector<int> m_cicluEulerGrade;
    vector<bool> m_cicluEulerFolMuchie;


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
