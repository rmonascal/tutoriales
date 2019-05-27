# ¿Cómo implementar el algoritmo de Kruskal en C++?

En este tutorial aprenderemos a implementar el algoritmo de Kruskal para hallar un árbol cobertor de costo mínimo para un grafo.

## El grafo *(o, más bien, sus conexiones)*

Antes de comenzar, tenemos que implementar una estructura de datos adecuada para almacenar la información de nuestro grafo. En este caso, sólo nos interesa almacenar al información de conexiones ordenadas por costo. Para esto, crearemos una estructura junto con su comparador (como hicimos cuando implementamos el algoritmo de Dijsktra) que represente dichas conexiones. Luego, esta información la colocaremos en una cola de prioridades que haga uso del comparador propuesto.

### El tamaño del grafo

Solemos mantener el tamaño máximo del grafo como una constante `MAX_N`. Aunque es posible crear un grafo que crezca/decrezca dinámicamente con cada caso, las peticiones/liberaciones de memoria consumen tiempo de procesamiento.

```
#define MAX_N 1000
```

En el ejemplo anterior definimos `MAX_N` como 1000, pero puede ser el valor que convenga dependiendo del problema a resolver.

### Estructura para almacenar conexiones

Nuestra estructura será una terna con `origen`, `destino` y `costo`:

```
struct conexion {
    int origen;
    int destino;
    double costo;
};
```

### Comparador entre conexiones

El comparador entre conexiones es una función que recibe dos conexiones y devuelve:

* Un número positivo si el primero es mayor que el segundo.
* Un número negitivo si el primero es menor que el segundo.
* Cero si el primero es igual al segundo.

Esta función debe ir envuelta dentro de una estructura para poder ser usada y la función misma será una versión sobrecargada de la operación de aplicación funcional (por lo tanto, será escrita como una instance de `operator()`).

```
struct comparador_para_conexiones {
    bool operator()(conexion const& primera, conexion const& segunda) {
        return primera.costo > segunda.costo;
    }
};
```

Finalmente podemos definir nuestra cola de prioridades para conexiones:

```
import<queue>
...
priority_queue<conexion, vector<conexion>, comparador_para_conexiones> conexiones;
```

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

Primeramente, leeremos la cantidad de nodos del grafo `N` y la cantidad de conexiones `M`. Luego, para cada conexión, crearemos un nuevo valor de tipo `conexion` y lo agregaremos a la cola de prioridades.

```
#include<iostream>
...
int N;
...
void leer_conexiones() {
    int M;
    cin >> N >> M;
    while (M--) {
        conexion con;
        cin >> con.origen >> con.destino >> con.costo;
        conexiones.push(con);
    }
}
```

Nótese que la cantidad de conexiones `M` es declarada como variable local del procedimiento, ya que su valor no es relevante al salir del mismo. Sin embargo `N` es declarada como variable global, ya que será usada en otros puntos del programa.


## Kruskal

La implementación del algoritmo de Kruskal hace uso de una estructura de datos especial para manejar conjuntos disjuntos.

### Conjuntos Disjuntos

Para implementar esta estructura de datos, usaremos un arreglo `padre` que mantenga, para cada elemento posible, cual es su padre en el árbol de representantes (recuerde que los representantes son aquellos elementos que son sus propios padres en este árbol).

Como tenemos `N` nodos, inicializamos nuestra estructura con dicho valor, haciendo que cada elemento sea su propio padre.

```
int padre[MAX_N];
...
void inicializar_conjuntos_disjuntos() {
    for (int i = 0 ; i < N ; i++)
        padre[i] = i;
}
```

Con nuestra estructura en pie, quisiéramos conocer el representante para un elemento particular *(recordemos que el representante de un elemento y su padre es igual)*. Además, haremos una compresión del árbol de representantes en el proceso, como vimos en clase *(asignando como padre del elemento actual, el representante que fue hallado)*.

```
int buscar_representante(int elem) {
    if (padre[elem] == elem)
        return elem;
    return padre[elem] = buscar_representante(padre[elem]);
}
```

Finalmente, necesitamos una forma de unir dos conjuntos. Para esto, buscaremos los representantes de cada uno y:

