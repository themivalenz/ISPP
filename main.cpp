#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/* Definimos la clase figura */
class Figura {
    public:
        int ID;
        vector<vector<int>> vertices;
};

const vector<string> explode(const string& s, const char& c);
const vector<Figura> getfigures(string filename);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int w = strtof(argv[2]);
        string filename(argv[1]);
        vector<Figura> figuras = getfigures(filename);
    }
    return 0;
}

/* Funcion que permite hacer split de strings */
const vector<string> explode(const string& s, const char& c) {
	string buff{""};
	vector<string> v;
	for(auto n:s) {
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	return v;
}

/* Obtenemos todas las figuras */
const vector<Figura> getfigures(string filename) {
    ifstream archivo(filename);
    string linea;
    vector<Figura> figuras;
    while (getline(archivo, linea)) {
        vector<string> fig_info{explode(linea, ' ')};
        int ID = stoi(fig_info[0]);
        int quantity = stoi(fig_info[1]);
        string temp = fig_info[2];
        vector<string> temp_vertices{explode(temp, ';')};
        vector<vector<int>> vertices;
        for(auto n:temp_vertices) {
            vector<string> coord{explode(n, ',')};
            vector<int> coordenada;
            coordenada.push_back(stoi(coord[0]));
            coordenada.push_back(stoi(coord[1]));
            vertices.push_back(coordenada); 
        }
        for(int i = 0; i < quantity; i++) {
            Figura fig;
            fig.ID = ID;
            fig.vertices = vertices;
            figuras.push_back(fig);
        }
    }
    return figuras; 
}