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
    shuffleRectangles(rectangulos);

    /* Aquí se almacena la mejor solución encontrada */
    int bestL = putRectangles(rectangulos, figuras, w);
    vector<Figura> bestPermutation = rectangulos;
    int areaTotal = bestL*w;
    float usage = (100.0*areaRectangles)/areaTotal;
    
    /* Si la solución anterior es infactible, se busca una nueva */
    while(usage > 100) {
        shuffleRectangles(rectangulos);
        bestL = putRectangles(rectangulos, figuras, w);
        bestPermutation = rectangulos;
        areaTotal = bestL*w;
        usage = (100.0*areaRectangles)/areaTotal;
    }

    /* Hill Climbing con Alguna Mejora */
    for (int k = 0; k < maxIter; k++) {
        
        /* Se genera una nueva permutación de rectángulos (exploración) */
        shuffleRectangles(rectangulos);

        /* Se realiza movimiento swap hasta encontrar alguna mejora (explotación) */
        for (int i = 0; i < rectangulos.size(); i++) {
            bool flag = false;
            for (int j = 0; j < i; j++) {
                swap(rectangulos, i, j);
                int L = putRectangles(rectangulos,figuras, w);
                int area = L*w;
                float usage = (100.0*areaRectangles)/area;
                if (usage <= 100.0 && L < bestL) {
                    flag = true;
                    bestL = L;
                    bestPermutation = rectangulos;
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
    areaTotal = bestL*w;

    /* Se calcula el porcentaje efectivo de uso */
    usage = (100.0*areaRectangles)/areaTotal;

    cout << "====== Best solution ======" << endl;
    cout << "Length: " << bestL << "; ";
    cout << "Usage %: " << usage << "; ";
    cout << "Time: " << double(end - start)/CLOCKS_PER_SEC << "[s]" << endl;
    printSolution(bestPermutation, figuras, w);
    return 0;
}
