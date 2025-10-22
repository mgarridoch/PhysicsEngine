#pragma once
#include <vector>
#include <memory> 
#include <SDL3/SDL.h>
#include "Body.h"

class Quadtree {
private:
    // Función auxiliar para determinar en qué cuadrante hijo cabe un cuerpo.
    int GetIndex(Body* body);
    // Límite de objetos que un nodo puede contener antes de subdividirse. (Ajustable)
    const int MAX_OBJECTS = 4;

    // Profundidad máxima de las subdivisiones para evitar una recursión infinita.
    const int MAX_LEVELS = 5;

    int m_level;                // La profundidad actual de este nodo (0 es la raíz).
    SDL_FRect m_bounds;         // El rectángulo que define los límites de este nodo.
    std::vector<Body*> m_objects; // Lista de *punteros* a los cuerpos en este nodo.

    // Punteros a los cuatro sub-nodos (hijos).
    // Usamos std::unique_ptr para que la memoria se gestione automáticamente.
    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;

public:
    // El constructor define los límites y el nivel del nodo.
    Quadtree(int level, SDL_FRect bounds);

    // Limpia el Quadtree, eliminando todos los objetos y sub-nodos.
    void Clear();

    // Inserta un objeto en el Quadtree.
    void Insert(Body* body);

    // Recupera todos los objetos que podrían colisionar con el área dada.
    void Query(const SDL_FRect& range, std::vector<Body*>& foundObjects) const;

private:
    // Subdivide el nodo actual en cuatro sub-nodos.
    void Subdivide();
};