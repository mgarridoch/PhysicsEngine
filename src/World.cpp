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
    // Función para chequear colisiones AABB entre dos cuerpos
    for (int i = 0; i < m_bodies.size(); ++i) {
    for (int j = i + 1; j < m_bodies.size(); ++j) {
        Body& a = m_bodies[i];
        Body& b = m_bodies[j];
        if (CheckAABBCollision(a, b)) {
            // Por ahora, solo imprime un mensaje
            std::cout << "COLISION DETECTADA entre cuerpo " << i << " y " << j << "!" << std::endl;
            ResolveCollision(a, b);
        }
    }
}

}

// Función para chequear colisiones AABB entre dos cuerpos
bool World::CheckAABBCollision(const Body& a, const Body& b) {
    // Calcular los bordes de A
    float aLeft = a.position.x - a.width / 2.0f;
    float aRight = a.position.x + a.width / 2.0f;
    float aTop = a.position.y - a.height / 2.0f;
    float aBottom = a.position.y + a.height / 2.0f;

    // Calcular los bordes de B
    float bLeft = b.position.x - b.width / 2.0f;
    float bRight = b.position.x + b.width / 2.0f;
    float bTop = b.position.y - b.height / 2.0f;
    float bBottom = b.position.y + b.height / 2.0f;

    // Verificar si hay colisión
    if (aRight < bLeft || aLeft > bRight || aBottom < bTop || aTop > bBottom) {
        return false; // No hay colisión
    }
    return true; // Hay colisión
}

// Función para resolver colisiones AABB entre dos cuerpos
// En World.cpp
void World::ResolveCollision(Body& a, Body& b) {
    // 1. Calcula la velocidad relativa entre los dos cuerpos.
    Vector2D relativeVelocity = b.velocity - a.velocity;

    // 2. Calcula la "normal de colisión", que es el vector que apunta desde el centro de 'a' hacia 'b'.
    //    ¡No olvides normalizarlo para que sea un vector unitario!
    Vector2D collisionNormal = b.position - a.position;
    collisionNormal.Normalize();

    // 3. Calcula la velocidad a lo largo de la normal.
    //    Usa el producto punto para proyectar la velocidad relativa sobre la normal.
    float velocityAlongNormal = relativeVelocity.Dot(collisionNormal);

    // Si los objetos ya se están separando, no hagas nada.
    if (velocityAlongNormal > 0) {
        return;
    }

    // 4. Define el coeficiente de restitución (¡ajústalo a tu gusto!).
    float e = 0.4f; // Un rebote bastante elástico

    // 5. Calcula la magnitud del impulso (la fórmula 2D).
    float j = -(1 + e) * velocityAlongNormal;
    j /= a.inverseMass + b.inverseMass;

    // 6. Aplica el impulso a cada cuerpo.
    Vector2D impulse = collisionNormal * j;
    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;
}