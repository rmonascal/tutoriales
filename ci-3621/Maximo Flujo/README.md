# ¿Cómo implementar máximo flujo en C++?

En este tutorial aprenderemos a implementar una solución al problema de conseguir el máximo flujo enm un grafo. Particularmente, implementaremos el algoritmo de *Edmonds-Karp*, una variante de *Ford-Fulkerson* que usa BFS para conseguir camino en aumento.

## El grafo

Antes de comenzar, tenemos que implementar una estructura de datos adecuada para almacenar la información de nuestro grafo. Para mayor comodidad, usaremos dos matrices para representar este información. Ambas matrices corresponderán a matrices de adyacencia.

### El tamaño del grafo

Solemos mantener el tamaño máximo del grafo como una constante `MAX_N`. Aunque es posible crear un grafo que crezca/decrezca dinámicamente con cada caso, las peticiones/liberaciones de memoria consumen tiempo de procesamiento.

```
#define MAX_N 1000
```

En el ejemplo anterior definimos `MAX_N` como 1000, pero puede ser el valor que convenga dependiendo del problema a resolver.

### Estructura para almacenar capacidades y flujos

Las capacidades de las conexiones y el flujo que pasa por cada una de ellas, serán almacenadas en un par de matrices cuadradas:

```
int capacidad[MAX_N][MAX_N], flujo[MAX_N][MAX_N];
```

donde:

* `capacidad[i][j]` es la capacidad de la conexión que va desde `i` hasta `j`
* `flujo[i][j]` es el flujo que pasa a través de la conexión que va desde `i` hasta `j`.

Nótese que debe cumplirse siempre que `0 <= flujo[i][j] <= capacidad[i][j]`.

## Lectura e inicialización del grafo

Las entradas para problemas de grafos vienen en muchas variedades. A continuación veremos una posible forma de leer la entrada y almacenarla en nuestra estructura de datos.

### Estructura del archivo

El archivo comenzará con dos números enteros: `N` y `M`, representando la cantidad de nodos y la cantidad de conexiones, respectivamente.

Le seguirán `M` lineas, donde cada línea tendrá tres números: `origen`, `destino` y `costo`, representando a una conexión que parte desde el nodo con índice `origin` y llega hasta el nodo con índice `destino`, con costo `costo`.

Por ejemplo:

```
5 6
0 1 1
0 2 1
1 3 2
2 3 1
3 4 3
0 4 2
```

### Procesamiento del archivo

La lectura de la entrada puede hacerse mediante el uso de el _stream_ de entrada `cin`, para lo cual tenemos que importar la librería `<iostream>`.

Primeramente, leeremos la cantidad de nodos del grafo `N` y la cantidad de conexiones `M`. Luego, para cada conexión, crearemos un nuevo valor de tipo `conexion` y lo agregaremos a la cola de prioridades.

```
#include<iostream>
...
int N;
...
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
```

Nótese que la cantidad de conexiones `M` es declarada como variable local del procedimiento, ya que su valor no es relevante al salir del mismo. Sin embargo `N` es declarada como variable global, ya que será usada en otros puntos del programa.

#### Limpieza del grafo

El procedimiento `limpiar_grafo()` reinicia las capacidades y flujos, colocando todo en cero (todos las conexiones tendrían capacidad cero).

```
void limpiar_grafo() {
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++) {
            capacidad[i][j] = 0;
            flujo[i][j] = 0;
        }
}
```


## Máximo flujo

La implementación del algoritmo de *Edmonds-Karp* (y, en general, cualquier variante de *Ford-Fulkerson*) sigue el mismo patrón:

* Hallar un camino en aumento (si no existe un camino en aumento, el algoritmo termina).
* Obtener la mínima capacidad restante (capacidad - flujo) en el camino encontrado.
* Actualizar los flujos para cada conexión de ese camino (de forma positiva para las conexiones tomadas y de forma negativa para las conexiones contrarias) con la mínima capadidad encontrada.
  * Si `a -> b` es una conexión en el camino, `b -> a` es la conexión contraria.

La implementación de esta idea se puede observar a continuación:

```
int maximo_flujo(int origen, int destino) {
    int flujo_total = 0;
    while (hallar_camino_en_aumento(origen, destino)) {
        int flujo_camino = obtener_flujo_camino(origen, destino);
        flujo_total += flujo_camino;
        actualizar_flujo(origen, destino, flujo_camino);
    }
    return flujo_total;
}
```

### ¿Cómo hallar un camino en aumento?

Para hallar un camino en aumento, usaremos una búsqueda en amplitud (mejor conocida como BFS). PAra esto, necesitamos declarar un arreglo de visitados. Además, llevaremos control de quién es el padre `P` de cada nodo `H` (qué nodo `P` agregó al nodo `H` a la cola, durante la ejecución del algoritmo).

