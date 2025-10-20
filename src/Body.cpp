#include "Body.h"
#include "Vector2D.h"

// Añade una fuerza al Body
void Body::AddForce(const Vector2D& force) {
    sumOfForces += force;
}

// Limpia las fuerzas acumuladas
void Body::ClearForces() {
    sumOfForces = Vector2D(0, 0);
}   