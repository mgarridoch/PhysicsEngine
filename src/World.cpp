#include "World.h"
#include "Body.h"
#include "Vector2D.h"
#include <cassert>
#include <iostream>

// Añade un Body al mundo
void World::AddBody(const Body& body) {
    m_bodies.push_back(body);
}

// Constructor que inicializa el Quadtree con los límites del mundo
World::World(const SDL_FRect& worldBounds) 
    : m_quadtree(0, worldBounds) {}

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

// Actualiza la simulación del mundo, lo haremos despues.
void World::Update(float dt, int screenWidth, int screenHeight) {
    // === PASO 1: MOVER TODOS LOS CUERPOS ===
    for (Body& body : m_bodies) {
        // Aplicamos la física de Euler como antes
        Vector2D acceleration = body.sumOfForces * body.inverseMass;
        body.velocity += acceleration * dt;
        body.position += body.velocity * dt;

        // Limpiamos las fuerzas para el siguiente frame
        body.ClearForces();
    }

    // === PASO 2: DETECTAR Y RESOLVER COLISIONES (CON QUADTREE) ===
    
    // Limpiamos y reconstruimos el Quadtree con las nuevas posiciones
    m_quadtree.Clear();
    for (Body& body : m_bodies) {
        m_quadtree.Insert(&body);
    }

    // Para cada cuerpo, comprobamos colisiones
    for (Body& body : m_bodies) {
        // --- A. Colisión con los bordes ---
        // (Movemos esta lógica aquí, dentro del bucle de colisión general)
        if (body.position.x - body.width / 2.0f < 0) {
            body.position.x = body.width / 2.0f;
            body.velocity.x *= -0.4f; // Coeficiente de restitución para los bordes
        }
        if (body.position.x + body.width / 2.0f > screenWidth) {
            body.position.x = screenWidth - body.width / 2.0f;
            body.velocity.x *= -0.4f;
        } 
        if (body.position.y - body.height / 2.0f < 0) {
            body.position.y = body.height / 2.0f;
            body.velocity.y *= -0.4f;
        } 
        if (body.position.y + body.height / 2.0f > screenHeight) {
            body.position.y = screenHeight - body.height / 2.0f;
            body.velocity.y *= -0.4f;
        }

        // --- B. Colisión con otros cuerpos ---
        std::vector<Body*> nearbyBodies;
        SDL_FRect bodyBounds = { /* ... cálculo de bodyBounds ... */ };
        m_quadtree.Query(bodyBounds, nearbyBodies);

        for (Body* other : nearbyBodies) {
            if (&body == other) {
                continue; // No chocar consigo mismo
            }

            if (CheckAABBCollision(body, *other)) {
                ResolveCollision(body, *other);
            }
        }
    }
}

// Función para resolver colisiones AABB entre dos cuerpos
void World::ResolveCollision(Body& a, Body& b) {
    // --- NUEVO: RESOLUCIÓN DE PENETRACIÓN BASADA EN MASA ---
    // Es importante que sea en masa para que los objetos estáticos (masa infinita) no se muevan.
    Vector2D distanceVec = b.position - a.position;
    float overlapX = (a.width / 2.0f + b.width / 2.0f) - std::abs(distanceVec.x);
    float overlapY = (a.height / 2.0f + b.height / 2.0f) - std::abs(distanceVec.y);

    if (overlapX > 0 && overlapY > 0) {
        float totalInverseMass = a.inverseMass + b.inverseMass;
        // Si la masa total inversa es cero, ambos objetos son estáticos. No hacemos nada.
        if (totalInverseMass == 0) {
            return;
        }

        // La colisión ocurre en el eje con menor superposición
        if (overlapX < overlapY) {
            float movePerIMass = overlapX / totalInverseMass;
            float moveA = movePerIMass * a.inverseMass;
            float moveB = movePerIMass * b.inverseMass;

            float direction = (distanceVec.x > 0 ? -1.0f : 1.0f);
            a.position.x += moveA * direction;
            b.position.x -= moveB * direction;
        } else {
            float movePerIMass = overlapY / totalInverseMass;
            float moveA = movePerIMass * a.inverseMass;
            float moveB = movePerIMass * b.inverseMass;

            float direction = (distanceVec.y > 0 ? -1.0f : 1.0f);
            a.position.y += moveA * direction;
            b.position.y -= moveB * direction;
        }
    }

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
    float e = 0.6f; // Un rebote bastante elástico

    // 5. Calcula la magnitud del impulso (la fórmula 2D).
    float j = -(1 + e) * velocityAlongNormal;
    j /= a.inverseMass + b.inverseMass;

    // 6. Aplica el impulso a cada cuerpo.
    Vector2D impulse = collisionNormal * j;
    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;
}