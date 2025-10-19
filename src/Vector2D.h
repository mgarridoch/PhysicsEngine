// Usamos .h como "El menu" o un resumen de las funciones y clases que vamos a implementar en el .cpp

#pragma once
#include <ostream>

class Vector2D {
public:
    float x;
    float y;

    // Constructores (a menudo se dejan en el .h por ser simples)
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}

    // --- Declaraciones de los operadores ---
    // Le decimos al compilador "esta función existe"
    Vector2D& operator+=(const Vector2D& other); 
    // La funcion funciona de la siguiente manera: "Vector2D& operator+=" significa que la función retorna una referencia a un objeto Vector2D.
    // "(const Vector2D& other)" significa que toma como parámetro una referencia constante a otro objeto Vector2D.
    Vector2D operator+(const Vector2D& other) const; 
    // El "const" al final indica que esta función no modificará el objeto actual, sino que solo lo usará para realizar la operación. 
    // Este no tiene &, ya que retorna una copia nueva, mientras que el otro retorna una referencia al mismo objeto.

    Vector2D& operator-=(const Vector2D& other);
    Vector2D operator-(const Vector2D& other) const;

    Vector2D& operator*=(float scalar);
    Vector2D operator*(float scalar) const;

    Vector2D& operator/=(float scalar);
    Vector2D operator/(float scalar) const;

    // Funciones especiales
    float Magnitude() const; // Retorna la magnitud (longitud) del vector, como un valor float.
    Vector2D& Normalize(); // Normaliza el vector actual
    float Dot(const Vector2D& other) const; // Producto punto entre dos vectores

};

// Sobrecarga del operador << para imprimir un Vector2D
// Esto le permite a std::cout saber cómo manejar nuestro objeto
inline std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}