#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct ponto{
    int id;
    double latitude, longitude;
};

int main(){
    ifstream arquivo("../problema_das_paradas_de_onibus/instances/08.ins");
    
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

        vertices.push_back(p);
    }

    for(int i = 0; i < vertices.size(); i++){
        cout << vertices[i].id << " - Latitude: " << vertices[i].latitude << " | Longitude: " << vertices[i].longitude << endl;
    }

    return 0;
}