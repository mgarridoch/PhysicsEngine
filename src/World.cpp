#include "World.h"
#include "Body.h"
#include "Vector2D.h"
#include <cassert>
#include <iostream>

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
void World::Update(float dt , int screenWidth, int screenHeight) {
    // Para cada 'body' en nuestra lista de 'm_bodies'...
    for (Body& body : m_bodies) {
        // Aquí dentro es donde aplicarás las 4 fórmulas de Euler. Calcular aceleracion, actualizar velocidad y posición y luego limpiar fuerzas.
        // 'body' es una referencia al objeto actual en la lista.
        Vector2D acceleration = body.sumOfForces * body.inverseMass; // NO usamos get ya que es un miembro público, sino habria que usar una función getAcceleration()
        body.velocity += acceleration * dt;
        body.position += body.velocity * dt;

        // Check for border collision (at any border)
        if (body.position.x - body.width / 2.0f < 0) { // Left border
            body.position.x = body.width / 2.0f;
            body.velocity.x *= -0.5f; // Simple bounce with damping

        }
        if (body.position.x + body.width / 2.0f > screenWidth) { // Right border
            body.position.x = screenWidth - body.width / 2.0f;
            body.velocity.x *= -0.5f;
        } 
        if (body.position.y - body.height / 2.0f < 0) { // Top border
            body.position.y = body.height / 2.0f;
            body.velocity.y *= -0.5f;
        } 
        if (body.position.y + body.height / 2.0f > screenHeight) { // Bottom border
            body.position.y = screenHeight - body.height / 2.0f; 
            body.velocity.y *= -0.5f;
        }

        // Clear forces after update
        body.ClearForces();

        
    }
}