#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

// registro para armazenar as informações de cada vértice, sendo ela um inteiro representando o ID, dois doubles representando a latitude e longitude, que não necessariamente serão coordenadas geograficasm, podendo ser também coordenadas cartesianas, além de um booleano representando se aquele vértice foi visitado ou não
struct ponto {
    int id;
    double latitude, longitude;
    bool visitado;
};

// definição do tipo para o vetor que armazena o caminho, sendo o primeiro elemento do tipo do registro ponto, e o segundo um double, que vai armazenar a distância
typedef vector<pair<ponto, double>> vp;


// função que recebe como parâmetros dois pontos, e retorna a distância entre eles baseada na fórmula de haversine
double distancia_dois_pontos_haversine(ponto p1, ponto p2) {
    const double raio_da_terra = 6378.137; //definição do raio da terra em km, ou seja, a distância entre os pontos também é retornada km

    // o arquivo de entrada tem as coordenadas dos pontos em graus. Aqui, para atender à Função de Haversine, as coordenadas devem ser passadas para radianos
    double lat1 = p1.latitude * M_PI / 180.0;
    double lon1 = p1.longitude * M_PI / 180.0;
    double lat2 = p2.latitude * M_PI / 180.0;
    double lon2 = p2.longitude * M_PI / 180.0;

    // para simplificar a leitura da fórmula, apenas a variação da latitude e longitude em comparação aos dois pontos.
    double delta_lat = lat2 - lat1;
    double delta_lon = lon2 - lon1;

    return 2 * raio_da_terra * asin(sqrt(pow(sin(delta_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(delta_lon / 2), 2)));
}

// função que recebe como parâmetros dois pontos, e retorna a distância entre eles baseada na fórmula da distância entre dois pontos cartesianos, para o caso de arquivos com pontos cartesioanos.
double distancia_dois_pontos_cartesianos(ponto p1, ponto p2) {
    //utilizamos ainda os termos latitude e longitude, porém se referem às coordenadas x e y dos pontos
    double x1 = p1.latitude;
    double y1 = p1.longitude;
    double x2 = p2.latitude;
    double y2 = p2.longitude;

    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

// função que constrói a matriz de distâncias do grafo, recebe como parâmetros o vetor de pontos e a string tipo de coordenada, que define qual fórmula para a distância será utilizada
vector<vector<double>> construir_matriz_distancias(const vector<ponto> &pontos, const string &tipo_de_coordenada) {
    //quantidade de vértices
    int n = pontos.size();

    //inicializa a matriz com todas as distâncias zeradas
    vector<vector<double>> matriz(n, vector<double>(n, 0.0));

    //realiza a fórmula da distância entre todos os pontos e armazena os resultados na matriz de distâncias.
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double distancia;
            //caso GEO indica coordenadas geográficas, logo, deve-se usar a fórmula de Haversine
            if (tipo_de_coordenada == "GEO") {
                distancia = distancia_dois_pontos_haversine(pontos[i], pontos[j]);
            } 
            //o else implica no caso EUC_2D, que indica coordenadas euclidianas, logo, deve-se usar a fórmula da distâncias entre dois pontos do plano cartesiano
            else {
                distancia = distancia_dois_pontos_cartesianos(pontos[i], pontos[j]);
            }
            matriz[i][j] = matriz[j][i] = distancia;
        }
    }
    return matriz;
}

