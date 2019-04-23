#define MAX_N 1000
#include <iostream>
#include <cstring>
#include <limits>

using namespace std;

double grafo[MAX_N][MAX_N];
int N;

void leer_grafo() {
	int M;
	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			grafo[i][j] = numeric_limits<double>::infinity();
	cin >> M;
	while (M--) {
		int origen, destino;
		cin >> origen >> destino >> grafo[origen][destino];
	}
}

double costo_minimo[MAX_N];

bool bellman_ford(int desde) {
	for (int i = 0; i < N; i++)
		costo_minimo[i] = numeric_limits<double>::infinity();
	costo_minimo[desde] = 0;
	int iters;
	for (iters = 0; iters <= N; iters++) {
		bool cambio = false;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			if (costo_minimo[j] + grafo[j][i] < costo_minimo[i]) {
				costo_minimo[i] = costo_minimo[j] + grafo[j][i];
				cambio = true;
			}
		if (!cambio)
			break;
	}
	return iters == N+1;
}

int main() {
	int T;
	cin >> T;
	while (T--) {
		leer_grafo();
		int Q;
		cin >> Q;
		while (Q--) {
			int desde;
			cin >> desde;
			bool ciclo_negativo = bellman_ford(desde);
			if (ciclo_negativo)
				cout << "Existe un ciclo de costo negativo!" << endl;
			else {
				for (int i = 0; i < N; i++)
					cout << "El camino de costo minimo entre "
					     << desde << " y " << i << " cuesta: "
					     << costo_minimo[i] << endl;
			}
		}
	}
	return 0;
}