#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct ponto{
    int id;
    double latitude, longitude;
    bool visitado;
};

double distancia_dois_pontos_haversine(ponto p1, ponto p2){
    const double raio_da_terra = 6378.137;

    double lat1 = p1.latitude * M_PI / 180.0;
    double lon1 = p1.longitude * M_PI / 180.0;
    double lat2 = p2.latitude * M_PI / 180.0;
    double lon2 = p2.longitude * M_PI / 180.0;

    double delta_lat = lat2 - lat1;
    double delta_lon = lon2 - lon1;

    return 2 * raio_da_terra * asin(sqrt(pow(sin(delta_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(delta_lon / 2), 2)));
}

double distancia_dois_pontos_cartesianos(ponto p1, ponto p2){
    double x1 = p1.latitude;
    double y1 = p1.longitude;
    double x2 = p2.latitude;
    double y2 = p2.longitude;

    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

typedef vector<pair<ponto, double>> vp;

vp insercao_mais_proxima(vector<ponto> &pontos, string tipo_de_coordenada){
    vp ciclo;

    ciclo.push_back({pontos[0], 0.0});
    pontos[0].visitado = true;

    int id_mais_proximo = -1;
    double menor_distancia = numeric_limits<double>::max();

    for (int i = 1; i < pontos.size(); i++){
        double distancia;

        if(tipo_de_coordenada == "GEO"){
            distancia = distancia_dois_pontos_haversine(pontos[0], pontos[i]);
        }
        else/*(tipo_de_coordenada == "EUC_2D")*/{
            distancia = distancia_dois_pontos_cartesianos(pontos[0], pontos[i]);
        }
        
        if (distancia < menor_distancia){
            menor_distancia = distancia;
            id_mais_proximo = i;
        }
    }

    ciclo.push_back({pontos[id_mais_proximo], menor_distancia});
    pontos[id_mais_proximo].visitado = true;

    while (ciclo.size() < pontos.size()){
        menor_distancia = numeric_limits<double>::max();
        id_mais_proximo = -1;

        for (int i = 0; i < pontos.size(); i++){
            if (!pontos[i].visitado){
                for (int j = 0; j < ciclo.size(); j++){
                    double distancia;

                    if(tipo_de_coordenada == "GEO"){
                        distancia = distancia_dois_pontos_haversine(pontos[i], ciclo[j].first);
                    }
                    else/*(tipo_de_coordenada == "EUC_2D")*/{
                        distancia = distancia_dois_pontos_cartesianos(pontos[i], ciclo[j].first);
                    }
                    
                    
                    if (distancia < menor_distancia){
                        menor_distancia = distancia;
                        id_mais_proximo = i;
                    }
                }
            }
        }

        double menor_peso = numeric_limits<double>::max();
        int melhor_posicao = -1;

        for (int i = 0; i < ciclo.size(); i++){
            int ponto1 = ciclo[i].first.id;
            int ponto2 = ciclo[(i + 1) % ciclo.size()].first.id;

            double peso;

            if(tipo_de_coordenada == "GEO"){
                peso = distancia_dois_pontos_haversine(pontos[id_mais_proximo], pontos[ponto1]) + distancia_dois_pontos_haversine(pontos[id_mais_proximo], pontos[ponto2]);
            }
            else{
                peso = distancia_dois_pontos_cartesianos(pontos[id_mais_proximo], pontos[ponto1]) + distancia_dois_pontos_cartesianos(pontos[id_mais_proximo], pontos[ponto2]);
            }
            

            if (peso < menor_peso){
                menor_peso = peso;
                melhor_posicao = i;
            }
        }

        ciclo.insert(ciclo.begin() + melhor_posicao + 1, {pontos[id_mais_proximo], menor_distancia});
        pontos[id_mais_proximo].visitado = true;
    }

    return ciclo;
}

int main(){
    ifstream arquivo("../problema_das_paradas_de_onibus/instances/10.ins");

    string linha;
    int numero_de_vertices;
    string tipo_de_coordenada;

    while (linha != "NODE_COORD_SECTION"){
        getline(arquivo, linha);

        if (linha.find("DIMENSION") != string::npos){
            stringstream ss(linha);
            string lixo;
            ss >> lixo >> lixo >> numero_de_vertices;
        }

        if (linha.find("EDGE_WEIGHT_TYPE") != string::npos){
            stringstream ss(linha);
            string lixo;
            ss >> lixo >> lixo >> tipo_de_coordenada;
        }
    }

    cout << "Número de vértices: " << numero_de_vertices << endl;
    cout << "Tipo de coordenadas: " << tipo_de_coordenada << endl;

    vector<ponto> pontos;

    for (int i = 0; i < numero_de_vertices; i++){
        getline(arquivo, linha);

        stringstream ss(linha);

        ponto p;
        ss >> p.id >> p.latitude >> p.longitude;
        p.visitado = false;

        pontos.push_back(p);
    }

    vp ciclo = insercao_mais_proxima(pontos, tipo_de_coordenada);
    ciclo.push_back({ciclo[0].first, tipo_de_coordenada == "GEO" ? distancia_dois_pontos_haversine(ciclo[0].first, ciclo.back().first) : distancia_dois_pontos_cartesianos(ciclo[0].first, ciclo.back().first)});
    double maior_distancia = 0;

    for (int i = 0; i < ciclo.size(); i++){
        cout << "Ponto: " << ciclo[i].first.id << " | Distância: " << ciclo[i].second << endl;

        if(ciclo[i].second > maior_distancia){
            maior_distancia = ciclo[i].second;
        }
    }

    cout << "A distância máxima é de: " << maior_distancia;

    return 0;
}
