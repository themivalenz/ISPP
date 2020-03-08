#include <iostream>
#include <algorithm>
#include <chrono> 
#include <fstream>
#include <random>
#include <string>
#include <vector>
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

/* Se cargan los polígonos de la instancia */
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
    return figuras; 
}

/* Se obtienen los rectángulos que contienen a cada polígono. 
 * Cada rectángulo queda descrito por dos vértices opuestos,
 * en este caso se utiliza el vértice inferior izquierdo y el 
 * vértice superior derecho. 
*/
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
    return rectangulos;
}

/* Se rota el rectángulo respecto al vértice inferior.
 * Sólo se rota en 90 grados, ya que otros ángulos de
 * rotación son equivalentes (no se consideran ángulos 
 * continuos).
*/
void rotateRectangle(Figura rectangulo) {
    int x0=rectangulo.vertices[0][0];
    int y0=rectangulo.vertices[0][1];
    int x1=rectangulo.vertices[1][0];
    int y1=rectangulo.vertices[1][1];
    rectangulo.vertices[1][0] = x0 + (y1-y0);
    rectangulo.vertices[1][1] = y0 + (x1-x0);
}

/* Determina si ubicar un rectángulo en cierta posición produce
 * solapamiento. Retorna true si hay overlap, y false en caso 
 * contrario.
*/
const bool overlap(vector<int> heights, int start, int end) {
    int t = heights[start];
    for (int i = start+1; i < end; i++) {
        if (heights[i] != t) {
            return true;
        }
    }
    return false;
}

/* Se obtiene el valor de la coordenada-y desde la cual no se produce 
 * overlap para ubicar el siguiente rectángulo.
*/
const int getHeight(vector<int> heights, int start, int end) {
    int t = heights[start];
    for (int i = start+1; i < end; i++) {
        if (heights[i] != t) {
            t = heights[i];
            break;
        }
    }
    return t;
} 

/* Se ubican los rectángulos en el contenedor de ancho w, y se 
 * retorna el largo necesario para ubicarlos todos sin que se 
 * solapen.
*/
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
            if (!overlap(heights, x, x + width_i)) {
                heights[j] += length_i;
            } else {
                heights[j] = getHeight(heights, x, x + width_i) + length_i;
            }
            if (heights[j] > L) {
                L = heights[j];
            }
        }
        x += width_i;
    }
    return L;
}

/* Realiza un swap entre dos rectángulos dentro de la permutación */
const vector<Figura> swap(vector<Figura> rectangulos, int i, int j) {
    vector<Figura> rectangles;
    Figura t_i = rectangulos[i];
    Figura t_j = rectangulos[j];
    for (int k = 0; k < rectangulos.size(); k++) {
        if (k == i) {
            rectangles.push_back(t_j);
        } else if (k == j) {
            rectangles.push_back(t_i);
        } else {
            rectangles.push_back(rectangulos[k]);
        }
    }
    return rectangles;
}

/* Permite calcular el área total que ocupan los rectángulos */
const int totalAreaRectangles(vector<Figura> rectangulos) {
    int total = 0;
    for (int i = 0; i < rectangulos.size(); i++) {
        int x0 = rectangulos[i].vertices[0][0];
        int y0 = rectangulos[i].vertices[0][1];
        int x1 = rectangulos[i].vertices[1][0];
        int y1 = rectangulos[i].vertices[1][1];
        total += (x1 - x0)*(y1 - y0);
    }
    return total;
}

/* Permite mostrar por pantalla la permutación de rectángulos. 
 * Muestra el ID del poligono asociado
*/
void printRectangles(vector<Figura> rectangulos) {
    for (int i = 0; i < rectangulos.size(); i++) {
        cout << rectangulos[i].ID << " ";
    }
    cout << endl;
}

/* Permite generar una nueva permutación de rectángulos */
const vector<Figura> shuffleRectangles(vector<Figura> rectangulos) {
    vector<int> v;
    vector<Figura> rectangles;
    int size = rectangulos.size();
    for (int i = 0; i < size; i++) {
        v.push_back(i);
    }
    shuffle(v.begin(), v.end(), default_random_engine {unsigned(time(0))});
    for (int i = 0; i < size; i++) {
        rectangles.push_back(rectangulos[v[i]]);
    }
    return rectangles;
}