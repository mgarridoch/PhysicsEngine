#include <SDL3/SDL.h>
#include <iostream>
#include "Vector2D.h"
#include "Body.h"
#include "World.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL3: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Physics Engine", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr); 
    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // --- CONFIGURACIÓN DE LA SIMULACIÓN ---
    World world;

    // --- LÓGICA DE TIEMPO ---
    const float dt = 1.0f / 60.0f; // El timestep FIJO para la física
    float accumulator = 0.0f;
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = 0;
    // --- FIN DE LA LÓGICA DE TIEMPO ---

    // Creamos un cuerpo que caerá
    Body fallingBody(Vector2D(SCREEN_WIDTH / 2.0f, 50.0f), 25.0f, 25.0f, 5.0f); // Posición inicial y masa
    world.AddBody(fallingBody);

    // Creamos cuerpo de "plataforma" estático, con masa 0 (infinita)
    Body groundBody(Vector2D(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f), 400.0f, 20.0f, 0.0f); // Masa 0 = infinita
    world.AddBody(groundBody);

    const Vector2D gravity(0.0f, 800.0f); // Gravedad (aumentamos y para que sea más visible)
    // --- FIN DE LA CONFIGURACIÓN ---

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        // --- 0. CÁLCULO DEL DELTA TIME REAL ---
        lastTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();
        // Delta time es el tiempo transcurrido en segundos
        float deltaTime = (float)(currentTime - lastTime) / (float)SDL_GetPerformanceFrequency();
        // Acumulamos el tiempo transcurrido
        accumulator += deltaTime;

        // --- 1. MANEJO DE EVENTOS ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        // --- 3. ACTUALIZACIÓN DE FÍSICA (FIXED TIMESTEP) ---
        // Mientras tengamos tiempo acumulado para dar un paso de física...
        while (accumulator >= dt) {
            // Aplicamos fuerzas (esto debería ir dentro del bucle de física)
            for (auto& body : world.GetBodies()) {
                 body.AddForce(gravity);
            }
            // ...actualizamos el mundo usando nuestro dt FIJO.
            world.Update(dt, SCREEN_WIDTH, SCREEN_HEIGHT);
            
            // Y restamos el tiempo que acabamos de simular.
            accumulator -= dt;
        }

        // --- 3. RENDERIZADO (DRAW) ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255); // Fondo azul
        SDL_RenderClear(renderer);

        // Dibujamos todos los cuerpos del mundo
        SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255); // Color amarillo para el objeto
        for (const auto& body : world.GetBodies()) {
            SDL_FRect bodyRect = {
                body.position.x - body.width / 2.0f, // Centramos el rectángulo en la posición
                body.position.y - body.height / 2.0f,
                body.width,
                body.height
            };
            SDL_RenderFillRect(renderer, &bodyRect);
        }

        SDL_RenderPresent(renderer);
    }

    // Limpieza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}