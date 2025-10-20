#pragma once
#include "Vector2D.h"

class Body {
public:
    Vector2D position;
    Vector2D velocity;
    Vector2D sumOfForces; // Para acumular las fuerzas de cada frame

    float mass;
    float inverseMass;    // 1 / mass (0 para masa infinita)

    Body(const Vector2D& position, float mass)
        : position(position), 
          velocity(Vector2D(0, 0)), 
          sumOfForces(Vector2D(0, 0)), 
          mass(mass) 
    {
        if (mass == 0.0f) {
            inverseMass = 0.0f;
        } else {
            inverseMass = 1.0f / mass;
        }
    }

    // Funciones que añadiremos para manipular el Body
    void AddForce(const Vector2D& force);
    void ClearForces(); // Limpia las fuerzas al final de cada frame
};