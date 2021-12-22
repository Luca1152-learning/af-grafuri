#include "Graf.h"

Graf::Graf(int n, int m) : m_n(n), m_m(m) {}

void Graf::ponderatCitesteListaMuchii(ifstream &in) {
    for (int i = 0; i < m_m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        m_listaMuchiiPonderat.push_back({c, {x, y}});
    }
}

void Graf::dfs(int k) {
    // Redimensioneaza
    m_dfsViz.resize(m_n + 1, 0);

    dfsRecursive(k);
}


const auto &Graf::bfs(int start) {
    // Redimensioneaza
    m_bfsDist.resize(m_n + 1, 0);

    m_bfsQueue.push(start);
    m_bfsDist[start] = 1;

    while (!m_bfsQueue.empty()) {
        int curr = m_bfsQueue.front();
        m_bfsQueue.pop();

        for (auto i: m_listaAd[curr]) {
            if (m_bfsDist[i] == 0) {
                m_bfsDist[i] = m_bfsDist[curr] + 1;
                m_bfsQueue.push(i);
            }
        }
    }
    return m_bfsDist;
}

bool Graf::potiConstruiGraf(vector<int> grade) {
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

void Graf::construiesteApcm() {
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

const auto &Graf::getApcmResult() {
    return m_apcmResult;
}

auto Graf::getApcmCost() {
    return m_apcmCost;
}

void Graf::neorientatCitesteListaMuchii(ifstream &in) {
    for (int i = 0; i < m_m; i++) {
        int x, y;
        in >> x >> y;
        m_listaMuchii.push_back({x, y});
    }
}

void Graf::neorientatListaMuchiiToListaAdiacenta() {
    for (auto &e: m_listaMuchii) {
        m_listaAd[e[0]].push_back(e[1]);
        m_listaAd[e[1]].push_back(e[0]);
    }
}

void Graf::neorientatCitesteListaAdiacenta(ifstream &in) {
    for (int i = 0; i < m_m; i++) {
        int x, y;
        in >> x >> y;
        m_listaAd[x].push_back(y);
        m_listaAd[y].push_back(x);
    }
}

int Graf::neorientatNrCompConexe() {
    int nrComp = 0;
    for (int i = 1; i <= m_n; i++) {
        if (!m_dfsViz[i]) {
            nrComp++;
            dfsRecursive(i);
        }
    }
    return nrComp;
}

const auto &Graf::neorientatBiconexe() {
    // Redimensioneaza
    m_biconexLow.resize(m_n + 1, 0);

    for (int i = 1; i <= m_n; i++) {
        if (!m_biconexLow[i]) {
            neorientatBiconexDfs(i, -1, 1);
        }
    }
    return m_biconexComps;
}

const auto &Graf::neorientatMuchiiCritice() {
    // Redimensioneaza
    m_criticeLow.resize(m_n + 1, 0);

    neorientatMuchiiCriticeDfs(0, -1, 1);

    // In rezultat, punem muchiile ce nu au fost marcate ca trebuind sa fie sterse
    for (auto &e: m_listaMuchii) {
        if (!m_criticeToRemove[{e[0], e[1]}]) {
            m_critice.push_back(e);
        }
    }

    return m_critice;
}

int Graf::diametru() {
    // Redimensioneaza
    m_diametruViz.resize(m_n + 1, false);

    diametruDFS(1, 1);
    m_diametruDistMax = 0;
    fill(m_diametruViz.begin(), m_diametruViz.end(), 0);
    diametruDFS(m_diametruNodMax, 1);

    return m_diametruDistMax;
}

void Graf::orientatCitesteListaAdiacenta(ifstream &in) {
    // Redimensioneaza
    m_listaAd.clear();
    m_listaAd.resize(m_n + 1);

    for (int i = 0; i < m_m; i++) {
        int x, y;
        in >> x >> y;
        m_listaAd[x].push_back(y);
    }
}

void Graf::orientatPonderatCitesteListaAdiacenta(ifstream &in) {
    // Redimensioneaza
    m_ponderatListaAd.clear();
    m_ponderatListaAd.resize(m_n + 1);

    for (int i = 0; i < m_m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        m_ponderatListaAd[x].push_back({y, c});
    }
}

void Graf::orientatPonderatCitesteMatricePonderi(ifstream &in) {
    // Redimensioneaza
    m_ponderatMatrice.clear();
    m_ponderatMatrice.resize(m_n + 1);
    for (int i = 1; i <= m_n; i++) {
        m_ponderatMatrice[i].resize(m_n + 1);
    }

    for (int i = 1; i <= m_n; i++) {
        for (int j = 1; j <= m_n; j++) {
            int p;
            in >> p;
            m_ponderatMatrice[i][j] = p;
        }
    }
}

const auto &Graf::orientatCtc() {
    // Redimensioneaza
    m_ctcId.resize(m_n + 1, 0);
    m_ctcLow.resize(m_n + 1, 0);
    m_ctcPeStiva.resize(m_n + 1, false);

    // Algoritmul lui Tarjan
    for (int i = 1; i <= m_n; i++) {
        // Nu am explorat nodul pana acum (neavand vreun id)
        if (m_ctcId[i] == 0) {
            orientatCtcDfs(i);
        }
    }
    return m_ctc;
}

void Graf::orientatRuleazaBellmanFord(int start) {
    // Redimensioneaza
    m_bellmanDist.resize(m_n + 1, INF);
    m_bellmanPuneriInCoada.resize(m_n + 1, 0);
    m_bellmanInQueue.resize(m_n + 1, 0);

    // Gaseste graful de costuri minime, plecand din start la celelalte n-1 noduri.
    // Putem avea circuit de cost negativ -> va fi detectat.

    // Incepem cu optimizarile plecand din nodul de start
    m_bellmanQueue.push(start);
    m_bellmanDist[start] = 0;
    m_bellmanInQueue[start] = true;
    m_bellmanPuneriInCoada[start] = 1;

    // Ne oprim cand nu mai avem nimic de optimizat / am gasit un circuit cu cost negativ
    while (!m_bellmanQueue.empty() && !m_bellmanCircuitCostNegativ) {
        int x = m_bellmanQueue.front();
        m_bellmanQueue.pop();

        // Marcam nodul curent ca ne mai fiind in queue
        m_bellmanInQueue[x] = false;

        // Luam toate arcele la rand si incercam sa optimizam distante, folosindu-le
        for (auto &e: m_ponderatListaAd[x]) {
            int y = e.first, c = e.second;

            // Am gasit un arc (de la x la y) ce optimizeaza costul lui y (= obtinem
            // o distanta mai mica din start->y daca mergem prin x)
            if (m_bellmanDist[y] > m_bellmanDist[x] + c) {
                m_bellmanDist[y] = m_bellmanDist[x] + c;

                // Daca y nu e deja in coada, pune-l (facem verificarea ca sa nu
                // il adaugam de mai multe ori in coada), pentru ca, optimizand
                // distanta pana la el, putem optimiza distante si plecand din el.
                if (!m_bellmanInQueue[y]) {
                    m_bellmanQueue.push(y);
                    m_bellmanInQueue[y] = true;

                    // Numara de cate ori au fost puse in coada nodurile. Daca un nod
                    // a fost pus de >= n ori (=> n optimizari), inseamna ca am gasit
                    // un circuit de cost negativ.
                    m_bellmanPuneriInCoada[y]++;
                    if (m_bellmanPuneriInCoada[y] >= m_n) {
                        m_bellmanCircuitCostNegativ = true;
                    }
                }
            }
        }
    }
}

bool Graf::circuitNegativBellmanFord() {
    return m_bellmanCircuitCostNegativ;
}

const auto &Graf::getBellmanFordDists() {
    return m_bellmanDist;
}

const auto &Graf::orientatRuleazaDijkstra(int start) {
    // Redimensioneaza
    m_dijkstraDist.resize(m_n + 1, INF);

    // Incepem algoritmul din nodul de start
    m_dijkstraDist[start] = 0;
    // Punem in set perechea {0, [nod start]}, 0 fiind distanta de la start pana la el insusi
    m_dijkstraMinHeap.insert({0, start});

    while (!m_dijkstraMinHeap.empty()) {
        // Procesam nodul de la distanta cea mai mica fata de start
        auto x = m_dijkstraMinHeap.begin()->second;
        m_dijkstraMinHeap.erase(m_dijkstraMinHeap.begin());

        for (auto &e: m_ponderatListaAd[x]) {
            auto y = e.first, c = e.second;

            // Obtinem un drum mai scurt (fata de cel gasit) daca trecem prin x
            if (m_dijkstraDist[y] > m_dijkstraDist[x] + c) {
                // Nodul y e deja marcat ca trebuind sa fie procesat => il scoatem din set, ca sa il readaugam
                // cu noua distanta, mai mica (ca sa nu pierdem timp incercand sa-l optimizam cu distanta veche
                // mai tarziu) -- 90p->100p
                if (m_dijkstraMinHeap.count({m_dijkstraDist[y], y}) > 0) {
                    m_dijkstraMinHeap.erase(m_dijkstraMinHeap.find({m_dijkstraDist[y], y}));
                }

                // Actualizam distanta lui y si il punem la procesat
                m_dijkstraDist[y] = m_dijkstraDist[x] + c;
                m_dijkstraMinHeap.insert({m_dijkstraDist[y], y});
            }
        }
    }

    return m_dijkstraDist;
}

void Graf::orientatRoyFloyd() {
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

const auto &Graf::orientatRoyFloydGetDists() {
    return m_royFloydDists;
}

void Graf::citesteInputFluxMaxim(ifstream &in) {
    // Redimensioneaza
    m_listaAd.clear();
    m_listaAd.resize(m_n + 1);
    m_fluxMaximCapacitate.clear();
    m_fluxMaximCapacitate.resize(m_n + 1);

    for (int i = 0; i < m_m; i++) {
        // Redimensioneaza
        m_fluxMaximCapacitate[i].resize(m_n + 1);

        int x, y, c;
        in >> x >> y >> c;
        m_fluxMaximCapacitate[x][y] = c;
        m_listaAd[x].push_back(y);
        m_listaAd[y].push_back(x);
    }
}

int Graf::orientatFluxMaximBFS(int start) {
    std::fill(m_fluxMaximParinti.begin(), m_fluxMaximParinti.end(), 0);

    m_fluxMaximQueue.push(start);
    m_fluxMaximParinti[start] = -1;

    while (!m_fluxMaximQueue.empty()) {
        int x = m_fluxMaximQueue.front();
        m_fluxMaximQueue.pop();

        // Din moment ce am facut graful unul neorientat (pentru a avea acces la vecinii nodului destinatie),
        // am putea sa ne intoarcem din nodul final in alte noduri inca nevizitate, dar nu vrem asta
        if (x == m_n) {
            continue;
        }

        for (auto &y: m_listaAd[x]) {
            // Daca nu am vizitat nodul y in bfs-ul curent si daca inca mai putem pompa
            // flux prin lantul x-y, viziteaza-l
            if (!m_fluxMaximParinti[y] && m_fluxMaximCapacitate[x][y] - m_fluxMaximFlux[x][y] > 0) {
                m_fluxMaximQueue.push(y);
                m_fluxMaximParinti[y] = x;
            }
        }
    }

    // Returnam daca am ajuns la destinatie cu parcurgerea curenta
    return m_fluxMaximParinti[m_n];
}

int Graf::orientatFluxMaxim() {
    // Redimensioneaza
    m_fluxMaximParinti.resize(m_n + 1, 0);

    // Algoritmul Edmonds-Karp

    int fluxMaximTotal = 0;

    while (orientatFluxMaximBFS()) {
        // Am creat arborele bfs -> ne intoarcem pe fiecare ruta posibila, folosindu-ne de vecinii
        // nodului final
        for (auto &x: m_listaAd[m_n]) {
            if (!m_fluxMaximParinti[x] || m_fluxMaximCapacitate[x][m_n] - m_fluxMaximFlux[x][m_n] == 0) {
                continue;
            }

            m_fluxMaximParinti[m_n] = x;

            // Gaseste fluxul minim ce poate fi pompat pe lantul gasit de bfs
            int fluxMinimLant = INF, curr = m_n;
            while (m_fluxMaximParinti[curr] != -1) {
                int prev = m_fluxMaximParinti[curr];
                fluxMinimLant = min(fluxMinimLant, m_fluxMaximCapacitate[prev][curr] - m_fluxMaximFlux[prev][curr]);
                curr = prev;
            }
            if (fluxMinimLant == 0) {
                continue;
            }

            // Actualizeaza fluxurile
            curr = m_n;
            while (m_fluxMaximParinti[curr] != -1) {
                int prev = m_fluxMaximParinti[curr];
                m_fluxMaximFlux[prev][curr] += fluxMinimLant;
                m_fluxMaximFlux[curr][prev] -= fluxMinimLant;
                curr = prev;
            }

            fluxMaximTotal += fluxMinimLant;
        }
    }

    return fluxMaximTotal;
}

void Graf::citesteInputCicluHamiltonian(ifstream &in) {
    // Redimensioneaza
    m_hamiltonListaAd.clear();
    m_hamiltonListaAd.resize(m_n + 1);

    // Citire aproape identica cu una standard prin lista de adiacenta, numai ca in loc sa spuna
    // vecinii ce pleaca din x, spune vecinii ce ajung in x (asta ne intereseaza in algoritm)
    for (int i = 0; i < m_m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        m_hamiltonListaAd[y].push_back({x, c});
        //                ^ y in loc de x-ul standard
    }
}

int Graf::orientatCostMinimCicluHamiltonian() {
    // Redimensioneaza
    m_hamiltonMinimDP.clear();
    m_hamiltonMinimDP.resize(1 << m_n);

    // Initializam costurile minime cu INFINIT
    for (int k = 0; k < 1 << m_n; k++) {
        // Redimensioneaza
        m_hamiltonMinimDP[k].resize(m_n + 1, INF);
    }

    // Costul minim pe un lant format din nodul 0 (bitul 0 e setat => 1, prima coordonata) si se termina
    // in 0 (a doua coordonata) este 0 -- de aici plecam cu calculul costurilor
    m_hamiltonMinimDP[1][0] = 0;

    for (int k = 0; k < 1 << m_n; k++) {
        // ^ fiecare combinatie de n noduri (bitul i e 1 => nodul i face parte din lant)

        for (int i = 0; i < m_n; i++) {
            // ^ nodul pe care vrem sa il adaugam in lant

            if (k & (1 << i)) {
                // ^ e nodul i in combinatia de noduri pe care o verificam acum?

                for (auto &pairJ: m_hamiltonListaAd[i]) {
                    int j = pairJ.first, c = pairJ.second;
                    // ^ cautam toate nodurile din care poti ajunge in i (lista de adiacenta nu e standard, ne
                    // spune nodurile ce ajung in x, nu cele ce pleaca din x)

                    if (k & (1 << j)) {
                        // ^ e nodul j in combinatia de noduri pe care o verificam acum?

                        // Daca da, actualizeaza costul minim pe care il poti obtine folosind nodurile k si care
                        // se termina in nodul i: compara [vechea valoare] cu [costul minim al lantului curent,
                        // excluzand nodul i, ce se termina in j + distanta de la j la i]
                        //
                        // Valoarea precedenta sigur a fost calculata (avand un k mai mic, din moment ce excludem
                        // un bit).
                        m_hamiltonMinimDP[k][i] = min(
                                m_hamiltonMinimDP[k][i],
                                m_hamiltonMinimDP[k & ~(1 << i)][j] + c
                        );
                    }
                }
            }
        }
    }

    // Acum avem toate lanturile de lungime n ce se termina in nodul 1, 2, .., n (daca exista).

    // Pentru rezultatul final, cautam minimul costurilor pentru lanturile ce se termina in noduri
    // ce au arc care duce catre nodul de start (ales de mine ca fiind 0 - oricare ar fi fost la fel de bun).
    int res = INF;
    for (auto &pair0: m_hamiltonListaAd[0]) {
        int i = pair0.first, c = pair0.second;
        res = min(res, m_hamiltonMinimDP[((1 << m_n) - 1)][i] + c);
    }

    return (res != INF) ? res : -1;
}

void Graf::neorientatCitesteMultigrafContorizat(ifstream &in) {
    // Redimensioneaza
    m_multigrafListaAd.clear();
    m_multigrafListaAd.resize(m_n + 1);

    for (int i = 0; i < m_m; i++) {
        int x, y;
        in >> x >> y;
        m_multigrafListaAd[x].push_back({y, i});
        m_multigrafListaAd[y].push_back({x, i});
    }
}

bool Graf::eCicluEuler() {
    // Redimensioneaza
    m_cicluEulerGrade.resize(m_n + 1, 0);

    // Putem gasi un ciclu eulerian intr-un graf daca fiecare nod are grad par

    // Numaram gradele fiecarui noid (din lista de adiacenta a multigrafului)
    for (int x = 1; x <= m_n; x++) {
        for (auto &y: m_multigrafListaAd[x]) {
            m_cicluEulerGrade[x]++;
        }
    }

    // Verificam ca gradele sunt numere pare
    for (int x = 1; x <= m_n; x++) {
        if (m_cicluEulerGrade[x] % 2 == 1) {
            return false;
        }
    }

    // Toate gradele au fost pare => putem construi ciclu eulerian
    return true;
}

const auto &Graf::gasesteCicluEuler() {
    // Redimensioneaza
    m_cicluEulerFolMuchie.resize(m_m + 1, false);

    // Facem un "dfsRecursive iterativ" - daca ar fi recursiv, am avea stack overflow pentru
    // grafuri mari. La "dfsRecursive"-ul asta, nu vrem sa se repete muchiile (si nu nodurile).

    // Cautam un ciclu eulerian plecand din nodul 1
    m_cicluEulerStack.push_back(1);

    // Procesam nodul curent la care s-a ajuns in parcurgerea dfsRecursive
    while (!m_cicluEulerStack.empty()) {
        int x = m_cicluEulerStack.back();

        // Cautam o muchie ce pleaca din x si care nu a fost folosita
        bool gasitMuchie = false;
        int y, i;
        while (!m_multigrafListaAd[x].empty() && !gasitMuchie) {
            auto &pairY = m_multigrafListaAd[x].back();
            m_multigrafListaAd[x].pop_back();
            y = pairY.first, i = pairY.second;
            if (!m_cicluEulerFolMuchie[i]) {
                gasitMuchie = true;
            }
        }

        if (gasitMuchie) {
            // Am gasit o muchie => continuam cu dfsRecursive-ul cu nodul y
            m_cicluEulerStack.push_back(y);
            m_cicluEulerFolMuchie[i] = true;
        } else {
            // Nu am gasit nicio muchie ce pleaca din x si care nu a fost folosita =>
            // => nu mai exista cicluri ce pleaca din x si ajung in x
            m_cicluEulerSol.push_back(x);
            m_cicluEulerStack.pop_back();
        }
    }

    return m_cicluEulerSol;
}

void Graf::dfsRecursive(int k) {
    m_dfsViz[k] = true;

    for (auto x: m_listaAd[k]) {
        if (!m_dfsViz[x]) {
            dfsRecursive(x);
        }
    }
}

void Graf::orientatCtcDfs(int x) {
    m_ctcStack.push(x);
    m_ctcPeStiva[x] = true;
    m_ctcId[x] = m_ctcLow[x] = ++m_ctcUltId;

    for (auto y: m_listaAd[x]) {
        // Nu am explorat nodul pana acum (neavand vreun id)
        if (m_ctcId[y] == 0) {
            orientatCtcDfs(y);
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

void Graf::neorientatBiconexAdd(int x, int y) {
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

void Graf::neorientatBiconexDfs(int x, int prev, int id) {
    // Initializam low-ul (nodul cel mai de sus din parcurgerea dfsRecursive in care putem ajunge)
    // si punem nodul curent pe stack
    m_biconexLow[x] = id;
    m_biconexStack.push(x);

    for (auto y: m_listaAd[x]) {
        // Ignoram cazul in care ne intoarcem din nodul in care am plecat
        if (y == prev) continue;

        // Nodul y nu a fost vizitat => viziteaza-l
        if (!m_biconexLow[y]) {
            // Viziteaza-l si actualizeaza low
            neorientatBiconexDfs(y, x, id + 1);
            m_biconexLow[x] = min(m_biconexLow[x], m_biconexLow[y]);

            // Am ajuns la originea ciclului / am dat peste un nod de mai jos din parcurgerea
            // dfsRecursive la care nu mai putem ajunge altfel (=> componenta biconexa)
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

void Graf::neorientatMuchiiCriticeDfs(int x, int prev, int id) {
    m_criticeLow[x] = id;

    for (auto y: m_listaAd[x]) {
        // Nu te intoarce in nodul din care ai plecat
        if (y == prev) continue;

        // Ruleaza dfsRecursive in continuare, cu un id mai mare
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

void Graf::diametruDFS(int x, int dist) {
    if (dist > m_diametruDistMax) {
        m_diametruDistMax = dist;
        m_diametruNodMax = x;
    }
    m_diametruViz[x] = 1;
    for (auto &y: m_listaAd[x]) {
        if (!m_diametruViz[y]) {
            diametruDFS(y, dist + 1);
        }
    }
}

void Graf::orientatRoyFloydSetup() {
    // Redimensioneaza
    m_royFloydDists.clear();
    m_royFloydDists.resize(m_n + 1);

    for (int i = 1; i <= m_n; i++) {
        // Redimensioneaza
        m_royFloydDists[i].resize(m_n + 1);

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
