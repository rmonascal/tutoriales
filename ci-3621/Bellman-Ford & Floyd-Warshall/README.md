# ¿Cómo implementar los algoritmos de Bellman-Ford y Floyd-Warshall en C++?

En este tutorial aprenderemos a implementar el algoritmo de Bellman-Ford para hallar los caminos de costo mínimo en un grafo dirigido, a partir de un nodo determinado, incluso si el grafo tiene costos negativos. También aprenderemos a implementar el algoritmo de Floyd-Warshall para hallar caminos de costo mínimo entre todos los pares de nodos. Para ambos algoritmos, en caso de existir ciclos de costos negativos, queremos reportar su existencia.

## El grafo

Antes de comenzar, tenemos que implementar una estructura de datos adecuada para almacenar la información de nuestro grafo. En general, nos va a interesar almacenar el grafo en una matriz de adyacencias, ya que se adapta mejor al algoritmo en cuestión.

### El tamaño del grafo

Solemos mantener el tamaño máximo del grafo como una constante `MAX_N`. Aunque es posible crear un grafo que crezca/decrezca dinámicamente con cada caso, las peticiones/liberaciones de memoria consumen tiempo de procesamiento.

```
#define MAX_N 1000
```

En el ejemplo anterior definimos `MAX_N` como 1000, pero puede ser el valor que convenga dependiendo del problema a resolver.

### Matriz de adyacencias

Para implementar una lista de adyacencia usaremos simplemente un arreglo de `double` de dos dimensiones.

```
double grafo[MAX_N][MAX_N];
int N;
```

De esta forma, `grafo[i][j]` tendrá el costo asociado al arco entre `i` y `j`. En caso de no haber un arco entre estos nodos, el costo será _infinito_, que podemos representar como `numeric_limits<double>::infinity()`.

## Lectura e inicialización del grafo

Las entradas para problemas de grafos vienen en muchas variedades. A continuación veremos una posible forma de leer la entrada y almacenarla en nuestra estructura de datos.

### Estructura del archivo

El archivo comenzará con dos números enteros: `N` y `M`, representando la cantidad de nodos y la cantidad de conexiones, respectivamente.

Le seguirán `M` lineas, donde cada línea tendrá tres números: `origen`, `destino` y `costo`, representando a una conexión que parte desde el nodo con índice `origin` y llega hasta el nodo con índice `destino`, con costo `costo`.

Por ejemplo:

```
5 6
0 1 1.0
0 2 1.0
1 3 2.0
2 3 1.0
3 4 3.0
0 4 2.0
```

### Procesamiento del archivo

La lectura de la entrada puede hacerse mediante el uso de el _stream_ de entrada `cin`, para lo cual tenemos que importar la librería `<iostream>`.

Primeramente, leeremos la cantidad de nodos del grafo `N` y procederemos a _limpiar el grafo_ (esto es, a asignar infinito a los costos de todos los posibles arcos).

```
#include<iostream>
...
void leer_grafo() {
	int M;
	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			grafo[i][j] = numeric_limits<double>::infinity();
	...
}
```

Una vez limpio el grafo, podemos leer la información presente de arcos:

```
	...
	cin >> M;
	while (M--) {
		int origen, destino;
		cin >> origen >> destino >> grafo[origen][destino];
	}
}
```

Nótese que la cantidad de conexiones `M` es declarada como variable local del procedimiento, ya que su valor no es relevante al salir del mismo.

Por esta misma razón es que podemos hacer `while (M--)`. Al salir del ciclo el valor de `M` será 0 _(cero)_ y su valor original de habrá perdido.

## Bellman-Ford

La implementación del algoritmo de Bellman-Ford hace uso de un arreglo de costos mínimos, que podemos declarar como una variable global:

```
...
double costo_minimo[MAX_N];
...
```

Implementaremos nuestra función `bellman_ford` como una función que devuelve un booleano. El valor de retorno será `true` si el grafo analizado contiene un ciclo de costo negativo y `false` de lo contrario. En cualquier caso, en el arreglo `costo_minimo` quedarán los costos calculados por el algoritmo.

En primer lugar, todos los caminos mínimos son infinitos (no tenemos información), excepto el camino trivial que parte del nodo inicial y tiene longitud nula.

```
...
double bellman_ford(int desde) {
	for (int i = 0; i < N; i++)
		costo_minimo[i] = numeric_limits<double>::infinity();
	costo_minimo[desde] = 0;
	...
```

Luego, comenzamos las iteraciones del algoritmo (llevando en una variable `iters` la cantidad de iteraciones que llevamos del algoritmo).

```
	...
	int iters;
	for (iters = 0; iters <= N; iters++) {
		...
```

Nótese que el algoritmo hará a lo sumo `N+1` iteraciones.

