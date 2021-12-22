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


    // dfs - https://www.infoarena.ro/problema/dfs
    void dfs(int k);

    int neorientatNrCompConexe();


    // bfs - https://www.infoarena.ro/problema/bfs
    const auto &bfs(int start);


    // Havel-Hakimi
    static bool potiConstruiGraf(vector<int> grade);


    // Arbore partial de cost minim - https://www.infoarena.ro/problema/apm
    void construiesteApcm();

    const auto &getApcmResult();

    auto getApcmCost();


    // CTC - https://www.infoarena.ro/problema/ctc
    const auto &orientatCtc();


    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    const auto &neorientatBiconexe();


    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    const auto &neorientatMuchiiCritice();


    // Diametru arbore - https://www.infoarena.ro/problema/darb
    int diametru();


    // Bellman-Ford - https://infoarena.ro/problema/bellmanford
    void orientatRuleazaBellmanFord(int start);

    bool circuitNegativBellmanFord();

    const auto &getBellmanFordDists();


    // Dijkstra - https://infoarena.ro/problema/dijkstra
    const auto &orientatRuleazaDijkstra(int start);


    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    void orientatRoyFloyd();

    const auto &orientatRoyFloydGetDists();


    // Flux maxim - https://infoarena.ro/problema/maxflow
    void citesteInputFluxMaxim(ifstream &in);

    int orientatFluxMaximBFS(int start = 1);

    int orientatFluxMaxim();


    // Ciclu hamiltonian de cost minim - https://www.infoarena.ro/problema/hamilton
    void citesteInputCicluHamiltonian(ifstream &in);

    int orientatCostMinimCicluHamiltonian();


    // Ciclu eulerian - https://www.infoarena.ro/problema/ciclueuler
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
    // dfs - https://www.infoarena.ro/problema/dfs
    void dfsRecursive(int k);


    // CTC - https://www.infoarena.ro/problema/ctc
    void orientatCtcDfs(int x);


    // Componente biconexe - https://www.infoarena.ro/problema/biconex
    void neorientatBiconexAdd(int x, int y);

    void neorientatBiconexDfs(int x, int prev, int id);


    // Muchii critice - https://leetcode.com/problems/critical-connections-in-a-network/
    void neorientatMuchiiCriticeDfs(int x, int prev, int id);


    // Diametru arbore - https://www.infoarena.ro/problema/darb
    void diametruDFS(int x, int dist);


    // RoyFloyd - https://www.infoarena.ro/problema/royfloyd
    void orientatRoyFloydSetup();
};

#endif //GRAFURI_GRAF_H
