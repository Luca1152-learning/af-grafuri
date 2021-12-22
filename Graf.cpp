#include "Graf.h"

Graf::Graf(int n, int m) : m_n(n), m_m(m) {}

void Graf::ponderatCitesteListaMuchii(ifstream &in) {
    for (int i = 0; i < m_m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        m_listaMuchiiPonderat.push_back({c, {x, y}});
    }
}

auto Graf::bfs(int start) {
    // Declara
    queue<int> q;
    vector<int> dist(m_n + 1, 0);

    q.push(start);
    dist[start] = 1;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (auto i: m_listaAd[curr]) {
            if (dist[i] == 0) {
                dist[i] = dist[curr] + 1;
                q.push(i);
            }
        }
    }
    return dist;
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

pair<int, vector<pair<int, int>>> Graf::apcm() {
    // Declara
    int cost = 0;
    vector<pair<int, int>> res;

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
            cost += c;
            res.push_back({x, y});
        }
    }

    return {cost, res};
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

int Graf::nrCompConexe() {
    vector<int> viz(m_n + 1, 0);

    int nrComp = 0;
    for (int i = 1; i <= m_n; i++) {
        if (!viz[i]) {
            nrComp++;
            nrCompConexeDfs(i, viz);
        }
    }
    return nrComp;
}

auto Graf::biconexe() {
    // Redimensioneaza
    list<list<int>> comps;
    stack<int> st;
    vector<int> low(m_n + 1, 0);

    for (int i = 1; i <= m_n; i++) {
        if (!low[i]) {
            biconexDfs(i, -1, 1, comps, st, low);
        }
    }
    return comps;
}

auto Graf::muchiiCritice() {
    // Declara
    map<pair<int, int>, bool> toRemove;
    vector<vector<int>> critice;
    vector<int> low(m_n + 1, 0); // Id-ul nodului minim in care te poti intoarce din nodul i

    muchiiCriticeDfs(0, -1, 1, toRemove, critice, low);

    // In rezultat, punem muchiile ce nu au fost marcate ca trebuind sa fie sterse
    for (auto &e: m_listaMuchii) {
        if (!toRemove[{e[0], e[1]}]) {
            critice.push_back(e);
        }
    }

    return critice;
}

