#define MAX_CAP 1000
#define MAX_N 1000
#include <iostream>
#include <queue>

using namespace std;

int capacidad[MAX_N][MAX_N], flujo[MAX_N][MAX_N];
int padre[MAX_N];
bool visitado[MAX_N];
int N;

void limpiar_grafo() {
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++) {
            capacidad[i][j] = 0;
            flujo[i][j] = 0;
        }
}

void leer_grafo() {
    int M;
    cin >> N >> M;
    limpiar_grafo();
    while (M--) {
        int origen, destino, cap;
        cin >> origen >> destino >> cap;
        
        capacidad[origen][destino] = cap;
        flujo[origen][destino] = 0;
        
        capacidad[destino][origen] = cap;
        flujo[destino][origen] = cap;
    }
}

bool bfs(int origen, int destino) {
    queue<int> cola;
    cola.push(origen);
    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        if (actual == destino)
            return true;
        if (visitado[actual])
            continue;
        visitado[actual] = true;
        for (int sig=0; sig<N; sig++) {
            if (flujo[actual][sig] < capacidad[actual][sig] && !visitado[sig]) {
                padre[sig] = actual;
                cola.push(sig);
            }
        }
    }
    return false;
}

bool hallar_camino_en_aumento(int origen, int destino) {
    for (int i=0; i<N; i++)
        visitado[i] = false;
    return bfs(origen, destino);
}

int obtener_flujo_camino(int origen, int destino) {
    int flujo_camino = MAX_CAP;
    while (origen != destino) {
        int anterior = padre[destino];
        flujo_camino = min(flujo_camino, capacidad[anterior][destino] - flujo[anterior][destino]);
        destino = anterior;
    }
    return flujo_camino;
}

void actualizar_flujo(int origen, int destino, int flujo_camino) {
    while (origen != destino) {
        int anterior = padre[destino];
        flujo[anterior][destino] += flujo_camino;
        flujo[destino][anterior] -= flujo_camino;
        destino = anterior;
    }
}

int maximo_flujo(int origen, int destino) {
    int flujo_total = 0;
    while (hallar_camino_en_aumento(origen, destino)) {
        int flujo_camino = obtener_flujo_camino(origen, destino);
        flujo_total += flujo_camino;
        actualizar_flujo(origen, destino, flujo_camino);
    }
    return flujo_total;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        leer_grafo();
        int origen, destino;
        cin >> origen >> destino;
        cout << "El flujo máximo desde " << origen << " hasta " << destino
             << " es: " << maximo_flujo(origen, destino) << "." << endl;
    }
    return 0;
}