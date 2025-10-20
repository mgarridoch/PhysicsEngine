#include "World.h"
#include "Body.h"

// Añade un Body al mundo
void World::AddBody(const Body& body) {
    m_bodies.push_back(body);
}

// Actualiza la simulación del mundo, lo haremos despues.
