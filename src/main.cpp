#include <SDL3/SDL.h>
#include <iostream>
#include "Vector2D.h"
#include "Body.h"
#include "World.h"

// Función principal
int main(int argc, char* argv[]) {

    // --- Pruebas Vector2D---
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
    // --- Fin pruebas Vector2D ---

    // --- Pruebas Body y World ---
    std::cout << "--- Iniciando pruebas de Body y World ---" << std::endl;
    World world;
    Body body(Vector2D(0, 0), 1.0f); // Cuerpo en el origen con masa 1.0
    std::cout << "Cuerpo inicial en posicion: " << body.position << " con masa: " << body.mass << std::endl;
    body.AddForce(Vector2D(10, 0)); // Añadir una fuerza hacia la derecha
    std::cout << "Fuerza aplicada al cuerpo: " << body.sumOfForces << std::endl;
    // Añadir el cuerpo al mundo
    world.AddBody(body);
    // Actualizar el mundo con un delta time de 1 segundo
    world.Update(1.0f);
    // Obtener el cuerpo actualizado y mostrar su nueva posición
    const Body& updatedBody = world.GetBody(0);
    std::cout << "Posicion del cuerpo despues de 1 segundo: " << updatedBody.position << std::endl;
    world.Update(1.0f); // Actualizar otro segundo
    const Body& updatedBody2 = world.GetBody(0);
    std::cout << "Posicion del cuerpo despues de 2 segundos: " << updatedBody2.position << std::endl;
    std::cout << "--- Fin de las pruebas ---" << std::endl;
    // --- Fin pruebas Body y World ---

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