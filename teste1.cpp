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

class KDNode {
    friend class KDTree;

private:
    ponto data;
    KDNode* left;
    KDNode* right;

public:
    KDNode(ponto p) {
        data = p;
        left = nullptr;
        right = nullptr;
    }
};

class KDTree {
    private:
        KDNode* root;

        KDNode* build(vector<ponto>& pontos, int depth) {
            if (pontos.empty()) {
                return nullptr;
            }

            int dimension = depth % 2;

            sort(pontos.begin(), pontos.end(), [dimension](const ponto& p1, const ponto& p2){
                return (dimension == 0) ? (p1.latitude < p2.latitude) : (p1.longitude < p2.longitude);
            });

            int mid = pontos.size() / 2;
            KDNode* node = new KDNode(pontos[mid]);

            vector<ponto> leftPoints(pontos.begin(), pontos.begin() + mid);
            vector<ponto> rightPoints(pontos.begin() + mid + 1, pontos.end());

            node->left = build(leftPoints, depth + 1);
            node->right = build(rightPoints, depth + 1);

            return node;
        }

        void nearestNeighbor(KDNode* node, const ponto& target, int depth, ponto& best, double& bestDist) {
            if (node == nullptr) return;

            // Se já foi visitado, apenas retorna
            if (node->data.visitado) {
                return;
            }

            int dimension = depth % 2;
            double d = distancia_dois_pontos_haversine(node->data, target);

            if (d < bestDist) {
                bestDist = d;
                best = node->data;
            }

            KDNode* next = (dimension == 0 && target.latitude < node->data.latitude) || 
                        (dimension == 1 && target.longitude < node->data.longitude)
                            ? node->left
                            : node->right;
            KDNode* other = next == node->left ? node->right : node->left;

            nearestNeighbor(next, target, depth + 1, best, bestDist);

            double delta = (dimension == 0) ? fabs(target.latitude - node->data.latitude) : fabs(target.longitude - node->data.longitude);
            if (delta < bestDist) {
                nearestNeighbor(other, target, depth + 1, best, bestDist);
            }
        }

        // Função para buscar o próximo ponto mais próximo quando KDTree não tem mais vizinhos diretos
        ponto findFallback(const ponto& target, const vector<ponto>& vertices) {
            ponto closest;
            double minDist = numeric_limits<double>::max();

            for (const auto& p : vertices) {
                if (!p.visitado) {
                    double d = distancia_dois_pontos_haversine(target, p);
                    if (d < minDist) {
                        minDist = d;
                        closest = p;
                    }
                }
            }

            return closest;
        }

    public:
        KDTree() : root(nullptr) {}

        void buildTree(vector<ponto>& pontos) {
            root = build(pontos, 0);
        }

        ponto findNearestNeighbor(const ponto& target, vector<ponto>& vertices) {
            ponto best;
            double bestDist = numeric_limits<double>::max();

            nearestNeighbor(root, target, 0, best, bestDist);

            // Se não encontramos nenhum vizinho pela KDTree, usamos o fallback
            if (bestDist == numeric_limits<double>::max()) {
                best = findFallback(target, vertices);
            }

            if (!best.visitado) {
                markAsVisited(root, best.id, vertices);
            }

            return best;
        }

        void markAsVisited(KDNode* node, int id, vector<ponto>& vertices) {
            if (node == nullptr) return;

            if (node->data.id == id) {
                node->data.visitado = true;
                auto it = find_if(vertices.begin(), vertices.end(), [id](const ponto& p) { return p.id == id; });
                if (it != vertices.end()) {
                    it->visitado = true;
                }
                return;
            }

            markAsVisited(node->left, id, vertices);
            markAsVisited(node->right, id, vertices);
        }

        KDNode* returnRoot(){
            return root;
        }
    };


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

    KDTree tree;
    tree.buildTree(vertices);

    int primeiroPonto = 50;

    // Marcar o ponto inicial como visitado
    vertices[primeiroPonto].visitado = true;
    tree.markAsVisited(tree.returnRoot(), vertices[primeiroPonto].id, vertices);

    vector<pair<int, double>> caminho;

    

    ponto atual = vertices[primeiroPonto];
    caminho.push_back({atual.id, 0.0});

    while (any_of(vertices.begin(), vertices.end(), [](const ponto& p) { return !p.visitado; })) {
        ponto proximo = tree.findNearestNeighbor(atual, vertices);

        double distancia = distancia_dois_pontos_haversine(atual, proximo);
        caminho.push_back({proximo.id, distancia});

        atual = proximo;
    }

    double distancia = distancia_dois_pontos_haversine(atual, vertices[primeiroPonto]);
    caminho.push_back({ vertices[primeiroPonto].id, distancia});

    cout << "Caminho percorrido: " << endl;

    for (const auto& [id, dist] : caminho) {
        cout << "id: " << id << " | Distância: " << dist << endl;
    }

    double maior_distancia = numeric_limits<double>::min(); // Inicializa com o menor valor possível

    // Iterando para encontrar a maior distância
    for (const auto& [id, dist] : caminho) {
        if (dist > maior_distancia) {
            maior_distancia = dist;
        }
    }

    cout << "A distância máxima entre dois vértices é de: " << maior_distancia;
    return 0;
}

