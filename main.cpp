#include <fstream>
#include "Graf.h"

using namespace std;

int main() {
    // Input rapid
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // I/O
    ifstream in("date.in");
    ofstream out("date.out");

    int n, m;
    in >> n >> m;

    Graf g(n, m);

    out.close();
    return 0;
}