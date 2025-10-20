#include "World.h"
#include "Body.h"
#include "Vector2D.h"
#include <cassert>

// Añade un Body al mundo
void World::AddBody(const Body& body) {
    m_bodies.push_back(body);
}

// Obtiene un Body por su índice
const Body& World::GetBody(int index) const {
    // Nos aseguramos de que el índice sea válido
    assert(index >= 0 && index < m_bodies.size());
    return m_bodies[index];
}

// Obtiene todos los cuerpos del mundo
std::vector<Body>& World::GetBodies() {
    return m_bodies;
}

// Actualiza la simulación del mundo, lo haremos despues.
void World::Update(float dt) {
    // Para cada 'body' en nuestra lista de 'm_bodies'...
    for (Body& body : m_bodies) {
        // Aquí dentro es donde aplicarás las 4 fórmulas de Euler. Calcular aceleracion, actualizar velocidad y posición y luego limpiar fuerzas.
        // 'body' es una referencia al objeto actual en la lista.
        Vector2D acceleration = body.sumOfForces * body.inverseMass; // NO usamos get ya que es un miembro público, sino habria que usar una función getAcceleration()
        body.velocity += acceleration * dt;
        body.position += body.velocity * dt;
        body.ClearForces();

        
    }
}