// algoritmo de inserção mais próxima. Retorna um vetor do tipo vp, em que cada elemento é uma tupla, sendo o primeiro elemento do tipo ponto e o segundo um double. Como parâmetros, recebe o vetor de pontos que armazena os vértices do grafo, além da matriz de distância para acessar a distância entre dois pontos em tempo constante
vp insercao_mais_proxima(vector<ponto> &pontos, const vector<vector<double>> &matriz_distancias) {
    vp ciclo;

    //arbitrariamente o algoritmo inicia do primeiro ponto do arquivo de entrada. Porém, como ao final o ciclo é encontrado, partindo de qualquer ponto teremos o mesmo resultado.
    ciclo.push_back({pontos[0], 0.0});
    pontos[0].visitado = true;

    int id_mais_proximo = -1;
    double menor_distancia = numeric_limits<double>::max();

    //encontra o ponto mais próximo ao ponto inicial
    for (int i = 1; i < pontos.size(); i++) {
        double distancia = matriz_distancias[0][i];
        if (distancia < menor_distancia) {
            menor_distancia = distancia;
            id_mais_proximo = i;
        }
    }

    //insere o ponto mais próximo ao ponto inicial no ciclo e o marca como visitado
    ciclo.push_back({pontos[id_mais_proximo], menor_distancia});
    pontos[id_mais_proximo].visitado = true;

    //percorre os demais pontos 
    while (ciclo.size() < pontos.size()) {
        menor_distancia = numeric_limits<double>::max();
        id_mais_proximo = -1;

        //pega um primeiro ponto e encontra o ponto mais próximo à esse primeiro
        for (int i = 0; i < pontos.size(); i++) {
            if (!pontos[i].visitado) {
                for (int j = 0; j < ciclo.size(); j++) {
                    double distancia = matriz_distancias[i][ciclo[j].first.id - 1];
                    if (distancia < menor_distancia) {
                        menor_distancia = distancia;
                        id_mais_proximo = i;
                    }
                }
            }
        }

        double menor_peso = numeric_limits<double>::max();
        int melhor_posicao = -1;

        for (int i = 0; i < ciclo.size(); i++) {
            int ponto1 = ciclo[i].first.id - 1; //primeiro ponto
            int ponto2 = ciclo[(i + 1) % ciclo.size()].first.id - 1; //acessa o ponto seguinte e, pela operação de resto, garante que, ao final do ciclo, o último ponto se conecte de volta ao primeiro

            //custo da inserção do novo ponto no ciclo, garantindo que as menores arestas serão construídas
            double peso = matriz_distancias[id_mais_proximo][ponto1] +
                          matriz_distancias[id_mais_proximo][ponto2];

            if (peso < menor_peso) {
                menor_peso = peso;
                melhor_posicao = i;
            }
        }

        ciclo.insert(ciclo.begin() + melhor_posicao + 1, {pontos[id_mais_proximo], menor_distancia});
        pontos[id_mais_proximo].visitado = true;
    }

    return ciclo;
}

int main() {
    string linha;
    int numero_de_vertices;
    string tipo_de_coordenada;

    while (linha != "NODE_COORD_SECTION") {
        getline(cin, linha);

        // a cada linha lida busca a palavra DIMENSION, indicando que naquela linha terá a quantidade de vértices.
        if (linha.find("DIMENSION") != string::npos) {
            stringstream ss(linha);
            string lixo;
            ss >> lixo >> lixo >> numero_de_vertices;
        }
        
        // realiza a mesma coisa feita para encontrar a dimensão do grafo, porém, agora, para encontrar o tipo das coordenadas, que definirá a fórmula usada para calcular as distâncias
        if (linha.find("EDGE_WEIGHT_TYPE") != string::npos) {
            stringstream ss(linha);
            string lixo;
            ss >> lixo >> lixo >> tipo_de_coordenada;
        }
    }

    vector<ponto> pontos;

    //realiza a leitura dos pontos e define visitado como falso para cada ponto
    for (int i = 0; i < numero_de_vertices; i++) {
        getline(cin, linha);

        stringstream ss(linha);

        ponto p;
        ss >> p.id >> p.latitude >> p.longitude;
        p.visitado = false;

        pontos.push_back(p);
    }

    // Construir a matriz de distâncias
    vector<vector<double>> matriz_distancias = construir_matriz_distancias(pontos, tipo_de_coordenada);

    vp ciclo = insercao_mais_proxima(pontos, matriz_distancias);

    //inserindo no ciclo o retorno ao primeiro vértice partindo do último vértice do ciclo
    ciclo.push_back({ciclo[0].first, matriz_distancias[ciclo[0].first.id - 1][ciclo.back().first.id - 1]});
    double maior_distancia = 0;

    for (int i = 0; i < ciclo.size(); i++) {
        cout << ciclo[i].first.id << " ";

        if (ciclo[i].second > maior_distancia) {
            maior_distancia = ciclo[i].second;
        }
    }

    cout << endl <<"A distância máxima é de: " << maior_distancia << endl;

    return 0;
}
