# Irregular Strip Packing Problem

## Instancias

Las instancias entregadas fueron preprocesadas para que todos los vértices de los polígonos se encontraran contenidos en el primer cuadrante del plano euclideano. El formato de las instancias es el siguiente:

```<ID_POLIGONO> <CANTIDAD> <x1,y1>;<x2,y2>;...;<xn,yn>```

Es decir, el ID del polígono, seguido de la cantidad en la que está presente y finalmente una lista de sus vértices en sentido antihorario.

## Ejecución

Para ejecutar el programa, se deben realizar los siguientes pasos:

1. Compilar las fuentes: solo se debe ejecutar el comando `make`.
2. Ejecutar el siguiente comando:

```./ISPP instances/<NOMBRE_DE_INSTANCIA>.txt <ANCHO_DEL_CONTAINER> <CANTIDAD_DE_RESTARTS>```

Los anchos para cada instancia son los siguientes:

| Nombre de Instancia | Ancho de *container* |
|---------------------|--------------------|
| albano              | 4900               |
| blaz                | 15                 |
| dagli               | 60                 |
| jakobs1             | 40                 |
| jakobs2             | 70                 |
| marquez             | 104                |
| poly2a              | 4000               |
| poly3a              | 4000               |
| poly4a              | 4000               |
| poly5a              | 4000               |
| shirts              | 40                 |
| swim                | 5752               |
| trousers            | 79                 |

Para facilitar el proceso de corrección, a continuación se dejan los comandos (solo debe cambiarse X por la cantidad de *restarts*):

```
./ISPP instances/albano.txt 4900 X
./ISPP instances/blaz.txt 15 X
./ISPP instances/dagli.txt 60 X
./ISPP instances/jakobs1.txt 40 X
./ISPP instances/jakobs2.txt 70 X
./ISPP instances/marques.txt 104 X
./ISPP instances/poly2a.txt 4000 X
./ISPP instances/poly3a.txt 4000 X
./ISPP instances/poly4a.txt 4000 X
./ISPP instances/poly5a.txt 4000 X
./ISPP instances/shirts.txt 40 X
./ISPP instances/swim.txt 5752 X
./ISPP instances/trousers.txt 79 X
```

## Output del programa

La salida del programa tiene el siguiente formato:

```
Calculating...
====== Best solution ======
Length: X; Usage %: Y; Time: Z[s]
<VÉRTICES_POLÍGONO_1>
...
<VÉRTICES_POLÍGONO_N>
```
* `Length` corresponde al largo mínimo obtenido.
* `Usage` corresponde al área efectiva (con respecto al total) que es utilizada por los rectángulos.
* `Time` corresponde al tiempo de cómputo empleado.
* Finalmente le siguen los vértices de los polígonos como se solicitaba.

## Consideraciones adicionales

La función `explode` se obtuvo desde http://www.cplusplus.com/articles/2wA0RXSz/ (visitada por última vez el 5 de Marzo del 2020).
