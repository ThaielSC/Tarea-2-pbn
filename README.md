# ICC3102 Programación de Bajo Nivel 2025-10 [cite: 1]

## Tarea 2 [cite: 1]

**Manejo de datos y punteros**

**Entrega:** miércoles 30 de abril a las 21:00 hrs. [cite: 1]

## 1. Introducción [cite: 2]

En esta tarea, se le pide que escriba un programa en C que gestione una base de datos de
canciones y permita al usuario generar listas personalizadas aplicando distintos filtros. [cite: 2, 3] El propósito
es ofrecer una herramienta rápida y eficiente para crear listas de canciones basadas en criterios como
duración, género o año de lanzamiento, y guardar los resultados en archivos de texto. [cite: 3]

## 2. Objetivo [cite: 4]

Crear un programa en C que: [cite: 4]

* Lea un archivo .csv con información de canciones. [cite: 4]
* Almacene los datos en memoria dinámica usando struct y punteros (sin duplicar registros). [cite: 5]
* Permita al usuario aplicar filtros interactivos (menú por consola). [cite: 6]
* Genere listas de canciones y las escriba en archivos .txt con un formato concreto. [cite: 7]

Los detalles de estos requisitos se entregan a continuación. [cite: 8]

## 3. Funcionamiento [cite: 8]

Su programa deberá maximizar la velocidad de ejecución y la eficiencia en el uso de memoria,
según las pautas que se dictan en este enunciado. [cite: 8]

i)  **Lectura del archivo (0,5 pts.)** El programa recibirá como argumento el nombre del .csv
    (p.ej. songs\_bd.csv), que puede estar compuesto de decenas de miles de filas con los siguientes
    atributos: [cite: 9, 10, 11]

    `song_id;song_title;`
    `artist; album; genre; release_date; duration; popularity; stream;`
    `language; explicit_content; label; composer; producer;`
    `collaboration` [cite: 9, 10, 11]

ii) **Almacenar cada canción en un arreglo de struct en memoria dinámica (1,5 pts.)** Para
    maximizar la velocidad, los datos deben permanecer en memoria. [cite: 11, 12] Para ahorrar memoria, evite
    copiar datos innecesariamente. [cite: 12]

iii) **Filtros (2,5 pts.)** El usuario podrá combinar estos filtros en cualquier orden: [cite: 13]

* Para los filtros de Artista, Álbum, Género o Idioma, el sistema listará todas las opcio-
    nes disponibles sin repetir. [cite: 13, 14] Si hay más de 100 valores distintos, se mostrarán de forma
    arbitraria únicamente 100 de ellos para facilitar la selección. [cite: 14]
* Para filtrar por año de lanzamiento, el programa pedirá dos valores numéricos consecutivos:
    ej:

    ```
    Año inicio:
    Año fin:
    ```

    Sólo las canciones cuyo año esté dentro de ese rango deben ser seleccionadas. [cite: 15]
* Para filtrar por duración (segundos), el programa pedirá dos valores numéricos consecuti-
    vos:

    ej:

    ```
    Duración mínima (s):
    Duración máxima (s):
    ```

    Sólo las canciones cuyo año esté dentro de ese rango deben ser seleccionadas. [cite: 16]

iv) **Generación de listas (1,5 pts.)**

    Después de cada filtro aplicado, se muestra en consola el número de canciones seleccionadas en
    la lista, si este es mayor a 0, se le pregunta al usuario si desea: [cite: 17, 18, 19]

* Aplicar otro filtro sobre los resultados ya seleccionados. [cite: 17, 18, 19]
* Visualizar las primeras 100 canciones en la lista (en consola). [cite: 17, 18, 19]
* Exportar la lista a un archivo de texto. [cite: 17, 18, 19]
* Salir para terminar la ejecución. [cite: 17, 18, 19]

    El archivo .txt debe tener por línea el siguiente formato para tener puntaje completo: `SONG_ID:`
    `ARTIST - SONG_TITLE (RELEASE_YEAR) - GENRE`. [cite: 19, 20] Es importante que los datos resultantes en la lista generada sean correctos y consistentes con
    los filtros aplicados. [cite: 20]

