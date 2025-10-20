#pragma once

#include "Body.h"
#include <vector> // ¡Importante incluir la librería para usar std::vector!

class World {
private: // Los detalles internos del mundo deben ser privados
    std::vector<Body> m_bodies; // "m_" es una convención común para miembros privados

public:
    // ¿Cómo sería la declaración de una función que añade un Body a nuestra lista?
    void AddBody(const Body& body);

    // La función principal que moverá todo. Con dt como delta time (tiempo transcurrido) desde el último frame
    void Update(float dt);

    // Necesitamos una función para obtener un Body, ya que si desde main.cpp añadimos un Body al World, no tendremos acceso directo a él. (es una copia)
    const Body& GetBody(int index) const;
};