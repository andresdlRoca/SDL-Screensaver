#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <omp.h>

using namespace std;

// Las dimensiones de la pantalla
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int THREADS;

// Las dimensiones del circulo
const int CIRCLE_RADIUS = 30;
const int MAX_CIRCLE_SPEED = 5;

// Los frames por segundo
const int FRAMES_PER_SECOND = 60;

// Estructura para el circulo
struct Circle
{
    // Posicion del circulo
    int x, y;

    // Velocidad del circulo
    int xVel, yVel;

    // Color del circulo
    SDL_Color color;
};

// Funcion para generar un color aleatorio
SDL_Color getRandomColor() {
    SDL_Color color;
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    return color;
}

// Funcion para dibujar un circulo relleno en el renderer de SDL
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    //Definir un rectangulo que enmarque el circulo
    SDL_Rect rect;
    rect.x = x - radius;
    rect.y = y - radius;
    rect.w = radius * 2;
    rect.h = radius * 2;

    // Cambio 1: Paralelizacion del render de cada circulo
    //Dibujar el circulo
    # pragma omp parallel for num_threads(THREADS)
    for(int dy = -radius; dy <= radius; dy++) {
        // Calcular la mitad de la linea horizontal en funcion de la distancia vertical
        int lineW = static_cast<int>(sqrt(radius * radius - dy * dy) * 2 + 0.5);
        
        // Calcular la posicion vertical del centro de la linea
        int lineY = y + dy;

        // Dibujar la linea horizontal
        SDL_Rect lineRect;
        lineRect.x = x - lineW / 2;
        lineRect.y = lineY;
        lineRect.w = lineW;
        lineRect.h = 1;

        # pragma omp critical
        SDL_RenderFillRect(renderer, &lineRect);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <Cantidad de circulos> <Numero de threads>" << endl;
        return 1;
    }

    // Inicializamos la cantidad de circulos0
    int N = atoi(argv[1]);

    // Inicializamos la cantidad de threads
    THREADS = atoi(argv[2]);


    // Chequeamos que la cantidad de circulos sea valida
    if (N <= 0) {
        cout << "La cantidad de circulos debe ser mayor a 0" << endl;
        return 1;
    }

    // Inicializamos SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "No se pudo inicializar SDL: " << SDL_GetError() << endl;
        return 1;
    }

    // Inicializamos SDL_ttf
    if (TTF_Init() < 0) {
        cout << "No se pudo inicializar SDL_ttf: " << TTF_GetError() << endl;
        return 1;
    }

    // Cargamos la fuente
    TTF_Font* font = TTF_OpenFont("slkscr.ttf", 14);
    if (font == NULL) {
        cout << "No se pudo cargar la fuente: " << TTF_GetError() << endl;
        return 1;
    }

    // Creamos la ventana
    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "No se pudo crear la ventana: " << SDL_GetError() << endl;
        return 1;
    }

    // Creamos el renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        cout << "No se pudo crear el renderer: " << SDL_GetError() << endl;
        return 1;
    }

    // SEED para los numeros aleatorios
    srand(time(0));

    // Creamos los circulos
    vector<Circle> circles(N);
    for(int i = 0; i < N; ++i) {
        // Crear circulos en posiciones aleatorias 
        circles[i].x = rand() % (SCREEN_WIDTH - 2 * CIRCLE_RADIUS) + CIRCLE_RADIUS;
        circles[i].y = rand() % (SCREEN_HEIGHT - 2 * CIRCLE_RADIUS) + CIRCLE_RADIUS;
        // Cambiar de posicion los circulos, ayuda a la animacion 
        circles[i].xVel = rand() % (2 * MAX_CIRCLE_SPEED) - MAX_CIRCLE_SPEED; 
        circles[i].yVel = rand() % (2 * MAX_CIRCLE_SPEED) - MAX_CIRCLE_SPEED; 
        // Generar un color aleatorio
        circles[i].color = getRandomColor();
    }

    // Variable para medir FPS
    Uint32 frameStart, frameTime;
    int displayedFPS = 0;
    int frame_count = 0;
    bool quit = false;

    // Contador de frames para sacar promedio de tiempo de procesamiento
    int actual_frame = 1;
    // Variable para guardar el tiempo de procesamiento promedio
    double process_time = 0;
    
    while(!quit) {
        // Resto del código de la iteración
        Uint32 frameStart = SDL_GetTicks();
        
        // Manejo de eventos
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpiamos la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 102, 255, 255);
        SDL_RenderClear(renderer);

        // Cambio 2: Parelilacion de animacion de circulos
        // Animacion de los circulos
        #pragma omp parallel for num_threads(THREADS) firstprivate(CIRCLE_RADIUS, SCREEN_WIDTH, SCREEN_HEIGHT)
        for (int i = 0; i < N; i++) {
            // Actualizar la posición de cada círculo
            circles[i].x += circles[i].xVel;
            circles[i].y += circles[i].yVel;

           // Mantener circulos dentro de los bordes de la pantalla, permite que reboten
            if (circles[i].x - CIRCLE_RADIUS <0 || circles[i].x + CIRCLE_RADIUS > SCREEN_WIDTH) {
                circles[i].xVel = -circles[i].xVel; 
            }
            if (circles[i].y - CIRCLE_RADIUS < 0 || circles[i].y + CIRCLE_RADIUS > SCREEN_HEIGHT) {
                circles[i].yVel = -circles[i].yVel; 
            }
        }

        // Dibujamos los circulos
        for (int i = 0; i < N; ++i) {
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, 255);
            SDL_RenderFillCircle(renderer, circles[i].x, circles[i].y, CIRCLE_RADIUS);
        }
        // Actualizar frames
        frame_count ++;

        // Calcular los FPS
        frameTime = SDL_GetTicks() - frameStart;

        if(frameTime > 0) {
            displayedFPS = (frame_count*1000)/frameTime;
            frame_count = 0;
            printf("FPS: %d\n", displayedFPS); // mostrar frame actualizado en consola
        }

        // Mostrar los FPS en la esquina superior izquierda
        string fpsText = "FPS: " + to_string(displayedFPS);
        SDL_Color textColor = {255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect;
        textRect.x = 10; // Posicion
        textRect.y = 10; // Posicion
        textRect.w = textSurface->w; // Ancho
        textRect.h = textSurface->h; // Alto

        // Dibujamos el texto
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // Se renderiza la pantalla
        SDL_Delay(4);
        SDL_RenderPresent(renderer);

        // Calculo de tiempo promedio
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        process_time = process_time == 0 ? frameTime : (process_time * (actual_frame - 1)) / actual_frame;
        
        // Se aumenta el numero de frames
        actual_frame++;
    }

    // Liberamos recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    // Print del tiempo de procesamiento promedio por frame
    cout << "Tiempo de procesamiento de frame promedio: " << process_time << "ms" << endl;

    return 0;
}
