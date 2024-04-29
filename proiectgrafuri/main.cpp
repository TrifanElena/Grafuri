#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;


unordered_map<string, vector<pair<string, int>>> citire_date(const string& distanta_orase) {
    unordered_map<string, vector<pair<string, int>>> graf;
    ifstream fisier(distanta_orase);
    string oras1, oras2;
    int distanta;
    while (fisier >> oras1 >> oras2 >> distanta) {
        graf[oras1].push_back(make_pair(oras2, distanta));
        graf[oras2].push_back(make_pair(oras1, distanta));
    }
    return graf;
}


unordered_map<string, int> dijkstra(const unordered_map<string, vector<pair<string, int>>>& graf, const string& start) {
    unordered_map<string, int> distante;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push(make_pair(0, start));
    distante[start] = 0;
    while (!pq.empty()) {
        string oras_curent = pq.top().second;
        int distanta_curenta = pq.top().first;
        pq.pop();
        if (distante.find(oras_curent) != distante.end() && distante[oras_curent] < distanta_curenta) {
            continue;
        }
        for (const auto& vecin : graf.at(oras_curent)) {
            string oras_vecin = vecin.first;
            int distanta_noua = distanta_curenta + vecin.second;
            if (distante.find(oras_vecin) == distante.end() || distanta_noua < distante[oras_vecin]) {
                distante[oras_vecin] = distanta_noua;
                pq.push(make_pair(distanta_noua, oras_vecin));
            }
        }
    }
    return distante;
}

void dfs(const unordered_map<string, vector<pair<string, int>>>& graf, unordered_map<string, bool>& vizitat, const string& nod) {
    vizitat[nod] = true;
    cout << nod << " ";
    for (const auto& vecin : graf.at(nod)) {
        if (!vizitat[vecin.first]) {
            dfs(graf, vizitat, vecin.first);
        }
    }
}


bool verificare_conexitate(const unordered_map<string, vector<pair<string, int>>>& graf) {
    unordered_map<string, bool> vizitat;
    for (const auto& pereche : graf) {
        vizitat[pereche.first] = false;
    }
    dfs(graf, vizitat, graf.begin()->first);
    for (const auto& pereche : vizitat) {
        if (!pereche.second) {
            return false;
        }
    }
    return true;
}

vector<pair<string, string>> kruskal(const unordered_map<string, vector<pair<string, int>>>& graf) {

    struct Muchie {
        string oras1, oras2;
        int distanta;
        bool operator<(const Muchie& other) const {
            return distanta < other.distanta;
        }
    };

    vector<Muchie> muchii;
    for (const auto& pereche : graf) {
        const string& oras1 = pereche.first;
        for (const auto& vecin : pereche.second) {
            const string& oras2 = vecin.first;
            int distanta = vecin.second;
            if (oras1 < oras2) {
                muchii.push_back({ oras1, oras2, distanta });
            }
        }
    }

    sort(muchii.begin(), muchii.end());


    vector<pair<string, string>> mst;
    unordered_map<string, string> tata;
    for (const auto& pereche : graf) {
        tata[pereche.first] = pereche.first;
    }

    for (const auto& muchie : muchii) {
        const string& oras1 = muchie.oras1;
        const string& oras2 = muchie.oras2;
        string radacina_oras1 = oras1, radacina_oras2 = oras2;

        while (tata[radacina_oras1] != radacina_oras1) {
            radacina_oras1 = tata[radacina_oras1];
        }
        while (tata[radacina_oras2] != radacina_oras2) {
            radacina_oras2 = tata[radacina_oras2];
        }

        if (radacina_oras1 != radacina_oras2) {
            mst.push_back({ oras1, oras2 });

            tata[radacina_oras1] = radacina_oras2;
        }
    }

    return mst;
}


unordered_map<string, string> colorare_graf(const unordered_map<string, vector<pair<string, int>>>& graf) {
    unordered_map<string, string> culori;


    vector<string> culori_disponibile = { "Rosu", "Verde", "Albastru", "Galben", "Portocaliu", "Violet" };


    for (const auto& pereche : graf) {
        const string& oras = pereche.first;
        vector<string> culori_vecini;


        for (const auto& vecin : pereche.second) {
            if (culori.find(vecin.first) != culori.end()) {
                culori_vecini.push_back(culori[vecin.first]);
            }
        }


        for (const string& culoare : culori_disponibile) {
            if (find(culori_vecini.begin(), culori_vecini.end(), culoare) == culori_vecini.end()) {
                culori[oras] = culoare;
                break;
            }
        }
    }

    return culori;
}


int main() {
    string nume_fisier = "C:\\Users\\elena\\Desktop\\proiectgrafuri\\distanta_orase.txt";
    auto graf = citire_date(nume_fisier);

    cout << "Lista de adiacenta:" << endl;
    for (const auto& pereche : graf) {
        cout << pereche.first << ": ";
        for (const auto& vecin : pereche.second) {
            cout << "(" << vecin.first << ", " << vecin.second << ") ";
        }
        cout << endl;
    }


    cout << "\nMatrice de adiacenta:" << endl;
    vector<string> orase;
    for (const auto& pereche : graf) {
        orase.push_back(pereche.first);
    }
    int n = orase.size();
    vector<vector<int>> matrice(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        matrice[i][i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        for (const auto& pereche : graf.at(orase[i])) {
            int j = distance(orase.begin(), find(orase.begin(), orase.end(), pereche.first));
            matrice[i][j] = pereche.second;
            matrice[j][i] = pereche.second;
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << (matrice[i][j] == 0 ? "0" : "1") << " ";
        }
        cout << endl;
    }

    cout << "\nLista muchiilor:" << endl;
    for (const auto& pereche : graf) {
        const string& oras1 = pereche.first;
        for (const auto& vecin : pereche.second) {
            const string& oras2 = vecin.first;
            int distanta = vecin.second;
            if (oras1 < oras2) {
                cout << "(" << oras1 << ", " << oras2 << ", " << distanta << ") ";
            }
        }
    }
    cout << endl;
    cout << endl;
    cout << "Graful este " << (verificare_conexitate(graf) ? "" : "ne") << "conex." << endl;


    string oras_start;
    cout << "\nIntroduceti orasul de plecare: ";
    cin >> oras_start;
    string oras_destinatie;
    cout << "Introduceti orasul destinatie: ";
    cin >> oras_destinatie;
    auto distante = dijkstra(graf, oras_start);


    cout << "\nCel mai scurt drum de la " << oras_start << " la " << oras_destinatie << " folosind Dijkstra este de " << distante[oras_destinatie] << " km." << endl;


    auto mst = kruskal(graf);

    cout << "\nArborele minim de acoperire (MST) este format din urmatoarele muchii:" << endl;
    for (const auto& muchie : mst) {
        cout << muchie.first << " - " << muchie.second << endl;
    }

    auto culori = colorare_graf(graf);

    cout << "\nColorarea grafurilor este urmatoarea:" << endl;
    for (const auto& pereche : culori) {
        cout << pereche.first << " este colorat cu " << pereche.second << endl;
    }

    return 0;
}