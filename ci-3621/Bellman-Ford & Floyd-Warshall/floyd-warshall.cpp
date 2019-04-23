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

double costo_minimo[MAX_N][MAX_N];

bool floyd_warshall() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			costo_minimo[i][j] = grafo[i][j];
		grafo[i][i] = 0;
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				if (costo_minimo[i][k] + costo_minimo[k][j] < costo_minimo[i][j])
					costo_minimo[i][j] = costo_minimo[i][k] + costo_minimo[k][j];
	for (int i = 0; i < N; i++)
		if (costo_minimo[i][i] < 0)
			return true;
	return false;
}

int main() {
	int T;
	cin >> T;
	while (T--) {
		leer_grafo();
		int Q;
		cin >> Q;
		bool ciclo_negativo = floyd_warshall();
		if (ciclo_negativo)
			cout << "Existe un ciclo de costo negativo! "
			     << "Los valores a continuacion pudieran estar mal formados" << endl;
		else
			cout << "No hay ciclos de costo negativo! "
			     << "Los valores a continuacion son confiables" << endl;
		while (Q--) {
			int desde, hasta;
			cin >> desde >> hasta;
			cout << "El camino de costo minimo entre "
			     << desde << " y " << hasta << " cuesta: "
			     << costo_minimo[desde][hasta] << endl;
		}
		cout << endl;
	}
	return 0;
}