Cada una de las iteraciones intenta mejorar `costo_minimo[i]` para todo posible nodo `i`, usando cualquier otro posible nodo `j` como intermediario.

Si el costo mínimo para llegar hasta `j` (que se encuentra en `costo_minimo[j]`) más el costo del arco que conecta a `j` con `i` (que se encuentra en `grafo[j][i]`) es menor que el costo mínimo conocido hasta el nodo `i` (que se encuentra en `costo_minimo[i]`), entonces su valor debe actualizarse:

```
			...
			if (costo_minimo[j] + grafo[j][i] < costo_minimo[i])
				costo_minimo[i] = costo_minimo[j] + grafo[j][i];
			...
```

Esto debe hacerse para cada posible `i` y cada posible `j`, además llevando control de si la información de costos mínimos ha cambiado:

```
		...
		bool cambio = false;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (costo_minimo[j] + grafo[j][i] < costo_minimo[i]) {
					costo_minimo[i] = costo_minimo[j] + grafo[j][i];
					cambio = true;
				}
		...
```

Si no hubo cambio alguno en una iteración tampoco habrán cambios futuros, por lo que no es necesario continuar iterando.

```
		...
		if (!cambio)
			break;
	}
	...
```

Como vimos en clase, el algoritmo de Bellman-Ford iterará a lo sumo `N` veces cuando el grafo suministrado está libre de ciclos de costo negativo. En caso contrario, podría iterar indefinidamente. Como cortamos las iteraciones en `N+1` (nótese la condición `iters <= N` que deja de cumplirse cuando `iters == N+1`), si `iters` llega a ser igual a `N+1`, la única posibilidad es que haya conseguido un ciclo de costo negativo. Si, por el contrario, `iters` nunca llega a valer `N+1` entonces no hay ciclos de costo negativo en el grafo.

Siendo así, la consulta por la existencia de ciclos negativos se puede resumir en una expresión simple al final del procesamiento:

```
	...
	return iters == N+1;
}
...
```

### Probando la implementación

Podemos probar nuestra implementación, extendiendo la estructura de nuestro archivo de entrada.

Después de la entrada ya expuesta, vendrá un número `Q`, representando la cantidad de consultas a realizarse.

Le seguirán `Q` lineas, cada uno con un valor `desde`. Para cada uno de estos valores vamos a querer conocer el costo del camino de costo mínimo entre el nodo con índice `desde` y todos los demás, a menos que exista un ciclo de costo negativo (en cuyo caso esos caminos de costo mínimo no están bien definidos).

Además, podemos agregar un valor `T` al inicio del archivo que represente la cantidad de casos de prueba a tratar.

Por ejemplo:

```
2
5 6
0 1 1.0
0 2 1.0
1 3 2.0
2 3 1.0
3 4 3.0
0 4 2.0
3
0
3
2
3 3
0 1 -3.0
1 2 1.0
2 0 1.0
1
0
```

Nótese que el segundo grafo en la entrada tiene un ciclo de costo negativo.

#### Cuerpo principal del programa

Implementamos ahora un `main` que una todo y realice las consultas propuestas:

```
...
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
```

#### Salida esperada

Si se ejecuta el programa con la entrada propuesta, la salida esperada ha de ser la siguiente.

```
El camino de costo minimo entre 0 y 0 cuesta: 0
El camino de costo minimo entre 0 y 1 cuesta: 1
El camino de costo minimo entre 0 y 2 cuesta: 1
El camino de costo minimo entre 0 y 3 cuesta: 2
El camino de costo minimo entre 0 y 4 cuesta: 2
El camino de costo minimo entre 3 y 0 cuesta: inf
El camino de costo minimo entre 3 y 1 cuesta: inf
El camino de costo minimo entre 3 y 2 cuesta: inf
El camino de costo minimo entre 3 y 3 cuesta: 0
El camino de costo minimo entre 3 y 4 cuesta: 3
El camino de costo minimo entre 2 y 0 cuesta: inf
El camino de costo minimo entre 2 y 1 cuesta: inf
El camino de costo minimo entre 2 y 2 cuesta: 0
El camino de costo minimo entre 2 y 3 cuesta: 1
El camino de costo minimo entre 2 y 4 cuesta: 4
Existe un ciclo de costo negativo!
```

### Ejecutando el programa

Para ejecutar el programa, basta con descargarse los archivos `bellman-ford.cpp` y `bellman-ford.in` del repositorio en donde se encuentra este tutorial.

Luego, desde la consola, navegar hasta la carpeta en donde se descargaran.

Para compilar el programa, puede hacer:

```
make bellman-ford
```

Para ejecutar el programa, puede hacer:

```
./bellman-ford < bellman-ford.in
```

## Floyd-Warshall

