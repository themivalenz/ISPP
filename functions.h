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
const vector<Figura> getFigures(string filename);
const vector<Figura> getRectangles(vector<Figura> figuras);
void rotateRectangle(Figura rectangulo);
const int putRectangles(vector<Figura> rectangulos, int w);