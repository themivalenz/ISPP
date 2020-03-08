#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc <= 3) {
        cout << "[Error] Se requieren los siguientes parámetros: ";
        cout << "<nombre_archivo.txt> <ancho_de_contenedor> <numero_de_iteraciones>" << endl;
        return 0;
    }
    
    /* Ancho del contenedor */
    int w = stoi(argv[2]);

    /* Máxima cantidad de iteraciones */
    int maxIter = 500;

    /* Se cargan las figuras de la instancia */
    string filename(argv[1]);
    vector<Figura> figuras = getFigures(filename);

    /* Se obtienen los rectangulos que contienen a los poligonos */
    vector<Figura> rectangulos = getRectangles(figuras);

    /* Se calcula el área que ocupan los rectángulos */
    int areaRectangles = totalAreaRectangles(rectangulos);
    
    cout << "Calculating..." << endl;

    /* Se comienza a medir el tiempo de ejecución */
    clock_t start = clock();

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
    clock_t end = clock();

    /* Se calcula el área total del contenedor */
    int areaTotal = bestL*w;

    /* Se calcula el porcentaje efectivo de uso */
    float usage = (100.0*areaRectangles)/areaTotal;

    cout << "Solution:" << endl;
    printRectangles(bestPermutation);
    cout << "Length: " << bestL << endl;
    cout << "Usage %: " << usage << endl;
    cout << "Time: " << (end - start)/CLOCKS_PER_SEC << "[s]" << endl;
    return 0;
}