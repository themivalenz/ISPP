#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;

/* Funcion que permite hacer split de strings. Recibe como parámetro 
 * un string y un caracter delimitador, y retorna un vector con los 
 * substrings correspondientes
*/
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

/* Se cargan los polígonos de la instancia. Recibe como parámetro el nombre del
 * archivo de texto de la instancia, y retorna un vector de instancias de la clase
 * Figura (es decir, los polígonos).
*/
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

/* Se obtienen los rectángulos que contienen a cada polígono. Recibe
 * como parámetro un vector de instancias de la clase Figura (los 
 * polígonos), y retorna un vector de instancias de la clase Figura (los 
 * rectángulos contenedores). Cada rectángulo queda descrito 
 * por dos vértices opuestos, en este caso se utiliza el 
 * vértice inferior izquierdo y el vértice superior derecho. 
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

/* Determina si ubicar un rectángulo en cierta posición produce
 * solapamiento. Recibe el vector de alturas heights, el índice 
 * del inicio y del fin del tramo que debe revisar. Retorna true
 * si hay overlap, y false en caso contrario.
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
 * overlap para ubicar el siguiente rectángulo. Recibe el vector de alturas 
 * heights, el índice del inicio y del fin del tramo que debe revisar. Retorna
 * la máxima altura encontrada.
*/
const int getHeight(vector<int> heights, int start, int end) {
    int t = heights[start];
    for (int i = start+1; i < end; i++) {
        if (heights[i] > t) {
            t = heights[i];
        }
    }
    return t;
}

/* Recibe un vector de instancias de la clase Figura y el ID de la
 * figura de la cual se quiere obtener sus vértices. Retorna el vector
 * con los vértices de la correspondiente figura.
*/
vector<vector<int>> getVertex(vector<Figura> figuras, int ID) {
    vector<vector<int>> vertex;
    for (int i = 0; i < figuras.size(); i++) {
        if (figuras[i].ID == ID) {
            vertex = figuras[i].vertices;
            break;
        }
    }
    return vertex;
}

/* Se ubican los rectángulos en el contenedor de ancho w, y se retorna el largo 
 * necesario para ubicarlos todos sin que se solapen. Para ello recibe dos 
 * vectores de instancias de la clase Figura: figuras (que contiene los polígonos)
 * y rectangulos (que contiene los rectángulos contenedores), además del ancho w
 * del container
*/
const int putRectangles(vector<Figura> rectangulos, vector<Figura> figuras, int w) {
    int L = -1;
    int x = 0;
    int h = 0;
    vector<int> heights(w, 0); 
    for (int i = 0; i < rectangulos.size(); i++) {
        /* largo del rectángulo actual (eje y) */
        int length_i = rectangulos[i].vertices[1][1] - rectangulos[i].vertices[0][1];
        /* ancho del rectángulo actual (eje x) */
        int width_i = rectangulos[i].vertices[1][0] - rectangulos[i].vertices[0][0];
	/* Si el rectángulo no cabe en la posición x actual, se reinicia su valor en 0 */
        if (x + width_i > w) {
            x = 0;
        }
	/* Si no hay overlap, se actualiza valor de altura con el valor actual de heights*/
        if (!overlap(heights, x, x + width_i)) {
            h = heights[x] + length_i;
        } else { /* si hay overlap, se obtiene máxima altura y se actualiza valor de altura */
            h = getHeight(heights, x, x + width_i) + length_i;
        }
	/* se actualiza el valor de altura máxima */
        L = max(L, h + length_i);
	/* se actualiza altura en el arreglo heights */
        for (int j = x; j < x + width_i; j++) {
            heights[j] = h;
        }
	/* se actualiza el valor de x */
        x += width_i;
    }
    return L;
}

/* Realiza un swap entre dos rectángulos dentro de la permutación */
void swap(vector<Figura> &rectangulos, int i, int j) {
    Figura t_j = rectangulos[j];
    rectangulos[j] = rectangulos[i];
    rectangulos[i] = t_j;
}

/* Permite calcular el área total que ocupan los rectángulos. Recibe un
 * vector de instancias de la clase Figura (correspondiente a los
 * rectángulos contenedores) y retorna el área total que ocupan
*/
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

/* Permite generar una nueva permutación de rectángulos */
void shuffleRectangles(vector<Figura> &rectangulos) {
    shuffle(rectangulos.begin(), rectangulos.end(), default_random_engine {unsigned(time(0))});
}

/* Función para mostrar los valores de las coordenadas de los polígonos tras ser ubicados
 * en el container.
*/
void printSolution(vector<Figura> rectangulos, vector<Figura> figuras, int w) {
    int x = 0;
    int h = 0;
    vector<int> heights(w, 0); 
    for (int i = 0; i < rectangulos.size(); i++) {
        // eje y
        int dy = rectangulos[i].vertices[1][1] - rectangulos[i].vertices[0][1];
        // eje x
        int dx = rectangulos[i].vertices[1][0] - rectangulos[i].vertices[0][0];
        if (x + dx > w) {
            x = 0;
        } else {
            if (!overlap(heights, x, x + dx)) {
                h = heights[x] + dy;
            } else {
                h = getHeight(heights, x, x + dx) + dy;
            }
        }
        for (int j = x; j < x + dx; j++) {
            heights[j] = h;
        }
        vector<vector<int>> vertex = getVertex(figuras, rectangulos[i].ID);
        for (int k = 0; k < vertex.size(); k++) {
            int xcoor = vertex[k][0];
            int ycoor = vertex[k][1]; 
            xcoor += x;
            ycoor += h;
            cout << "(" << xcoor << "," << ycoor << ") ";
        }
        cout << endl;
        x += dx;
    }
}
