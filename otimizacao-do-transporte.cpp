#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct ponto {
    int id;
    double latitude, longitude;
    bool visitado;
};

double distancia_dois_pontos_haversine(ponto p1, ponto p2) {
    const double raio_da_terra = 6378.137;

    double lat1 = p1.latitude * M_PI / 180.0;
    double lon1 = p1.longitude * M_PI / 180.0;
    double lat2 = p2.latitude * M_PI / 180.0;
    double lon2 = p2.longitude * M_PI / 180.0;

    double delta_lat = lat2 - lat1;
    double delta_lon = lon2 - lon1;

    return 2 * raio_da_terra * asin(sqrt(pow(sin(delta_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(delta_lon / 2), 2)));
}

int main() {
    ifstream arquivo("../problema_das_paradas_de_onibus/instances/01.ins");

    string linha;
    int numero_de_vertices;

    while (linha != "NODE_COORD_SECTION") {
        getline(arquivo, linha);

        if (linha.find("DIMENSION") != string::npos) {
            stringstream ss(linha);
            string lixo;
            ss >> lixo >> lixo >> numero_de_vertices;
        }
    }

    cout << "Número de vértices: " << numero_de_vertices << endl;

    vector<ponto> vertices;

    for (int i = 0; i < numero_de_vertices; i++) {
        getline(arquivo, linha);

        stringstream ss(linha);

        ponto p;
        ss >> p.id >> p.latitude >> p.longitude;
        p.visitado = false;

        vertices.push_back(p);
    }
}