La implementación del algoritmo de Floyd-Warshall hace uso de una matriz de costos mínimos (matriz de alcance), que podemos declarar como una variable global:

```
...
double costo_minimo[MAX_N][MAX_N];
...
```

Implementaremos nuestra función `floyd_warshall` como una función que devuelve un booleano. El valor de retorno será `true` si el grafo analizado contiene un ciclo de costo negativo y `false` de lo contrario. En cualquier caso, en la matriz `costo_minimo` quedarán los costos calculados por el algoritmo.

En primer lugar, la única información que tenemos sobre costos es la que ya proviene del grafo a analizar. Además, podemos suponer que el costo mínimo de cada nodo a sí mismo es cero (afirmación que se falsifica si existen ciclos de costo negativo en el grafo).

```
...
bool floyd_warshall() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			costo_minimo[i][j] = grafo[i][j];
		grafo[i][i] = 0;
	}
	...
```

El cuerpo principal del algoritmo selecciona dos nodos y verifica si puede construirse un mejor camino que el ya conocido, usando un nodo adicional en el camino entre ambos. Esto es, se verifica que, para cada par de nodos `i` y `j`, exista un `k` tal que el costo desde `i` hasta `j` (que se encuentra en `costo_minimo[i][j]`) sea mayor que la suma de los costos desde `i` hasta `k` (que se encuentra en `costo_minimo[i][k]`) y desde `k` hasta `j` (que se encuentra en `costo_minimo[k][j]`).

Es importante el orden de los ciclos, o la respuesta podría ser incorrecta. Primeramente debe iterarse sobre el nodo intermedio `k`, luego sobre los nodos `i` y `j`.

```
	...
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (costo_minimo[i][k] + costo_minimo[k][j] < costo_minimo[i][j])
					costo_minimo[i][j] = costo_minimo[i][k] + costo_minimo[k][j];
	...
```

Si después de conseguir los costos mínimos entre cada par de nodos, algún valor en la diagonal de la matriz de costos llega a ser negativa, entonces tenemos un ciclo de costo negativo. De lo contrario, el grafo está libre de tales ciclos:

```
	...
	for (int i = 0; i < N; i++)
		if (costo_minimo[i][i] < 0)
			return true;
	return false;
}
...
```

### Probando la implementación

Podemos probar nuestra implementación, extendiendo la estructura de nuestro archivo de entrada.

Después de la entrada ya expuesta, vendrá un número `Q`, representando la cantidad de consultas a realizarse.

Le seguirán `Q` lineas, cada uno con dos valores `desde` y `hasta`. Para cada uno de estos valores vamos a querer conocer el costo del camino de costo mínimo entre el nodo con índice `desde` y el nodos con índice `hasta`, a menos que exista un ciclo de costo negativo (en cuyo caso esos caminos de costo mínimo no están bien definidos).

Además, podemos agregar un valor `T` al inicio del archivo que represente la cantidad de casos de prueba a tratar.

Por ejemplo:

```
2
5 6
0 1 1.0
0 2 1.0
1 3 2.0
2 3 1.0
3 4 3.0
0 4 2.0
3
0 3
3 1
2 3
3 3
0 1 -3.0
1 2 1.0
2 0 1.0
1
0 1
```

Nótese que el segundo grafo en la entrada tiene un ciclo de costo negativo.

#### Cuerpo principal del programa

Implementamos ahora un `main` que una todo y realice las consultas propuestas:

```
...
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
```

#### Salida esperada

Si se ejecuta el programa con la entrada propuesta, la salida esperada ha de ser la siguiente.

```
No hay ciclos de costo negativo! Los valores a continuacion son confiables
El camino de costo minimo entre 0 y 3 cuesta: 2
El camino de costo minimo entre 3 y 1 cuesta: inf
El camino de costo minimo entre 2 y 3 cuesta: 1

Existe un ciclo de costo negativo! Los valores a continuacion pudieran estar mal formados
El camino de costo minimo entre 0 y 1 cuesta: -3
```

### Ejecutando el programa

Para ejecutar el programa, basta con descargarse los archivos `floyd-warhsall.cpp` y `floyd-warshall.in` del repositorio en donde se encuentra este tutorial.

Luego, desde la consola, navegar hasta la carpeta en donde se descargaran.

Para compilar el programa, puede hacer:

```
make floyd-warshall
```

Para ejecutar el programa, puede hacer:

```
./floyd-warshall < floyd-warshall.in
```

## Ejercicios recomendados en SPOJ

A continuación algunos ejercicios con los que pueden practicar aplicar Bellman-Ford y Floyd-Warshall a problemas particulares.

* Arbitrage: [ARBITRAG](https://www.spoj.com/problems/ARBITRAG/)
* Defend The Rohan: [ROHAAN](https://www.spoj.com/problems/ROHAAN/)