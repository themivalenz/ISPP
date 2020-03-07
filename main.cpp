#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "functions.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        int w = stoi(argv[2]);
        string filename(argv[1]);
        vector<Figura> figuras = getFigures(filename);
        vector<Figura> rectangulos = getRectangles(figuras);
        int L = putRectangles(rectangulos, w);
        cout << L << endl;
    } else {
        cout << "Error: se requiere nombre de archivo y ancho del contenedor" << endl;
    }
    return 0;
}