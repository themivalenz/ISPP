#include <iostream>
#include <algorithm>
#include <chrono> 
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc <= 3) {
        cout << "Error: se requiere nombre de archivo, ancho del contenedor y maximo de iteraciones" << endl;
        return 0;
    }
    
    /* Ancho del contenedor */
    int w = stoi(argv[2]);

    /* Máxima cantidad de iteraciones */
    int maxIter = stoi(argv[3]);

    /* Se cargan las figuras de la instancia */
    string filename(argv[1]);
    vector<Figura> figuras = getFigures(filename);

    /* Se obtienen los rectangulos que contienen a los poligonos */
    vector<Figura> rectangulos = getRectangles(figuras);

    /* Se calcula el área que ocupan los rectángulos */
    int areaRectangles = totalAreaRectangles(rectangulos);
    
    cout << "Calculating..." << endl;

    /* Se comienza a medir el tiempo de ejecución */
    auto start = high_resolution_clock::now();

    /* Solución inicial: una permutación de rectángulos */
    rectangulos = shuffleRectangles(rectangulos);

    /* Aquí se almacena la mejor solución encontrada */
    int bestL = putRectangles(rectangulos, w);
    vector<Figura> bestPermutation = rectangulos;

    /* Hill Climbing con Alguna Mejora */
    for (int k = 0; k < maxIter; k++) {
        
        /* Se genera una nueva permutación de rectángulos (exploración) */
        rectangulos = shuffleRectangles(rectangulos);

        /* Se realiza movimiento swap hasta encontrar alguna mejora (explotación) */
        for (int i = 0; i < rectangulos.size(); i++) {
            bool flag = false;
            for (int j = 0; j < i; j++) {
                vector<Figura> rect = swap(rectangulos, i, j);
                int L = putRectangles(rect, w);
                if (L < bestL) {
                    flag = true;
                    bestL = L;
                    bestPermutation = rect;
                    break;
                }
            }
            if (flag) {
                break;
            }
        }
    }

    /* Se obtiene tiempo de cómputo */
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    /* Se calcula el área total del contenedor */
    int areaTotal = bestL*w;

    /* Se calcula el porcentaje efectivo de uso */
    float usage = (100.0*areaRectangles)/areaTotal;

    cout << "Solution:" << endl;
    cout << printRectangles;
    cout << "Length: " << bestL << endl;
    cout << "Usage %: " << usage << endl;
    cout << "Time: " << duration.count() << "[ms]" << endl;

    return 0;
}