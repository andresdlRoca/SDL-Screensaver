# Proyecto 1 - Computacion Paralela y Distribuida


## Screensaver secuencial
[Programa secuencial](screensaver.cpp)  

Este programa es un screensaver interactivo implementado en C++ utilizando la biblioteca Simple DirectMedia Layer (SDL). El screensaver muestra varios círculos de colores que se mueven por la ventana, rebotando en las paredes de la misma. Además, muestra los frames por segundo (FPS) en la esquina superior izquierda de la pantalla para indicar la velocidad de renderización. Vale la pena mencionar que esto se realizo de manera secuencial, pero se implemento de manera paralela para comparar los resultados.

### Prerequisitos
- libsdl2-dev
- libsdl2-ttf-dev
```
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```
### Dependencias
- SDL2

### Compilacion
Para compilar el programa se debe ejecutar el siguiente comando:
```
g++ screensaver.cpp -o screensaver -lSDL2main -lSDL2 -lSDL2_ttf
```
Las banderas que se utilizan para el compilador nos sirven para enlazar las bibliotecas de SDL2 y SDL2_ttf.

### Uso
El programa antes de ejecutarse le pedira al usuario que ingrese el numero de circulos que desea que se muestren en pantalla. Una vez que se ingrese el numero de circulos, se mostrara la ventana con los circulos moviendose junto con su respectivo contador de FPS.  

  
N es el numero de circulos que se dibujaran dentro de la ventana.
```
./screensaver <N>
```

## Screensaver paralelo
[Programa paralelo]()

### Prerequisitos

### Dependencias

### Compilacion

### Uso


## Autores
👤 Diego Cordova
- <a href= "https://github.com/Diego-CB">Github</a>

👤 Gabriela Contreras
- <a href="https://github.com/Paola-Contreras">Github</a>

👤 Andrés de la Roca  
- <a href = "https://www.linkedin.com/in/andr%C3%A8s-de-la-roca-pineda-10a40319b/">Linkedin</a>  
- <a href="https://github.com/andresdlRoca">Github</a>