```
#include <queue>
...
int padre[MAX_N];
...
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
...
bool hallar_camino_en_aumento(int origen, int destino) {
    for (int i=0; i<N; i++)
        visitado[i] = false;
    return bfs(origen, destino);
}
```

Nótese que consideramos una conexión, entre los nodos `actual` y `sig`, como válida para transitarse por la búsqueda, cuando `flujo[actual][sig] < capacidad[actual][sig]` y `!visitado[sig]`. Sólo expandimos nodos a los que pueda transitar con flujo positivo y que no hayan sido visitados previamente (nótese también que el arreglo de visitados se reinicia cada vez que se comienza a buscar un camino en aumento).

Cuando logramos recorrer una conexión entre los nodos `actual` y `sig`, establecemos que el padre de `sig` es `actual` y agregamos a `sig` a la cola.

### ¿Cómo obtener la capacidad del camino hallado?

Una vez conseguimos un camino en aumento, debemos conseguir el flujo que dicho camino proporciona a la solución. Esto lo podemos lograr usando el arreglo de padres construido durante el BFS.

Si queremos hallar la capacidad mínima entre dos nodos `origen` y `destino`, tenemos dos casos:

* Si `origen == destino` entonces la capacidad es ilimitada (es infinita, pues no está transcurriendo ninguna conexión).
* Si `origen != destino` entonces la capacidad mínima es el minimo entre:
  * La capacidad de la conexión `padre[destino] -> destino`.
  * La capacidad mínima entre `origen` y `padre[destino]`.

Usando este razonamiento, podemos derivar un procedimiento que consigue la capacidad mínima del camino entre `origen` y `destino`. Para esto debemos considerar un `MAX_CAP` que vendría a representar a la capacidad infinita (sirve cualquier número que sea más grande que todas las capacidades del grafo).

```
#define MAX_CAP 1000
...
int obtener_flujo_camino(int origen, int destino) {
    int flujo_camino = MAX_CAP;
    while (origen != destino) {
        int anterior = padre[destino];
        flujo_camino = min(flujo_camino, capacidad[anterior][destino] - flujo[anterior][destino]);
        destino = anterior;
    }
    return flujo_camino;
}
```

### ¿Cómo actualizar los flujos del camino?

De igual forma que conseguimos la capacidad mínima del camino, podemos usar la relación de `padre` entre nodos para actualizar los flujos en el camino encontrado.

```
void actualizar_flujo(int origen, int destino, int flujo_camino) {
    while (origen != destino) {
        int anterior = padre[destino];
        flujo[anterior][destino] += flujo_camino;
        flujo[destino][anterior] -= flujo_camino;
        destino = anterior;
    }
}
```

Nótese que el flujo en la dirección que toma la conexión del camino encontrado aumenta, y el flujo para la conexión contraria disminuye.

### Probando la implementación

Podemos probar nuestra implementación, extendiendo la estructura de nuestro archivo de entrada. Podemos agregar un valor `T` al inicio del archivo que represente la cantidad de casos de prueba a tratar. Además, podemos agregarle un par de nodos `origen` y `destino` con los que queramos aplicar nuestro algoritmo.

Por ejemplo:

```
2
5 6
0 1 1
0 2 1
1 3 2
2 3 1
3 4 3
0 4 2
0 4
3 3
0 1 3
1 2 1
0 2 1
0 2
```

#### Cuerpo principal del programa

Implementamos ahora un `main` que una todo y calcule los costos de un árbol cobertor mínimo para cada grafo en la entrada:

```
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
```

#### Salida esperada

Si se ejecuta el programa con la entrada propuesta, la salida esperada ha de ser la siguiente.

```
El flujo máximo desde 0 hasta 4 es: 4.
El flujo máximo desde 0 hasta 2 es: 2.
```

### Ejecutando el programa

Para ejecutar el programa, basta con descargarse los archivos `maxikmo-flujo.cpp` y `maximo-flujo.in` del repositorio en donde se encuentra este tutorial.

Luego, desde la consola, navegar hasta la carpeta en donde se descargaran.

Para compilar el programa, puede hacer:

```
make maximo-flujo
```

Para ejecutar el programa, puede hacer:

```
./maximo-flujo < maximo-flujo.in
```

## Ejercicios recomendados en SPOJ

A continuación algunos ejercicios con los que pueden practicar aplicar Kruskal a problemas particulares.

* Potholers: [POTHOLE](https://www.spoj.com/problems/POTHOLE/)
* Dungeon of Death: [QUEST4](https://www.spoj.com/problems/QUEST4/) y Muddy Fields: [MUDDY](https://www.spoj.com/problems/MUDDY/) (el mismo código debería servir para ambos problemas)
* Angels and Devils: [ANGELS](https://www.spoj.com/problems/ANGELS/)