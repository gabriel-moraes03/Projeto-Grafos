#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct ponto{
    int id;
    double latitude, longitude;
    bool visitado;
};

double distancia_dois_pontos_haversine(ponto p1, ponto p2){
    float raio_da_terra = 6378.137;

    double lat1 = p1.latitude * M_PI / 180.0;
    double lon1 = p1.longitude * M_PI / 180.0;
    double lat2 = p2.latitude * M_PI / 180.0;
    double lon2 = p2.longitude * M_PI / 180.0;

    double delta_lat = lat2 - lat1;
    double delta_lon = lon2 - lon1;

    return (2*raio_da_terra*asin(sqrt((pow(sin((delta_lat)/2),2)) + (cos(lat1) * cos(lat2) * (pow(sin((delta_lon)/2),2))))));
}

class KDNode{
    friend class KDTree;

    private:
        ponto data;
        KDNode* left;
        KDNode* right;
    
    public:
        KDNode (ponto p){
            data = p;
            left = nullptr;
            right = nullptr;
        }
};

class KDTree{
    private:
        KDNode* root; 

        KDNode* build (vector<ponto>& pontos, int depth ){
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
        }

        void nearestNeighbor(KDNode* node, const ponto& target, int depth, ponto& best, double& bestDist){
            int dimension = depth % 2;

            double d = distancia_dois_pontos_haversine(node->data, target);
            if (d < bestDist){
                bestDist = d;
                best = node->data;
            }

            KDNode* next = nullptr;
            KDNode* other = nullptr;

            if ((dimension == 0 && target.latitude < node->data.latitude) or (dimension == 1 && target.longitude < node->data.longitude)) {
            next = node->left;
            other = node->right;
            } 
            else {
                next = node->right;
                other = node->left;
            }

            nearestNeighbor(next, target, depth + 1, best, bestDist);

            double delta = (dimension == 0) ? fabs(target.latitude - node->data.latitude) : fabs(target.longitude - node->data.longitude);
        }

    public:
        KDTree(){
            root = nullptr;
        }

        void buildTree (vector<ponto>& pontos){
            root = build (pontos, 0);
        }

        ponto findNearestNeighbor(const ponto& target){
            ponto best;
            double bestDist = numeric_limits<double>::max();

            nearestNeighbor(root, target, 0, best, bestDist);

            return best;
        }
};

int main(){
    ifstream arquivo("../problema_das_paradas_de_onibus/instances/01.ins");
    
    string linha;

    int numero_de_vertices;

    while (linha != "NODE_COORD_SECTION"){
        getline(arquivo,linha);
        
        if (linha.find("DIMENSION") != string::npos) {
            stringstream ss(linha);
            string lixo;
            
            // Ignora os dados anteriores ao número (como "DIMENSION :")
            ss >> lixo >> lixo >> numero_de_vertices;  // Ignora "DIMENSION" e ":"
        }
    }    

    cout << "Número de vértices: " << numero_de_vertices << endl;

    vector <ponto> vertices;

    for (int i = 0; i < numero_de_vertices; i++){
        getline(arquivo,linha);

        stringstream ss (linha);

        ponto p;

        ss >> p.id >> p.latitude >> p.longitude;

        p.visitado = false;

        vertices.push_back(p);
    }

    KDTree tree;
    tree.buildTree(vertices);

    
    
    return 0;
}