#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>

using namespace std;

// Las dimensiones de la pantalla
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Las dimensiones del circulo
const int CIRCLE_RADIUS = 20;

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

    //Dibujar el circulo
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

        SDL_RenderFillRect(renderer, &lineRect);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <Cantidad de circulos>" << endl;
        return 1;
    }

    // Inicializamos la cantidad de circulos
    int N = atoi(argv[1]);

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
        circles[i].x = rand() % (SCREEN_WIDTH - 2 * CIRCLE_RADIUS) + CIRCLE_RADIUS;
        circles[i].y = rand() % (SCREEN_HEIGHT - 2 * CIRCLE_RADIUS) + CIRCLE_RADIUS;
        circles[i].color = getRandomColor();
    }

    // Variable para medir FPS
    Uint32 frameStart, frameTime;
    int framecount =0;

    bool quit = false;

    while(!quit) {
        frameStart = SDL_GetTicks();

        // Manejo de eventos
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpiamos la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujamos los circulos
        for (int i = 0; i < N; ++i) {
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, 255);
            SDL_RenderFillCircle(renderer, circles[i].x, circles[i].y, CIRCLE_RADIUS);
        }

        // Mostrar FPS
        frameTime = SDL_GetTicks() - frameStart;
        framecount++;
        if(frameTime < 1000 / FRAMES_PER_SECOND) {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - frameTime);
        }

        SDL_RenderPresent(renderer);

    }

    // Liberamos recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}