int Graf::diametru() {
    // Declara
    int nodMax = 0, distMax = 0;
    vector<bool> viz(m_n + 1, false);

    diametruDfs(1, 1, viz, nodMax, distMax);
    distMax = 0;
    fill(viz.begin(), viz.end(), 0);
    diametruDfs(nodMax, 1, viz, nodMax, distMax);

    return distMax;
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

auto Graf::ctc() {
    // Declara
    int ultId = 0;
    vector<int> id(m_n + 1, 0), low(m_n + 1, 0);
    vector<bool> peStiva(m_n + 1, false);
    stack<int> st;
    list<list<int>> ctc;

    // Algoritmul lui Tarjan
    for (int i = 1; i <= m_n; i++) {
        // Nu am explorat nodul pana acum (neavand vreun id)
        if (id[i] == 0) {
            ctcDfs(i, ultId, id, low, peStiva, st, ctc);
        }
    }
    return ctc;
}

pair<bool, vector<int>> Graf::bellmanFord(int start) {
    // Declara
    vector<int> dist(m_n + 1, INF), puneriInCoada(m_n + 1, 0), inCoada(m_n + 1, 0);
    queue<int> q;
    bool circuitCostNegativ = false;

    // Gaseste graful de costuri minime, plecand din start la celelalte n-1 noduri.
    // Putem avea circuit de cost negativ -> va fi detectat.

    // Incepem cu optimizarile plecand din nodul de start
    q.push(start);
    dist[start] = 0;
    inCoada[start] = true;
    puneriInCoada[start] = 1;

    // Ne oprim cand nu mai avem nimic de optimizat / am gasit un circuit cu cost negativ
    while (!q.empty() && !circuitCostNegativ) {
        int x = q.front();
        q.pop();

        // Marcam nodul curent ca ne mai fiind in queue
        inCoada[x] = false;

        // Luam toate arcele la rand si incercam sa optimizam distante, folosindu-le
        for (auto &e: m_ponderatListaAd[x]) {
            int y = e.first, c = e.second;

            // Am gasit un arc (de la x la y) ce optimizeaza costul lui y (= obtinem
            // o distanta mai mica din start->y daca mergem prin x)
            if (dist[y] > dist[x] + c) {
                dist[y] = dist[x] + c;

                // Daca y nu e deja in coada, pune-l (facem verificarea ca sa nu
                // il adaugam de mai multe ori in coada), pentru ca, optimizand
                // distanta pana la el, putem optimiza distante si plecand din el.
                if (!inCoada[y]) {
                    q.push(y);
                    inCoada[y] = true;

                    // Numara de cate ori au fost puse in coada nodurile. Daca un nod
                    // a fost pus de >= n ori (=> n optimizari), inseamna ca am gasit
                    // un circuit de cost negativ.
                    puneriInCoada[y]++;
                    if (puneriInCoada[y] >= m_n) {
                        circuitCostNegativ = true;
                    }
                }
            }
        }
    }

    return {circuitCostNegativ, dist};
}

auto Graf::dijkstra(int start) {
    // Declara
    vector<int> dist(m_n + 1, INF);
    set<pair<int, int>> minHeap; // "min".. doar e un ordered set (crescator)

    // Incepem algoritmul din nodul de start
    dist[start] = 0;
    // Punem in set perechea {0, [nod start]}, 0 fiind distanta de la start pana la el insusi
    minHeap.insert({0, start});

    while (!minHeap.empty()) {
        // Procesam nodul de la distanta cea mai mica fata de start
        auto x = minHeap.begin()->second;
        minHeap.erase(minHeap.begin());

        for (auto &e: m_ponderatListaAd[x]) {
            auto y = e.first, c = e.second;

            // Obtinem un drum mai scurt (fata de cel gasit) daca trecem prin x
            if (dist[y] > dist[x] + c) {
                // Nodul y e deja marcat ca trebuind sa fie procesat => il scoatem din set, ca sa il readaugam
                // cu noua distanta, mai mica (ca sa nu pierdem timp incercand sa-l optimizam cu distanta veche
                // mai tarziu) -- 90p->100p
                if (minHeap.count({dist[y], y}) > 0) {
                    minHeap.erase(minHeap.find({dist[y], y}));
                }

                // Actualizam distanta lui y si il punem la procesat
                dist[y] = dist[x] + c;
                minHeap.insert({dist[y], y});
            }
        }
    }

    return dist;
}

auto Graf::royFloyd() {
    vector<vector<int>> dists;
    royFloydSetup(dists);

    // Nodul k = nodul pe care incercam sa il integram in drumul de la i la j
    for (int k = 1; k <= m_n; k++) {
        // Verificam toate drumurile daca pot fi scurtate folosind k drept nod intermediar
        // (i -> ... -> k -> ... -> j)
        for (int i = 1; i <= m_n; i++) {
            for (int j = 1; j <= m_n; j++) {
                if (dists[i][k] + dists[k][j] < dists[i][j]) {
                    dists[i][j] = dists[i][k] + dists[k][j];
                }
            }
        }
    }

    return dists;
}

pair<vector<vector<int>>, vector<vector<int>>> Graf::citesteInputFluxMaxim(ifstream &in) {
    vector<vector<int>> capacitate(m_n + 1), flux(m_n + 1);

    for (int i = 0; i < m_m; i++) {
        // Redimensioneaza
        capacitate[i].resize(m_n + 1);

        int x, y, c;
        in >> x >> y >> c;
        capacitate[x][y] = c;
        m_listaAd[x].push_back(y);
        m_listaAd[y].push_back(x);
    }
}

int Graf::fluxMaximBfs(
        int start, vector<vector<int>> &capacitate, vector<vector<int>> &flux,
        vector<int> &parinti, queue<int> &q
) {
    std::fill(parinti.begin(), parinti.end(), 0);

    q.push(start);
    parinti[start] = -1;

    while (!q.empty()) {
        int x = q.front();
        q.pop();

        // Din moment ce am facut graful unul neorientat (pentru a avea acces la vecinii nodului destinatie),
        // am putea sa ne intoarcem din nodul final in alte noduri inca nevizitate, dar nu vrem asta
        if (x == m_n) {
            continue;
        }

        for (auto &y: m_listaAd[x]) {
            // Daca nu am vizitat nodul y in bfs-ul curent si daca inca mai putem pompa
            // flux prin lantul x-y, viziteaza-l
            if (!parinti[y] && capacitate[x][y] - flux[x][y] > 0) {
                q.push(y);
                parinti[y] = x;
            }
        }
    }

    // Returnam daca am ajuns la destinatie cu parcurgerea curenta
    return parinti[m_n];
}

int Graf::fluxMaxim(vector<vector<int>> &capacitate, vector<vector<int>> &flux) {
    // Declara
    vector<int> parinti(m_n + 1, 0);
    queue<int> q;

    // Algoritmul Edmonds-Karp

    int fluxMaximTotal = 0;

    while (fluxMaximBfs(1, capacitate, flux, parinti, q)) {
        // Am creat arborele bfs -> ne intoarcem pe fiecare ruta posibila, folosindu-ne de vecinii
        // nodului final
        for (auto &x: m_listaAd[m_n]) {
            if (!parinti[x] || capacitate[x][m_n] - flux[x][m_n] == 0) {
                continue;
            }

            parinti[m_n] = x;

            // Gaseste fluxul minim ce poate fi pompat pe lantul gasit de bfs
            int fluxMinimLant = INF, curr = m_n;
            while (parinti[curr] != -1) {
                int prev = parinti[curr];
                fluxMinimLant = min(fluxMinimLant, capacitate[prev][curr] - flux[prev][curr]);
                curr = prev;
            }
            if (fluxMinimLant == 0) {
                continue;
            }

            // Actualizeaza fluxurile
            curr = m_n;
            while (parinti[curr] != -1) {
                int prev = parinti[curr];
                flux[prev][curr] += fluxMinimLant;
                flux[curr][prev] -= fluxMinimLant;
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

int Graf::hamiltonCostMinim() {
    // Declara
    vector<vector<int>> dp(1 << m_n);

    // Initializam costurile minime cu INFINIT
    for (int k = 0; k < 1 << m_n; k++) {
        // Redimensioneaza
        dp[k].resize(m_n + 1, INF);
    }

    // Costul minim pe un lant format din nodul 0 (bitul 0 e setat => 1, prima coordonata) si se termina
    // in 0 (a doua coordonata) este 0 -- de aici plecam cu calculul costurilor
    dp[1][0] = 0;

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
                        dp[k][i] = min(dp[k][i], dp[k & ~(1 << i)][j] + c);
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
        res = min(res, dp[((1 << m_n) - 1)][i] + c);
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

bool Graf::areCicluEulerian() {
    // Declara
    vector<int> grade(m_n + 1, 0);

    // Putem gasi un ciclu eulerian intr-un graf daca fiecare nod are grad par

    // Numaram gradele fiecarui noid (din lista de adiacenta a multigrafului)
    for (int x = 1; x <= m_n; x++) {
        for (auto &y: m_multigrafListaAd[x]) {
            grade[x]++;
        }
    }

    // Verificam ca gradele sunt numere pare
    for (int x = 1; x <= m_n; x++) {
        if (grade[x] % 2 == 1) {
            return false;
        }
    }

    // Toate gradele au fost pare => putem construi ciclu eulerian
    return true;
}

auto Graf::cicluEulerian() {
    // Declara
    vector<int> sol, st;
    vector<bool> folMuchie(m_m + 1, false);

    // Facem un "nrCompConexeDfs iterativ" - daca ar fi recursiv, am avea stack overflow pentru
    // grafuri mari. La "nrCompConexeDfs"-ul asta, nu vrem sa se repete muchiile (si nu nodurile).

    // Cautam un ciclu eulerian plecand din nodul 1
    st.push_back(1);

    // Procesam nodul curent la care s-a ajuns in parcurgerea nrCompConexeDfs
    while (!st.empty()) {
        int x = st.back();

        // Cautam o muchie ce pleaca din x si care nu a fost folosita
        bool gasitMuchie = false;
        int y, i;
        while (!m_multigrafListaAd[x].empty() && !gasitMuchie) {
            auto &pairY = m_multigrafListaAd[x].back();
            m_multigrafListaAd[x].pop_back();
            y = pairY.first, i = pairY.second;
            if (!folMuchie[i]) {
                gasitMuchie = true;
            }
        }

        if (gasitMuchie) {
            // Am gasit o muchie => continuam cu nrCompConexeDfs-ul cu nodul y
            st.push_back(y);
            folMuchie[i] = true;
        } else {
            // Nu am gasit nicio muchie ce pleaca din x si care nu a fost folosita =>
            // => nu mai exista cicluri ce pleaca din x si ajung in x
            sol.push_back(x);
            st.pop_back();
        }
    }

    return sol;
}

void Graf::nrCompConexeDfs(int k, vector<int> &viz) {
    viz[k] = true;

    for (auto x: m_listaAd[k]) {
        if (!viz[x]) {
            nrCompConexeDfs(x, viz);
        }
    }
}

void Graf::ctcDfs(
        int x, int &ultId, vector<int> &id, vector<int> &low, vector<bool> &peStiva,
        stack<int> &st, list<list<int>> &ctc
) {
    st.push(x);
    peStiva[x] = true;
    id[x] = low[x] = ++ultId;

    for (auto y: m_listaAd[x]) {
        // Nu am explorat nodul pana acum (neavand vreun id)
        if (id[y] == 0) {
            ctcDfs(y, ultId, id, low, peStiva, st, ctc);
        }

        // Am intalnit un nod care inca nu a fost atribuit unei componente conexe.
        // Poate nodul curent face parte din viitoarea componenta conexa, a carei (posibila) sursa
        // a fost gasita de y.
        if (peStiva[y]) {
            low[x] = min(low[x], low[y]);
        }
    }

    // Am ajuns la nodul de start al ctc-ului explorat in prezent
    if (id[x] == low[x]) {
        list<int> compCurr;
        while (true) {
            auto y = st.top();
            st.pop();

            peStiva[y] = false;
            compCurr.push_back(y);

            if (y == x) break;
        }
        ctc.push_back(compCurr);
    }
}

void Graf::biconexAdd(
        int x, int y, list<list<int>> &comps, stack<int> &st
) {
    // Creeaza o noua componenta pentru afisare
    list<int> comp;

    // Adauga in componenta toate nodurile pana la y, inclusiv y
    while (st.top() != y) {
        comp.push_back(st.top());
        st.pop();
    }
    comp.push_back(y);
    st.pop();

    // Adauga in componenta si pe x, separat (in caz ca e un gap in stack intre y si x)
    // ^ gap-ul poate aparea daca intalnim mai multe componente biconexe ce se intorc in acelasi nod
    comp.push_back(x);

    comps.push_back(comp);
}

void Graf::biconexDfs(
        int x, int prev, int id, list<list<int>> &comps, stack<int> &st,
        vector<int> &low
) {
    // Initializam low-ul (nodul cel mai de sus din parcurgerea nrCompConexeDfs in care putem ajunge)
    // si punem nodul curent pe stack
    low[x] = id;
    st.push(x);

    for (auto y: m_listaAd[x]) {
        // Ignoram cazul in care ne intoarcem din nodul in care am plecat
        if (y == prev) continue;

        // Nodul y nu a fost vizitat => viziteaza-l
        if (!low[y]) {
            // Viziteaza-l si actualizeaza low
            biconexDfs(y, x, id + 1, comps, st, low);
            low[x] = min(low[x], low[y]);

            // Am ajuns la originea ciclului / am dat peste un nod de mai jos din parcurgerea
            // nrCompConexeDfs la care nu mai putem ajunge altfel (=> componenta biconexa)
            if (low[y] >= id) {
                biconexAdd(x, y, comps, st);
            }
        }
            // Nodul y a fost vizitat => doar actualizeaza min-ul in caz ca e nevoie,
            // fara sa risti sa afisezi o componenta biconexa de doua ori
        else {
            low[x] = min(low[x], low[y]);
        }
    }
}

void Graf::muchiiCriticeDfs(
        int x, int prev, int id, map<pair<int, int>, bool> &toRemove,
        vector<vector<int>> &critice, vector<int> &low
) {
    low[x] = id;

    for (auto y: m_listaAd[x]) {
        // Nu te intoarce in nodul din care ai plecat
        if (y == prev) continue;

        // Ruleaza nrCompConexeDfs in continuare, cu un id mai mare
        if (low[y] == 0) muchiiCriticeDfs(y, x, id + 1, toRemove, critice, low);

        // Nodul vizitat din cel curent face parte dintr-un ciclu,
        // asa ca trebuie sa excludem muchia x-y
        if (low[y] < id + 1) {
            toRemove[{x, y}] = toRemove[{y, x}] = true;
        }

        // Actualizeaza low-ul nodului curent
        low[x] = min(low[x], low[y]);
    }
}

void Graf::diametruDfs(
        int x, int dist, vector<bool> &viz, int &nodMax, int &distMax
) {
    if (dist > distMax) {
        distMax = dist;
        nodMax = x;
    }
    viz[x] = 1;
    for (auto &y: m_listaAd[x]) {
        if (!viz[y]) {
            diametruDfs(y, dist + 1, viz, nodMax, distMax);
        }
    }
}

void Graf::royFloydSetup(vector<vector<int>> &dists) {
    // Redimensioneaza
    dists.resize(m_n + 1);

    for (int i = 1; i <= m_n; i++) {
        // Redimensioneaza
        dists[i].resize(m_n + 1);

        for (int j = 1; j <= m_n; j++) {
            if (i == j) {
                dists[i][j] = 0;
            } else if (m_ponderatMatrice[i][j] == 0) {
                dists[i][j] = INF;
            } else {
                dists[i][j] = m_ponderatMatrice[i][j];
            }
        }
    }
}
