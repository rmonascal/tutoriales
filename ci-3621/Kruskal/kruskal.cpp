#define MAX_N 1000
#include <iostream>
#include <queue>

using namespace std;

int padre[MAX_N];
int N;

void inicializar_conjuntos_disjuntos() {
    for (int i = 0 ; i < N ; i++)
        padre[i] = i;
}

int buscar_representante(int elem) {
    if (padre[elem] == elem)
        return elem;
    return padre[elem] = buscar_representante(padre[elem]);
}

bool unir_conjuntos(int elem_a, int elem_b) {
    int repr_a = buscar_representante(elem_a);
    int repr_b = buscar_representante(elem_b);
    if (repr_a == repr_b)
        return false;
    padre[repr_a] = repr_b;
    return true;
}

struct conexion {
    int origen;
    int destino;
    double costo;
};

struct comparador_para_conexiones {
    bool operator()(conexion const& primera, conexion const& segunda) {
        return primera.costo > segunda.costo;
    }
};

priority_queue<conexion, vector<conexion>, comparador_para_conexiones> conexiones;

void leer_conexiones() {
    int M;
    cin >> N >> M;
    while (M--) {
        conexion con;
        cin >> con.origen >> con.destino >> con.costo;
        conexiones.push(con);
    }
}

double kruskal() {
    double costo_total = 0;
    inicializar_conjuntos_disjuntos();
    while (!conexiones.empty()) {
        conexion con = conexiones.top();
        conexiones.pop();
        if (unir_conjuntos(con.origen, con.destino))
            costo_total += con.costo;
    }
    return costo_total;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        leer_conexiones();
        cout << "El costo de un árbol cobertor minimo es: " << kruskal() << "." << endl;
    }
    return 0;
}