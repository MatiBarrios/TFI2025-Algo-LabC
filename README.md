# Navegación Robótica

Este archivo está recontra escrito por una IA jajajaja

Este proyecto simula la navegación autónoma de un robot en un entorno bidimensional mapeado. Utiliza algoritmos de búsqueda para encontrar la ruta más corta hacia un destino, teniendo en cuenta los obstáculos y los pesos de cada celda en el mapa.

## Estructura del Proyecto

El proyecto está organizado en las siguientes carpetas y archivos:

- **src/**: Contene el código fuente del proyecto.
  - **main.c**: Punto de entrada de la aplicación.
  - **mapa.c**: Funciones para cargar y gestionar los datos del mapa.
  - **mapa.h**: Declaraciones de funciones y estructuras para el manejo del mapa.
  - **jugadores.c**: Gestión de datos de los jugadores y sus posiciones iniciales.
  - **jugadores.h**: Declaraciones de funciones para la gestión de jugadores.
  - **io.c**: Funciones de entrada/salida para interactuar con el usuario.
  - **io.h**: Declaraciones de funciones de entrada/salida.
  - **rutas.c**: Funciones para visualizar la ruta planificada y el movimiento del robot.
  - **rutas.h**: Declaraciones de funciones para la visualización de rutas.
  - **tipos.h**: Definiciones de tipos de datos comunes utilizados en el proyecto.

- **datos/**: Contiene los datos de prueba.
  - **mapas/**: Archivos de texto con representaciones matriciales de diferentes mapas.
    - **mapa1.txt**: Primer mapa de prueba.
    - **mapa2.txt**: Segundo mapa de prueba.
    - **mapa3.txt**: Tercer mapa de prueba.
  - **output/**: Archivos de texto los .
  - **ejemplos_entrada.txt**: Ejemplos de entrada para probar el programa.

- **Makefile**: Instrucciones para compilar el proyecto.

- **.gitignore**: Archivos y directorios que deben ser ignorados por el control de versiones.

## Instrucciones de Uso

1. **Compilación**: lo compile usando "gcc src\mapa.c src\rutas.c src\jugadores.c src\io.c src\main.c -o navegacion_robot.exe" en la terminal. Se puede usar el archivo Makefile tambien que está en el directorio.
2. **Ejecución**: ejecuta el programa. Se te pedirá que ingreses la posición inicial de los jugadores y los destinos.
3. **Interacción**: Sigue las instrucciones en pantalla para interactuar con el programa y visualizar la ruta planificada. Tambien podemos cambiar un par de cosas sobre como se ingresan los jugadores y bla

## Contribuciones


## Licencia

Este proyecto está bajo la Licencia MIT. Puedes usar, modificar y distribuir el código según los términos de esta licencia.