* Si son iguales, no se hace nada (y devolveremos `false`, indicando que no hubo cambio).
* Si son diferentes, el padre de uno de los representantes se convierte en el otro (y devolvemos `true`, indicando que sí hubo un cambio).

```
bool unir_conjuntos(int elem_a, int elem_b) {
    int repr_a = buscar_representante(elem_a);
    int repr_b = buscar_representante(elem_b);
    if (repr_a == repr_b)
        return false;
    padre[repr_a] = repr_b;
    return true;
}
```

### Cuerpo Principal de Kruskal

Una vez tenemos nuestra estructura de conjuntos disjuntos, la implementación del algoritmo de Kruskal es sencilla. Haremos una versión que calcula el costo total de un árbol cobertor mínimo, pero el algoritmo se puede modificar para calcular el árbol como tal.

En primer lugar, inicializamos el costo total en 0 y los conjuntos disjuntos tal que cada nodo sea su propia componente conexa (cada nodo es su propio representante).

```
double kruskal() {
    double costo_total = 0;
    inicializar_conjuntos_disjuntos();
    ...
```

Luego, mientras hayan conexiones disponibles en el grafo, tomamos la siguiente que tenga el menor costo (sencillo, gracias a la cola de prioridades que definimos anteriormente).

```
    ...
    while (!conexiones.empty()) {
        conexion con = conexiones.top();
        conexiones.pop();
        ...
```

Si los extremos de la conexión actual pertenecen a la misma componente conexa, entonces no se debe incluir en el árbol. De lo contrario, se incluye en el árbol y su costo se suma (sencillo, gracias a la estructura de conjuntos disjuntos que definimos anteriormente).

```
        ...
        if (unir_conjuntos(con.origen, con.destino))
            costo_total += con.costo;
    }
    ...
```

Finalmente devolvemos el costo total calculado como resultado de la función.

```
    ...
    return costo_total;
}
```

### Probando la implementación

Podemos probar nuestra implementación, extendiendo la estructura de nuestro archivo de entrada. Podemos agregar un valor `T` al inicio del archivo que represente la cantidad de casos de prueba a tratar.

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
3 3
0 1 -3.0
1 2 1.0
2 0 1.0
```

Nótese que el segundo grafo en la entrada tiene costos negativos.

#### Cuerpo principal del programa

Implementamos ahora un `main` que una todo y calcule los costos de un árbol cobertor mínimo para cada grafo en la entrada:

```
int main() {
    int T;
    cin >> T;
    while (T--) {
        leer_conexiones();
        cout << "El costo de un árbol cobertor minimo es: " << kruskal() << "." << endl;
    }
    return 0;
}
```

#### Salida esperada

Si se ejecuta el programa con la entrada propuesta, la salida esperada ha de ser la siguiente.

```
El costo de un árbol cobertor minimo es: 5.
El costo de un árbol cobertor minimo es: -2.
```

### Ejecutando el programa

Para ejecutar el programa, basta con descargarse los archivos `kruskal.cpp` y `kruskal.in` del repositorio en donde se encuentra este tutorial.

Luego, desde la consola, navegar hasta la carpeta en donde se descargaran.

Para compilar el programa, puede hacer:

```
make kruskal
```

Para ejecutar el programa, puede hacer:

```
./kruskal < kruskal.in
```

## Ejercicios recomendados en SPOJ

A continuación algunos ejercicios con los que pueden practicar aplicar Kruskal a problemas particulares.

* Cobbled streets: [CSTREET](https://www.spoj.com/problems/CSTREET/)
* Inventing Test Data: [INVENT](https://www.spoj.com/problems/INVENT/).

Algunas pistas para el problema [INVENT](https://www.spoj.com/problems/INVENT/):

  * Hay que usar `long long` para los costos, ya que `int` no alcanza.
  * Un grafo completo es un grafo donde todos los nodos están conectados con todos los demás. Por lo tanto, un grafo completo con *N* nodos tiene *N x (N - 1) / 2* conexiones.
  * Para crear un grafo completo a partir de la unión de dos grafos completos: uno de *N* nodos y el otro *M* nodos, hacen falta *N x M* nuevas conexiones.