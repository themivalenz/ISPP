#include <string>
#include <vector>

using namespace std;

/* Se define la clase figura con los siguientes parámetros:
 * ID: identificador del polígono.
 * vertices: conjunto de vértices que definen al polígono
*/
class Figura {
    public:
        int ID;
        vector<vector<int>> vertices;
};


/* headers de funciones utilizadas. Cada una está descrita 
 * brevemente en functions.cpp
*/
const vector<string> explode(const string& s, const char& c);
const vector<Figura> getFigures(string filename);
const vector<Figura> getRectangles(vector<Figura> figuras);
void rotateRectangle(Figura &rectangulo);
vector<vector<int>> getVertex(vector<Figura> figuras, int ID);
const int putRectangles(vector<Figura> rectangulos, vector<Figura> figuras, int w);
const bool overlap(vector<int> heights, int start, int end);
const int getHeight(vector<int> heights, int start, int end);
void swap(vector<Figura> &rectangulos, int i, int j);
const int totalAreaRectangles(vector<Figura> rectangulos);
void shuffleRectangles(vector<Figura> &rectangulos);
void printSolution(vector<Figura> rectangulos, vector<Figura> figuras, int w);