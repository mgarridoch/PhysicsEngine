// Usamos el .cpp como "la cocina" donde implementamos las funciones y clases declaradas en el .h

#include <cassert>
#include "Vector2D.h" 
#include <cmath>

// Implementación de operator+=
Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this; // con *this retornamos el objeto actual, no solo el puntero
}

// Implementación de operator+
Vector2D Vector2D::operator+(const Vector2D& other) const {
    // Creamos una copia de nuestro vector actual y usamos el += que ya definimos.
    return Vector2D(x, y) += other;
}

// Implementación de operator-=
Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// Implementación de operator-
Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x, y) -= other;
}

// Implementación de operator*=
Vector2D& Vector2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// Implementación de operator*
Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x, y) *= scalar;
}

// Implementación de operator/=
Vector2D& Vector2D::operator/=(float scalar) {
    assert(scalar != 0 && "¡Error: División de Vector2D por cero!");
    if (scalar != 0) {
        x /= scalar;
        y /= scalar;
    }
    return *this;
}

// Implementación de operator/
Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(x, y) /= scalar;
}

// implementación de magnitude, utilizando la función sqrtf de la librería cmath.
float Vector2D::Magnitude() const {
    return sqrtf(x * x + y * y);
}

// Implementación de Normalize
Vector2D& Vector2D::Normalize() {
    float mag = Magnitude();
    assert(mag != 0 && "¡Error: Normalización de Vector2D con magnitud cero!");
    if (mag != 0) {
        x /= mag;
        y /= mag;
    }
    return *this;
}

// Implementacion de dotproduct
float Vector2D::Dot(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