## 4. Recomendaciones [cite: 21]

* Recuerde que su tarea será evaluada por funcionalidades. [cite: 21, 22] Esto tiene dos implicancias prácticas
    importantes: 1) más vale una tarea en que funcionan la mitad de las cosas que una que está a
    punto de funcionar pero no compila, y 2) antes de desesperarse o bloquearse, divida la tarea
    en funcionalidades e impleméntelas una a una de manera de ir asegurando puntaje. [cite: 22]
* Comience el desarrollo con anticipación. [cite: 23, 24, 25] Es muy probable que en el camino encuentre proble-
    mas que requieran tiempo para pensar o discutir con el profesor o los ayudantes. [cite: 25]

## 5. Consideraciones Generales sobre la Evaluación [cite: 26]

En cada ítem se reservará una parte del puntaje para la eficiencia de la solución, en términos
de cantidad total de memoria utilizada y velocidad de respuesta. [cite: 26]

Recuerde que está prohibido usar materia no vista en clases, salvo que se autorice explícitamente
lo contrario. [cite: 27, 28, 29] En este contexto, está permitido usar punteros (incluidos punteros a funciones), `malloc`
y sus funciones relacionadas, `struct`, y `typedef`. [cite: 28, 29] Por su importancia para la tarea, se subirá a la
página del curso en Canvas un video explicativo sobre `struct`. [cite: 29]

Su programa debe ser robusto frente a datos que no corresponden. [cite: 30, 31, 32] Esto quiere decir que su
programa no debe caerse en caso de que el usuario haga operaciones matemáticas no válidas (p.ej.,
dividir por cero si fuera el caso). [cite: 31, 32] Sin embargo, se puede asumir que el usuario será amigable, en el
sentido de que cuando le soliciten un número este no ingresará letras, por ejemplo. [cite: 32]

## 6. Desarrollo, compilación y entrega [cite: 33]

Esta tarea puede ser resuelta en grupos de máximo 2 personas. [cite: 33, 34, 35] El plazo para la entrega de la
tarea vence impostergablemente el miércoles 30 de abril a las 21:00 hrs. [cite: 34, 35] Cuide su tarea para que
no sea copiada parcial o íntegramente por otros. [cite: 34, 35] Todas las tareas entregadas serán comparadas
por un sistema automático de detección de plagio. [cite: 35, 36] Cualquier sospecha de copia (de otras tareas
o de internet) podrá ser denunciada, investigada y eventualmente penalizada de conformidad al
Reglamento del Alumno. [cite: 36, 37]

Formato de entrega: Subir un único archivo con el código de su programa al módulo de ta-
reas de la página del curso, con un nombre de archivo que incluya ambos apellidos de los autores
de la tarea separados por guiones, seguido de "Tarea2.c", de la forma "Prat Chacon-Carrera
Pinto-Tarea2.c ". [cite: 37] El incumplimiento de este requisito de nombre de archivo será penalizado con
un descuento de un punto en la nota. [cite: 37] Los archivos compilados no serán tomados en cuenta, si se
llega a subir sólo un archivo compilado, este será ignorado, y será evaluado con nota 1. [cite: 37, 38]

En el momento de compilación, se deberá indicar las
siguientes flags al compilador:

`-Wall -Wextra Wundef -Werror -Wuninitialized -Winit-self` [cite: 38]

Aquellas tareas que no compilen de la forma básica (`gcc -o salida entrada`) serán evaluadas
con nota 1. [cite: 38] Las que compilen, pero se caigan durante la ejecución, serán evaluadas con nota máxima
3. [cite: 38, 39, 40]

Su programa podría ser evaluado con múltiples casos de prueba y deberá ser capaz de ejecu-
tarlos todos de manera correcta. [cite: 39, 40] De fallar en algún caso de prueba serán descontados los puntos
correspondientes a dicho caso. g[cite: 39, 40]g
