#include <SDL3/SDL.h>
#include <iostream>
#include "Vector2D.h"

// Función principal
int main(int argc, char* argv[]) {
    // --- INICIO DE LABORATORIO DE PRUEBAS ---
    std::cout << "--- Iniciando pruebas de Vector2D ---" << std::endl;
    Vector2D a(3.0f, 4.0f);
    Vector2D b(1.0f, 2.0f);

    // Prueba 1: Suma
    Vector2D suma = a + b;
    std::cout << "Prueba de Suma: " << a << " + " << b << " = " << suma << std::endl;

    // Prueba 2: Magnitud
    // Resultado esperado: sqrt(3*3 + 4*4) = sqrt(9 + 16) = sqrt(25) = 5
    std::cout << "Prueba de Magnitud de a: " << a.Magnitude() << std::endl;

    // Prueba 3: Multiplicación por escalar
    Vector2D producto = a * 2.0f;
    std::cout << "Prueba de Producto: " << a << " * 2.0 = " << producto << std::endl;

    // Prueba 4: Normalización
    Vector2D a_original = a; // Guardamos el original
    a.Normalize();
    std::cout << "Prueba de Normalizacion de " << a_original << ": " << a << std::endl;
    std::cout << "Magnitud del vector normalizado: " << a.Magnitude() << std::endl; // Debería ser ~1.0

    std::cout << "--- Fin de las pruebas ---" << std::endl;
    // --- FIN DE LABORATORIO DE PRUEBAS ---

    // Inicializar SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL3: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Crear una ventana, si falla, mostrar un mensaje de error
    SDL_Window* window = SDL_CreateWindow("Physics Engine", 800, 600, 0);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Crear un renderizador, si falla, mostrar un mensaje de error
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr); 
    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Zona de prueba: limpiar la pantalla con un color azul
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_RenderClear(renderer); // Limpiar la pantalla
    SDL_RenderPresent(renderer); // Presentar el contenido renderizado
    SDL_Delay(10000); // Esperar 10 segundos para ver la ventana
    SDL_DestroyRenderer(renderer); // Limpiar el renderizador
    SDL_DestroyWindow(window); // Limpiar la ventana
    SDL_Quit(); // Salir de SDL

    return 0;
}