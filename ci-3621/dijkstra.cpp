#define MAX_N 1000
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <limits>

using namespace std;

struct arco {
   int destino;
   double costo;
};

vector<arco> adyacentes[MAX_N];
int N;

void limpiar_grafo() {
	for (int i=0; i<N; i++)
		adyacentes[i].clear();
}

void leer_grafo() {
	int M;
	cin >> N >> M;
	while (M--) {
		int origen;
		arco nuevo_arco;
		cin >> origen >> nuevo_arco.destino >> nuevo_arco.costo;
		adyacentes[origen].push_back(nuevo_arco);
	}
}

struct info {
	int nodo;
	double costo_acumulado;
	info(int nodo, double costo_acumulado) :
		nodo(nodo),
		costo_acumulado(costo_acumulado) {}
};

struct comparador_para_info {
	bool operator()(info const& primero, info const& segundo) {
		return primero.costo_acumulado > segundo.costo_acumulado;
	}
};

bool visitado[MAX_N];

double dijkstra(int desde, int hasta) {
	priority_queue<info, vector<info>, comparador_para_info> cola;
	cola.push(info(desde, 0));
	memset(visitado, false, sizeof(visitado));
	while (!cola.empty()) {
		info actual = cola.top();
		cola.pop();
		if (visitado[actual.nodo])
			continue;
		visitado[actual.nodo] = true;
		if (actual.nodo == hasta)
			return actual.costo_acumulado;
		for (int i=0; i<adyacentes[actual.nodo].size(); i++) {
			arco siguiente = adyacentes[actual.nodo][i];
			cola.push(info(siguiente.destino,
			               actual.costo_acumulado + siguiente.costo));
		}
	}
	return numeric_limits<double>::infinity();
}

int main() {
	int T;
	cin >> T;
	while (T--) {
		leer_grafo();
		int Q;
		cin >> Q;
		while (Q--) {
			int desde, hasta;
			cin >> desde >> hasta;
			cout << dijkstra(desde, hasta) << endl;
		}
		limpiar_grafo();
	}
	return 0;
}