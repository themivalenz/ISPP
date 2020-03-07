#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "functions.h"

using namespace std;

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
const vector<Figura> getFigures(string filename) {
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
    cout << "All polygons getted!" << endl;
    return figuras; 
}

const vector<Figura> getRectangles(vector<Figura> figuras) {
    vector<Figura> rectangulos;
    for (int i = 0; i < figuras.size(); i++) {
        int xmin = 2147483647;
        int xmax = -2147483648;
        int ymin = 2147483647;
        int ymax = -2147483648;
        vector<int> min;
        vector<int> max;
        for (int j = 0; j < figuras[i].vertices.size(); j++) {
            int x = figuras[i].vertices[j][0];
            int y = figuras[i].vertices[j][1];
            if (x > xmax) {
                xmax = x;
            }
            if (y > ymax) {
                ymax = y;
            }
            if (x < xmin) {
                xmin = x;
            }
            if (y < ymin) {
                ymin = y;
            }
        }
        min.push_back(xmin);
        min.push_back(ymin);
        max.push_back(xmax);
        max.push_back(ymax);
        Figura fig;
        fig.ID = figuras[i].ID;
        fig.vertices.push_back(min);
        fig.vertices.push_back(max);
        rectangulos.push_back(fig);
    }
    cout << "All figures packaged!" << endl;
    return rectangulos;
}

void rotateRectangle(Figura rectangulo) {
    int x0=rectangulo.vertices[0][0];
    int y0=rectangulo.vertices[0][1];
    int x1=rectangulo.vertices[1][0];
    int y1=rectangulo.vertices[1][1];
    rectangulo.vertices[1][0] = x0 + (y1-y0);
    rectangulo.vertices[1][1] = y0 + (x1-x0);
}

const int putRectangles(vector<Figura> rectangulos, int w) {
    int L = -1;
    int x = 0;
    vector<int> heights(w, 0); 
    for (int i = 0; i < rectangulos.size(); i++) {
        int length_i = rectangulos[1].vertices[1][1];
        int width_i = rectangulos[1].vertices[1][0];
        if (x + width_i > w) {
            x = 0;
        }
        for (int j = x; j < x + width_i; j++) {
            heights[j] += length_i;
            if (heights[j] > L) {
                L = heights[j];
            }
        }
        x += width_i;
    }
    return L;
}